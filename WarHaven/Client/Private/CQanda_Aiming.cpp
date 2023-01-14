#include "stdafx.h"
#include "CQanda_Aiming.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"

#include "CAnimWeapon.h"
#include "HIerarchyNode.h"
#include "CProjectile.h"
#include "CUnit_QANDA.h"

#include "CRectEffects.h"
#include "CAnimWeapon_Crow.h"

CQanda_Aiming::CQanda_Aiming()
{
}

CQanda_Aiming::~CQanda_Aiming()
{
}

CQanda_Aiming* CQanda_Aiming::Create()
{
	CQanda_Aiming* pInstance = new CQanda_Aiming();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_Aiming");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_Aiming::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 6;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_AIMING_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 1.5f;
	m_iStateChangeKeyFrame = 999;

	return __super::Initialize();
}

void CQanda_Aiming::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType, CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_QANDA);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
	static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_Charge();

	static_cast<CUnit_Qanda*>(pOwner)->Turn_FeatherEffect(false);
	static_cast<CUnit_Qanda*>(pOwner)->Turn_SteamEffect(true);
}

STATE_TYPE CQanda_Aiming::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(LBUTTON, AWAY) || KEY(RBUTTON, TAP))
	{
		static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_Shoot();
		m_bShoot = true;
		return STATE_ATTACK_SHOOT_QANDA;
	}

	if (m_fSndTime <= 0.f)
		m_iSndIdx = CFunctor::Play_LoopSound(L"Effect_Charge01_Qanda", CHANNEL_EFFECTS);


	m_fSndTime += fDT(0);
	if (m_fSndTime >= 3.f)
		m_fSndTime = 0.f;

	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Aiming::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);

		GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_EFFECTS, m_iSndIdx);

	if (!m_bShoot)
		static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->ChangeColor_End();


	if (!m_CrowSteam.empty())
	{
		for (auto& elem : m_CrowSteam)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_CrowSteam.clear();
	}
}

STATE_TYPE CQanda_Aiming::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

void CQanda_Aiming::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
