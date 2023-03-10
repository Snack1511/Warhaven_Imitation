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

	/*if (!pOwner->Is_Air())
		CUser::Get_Instance()->SetActive_InteractUI(false);*/

	if (m_bAttackTrigger)
	{
		// 공격 진입
		if (pOwner->Is_Weapon_R_Collision())
		{
			_float4 vHitPos = pOwner->Get_HitPos();
			_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

			//HitPos가 발 보다 아래, 즉 땅을 때린 경우에는 튕겨나는게 아니라 작은 파티클과 진동만.
			if (vHitPos.y <= vPos.y + 0.1f)
			{
				Hit_GroundEffect(pOwner);
			}
			else
			{
				pOwner->Create_Light(vHitPos, 2.5f, 0.f, 0.f, 0.f, 0.05f, RGB(255, 255, 255));

				CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HitSmokeParticle_0"), pOwner->Get_HitMatrix());
				return m_eBounceState;
			}

			m_bAttackTrigger = false;
		}
	}

	if (pOwner->Get_Status().fDamageMultiplier != 1.f ||
		pOwner->Get_Status().fDamageMultiplier < 1.f  - FLT_MIN||
		pOwner->Get_Status().fDamageMultiplier > 1.f + FLT_MIN)
	{
		if(!m_bStopDamagePumping)
			m_fDamagePumping = pOwner->Get_Status().fDamageMultiplier;
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

void CState::Init_CommonState_Player()
{
	m_vecAdjState.push_back(STATE_REVIVE_PLAYER);
	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);
	m_vecAdjState.push_back(STATE_CANNON_PLAYER);
	//m_vecAdjState.push_back(STATE_GLIDING);
	//m_vecAdjState.push_back(STATE_TRANSFORM);
}

void CState::Init_CommonState_Hero_Player()
{
	m_vecAdjState.push_back(STATE_REVIVE_PLAYER);
	m_vecAdjState.push_back(STATE_CANNON_PLAYER);
	//m_vecAdjState.push_back(STATE_GLIDING);
	//m_vecAdjState.push_back(STATE_TRANSFORM);
}

void CState::Init_CommonState_AI()
{
	//m_vecAdjState.push_back(STATE_REVIVE_PLAYER);
	//m_vecAdjState.push_back(STATE_CHANGE_PLAYER);
	//m_vecAdjState.push_back(STATE_CANNON_PLAYER);
	//m_vecAdjState.push_back(STATE_TRANSFORM);
}


void CState::Init_CommonState_Hero_AI()
{

}

void CState::Init_AttackState_Priest()
{
	m_vecAdjState.push_back(STATE_PROJECTILECATCH_BEGIN_PRIEST);
	m_vecAdjState.push_back(STATE_WINDATTACK_PRIEST);
	m_vecAdjState.push_back(STATE_CURE_BEGIN_PRIEST);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PRIEST);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PRIEST);
}


void CState::Hit_GroundEffect(CUnit* pOwner)
{
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

	Play_Sound(L"Effect_Bounce");

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HitSmokeParticle_0"), pOwner->Get_HitMatrix());

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cannon_Ground", pOwner->Get_HitPos());

	if (STATE_WARRIOR_OXEN_LOOPATTACK == pOwner->Get_CurState())
	{
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Oxen_Smoke_0"), pOwner->Get_HitPos());
	}

	else if (CLASS_TYPE::ENGINEER == pOwner->Get_Status().eClass)
	{
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SmashSoilParticle", pOwner->Get_HitPos());
	}
}

void CState::Play_Voice(CUnit* pOwner, wstring strName, _float fVol, _int iRand)
{
	if (!pOwner)
		return;

	_int iRandom = random(0, iRand);

	if (iRandom != 0)
		return;

	CLASS_TYPE eClass = pOwner->Get_OwnerPlayer()->Get_CurClass();
	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	wstring strKey = strName;


	switch (eClass)
	{
	case Client::WARRIOR:
		strKey += L"_Warrior";
		break;
	case Client::ARCHER:
		strKey += L"_Archer";
		break;
	case Client::PALADIN:
		strKey += L"_Paladin";
		break;
	case Client::PRIEST:
		strKey += L"_Priest";
		break;
	case Client::ENGINEER:
		strKey += L"_Warhammer";
		break;
	case Client::FIONA:
		strKey += L"_Fiona";
		break;
	case Client::QANDA:
		strKey += L"_Qanda";
		break;
	case Client::LANCER:
		strKey += L"_Lancer";
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}

	CFunctor::Play_Sound(strKey, CHANNEL_VOICE, vPos, fVol);
}


void CState::Check_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator)
{
    _uint iCurKeyFrame = pAnimator->Get_CurAnimFrame();
    
    _uint iSize = (_uint)m_vecKeyFrameEvent.size();

    for (_uint i = 0; i < iSize; ++i)
    {
		if (iCurKeyFrame <= 1)
		{
			if (m_vecKeyFrameEvent[i].bLoop)
			{
				m_vecKeyFrameEvent[i].bExecuted = false;
			}
		}

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

	//Dir : 실제 이동방향
	_float4 vDir;
	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
	_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

	_uint iStateDir = iDirection;

	if (m_bStraight)
		iStateDir = STATE_DIRECTION_N;

	switch (iStateDir)
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

void CState::Prevent_Oneframe(CUnit* pOwner)
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

	//Dir : 실제 이동방향
	_float4 vDir;
	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
	_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

	switch (Get_Direction())
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
	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);
}



void CState::Set_Direction_Front_AI(_int& iDirectionRand)
{
	switch (iDirectionRand)
	{
	case STATE_DIRECTION_NW:
	case STATE_DIRECTION_SW:

		iDirectionRand = STATE_DIRECTION_NW;
		break;

	case STATE_DIRECTION_N:
	case STATE_DIRECTION_S:

		iDirectionRand = STATE_DIRECTION_N;

		break;

	case STATE_DIRECTION_NE:
	case STATE_DIRECTION_SE:

		iDirectionRand = STATE_DIRECTION_NE;
		break;

	case STATE_DIRECTION_E:
	

		iDirectionRand = STATE_DIRECTION_E;

		break;

	
	case STATE_DIRECTION_W:

		iDirectionRand = STATE_DIRECTION_W;

		break;


	default:
		break;
	}
}

void CState::Set_Direction_Back_AI(_int& iDirectionRand)
{
	switch (iDirectionRand)
	{
	case STATE_DIRECTION_NW:
	case STATE_DIRECTION_SW:

		iDirectionRand = STATE_DIRECTION_SW;
		break;

	case STATE_DIRECTION_N:
	case STATE_DIRECTION_S:

		iDirectionRand = STATE_DIRECTION_S;

		break;

	case STATE_DIRECTION_NE:
	case STATE_DIRECTION_SE:

		iDirectionRand = STATE_DIRECTION_SE;
		break;

	case STATE_DIRECTION_E:


		iDirectionRand = STATE_DIRECTION_E;

		break;


	case STATE_DIRECTION_W:

		iDirectionRand = STATE_DIRECTION_W;

		break;


	default:
		break;
	}
}



_float	CState::Get_Length(CUnit* pOwner)
{
	CUnit* pUnit = pOwner->Get_TargetUnit();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	//	_float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

	return vLook.Length();
}


_float CState::Move_Direction_Loop_AI(CUnit* pOwner)
{
	CUnit* pUnit = pOwner->Get_TargetUnit();

	if (!pUnit)
		return 0.f;

	CTransform* pMyTransform = pOwner->Get_Transform();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
//	_float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

	_float fLength = vLook.Length();

	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	vLook.Normalize();
//	vRight.Normalize();

	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	pMyPhysicsCom->Set_Accel(m_fMyAccel);

	return fLength;
}

void CState::DoMove_AI(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	CUnit* pUnit = m_pCurrentTargetUnit;
	if (!pUnit)
	{
		pUnit = pOwner;

		if (!pOwner)
			return;
	}
		



	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);

	if (vLook.Is_Zero())
		return;

		_float fLength = vLook.Length();


	vLook.Normalize();
	vRight.Normalize();

	vLook.y = 0.f;
	vRight.y = 0.f;

	_float4 vDir;

	//m_fMaxSpeed = 1.f;

	switch (m_iDirectionRand)
	{
	case STATE_DIRECTION_NW:
		vDir = vRight * -1.f + vLook;

		break;

	case STATE_DIRECTION_NE:
		vDir = vRight * 1.f + vLook * 1.f;

		break;

	case STATE_DIRECTION_SW:
		vDir = vRight * -1.f + vLook * -1.f;


		break;

	case STATE_DIRECTION_SE:

		vDir = vRight * 1.f + vLook * -1.f;


		break;

	case STATE_DIRECTION_N:
		vDir = vLook;


		break;

	case STATE_DIRECTION_S:
		vDir = vLook * -1.f;


		break;

	case STATE_DIRECTION_W:
		vDir = vRight * -1.f;


		break;

	case STATE_DIRECTION_E:
		vDir = vRight * 1.f;

		break;

	default:
		break;
	}


	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);


	vDir.y = 0.f;

	_float4 vMyLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	vMyLook.y = 0.f;

	if(pOwner->Get_OwnerPlayer()->Get_CurClass() == LANCER)
		pMyPhysicsCom->Set_Dir(vMyLook);
	else
		pMyPhysicsCom->Set_Dir(vDir);

	pMyPhysicsCom->Set_Accel(m_fMyAccel);

}

void CState::DoMove_AI_NoTarget(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	_float4 vRightDir;
	_float4 vLookDir;

	_float4 vLook, vRight;


	if (m_vAIRandLook.x >= 0.f && m_vAIRandLook.y >= 0.f && m_vAIRandLook.z >= 0.f)
	{
		vLook = m_vAIRandLook;
		vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT); //GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
	}
	else
	{
		vLook = pOwner->Get_FollowCamLook();
		vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT); //GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
	}



	vLook.y = 0.f;
	vRight.y = 0.f;

	//Dir : 실제 이동방향
	_float4 vDir;

	switch (m_iDirectionRand)
	{
	case STATE_DIRECTION_NW:
		vDir = vRight * -1.f + vLook;

		break;

	case STATE_DIRECTION_NE:
		vDir = vRight * 1.f + vLook * 1.f;


		break;

	case STATE_DIRECTION_SW:
		vDir = vRight * -1.f + vLook * -1.f;


		break;

	case STATE_DIRECTION_SE:

		vDir = vRight * 1.f + vLook * -1.f;


		break;

	case STATE_DIRECTION_N:
		vDir = vLook;


		break;

	case STATE_DIRECTION_S:
		vDir = vLook * -1.f;


		break;

	case STATE_DIRECTION_W:
		vDir = vRight * -1.f;

		break;

	case STATE_DIRECTION_E:
		vDir = vRight;


		break;

	default:
		break;
	}

	vDir.y = 0.f;

	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	
	if (!vDir.Is_Zero())
		pMyPhysicsCom->Set_Dir(vDir);

	pMyPhysicsCom->Set_Accel(m_fMyAccel);
}



_float CState::Get_TargetLook_Length(CUnit* pOwner)
{
	CUnit* pUnit = pOwner->Get_TargetUnit();

	if (!pUnit)
		return 0.f;

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);

	return vLook.Length();
}

_float4 CState::Get_TargetLook(CGameObject* pSourObject, CGameObject* pDestUnit, _bool bFixY)
{
	if (!pSourObject)
	{
		Call_MsgBox(L"PlayerUnit Is Null");
		return _float4(0.f, 0.f, 0.f);
	}

	if (!pDestUnit)
	{
		Call_MsgBox(L"OtherUnit Is Null");
		return _float4(0.f, 0.f, 0.f);
	}

	_float4 vLook = pDestUnit->Get_Transform()->Get_World(WORLD_POS) - pSourObject->Get_Transform()->Get_World(WORLD_POS);
	vLook.Normalize();

	if (bFixY)
		vLook.y = 0.f;

	return vLook;
}

void CState::Follow_Move(CGameObject* pSourObject, CGameObject* pDestUnit, _bool bFixY)
{
	_float4 vLook = Get_TargetLook(pSourObject, pDestUnit, bFixY);

	CTransform* pMyTransform = pSourObject->Get_Transform();
	
	CPhysics* pMyPhysicsCom = nullptr;
	pMyPhysicsCom = dynamic_cast<CUnit*>(pSourObject)->Get_PhysicsCom();

	if (pMyPhysicsCom)
	{
		Call_MsgBox(L"SourObject is not unit");
		return;
	}

	vLook.y = 0.f;

	if (!vLook.Is_Zero())
		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	pMyPhysicsCom->Set_Dir(vLook);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);
}

void CState::Physics_Setting(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax, _bool bBackStep)
{

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	_float4 vCamLook = pOwner->Get_FollowCamLook();
	
	vCamLook.y = 0.f;


	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (bBackStep)
		vCamLook *= -1.f;

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamLook);

	//최대속도 설정

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

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (!bRight)
		vCamRight *= -1.f;

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamRight);

	//최대속도 설정

	pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	if (bSpeedasMax)
		pMyPhysicsCom->Set_SpeedasMax();
}
void CState::Physics_Setting_AI(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax, _bool bBackStep)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	CUnit* pUnit = pOwner->Get_TargetUnit();
	if (!pUnit)
		return;
	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	
	vLook.y = 0.f;


	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	if (bBackStep)
		vLook *= -1.f;

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vLook);

	//최대속도 설정

	pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	if (bSpeedasMax)
		pMyPhysicsCom->Set_SpeedasMax();
}



void CState::Physics_Setting_Right_AI(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax, _bool bRight)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	CUnit* pUnit = pOwner->Get_TargetUnit();
	if (!pUnit)
		return;

	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);

	vRight.y = 0.f;
	vLook.y = 0.f;

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	if (!bRight)
		vRight *= -1.f;

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vRight);

	//최대속도 설정

	pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	if (bSpeedasMax)
		pMyPhysicsCom->Set_SpeedasMax();
}

void CState::Play_Sound(wstring wstrFileName, _uint iGroupIndex, _float fVolume)
{
	CFunctor::Play_Sound(wstrFileName, iGroupIndex, m_pOwner->Get_Transform()->Get_World(WORLD_POS), fVolume);
}

void CState::Enable_ModelParts(CUnit* pOwner, _uint iPartType, _bool bEnable)
{
	GET_COMPONENT_FROM(pOwner, CModel)->Enable_ModelParts(iPartType, bEnable);
}


void CState::Add_KeyFrame(_uint iKeyFrameIndex, _uint iSequence, _bool bLoop)
{
    KEYFRAME_EVENT  tEvent;
    tEvent.iKeyFrame = iKeyFrameIndex;
	tEvent.iSequence = iSequence;
	tEvent.bLoop = bLoop;
    m_vecKeyFrameEvent.push_back(tEvent);

	_uint iSize = (_uint)m_vecKeyFrameEvent.size();

	if (iSize > 5)
	{
		int i = 0;
	}
    
}
