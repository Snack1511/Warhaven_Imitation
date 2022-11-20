#include "stdafx.h"
#include "CSprint_Begin_Valkyrie.h"

#include  "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Begin_Valkyrie::CSprint_Begin_Valkyrie()
{
}

CSprint_Begin_Valkyrie::~CSprint_Begin_Valkyrie()
{
}

CSprint_Begin_Valkyrie* CSprint_Begin_Valkyrie::Create()
{
    CSprint_Begin_Valkyrie* pInstance = new CSprint_Begin_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Begin_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Begin_Valkyrie::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 45;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_BEGIN_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.F;

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.15f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_SPRINT_LOOP_VALKYRIE);
    m_vecAdjState.push_back(STATE_SPRINT_END_VALKYRIE);
    m_vecAdjState.push_back(STATE_SPRINTATTACK_BEGIN_VALKYRIE);

    

    m_vecAdjState.push_back(STATE_SPRINT_JUMP_VALKYRIE);
//	m_vecAdjState.push_back(STATE_SPRINTATTACK_BEGIN_VALKYRIE);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    //m_vecAdjState.push_back(STATE_VERTICALATTACK_VALKYRIE_R);

    //m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);
    //m_vecAdjState.push_back(STATE_GROGGYATTACK_VALKYRIE);
    //m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_VALKYRIE);
    //m_vecAdjState.push_back(STATE_INSTALL_BEIGN_VALKYRIE);


	Add_KeyFrame(10, 0);

    return S_OK;
}

void CSprint_Begin_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	Physics_Setting(pOwner->Get_Status().fSprintSpeed * 0.8f, pOwner, false);


	if (ePrevType == STATE_RUNBEGIN_VALKYRIE_L || ePrevType == STATE_RUNBEGIN_VALKYRIE_R)
	{
		m_fInterPolationTime = 0.f;
	}
	else
	{
		m_fInterPolationTime = 0.1f;
	}






    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Begin_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_SPRINT_JUMPFALL_VALKYRIE;

    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    _float4 vCamLook = pOwner->Get_FollowCamLook();
    vCamLook.y = 0.f;
    _float4 vCamRight = pOwner->Get_FollowCamRight();
    vCamRight.y = 0.f;

    _float4 vDir = pOwner->Get_FollowCamLook();


    _uint iFrame = pAnimator->Get_CurAnimFrame();

    _float4 vFinalDir = ZERO_VECTOR;

    if (KEY(W, HOLD))
    {
        vFinalDir += vCamLook;
    }
    if (KEY(A, HOLD))
    {
        vFinalDir -= vCamRight;
    }
    if (KEY(S, HOLD))
    {
        vFinalDir -= vCamLook;
    }
    if (KEY(D, HOLD))
    {
        vFinalDir += vCamRight;
    }

    if (vFinalDir.Is_Zero())
    {
        vFinalDir = pMyTransform->Get_World(WORLD_LOOK);
    }



    vFinalDir.Normalize();
    vDir.y = 0.f;

    pMyTransform->Set_LerpLook(vFinalDir, m_fMyMaxLerp);
    _float4 vUnitLook = pMyTransform->Get_MyWorld(WORLD_LOOK);
    pMyPhysicsCom->Set_Dir(vUnitLook);
    pMyPhysicsCom->Set_Accel(m_fMyAccel);



    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Begin_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	//pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CSprint_Begin_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 움직인다.
    */

    if (KEY(LSHIFT, HOLD))
    {
        if (
            KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD)
            )
        {
            return m_eStateType;
        }
    }


    return STATE_END;
}

void CSprint_Begin_Valkyrie::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
	{

		//CTransform* pMyTransform = pOwner->Get_Transform();
		//CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


		//m_fMyAccel = 10.f;
		//pMyPhysicsCom->Set_Accel(m_fMyAccel);

	}


	default:
		break;
	}


}
