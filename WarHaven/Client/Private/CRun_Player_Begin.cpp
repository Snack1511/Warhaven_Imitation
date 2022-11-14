#include "stdafx.h"
#include "CRun_Player_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player_Begin::CRun_Player_Begin()
{
}

CRun_Player_Begin::~CRun_Player_Begin()
{
}

HRESULT CRun_Player_Begin::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


    m_iStateChangeKeyFrame = 21;

    m_fInterPolationTime = 0.f;


	m_fMyAccel = 10.f;
	m_fAnimSpeed = 4.f;

	m_iStateChangeKeyFrame = 21;

	m_fInterPolationTime = 0.1f;

	m_fMaxSpeed = 4.f;


    return S_OK;
}

void CRun_Player_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	Physics_Setting(m_fMaxSpeed, pOwner, false);


	//CTransform* pMyTransform = pOwner->Get_Transform();
	//CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	//vCamLook.y = 0.f;

	////1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	//pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	////실제 움직이는 방향
	//pMyPhysicsCom->Set_Dir(vCamLook);

	////최대속도 설정
	//pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunBeginSpeed);

	//bAnimTrigger = false;

	Add_KeyFrame(5, 0);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Player_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
        return STATE_SPRINT_BEGIN_PLAYER;

	_uint iDirection = Get_Direction();

	Change_Location_Begin(iDirection, pAnimator);


	Move(iDirection, pOwner);



    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_Player_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    // 걸어간다.
    if (KEY(W, HOLD) ||
        KEY(A, HOLD) ||
        KEY(S, HOLD) ||
        KEY(D, HOLD))
    {

        return m_eStateType;
    }


    return STATE_END;
}


void CRun_Player_Begin::Change_Location_Begin(_uint iDirection, CAnimator* pAnimator)
{
	if (!m_bMoveTrigger)
	{
		_int iFrame = pAnimator->Get_CurAnimFrame();

		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];

		pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
		pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);

		pAnimator->Set_CurFrame(iFrame);
	}


	m_bMoveTrigger = true;
}

void CRun_Player_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
	{
		CTransform* pMyTransform = pOwner->Get_Transform();
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

		//임시
		pMyPhysicsCom->Get_Physics().bAir = false;

		_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vCamLook.y = 0.f;

		//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
		pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

		//실제 움직이는 방향
		pMyPhysicsCom->Set_Dir(vCamLook);

		//최대속도 설정
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunBeginSpeed);

	}


	default:
		break;
	}


}