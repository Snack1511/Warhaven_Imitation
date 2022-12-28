#include "stdafx.h"
#include "CStop_Priest.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_Priest::CStop_Priest()
{
}

CStop_Priest::~CStop_Priest()
{
}

CStop_Priest* CStop_Priest::Create()
{
	CStop_Priest* pInstance = new CStop_Priest();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_Priest");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_Priest::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);

	Init_AttackState_Priest();
	Init_CommonState_Player();

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 25;

	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 26;

	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 27;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 27;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 27;

	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 28;


	return S_OK;
}

void CStop_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	if (pData)
	{
		_uint* pDirection = (_uint*)(pData);
		if (*pDirection >= STATE_DIRECTION_END)
			*pDirection = STATE_DIRECTION_N;

		m_iAnimIndex = m_iDirectionAnimIndex[*pDirection];



		delete pDirection;
	}


	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CStop_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CStop_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE)
		)
	{
		return m_eStateType;
	}

	return STATE_END;
}

