#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalDown_Fiona_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_Attack_HorizontalDown_Fiona_R::CState_Combat_Attack_HorizontalDown_Fiona_R()
{
}

CState_Combat_Attack_HorizontalDown_Fiona_R::~CState_Combat_Attack_HorizontalDown_Fiona_R()
{
}

CState_Combat_Attack_HorizontalDown_Fiona_R* CState_Combat_Attack_HorizontalDown_Fiona_R::Create()
{
	CState_Combat_Attack_HorizontalDown_Fiona_R* pInstance = new CState_Combat_Attack_HorizontalDown_Fiona_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_HorizontalDown_Fiona_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_Attack_HorizontalDown_Fiona_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_eBounceState = AI_STATE_COMMON_BOUNCE_FIONA_R;


	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalDown_Fiona_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	CColorController::COLORDESC m_tColorDesc;
	ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

	m_tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	m_tColorDesc.fFadeInStartTime = 0.f;
	m_tColorDesc.fFadeInTime = 0.5f;
	m_tColorDesc.fFadeOutStartTime = 0.5f;
	m_tColorDesc.fFadeOutTime = 0.02f;

	m_tColorDesc.vTargetColor = _float4((224.f / 255.f), (143.f / 255.f), (54.f / 255.f), 0.f);

	m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	m_tColorDesc.iStartKeyFrame = 3;
	m_tColorDesc.iEndKeyFrame = 4;


	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(m_tColorDesc);

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Fiona_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CState_Combat_Attack_HorizontalDown_Fiona_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalDown_Fiona_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
