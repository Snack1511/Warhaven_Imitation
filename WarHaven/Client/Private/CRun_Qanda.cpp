#include "stdafx.h"
#include "CRun_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"

#include "CAnimWeapon_Crow.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Qanda::CRun_Qanda()
{
}

CRun_Qanda::~CRun_Qanda()
{
}

CRun_Qanda* CRun_Qanda::Create()
{
    CRun_Qanda* pInstance = new CRun_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Qanda::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 10.f;


    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_QANDA);
    m_vecAdjState.push_back(STATE_JUMP_QANDA);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_QANDA);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_QANDA);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

    m_vecAdjState.push_back(STATE_GUARD_QANDA);
    m_vecAdjState.push_back(STATE_SHADOWSTEP_QANDA);

    /*m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);*/


   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 8;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 9;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 10;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 11;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 12;

   m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

   Init_CommonState_Hero_Player();

    return S_OK;
}

void CRun_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

    if (pAnimCrow->Get_Phase() == CAnimWeapon_Crow::ePhyxState::eIDLE)
    {
        static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->Set_PhiysicsSpeed(m_fMaxSpeed);
    }
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    _uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.05f);

    if (iDirection < 8)
        m_iCurDirection = iDirection;


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_QANDA;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        _uint* pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_QANDA, (void*)pInt);
        return STATE_END;

    }

    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(C, NONE))
    {
        // 걸어간다.
        if (KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD))
        {

            return m_eStateType;
        }

    }

    return STATE_END;
}
