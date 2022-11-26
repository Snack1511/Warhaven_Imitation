#include "stdafx.h"
#include "CRun_AI_TG_Warrior_R.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_AI_TG_Warrior_R::CRun_AI_TG_Warrior_R()
{
}

CRun_AI_TG_Warrior_R::~CRun_AI_TG_Warrior_R()
{
}

CRun_AI_TG_Warrior_R* CRun_AI_TG_Warrior_R::Create()
{
    CRun_AI_TG_Warrior_R* pInstance = new CRun_AI_TG_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_AI_TG_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_AI_TG_Warrior_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_RUN_WARRIOR_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복
   // m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 26;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 27;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 28;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 29;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 30;


    return S_OK;
}

void CRun_AI_TG_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_AI_TG_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand == 0)
        return AI_STATE_WALK_WARRIOR_R;

    if (m_bAIMove)
        return AI_STATE_RUNBEGIN_WARRIOR_L;



    CUnit* pUnit = pOwner->Get_TargetUnit();



    CTransform* pMyTransform = pOwner->Get_Transform();

    _float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    _float fLength = vLook.Length();

    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    vLook.Normalize();
    vRight.Normalize();

    pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

    pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

    pMyPhysicsCom->Set_Accel(m_fMyAccel);

    if (fLength < 2.f)
    {
        _int iRand = rand() % 20;

        if(iRand < 15)
            return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

        else
            return AI_STATE_GUARD_BEGIN_WARRIOR;

        //switch (iRand)
        //{

        //case 0:
        //case 1:
        //case 2:

        //    return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

        //case 3:

        //    return AI_STATE_GUARD_BEGIN_WARRIOR;

        //default:
        //    break;
        //}
    }
    



    //if (fLength < 2.f)
    //    return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

  //  if (fLength > 6.f)
  //  {
        if (!vLook.Is_Zero())
            pMyPhysicsCom->Set_Dir(vLook);
        

        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);

//    }
    //else
    //{

    //    _float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //    _float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);

    //    _float4 vDir = vLook + vRight * -1.f;

    //    if (vRight.z > 0.9f)
    //    {
    //        if ((!vLook.Is_Zero()))
    //            pMyPhysicsCom->Set_Dir(vLook);

    //        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);
    //    }
    //}







    //if (pOwner->Is_Air())
    //    return STATE_JUMPFALL_PLAYER_R;

    //if (
    //    KEY(W, NONE) &&
    //    KEY(A, NONE) &&
    //    KEY(S, NONE) &&
    //    KEY(D, NONE)
    //    )
    //{
    //    _uint* pInt = new _uint;
    //    *pInt = m_iCurDirection;
    //    pOwner->Enter_State(STATE_STOP_PLAYER_R, (void*)pInt);
    //    return STATE_END;

    //}

    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_AI_TG_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_AI_TG_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. CTRL 을 누르지 않은 상태
    2. WASD 를 누른 상태
    */

	return __super::Check_Condition(pOwner, pAnimator);
   
}
