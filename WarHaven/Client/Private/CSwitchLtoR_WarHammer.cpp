#include "stdafx.h"
#include "CSwitchLtoR_WarHammer.h"

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

CSwitchLtoR_WarHammer::CSwitchLtoR_WarHammer()
{
}

CSwitchLtoR_WarHammer::~CSwitchLtoR_WarHammer()
{
}

CSwitchLtoR_WarHammer* CSwitchLtoR_WarHammer::Create()
{
    CSwitchLtoR_WarHammer* pInstance = new CSwitchLtoR_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchLtoR_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchLtoR_WarHammer::Initialize()
{
    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 28;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_L_TO_R_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);



    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);


    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


    return S_OK;
}

void CSwitchLtoR_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    




    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchLtoR_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
        return STATE_CHARGE_FRONT_WARHAMMER_R;

    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return STATE_VERTICALATTACK_WARHAMMER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchLtoR_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSwitchLtoR_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::TAB)
        return m_eStateType;

    return STATE_END;
}
