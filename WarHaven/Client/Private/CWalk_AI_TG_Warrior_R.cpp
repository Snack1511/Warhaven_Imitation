#include "stdafx.h"
#include "CWalk_AI_TG_Warrior_R.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_AI_TG_Warrior_R::CWalk_AI_TG_Warrior_R()
{
}

CWalk_AI_TG_Warrior_R::~CWalk_AI_TG_Warrior_R()
{
}

CWalk_AI_TG_Warrior_R* CWalk_AI_TG_Warrior_R::Create()
{
    CWalk_AI_TG_Warrior_R* pInstance = new CWalk_AI_TG_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_AI_TG_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_AI_TG_Warrior_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_WALK_WARRIOR_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.

 //   m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	//m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);
 //   m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);


//    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


	// 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;

                            



    return S_OK;
}

void CWalk_AI_TG_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */
    

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_AI_TG_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CUnit* pUnit = pOwner->Get_TargetUnit();

    _float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);

    _float fLength = vLook.Length();

    if (fLength < 2.5f)
        return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;


    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_iRand == 0)
            return AI_STATE_RUN_WARRIOR_R;
        else
            --m_iRand;
    }

    //if (m_bAIMove)
    //    return AI_STATE_RUNBEGIN_WARRIOR_L;



    //CUnit* pUnit = pOwner->Get_TargetUnit();



    //CTransform* pMyTransform = pOwner->Get_Transform();

    //_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
    //_float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    //_float fLength = vLook.Length();

    //CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    //vLook.Normalize();
    //vRight.Normalize();

    //pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

    //pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

    //pMyPhysicsCom->Set_Accel(m_fMyAccel);

    //if (fLength < 2.f)
    //{
    //    _int iRand = rand() % 20;

    //    if (iRand < 15)
    //        return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

    //    else
    //        return AI_STATE_GUARD_BEGIN_WARRIOR;

    //}


    //if (fLength > 6.f)
    //{
    //    if (!vLook.Is_Zero())
    //        pMyPhysicsCom->Set_Dir(vLook);


    //    Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);

    //}
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
    //    else
    //    {
    //        if (!vLook.Is_Zero())
    //            pMyPhysicsCom->Set_Dir(vDir);

    //        Change_Location_Loop(STATE_DIRECTION_NW, pAnimator, 0.1f);
    //    }
    //}



    return __super::Tick(pOwner, pAnimator);
}

void CWalk_AI_TG_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_AI_TG_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
