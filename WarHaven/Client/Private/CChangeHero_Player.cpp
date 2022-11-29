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
	m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 24;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHANGE_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fAnimSpeed = 1.5f;

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
	CPlayer* pPlayer = pOwner->Get_OwnerPlayer();


	CPlayer::CLASS_DEFAULT eDefaultType = (CPlayer::CLASS_DEFAULT)pPlayer->Get_CurrentDefaultClass();

	m_iAnimIndex = pPlayer->Get_ChangeHeroIndex(eDefaultType);

	GAMEINSTANCE->Start_RadialBlur(0.015f);

	pOwner->Enable_HitBoxColliders(false);

	/* Owner�� Animator Set Idle�� */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CChangeHero_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();

	pOwner->Shake_Camera(1.5f, 0.1f);

	if (pAnimator->Is_CurAnimFinished())
	{
		Set_HeroType(pOwner, m_eChangeClassType);
		pOwner->Shake_Camera(10.f, 1.f);
		GAMEINSTANCE->Stop_RadialBlur();

		return STATE_END;
	}


	return __super::Tick(pOwner, pAnimator);
}

void CChangeHero_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_HitBoxColliders(true);
}

STATE_TYPE CChangeHero_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	_bool bReturn = false;

	if (pOwner->Get_OwnerPlayer()->AbleHero())
	{

		if (KEY(NUM1, TAP))
		{
			m_eChangeClassType = FIONA;
			return m_eStateType;

		}
		else if (KEY(NUM2, TAP))
		{
			m_eChangeClassType = QANDA;
			return m_eStateType;
		}
		else if (KEY(NUM3, TAP))
		{
			m_eChangeClassType = HOEDT;
			return m_eStateType;
		}
		else if (KEY(NUM4, TAP))
		{
			m_eChangeClassType = LANCER;
			return m_eStateType;
		}


	}
	/*else
	{
		if (pOwner->Get_OwnerPlayer()->IsHero())
		{
			if (KEY(NUM1, TAP))
			{
				pOwner->Get_OwnerPlayer()->IsHero() = false;

				CUser::Get_Instance()->Set_HUD((CLASS_TYPE)pOwner->Get_OwnerPlayer()->Get_CurrentDefaultClass());
			}
		}
	}*/

	return STATE_END;
}

STATE_TYPE CChangeHero_Player::Set_HeroType(CUnit* pOwner, CLASS_TYPE eClass)
{
	pOwner->Get_OwnerPlayer()->IsHero() = true;
	pOwner->On_ChangeToHero((CPlayer::CLASS_HREO)eClass);

	CUser::Get_Instance()->Set_HUD(eClass);

	return m_eStateType;
}
