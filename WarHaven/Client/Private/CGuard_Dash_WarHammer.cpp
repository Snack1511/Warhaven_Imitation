#include "stdafx.h"
#include "CGuard_Dash_WarHammer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Dash_WarHammer::CGuard_Dash_WarHammer()
{
}

CGuard_Dash_WarHammer::~CGuard_Dash_WarHammer()
{
}

CGuard_Dash_WarHammer* CGuard_Dash_WarHammer::Create()
{
    CGuard_Dash_WarHammer* pInstance = new CGuard_Dash_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Dash_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Dash_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARDDASH_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 37;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_WARHAMMER);

	m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	Init_CommonState_Player();


    return S_OK;
}

void CGuard_Dash_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	Physics_Setting(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Dash_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CGuard_Dash_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Dash_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

   
    return STATE_END;
}
