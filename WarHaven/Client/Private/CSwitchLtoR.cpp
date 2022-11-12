#include "stdafx.h"
#include "CSwitchLtoR.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CSwitchLtoR::CSwitchLtoR()
{
}

CSwitchLtoR::~CSwitchLtoR()
{
}

CSwitchLtoR* CSwitchLtoR::Create()
{
    CSwitchLtoR* pInstance = new CSwitchLtoR();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchLtoR");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchLtoR::Initialize()
{
    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 27;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_L_TO_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 31;

    // 선형 보간 시간
    m_fInterPolationTime = 0.2f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 4.2f;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_ATTACK_UPPER_MIDDLE_PLAYER_R);

    return S_OK;
}

void CSwitchLtoR::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    




    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSwitchLtoR::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_UPPER_MIDDLE_PLAYER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchLtoR::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSwitchLtoR::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::R)
        return m_eStateType;

    return STATE_END;
}
