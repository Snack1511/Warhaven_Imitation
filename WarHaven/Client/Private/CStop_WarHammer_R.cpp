#include "stdafx.h"
#include "CStop_WarHammer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_WarHammer_R::CStop_WarHammer_R()
{
}

CStop_WarHammer_R::~CStop_WarHammer_R()
{
}

CStop_WarHammer_R* CStop_WarHammer_R::Create()
{
	CStop_WarHammer_R* pInstance = new CStop_WarHammer_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_WarHammer_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_WarHammer_R::Initialize()
{
	__super::Initialize();

	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 35;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 34;

	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 35;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 35;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 35;

	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 36;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 36;

	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 37;

	return S_OK;
}

void CStop_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		//return STATE_RUN_BEGIN_WARHAMMER_R;


	return __super::Tick(pOwner, pAnimator);
}

void CStop_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CStop_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* WARHAMMER가 Attack 으로 오는 조건
	1.  Run 에서 WASD 키를 누르지 않았다면
	*/


	//return STATE_END;
}

void CStop_WarHammer_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
