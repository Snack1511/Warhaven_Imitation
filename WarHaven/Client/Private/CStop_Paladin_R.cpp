#include "stdafx.h"
#include "CStop_Paladin_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_Paladin_R::CStop_Paladin_R()
{
}

CStop_Paladin_R::~CStop_Paladin_R()
{
}

CStop_Paladin_R* CStop_Paladin_R::Create()
{
	CStop_Paladin_R* pInstance = new CStop_Paladin_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_Paladin_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_Paladin_R::Initialize()
{
	__super::Initialize();

	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_PALADIN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 23;

	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 24;

	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 25;

	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 26;

	return S_OK;
}

void CStop_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		//return STATE_RUN_BEGIN_VALKYRIE_R;


	return __super::Tick(pOwner, pAnimator);
}

void CStop_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CStop_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* VALKYRIE가 Attack 으로 오는 조건
	1.  Run 에서 WASD 키를 누르지 않았다면
	*/


	//return STATE_END;
}

void CStop_Paladin_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
