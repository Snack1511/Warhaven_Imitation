#include "stdafx.h"
#include "CQanda_Attack_Begin_Sniping.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CQanda_Attack_Begin_Sniping::CQanda_Attack_Begin_Sniping()
{
}

CQanda_Attack_Begin_Sniping::~CQanda_Attack_Begin_Sniping()
{
}

CQanda_Attack_Begin_Sniping* CQanda_Attack_Begin_Sniping::Create()
{
	CQanda_Attack_Begin_Sniping* pInstance = new CQanda_Attack_Begin_Sniping();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_Attack_Begin_Sniping");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_Attack_Begin_Sniping::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_BEGIN_SNIPING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 3.5f;

	m_vecAdjState.push_back(STATE_ATTACK_SHOOT_SNIPING_QANDA);


	return S_OK;
}

void CQanda_Attack_Begin_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->On_Use(CUnit::SKILL2);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CQanda_Attack_Begin_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Attack_Begin_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CQanda_Attack_Begin_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (!pOwner->Can_Use(CUnit::SKILL2))
		return STATE_END;

	if (KEY(Q, HOLD))
		return m_eStateType;


	return STATE_END;
}

void CQanda_Attack_Begin_Sniping::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{


}
