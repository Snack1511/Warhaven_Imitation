#include "stdafx.h"
#include "CStop_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_Qanda::CStop_Qanda()
{
}

CStop_Qanda::~CStop_Qanda()
{
}

CStop_Qanda* CStop_Qanda::Create()
{
	CStop_Qanda* pInstance = new CStop_Qanda();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_Qanda");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_Qanda::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_QANDA);
	m_vecAdjState.push_back(STATE_WALK_QANDA);
	m_vecAdjState.push_back(STATE_RUN_QANDA);
	m_vecAdjState.push_back(STATE_GUARD_QANDA);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);

	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 13;

	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 14;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 14;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 14;

	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 15;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 15;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 15;

	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 16;


	return S_OK;
}

void CStop_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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

STATE_TYPE CStop_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CStop_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CStop_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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

