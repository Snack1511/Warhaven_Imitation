#include "stdafx.h"
#include "CRun_WarHammer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_WarHammer_L::CRun_WarHammer_L()
{
}

CRun_WarHammer_L::~CRun_WarHammer_L()
{
}

CRun_WarHammer_L* CRun_WarHammer_L::Create()
{
    CRun_WarHammer_L* pInstance = new CRun_WarHammer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_WarHammer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_WarHammer_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 20;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);

    // 15

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 19;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 21;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 22;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 34;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 35;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 23;


    return S_OK;
}

void CRun_WarHammer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
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

STATE_TYPE CRun_WarHammer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_WARHAMMER_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        _uint*   pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_WARHAMMER_L, (void*)pInt);
        return STATE_END;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_WarHammer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_WarHammer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Walk로 오는 조건
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
