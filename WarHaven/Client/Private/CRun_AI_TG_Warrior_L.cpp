#include "stdafx.h"
#include "CRun_AI_TG_Warrior_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_AI_TG_Warrior_L::CRun_AI_TG_Warrior_L()
{
}

CRun_AI_TG_Warrior_L::~CRun_AI_TG_Warrior_L()
{
}

CRun_AI_TG_Warrior_L* CRun_AI_TG_Warrior_L::Create()
{
    CRun_AI_TG_Warrior_L* pInstance = new CRun_AI_TG_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_AI_TG_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_AI_TG_Warrior_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 22;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_RUN_WARRIOR_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);

	//m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);

    // 15

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;


    return S_OK;
}

void CRun_AI_TG_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */
  /*  CColorController::COLORDESC m_tColorDesc;
    ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

    m_tColorDesc.eFadeStyle = CColorController::TIME;
    m_tColorDesc.fFadeInStartTime = 1.f;
    m_tColorDesc.fFadeInTime = 1.f;
    m_tColorDesc.fFadeOutStartTime = 1.f;
    m_tColorDesc.fFadeOutTime = 1.f;

    m_tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 0.f);

    m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    m_tColorDesc.iStartKeyFrame = 3;
    m_tColorDesc.iEndKeyFrame = 6;


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);*/



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_AI_TG_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand == 0)
        return AI_STATE_WALK_WARRIOR_L;

    if (m_bAIMove)
        return AI_STATE_RUNBEGIN_WARRIOR_R;

    CUnit* pUnit = pOwner->Get_TargetUnit();

    pOwner->Set_LookToTarget();



    _float4 vLook = (pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS));
    _float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    _float fLength = vLook.Length();

    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    vLook.Normalize();
    vRight.Normalize();

    pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);


    pMyPhysicsCom->Set_Accel(m_fMyAccel);

    if (fLength < 2.f)
    {
        _int iRand = rand() % 20;

        if (iRand < 15)
            return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

        else
            return AI_STATE_GUARD_BEGIN_WARRIOR;

        //switch (iRand)
        //{

        //case 0:
        //case 1:

        //    return AI_STATE_ATTACK_HORIZONTALMIDDLE_L;

        //case 2:

        //    return AI_STATE_GUARD_BEGIN_WARRIOR;

        //default:
        //    break;
        //}
    }


    //if (fLength > 6.f)
    //{
        if (!vLook.Is_Zero())
            pMyPhysicsCom->Set_Dir(vLook);


        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);

    //}
    //else
    //{

    //    _float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //    _float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);


    //    if (vRight.z > 0.9f)
    //    {
    //        if ((!vLook.Is_Zero()))
    //            pMyPhysicsCom->Set_Dir(vLook);

    //        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);
    //    }
    //}



    return __super::Tick(pOwner, pAnimator);
}

void CRun_AI_TG_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_AI_TG_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    2. WASD 를 누른 상태
    */

	return __super::Check_Condition(pOwner, pAnimator);

	//if (KEY(CTRL, NONE))
	//{
	//	if (KEY(W, HOLD) ||
	//		KEY(A, HOLD) ||
	//		KEY(S, HOLD) ||
	//		KEY(D, HOLD))
	//	{
	//		return m_eStateType;
	//	}
	//}


    return STATE_END;
}
