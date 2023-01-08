#include "stdafx.h"
#include "CState_Common_ChangeHero_AI.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Priest.h"

#include "CPlayer.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_ChangeHero_AI::CState_Common_ChangeHero_AI()
{
}

CState_Common_ChangeHero_AI::~CState_Common_ChangeHero_AI()
{
}

CState_Common_ChangeHero_AI* CState_Common_ChangeHero_AI::Create()
{
	CState_Common_ChangeHero_AI* pInstance = new CState_Common_ChangeHero_AI();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Common_ChangeHero_AI");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Common_ChangeHero_AI::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMMON_CHANGE_HERO;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fAnimSpeed = 1.5f;

	m_iStateChangeKeyFrame = 99;

	return S_OK;
}

void CState_Common_ChangeHero_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_iRand = random(0, 3);

	CPlayer* pPlayer = pOwner->Get_OwnerPlayer();

	CLASS_TYPE eDefaultType = pPlayer->Get_CurClass();

	m_iAnimIndex = pPlayer->Get_ChangeHeroIndex(eDefaultType);

	/* 히트박스 끄기 */
	pOwner->Enable_HitBoxColliders(false);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinFlare", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));//henshin flare


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_ChangeHero_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();

	pOwner->Shake_Camera(1.f, 0.1f);

	if (pAnimator->Is_CurAnimFinished())
	{
		switch (m_iRand)
		{
		case 0:
			m_eChangeClassType = FIONA;
			break;
		default:
			m_eChangeClassType = FIONA;
			break;
		}

		Set_HeroType(pOwner, m_eChangeClassType);

		return STATE_END;
	}

	return __super::Tick(pOwner, pAnimator);
}

void CState_Common_ChangeHero_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_HitBoxColliders(true);
}

STATE_TYPE CState_Common_ChangeHero_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return STATE_END;
}

STATE_TYPE CState_Common_ChangeHero_AI::Set_HeroType(CUnit* pOwner, CLASS_TYPE eClass)
{
	//henshin particle
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinParticle", pOwner->Get_Transform()->Get_World(WORLD_POS));//henshin flare

	_uint iIndex = (_uint)eClass;
	pOwner->On_ChangeToHero(iIndex);


	return m_eStateType;
}
