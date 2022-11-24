#include "stdafx.h"
#include "CState.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CState_Manager.h"

#include "CUser.h"

#include "CUnit.h"



CState::CState()
{
}

CState::~CState()
{
}

void CState::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{

	int a = 0;

}

void CState::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	CUser::Get_Instance()->Clear_KeyCommands();
    m_pOwner = pOwner;
    m_fTimeAcc = 0.f;
    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex, m_eAnimDivide);
    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, m_fInterPolationTime);
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);
    m_bExecuted = false;

}

STATE_TYPE CState::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    //if (!m_wstrSoundKey.empty())
    //{
    //    m_fLoopSoundAcc += fDT(0);

    //    if (m_fLoopSoundAcc > m_fSoundLoopTime)
    //    {
    //        m_fLoopSoundAcc = 0.f;
    //        //CFunctor::Play_Sound(m_wstrSoundKey, m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

    //    }
    //}


	if (m_bAttackTrigger)
	{
		// ���� ����
		if (pOwner->Is_Weapon_R_Collision())
		{
			_float4 vHitPos = pOwner->Get_HitPos();
			_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

			//HitPos�� �� ���� �Ʒ�, �� ���� ���� ��쿡�� ƨ�ܳ��°� �ƴ϶� ���� ��ƼŬ�� ������.
			if (vHitPos.y <= vPos.y + 0.1f)
			{
				Hit_GroundEffect(pOwner);
			}
			else
			{
				CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), pOwner->Get_HitMatrix());
				return m_eBounceState;
			}

			m_bAttackTrigger = false;
		}
	}

    Check_KeyFrameEvent(pOwner, pAnimator);

    STATE_TYPE eType = STATE_END;

    if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
        return eType;

    for (auto& elem : m_vecAdjState)
    {
        eType = CState_Manager::Get_Instance()->Get_State(elem)->Check_Condition(pOwner, pAnimator);

        if (eType != STATE_END)
            break;
    }


    return eType;
}


void CState::Re_Enter(CUnit* pOwner, CAnimator* pAnimator, _float fInterpolationTime, _float fAnimSpeed)
{

    if (fInterpolationTime >= 0.f)
        m_fInterPolationTime = fInterpolationTime;
    
    if (fAnimSpeed >= 0.f)
        m_fAnimSpeed = fAnimSpeed;

    Enter(pOwner, pAnimator, m_eStateType);
}

void CState::Hit_GroundEffect(CUnit* pOwner)
{
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), pOwner->Get_HitMatrix());
}


void CState::Check_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator)
{
    _uint iCurKeyFrame = pAnimator->Get_CurAnimFrame();
    
    _uint iSize = (_uint)m_vecKeyFrameEvent.size();

    for (_uint i = 0; i < iSize; ++i)
    {
        if (iCurKeyFrame >= m_vecKeyFrameEvent[i].iKeyFrame)
        {
            if (m_vecKeyFrameEvent[i].bExecuted)
                continue;

            On_KeyFrameEvent(pOwner, pAnimator, m_vecKeyFrameEvent[i], m_vecKeyFrameEvent[i].iSequence);
            m_vecKeyFrameEvent[i].bExecuted = true;
        }
    }
}
_uint CState::Get_Direction()
{
	if (KEY(W, HOLD))
	{
		// Key(CTRL + W + A)
		if (KEY(A, HOLD))
		{
			return STATE_DIRECTION_NW;
		}

		// Key(CTRL + W + D)
		else if (KEY(D, HOLD))
		{
			return STATE_DIRECTION_NE;
		}

		// Key(CTRL + W)
		else
		{
			return STATE_DIRECTION_N;
		}


	}

	// Key(CTRL + S)
	else if (KEY(S, HOLD))
	{

		// Key(CTRL + S + A)
		if (KEY(A, HOLD))
		{
			return STATE_DIRECTION_SW;
		}

		// Key(CTRL + S + D)
		else if (KEY(D, HOLD))
		{
			return STATE_DIRECTION_SE;
		}

		// Key(CTRL + S)
		else
		{
			return STATE_DIRECTION_S;
		}
	}

	// Key(CTRL + A)
	else if (KEY(A, HOLD))
	{
		return STATE_DIRECTION_W;
	}

	// Key(CTRL + D)
	else if (KEY(D, HOLD))
	{
		return STATE_DIRECTION_E;
	}

	return STATE_DIRECTION_END;
}

_uint CState::Get_Direction_Four()
{
	if (KEY(W, HOLD))
	{
		return STATE_DIRECTION_N;
	}

	// Key(CTRL + S)
	else if (KEY(S, HOLD))
	{
		return STATE_DIRECTION_S;
	}

	// Key(CTRL + A)
	else if (KEY(A, HOLD))
	{
		return STATE_DIRECTION_W;
	}

	// Key(CTRL + D)
	else if (KEY(D, HOLD))
	{
		return STATE_DIRECTION_E;
	}

	return STATE_DIRECTION_END;
}


_uint CState::Move_Direction_Loop(CUnit* pOwner, CAnimator* pAnimator, _float fInterPolationTime)
{
	_uint iDirection = Get_Direction();

	Change_Location_Loop(iDirection, pAnimator, fInterPolationTime);

	return DoMove(iDirection, pOwner);

}



void CState::Change_Location_Loop(_uint iDirection, CAnimator* pAnimator, _float fInterPolationTime)
{
	if (iDirection == STATE_DIRECTION_END)
		return;

	if (m_iAnimIndex != m_iDirectionAnimIndex[iDirection])
	{
		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];

		pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex, m_eAnimDivide);
		pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fDirectionAnimSpeed[iDirection]);
		pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, fInterPolationTime);
	}

}



_uint CState::DoMove(_uint iDirection, CUnit* pOwner)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	_float4 vRightDir;
	_float4 vLookDir;

	_float4 vCamLook, vCamRight;

	pOwner->Get_FollowCamLook();
	vCamLook = pOwner->Get_FollowCamLook();
	vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);


	vCamLook.y = 0.f;

	//Dir : ���� �̵�����
	_float4 vDir;
	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
	_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

	switch (iDirection)
	{
	case STATE_DIRECTION_NW:
		vDir = vCamRight * -1.f + vCamLook;

		break;

	case STATE_DIRECTION_NE:
		vDir = vCamRight * 1.f + vCamLook * 1.f;


		break;

	case STATE_DIRECTION_SW:
		vDir = vCamRight * -1.f + vCamLook * -1.f;


		break;

	case STATE_DIRECTION_SE:

		vDir = vCamRight * 1.f + vCamLook * -1.f;


		break;

	case STATE_DIRECTION_N:
		vDir = vCamLook;


		break;

	case STATE_DIRECTION_S:
		vDir = vCamLook * -1.f;


		break;

	case STATE_DIRECTION_W:
		vDir = vCamRight * -1.f;

		break;

	case STATE_DIRECTION_E:
		vDir = vCamRight * 1.f;




		break;

	default:
		break;
	}

	vDir.y = 0.f;

	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (iDirection == STATE_DIRECTION_S || 
		iDirection == STATE_DIRECTION_SW ||
		iDirection == STATE_DIRECTION_SE 
		)
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fWalkSpeed);
	else
		pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	if (!vDir.Is_Zero())
		pMyPhysicsCom->Set_Dir(vDir);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);


	return iDirection;


}

void CState::Follow_MouseLook(CUnit* pOwner)
{
	_float4 vCamLook = pOwner->Get_FollowCamLook();
	vCamLook.y = 0.f;
	vCamLook.Normalize();

	pOwner->Get_Transform()->Set_LerpLook(vCamLook, 0.4f);

}

void CState::Follow_MouseLook_Turn(CUnit* pOwner)
{
	_float4 vCamLook = pOwner->Get_FollowCamLook_Turn();
	vCamLook.y = 0.f;
	vCamLook.Normalize();

	pOwner->Get_Transform()->Set_LerpLook(vCamLook, 0.4f);
}



void CState::Physics_Setting(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax, _bool bBackStep)
{

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	_float4 vCamLook = pOwner->Get_FollowCamLook();
	
	vCamLook.y = 0.f;


	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (bBackStep)
		vCamLook *= -1.f;

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����

	pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	if(bSpeedasMax)
		pMyPhysicsCom->Set_SpeedasMax();

}

void	CState::Physics_Setting_Right(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax, _bool bRight)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	_float4 vCamRight = pOwner->Get_FollowCamRight();
	_float4 vCamLook = pOwner->Get_FollowCamLook();

	vCamRight.y = 0.f;
	vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (!bRight)
		vCamRight *= -1.f;

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamRight);

	//�ִ�ӵ� ����

	pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	if (bSpeedasMax)
		pMyPhysicsCom->Set_SpeedasMax();
}

void CState::Enable_ModelParts(CUnit* pOwner, _uint iPartType, _bool bEnable)
{
	GET_COMPONENT_FROM(pOwner, CModel)->Enable_ModelParts(iPartType, bEnable);
}


void CState::Add_KeyFrame(_uint iKeyFrameIndex, _uint iSequence)
{
    KEYFRAME_EVENT  tEvent;
    tEvent.iKeyFrame = iKeyFrameIndex;
	tEvent.iSequence = iSequence;
    m_vecKeyFrameEvent.push_back(tEvent);

	_uint iSize = (_uint)m_vecKeyFrameEvent.size();

	if (iSize > 5)
	{
		int i = 0;
	}
    
}
