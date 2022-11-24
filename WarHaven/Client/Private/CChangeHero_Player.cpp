#include "stdafx.h"
#include "CChangeHero_Player.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CPlayer.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CChangeHero_Player::CChangeHero_Player()
{
}

CChangeHero_Player::~CChangeHero_Player()
{
}

CChangeHero_Player* CChangeHero_Player::Create()
{
	CChangeHero_Player* pInstance = new CChangeHero_Player();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CChangeHero_Player");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CChangeHero_Player::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHANGE_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 23;



	return S_OK;
}

void CChangeHero_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();

	CUnit::CLASS_TYPE eDefaultType = pOwner->Get_Status().eClass;

	m_iAnimIndex = pPlayer->Get_ChangeHeroIndex(eDefaultType);


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CChangeHero_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();


	if (pAnimator->Is_CurAnimFinished())
	{
		switch (pOwner->Get_HeroType())
		{
		case CUnit::FIONA:

			pPlayer->Change_HeroUnit(CPlayer::CLASS_HREO_FIONA);
			return STATE_IDLE_VALKYRIE_R;

		case CUnit::QANDA:

			pPlayer->Change_HeroUnit(CPlayer::CLASS_HREO_FIONA);
			return STATE_IDLE_VALKYRIE_R;

		case CUnit::HOEDT:

			pPlayer->Change_HeroUnit(CPlayer::CLASS_HREO_FIONA);
			return STATE_IDLE_VALKYRIE_R;

		case CUnit::LANCER:

			pPlayer->Change_HeroUnit(CPlayer::CLASS_HREO_FIONA);
			return STATE_IDLE_VALKYRIE_R;

		default:
			Call_MsgBox(L"변신 상태 안됨.");
			break;
		}
	}


	return __super::Tick(pOwner, pAnimator);
}

void CChangeHero_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CChangeHero_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	_bool bReturn = false;

	//if (pOwner->Get_Status().bAbleHero)
	{
		if (KEY(NUM1, TAP))
		{
			pOwner->Get_HeroType() = CUnit::FIONA;
			return m_eStateType;
		}
		else if (KEY(NUM2, TAP))
		{
			pOwner->Get_HeroType() = CUnit::QANDA;
			return m_eStateType;
		}

		else if (KEY(NUM3, TAP))
		{
			pOwner->Get_HeroType() = CUnit::HOEDT;
			return m_eStateType;
		}
		else if (KEY(NUM4, TAP))
		{
			pOwner->Get_HeroType() = CUnit::LANCER;
			return m_eStateType;
		}
	}


	return STATE_END;
}
