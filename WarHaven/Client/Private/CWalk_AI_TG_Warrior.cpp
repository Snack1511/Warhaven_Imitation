#include "stdafx.h"
#include "CWalk_AI_TG_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_AI_TG_Warrior::CWalk_AI_TG_Warrior()
{
}

CWalk_AI_TG_Warrior::~CWalk_AI_TG_Warrior()
{
}

HRESULT CWalk_AI_TG_Warrior::Initialize()
{
    m_fInterPolationTime = 0.2f;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;
	

    return S_OK;
}

void CWalk_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

    m_iRand = rand() % 2;
    m_iDirectionRand = rand() % 8;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	CUnit* pUnit = pOwner->Get_TargetUnit();


	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

	_float fLength = vLook.Length();

	vLook.Normalize();
	vRight.Normalize();

	//pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	//pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	//pMyPhysicsCom->Set_Accel(m_fMyAccel);

	vLook.y = 0.f;

	//pOwner->Get_FollowCamLook();
	//vLook = pOwner->Get_FollowCamLook();
	//vRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);

	//Dir : 실제 이동방향
	_float4 vDir;

	if (vRight.z > 0.9f || vRight.x > 0.9f)
	{
		if (m_iDirectionRand == STATE_DIRECTION_NE ||
			m_iDirectionRand == STATE_DIRECTION_NW ||
			m_iDirectionRand == STATE_DIRECTION_N  ||
			m_iDirectionRand == STATE_DIRECTION_E)
		{
			vDir = vLook;
			Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);
		}


		if (m_iDirectionRand == STATE_DIRECTION_SE ||
			m_iDirectionRand == STATE_DIRECTION_SW ||
			m_iDirectionRand == STATE_DIRECTION_S  ||
			m_iDirectionRand == STATE_DIRECTION_W)
		{
			vDir = vLook * -1.f;
			Change_Location_Loop(STATE_DIRECTION_S, pAnimator, 0.1f);
		}

		if ((!vLook.Is_Zero()))
			pMyPhysicsCom->Set_Dir(vLook);

		

		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fWalkSpeed);


		vDir.y = 0.f;

		if (!vDir.Is_Zero())
			pMyPhysicsCom->Set_Dir(vDir);

		pMyPhysicsCom->Set_Accel(m_fMyAccel);


		return __super::Tick(pOwner, pAnimator);

	}
	else
	{
		_float fSpeed = 1.f;

		switch (m_iDirectionRand)
		{
		case STATE_DIRECTION_NW:
			vDir = vRight * -1.f + vLook;

			break;

		case STATE_DIRECTION_NE:
			vDir = vRight * 1.f + vLook * 1.f;
			fSpeed = 0.7f;

			break;

		case STATE_DIRECTION_SW:
			vDir = vRight * -1.f + vLook * -1.f;
			fSpeed = 0.8f;


			break;

		case STATE_DIRECTION_SE:

			vDir = vRight * 1.f + vLook * -1.f;
			fSpeed = 0.8f;


			break;

		case STATE_DIRECTION_N:
			vDir = vLook;


			break;

		case STATE_DIRECTION_S:
			vDir = vLook * -1.f;


			break;

		case STATE_DIRECTION_W:
			vDir = vRight * -1.f;
			fSpeed = 0.7f;


			break;

		case STATE_DIRECTION_E:
			vDir = vRight * 1.f;
			fSpeed = 0.7f;

			break;

		default:
			break;
		}
	

		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fWalkSpeed * fSpeed);


		vDir.y = 0.f;

		if (!vDir.Is_Zero())
			pMyPhysicsCom->Set_Dir(vDir);

		pMyPhysicsCom->Set_Accel(m_fMyAccel);

		Change_Location_Loop(m_iDirectionRand, pAnimator, 0.05f);
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 
    */


        //// 천천히 
        //if (KEY(C, HOLD))
        //{
        //    // 걸어간다.
        //    if (KEY(W, HOLD) ||
        //        KEY(A, HOLD) ||
        //        KEY(S, HOLD) ||
        //        KEY(D, HOLD))
        //    {

        //        return m_eStateType;
        //    }

        //}
        //CTRL 로 바꾸셈.
      
      

   


    return STATE_END;
}

