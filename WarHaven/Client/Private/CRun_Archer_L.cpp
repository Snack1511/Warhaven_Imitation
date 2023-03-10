#include "stdafx.h"
#include "CRun_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Archer_L::CRun_Archer_L()
{
}

CRun_Archer_L::~CRun_Archer_L()
{
}

CRun_Archer_L* CRun_Archer_L::Create()
{
    CRun_Archer_L* pInstance = new CRun_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Archer_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_ARCHER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_L);
    m_vecAdjState.push_back(STATE_JUMP_ARCHER_L);
   /* m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);


	m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);*/

    // 15

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 21;


    return S_OK;
}

void CRun_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
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


    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(m_tColorDesc);*/



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_ARCHER_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        _uint*   pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_ARCHER_L, (void*)pInt);
        return STATE_END;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Walk로 오는 조건
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
