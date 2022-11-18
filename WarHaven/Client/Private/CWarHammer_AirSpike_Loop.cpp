#include "stdafx.h"
#include "CWarHammer_AirSpike_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_AirSpike_Loop::CWarHammer_AirSpike_Loop()
{
}

CWarHammer_AirSpike_Loop::~CWarHammer_AirSpike_Loop()
{
}

CWarHammer_AirSpike_Loop* CWarHammer_AirSpike_Loop::Create()
{
	CWarHammer_AirSpike_Loop* pInstance = new CWarHammer_AirSpike_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_AirSpike_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_AirSpike_Loop::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_AIRSPIKE_LOOP_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);
	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	m_vecAdjState.push_back(STATE_AIRSPIKE_END_WARHAMMER);




	 // return __super::Initialize();

	return S_OK;
}

void CWarHammer_AirSpike_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;
	Physics_Setting(m_fMaxSpeed, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_AirSpike_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		return STATE_AIRSPIKE_END_WARHAMMER;
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_AirSpike_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWarHammer_AirSpike_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Loop로 오는 조건
	1.  Begin 이 끝났을 경우.
	*/
	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}
