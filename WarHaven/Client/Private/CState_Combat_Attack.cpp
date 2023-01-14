#include "stdafx.h"
#include "CState_Combat_Attack.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CState_Combat_Attack::CState_Combat_Attack()
{
}

CState_Combat_Attack::~CState_Combat_Attack()
{
}

HRESULT CState_Combat_Attack::Initialize()
{
    __super::Initialize();

    return S_OK;
}

void CState_Combat_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = m_iStopIndex; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Play_Voice(pOwner, L"Voice_Attack", m_fAIDeafultVolume);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}

