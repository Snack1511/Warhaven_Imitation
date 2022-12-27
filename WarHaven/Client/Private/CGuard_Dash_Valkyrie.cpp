#include "stdafx.h"
#include "CGuard_Dash_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Dash_Valkyrie::CGuard_Dash_Valkyrie()
{
}

CGuard_Dash_Valkyrie::~CGuard_Dash_Valkyrie()
{
}

CGuard_Dash_Valkyrie* CGuard_Dash_Valkyrie::Create()
{
	CGuard_Dash_Valkyrie* pInstance = new CGuard_Dash_Valkyrie();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CGuard_Dash_Valkyrie");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CGuard_Dash_Valkyrie::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 48;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARDDASH_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 30;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_VALKYRIE);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);

	Init_CommonState_Hero_Player();

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 48;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 49;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 50;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 51;

	return S_OK;
}

void CGuard_Dash_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

	if (KEY(W, TAP))
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];
		Physics_Setting(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, false);
	}
	else if (KEY(A, TAP))
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];
		Physics_Setting_Right(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, false);
	}
	else if (KEY(S, TAP))
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_S];
		Physics_Setting(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, true);
	}
	else if (KEY(D, TAP))
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];
		Physics_Setting_Right(pOwner->Get_Status().fGuardDashSpeed, pOwner, true, true);
	}


	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Dash_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);

	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_VALKYRIE_R;

    return __super::Tick(pOwner, pAnimator);
}

void CGuard_Dash_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Dash_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(W, TAP) || KEY(A, TAP) || KEY(S, TAP) || KEY(D, TAP))
		return m_eStateType;


    return STATE_END;
}
