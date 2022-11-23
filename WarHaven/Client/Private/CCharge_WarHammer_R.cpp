#include "stdafx.h"
#include "CCharge_WarHammer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CCharge_WarHammer_R::CCharge_WarHammer_R()
{
}

CCharge_WarHammer_R::~CCharge_WarHammer_R()
{
}

CCharge_WarHammer_R* CCharge_WarHammer_R::Create()
{
	CCharge_WarHammer_R* pInstance = new CCharge_WarHammer_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CCharge_WarHammer_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CCharge_WarHammer_R::Initialize()
{

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHARGE_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	/* Setting for Blendable */

	m_iChargeChangeKeyFrame = 10;

	m_eAnimLeftorRight = ANIM_BASE_R;
	m_iIdle_Index = 14;

	m_eWalkState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eJumpState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eLandState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eFallState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eRunState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eIdleState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eBounceState = STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R;
	m_eChargeAttackState = STATE_CHARGEATTACK_WARHAMMER_R;


	return __super::Initialize();
}

void CCharge_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fInterPolationTime = 1.f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CCharge_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CCharge_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CCharge_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{

		_float fDot = CUtility_Transform::Get_LookRotateAngle(pOwner->Get_FollowCamLook());
		if (fabs(fDot) > 0.96f)
		{
			m_eStateType = STATE_CHARGE_WARHAMMER_R;
		}
		else
		{
			m_eStateType = STATE_CHARGE_FRONT_WARHAMMER_R;
		}

		return m_eStateType;
	}

	return STATE_END;
	
	//return __super::Check_Condition(pOwner, pAnimator);
}

void CCharge_WarHammer_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
