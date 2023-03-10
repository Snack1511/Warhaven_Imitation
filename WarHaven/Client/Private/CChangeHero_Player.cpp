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
#include "CPlayerInfo.h"


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

	m_fAnimSpeed = 1.5f;

	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 23;

	m_fAnimSpeed = 1.5f;

	return S_OK;
}

void CChangeHero_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CPlayer* pPlayer = pOwner->Get_OwnerPlayer();

	CLASS_TYPE eDefaultType = pPlayer->Get_CurClass();

	m_iAnimIndex = pPlayer->Get_ChangeHeroIndex(eDefaultType);

	// 히어로 변신 게이지 활성화
	CUser::Get_Instance()->SetActive_HeroTransformGauge(true);

	/*가운데 집중 블러, 색수차 키기*/
	GAMEINSTANCE->Start_RadialBlur(0.015f);
	GAMEINSTANCE->Start_ChromaticAberration(30.f);

	/* 히트박스 끄기 */
	pOwner->Enable_HitBoxColliders(false);
	
	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinFlare", pOwner, vPos);//henshin flare
	CFunctor::Play_Sound(L"Effect_OnChangeHero", CHANNEL_EFFECTS, vPos, 1.f);

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CChangeHero_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();

	pOwner->Shake_Camera(1.f, 0.1f);

	if (pAnimator->Is_CurAnimFinished())
	{
		Set_HeroType(pOwner, m_eChangeClassType);

		pOwner->Shake_Camera(8.f, 1.f);
		GAMEINSTANCE->Stop_RadialBlur();
		GAMEINSTANCE->Stop_ChromaticAberration();
		if (m_eChangeClassType == LANCER)
			CFunctor::Play_Sound(L"Effect_HorseStop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS), 1.f);

		_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

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

#ifdef FIONA_TH
			m_eChangeClassType = FIONA;
			return m_eStateType;
#endif // FIONA_TH

		
		}
		else if (KEY(NUM2, TAP))
		{

#ifdef QANDA_TH
			m_eChangeClassType = QANDA;
			return m_eStateType;
#endif // QANDA_TH

		}
		//else if (KEY(NUM3, TAP))
		//{
		//	m_eChangeClassType = HOEDT;
		//	return m_eStateType;
		//}
		else if (KEY(NUM4, TAP))
		{

#ifdef LANCER_TH
			m_eChangeClassType = LANCER;
			return m_eStateType;
#endif // LANCER_TH

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
	//henshin particle
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HenshinParticle", pOwner->Get_Transform()->Get_World(WORLD_POS));//henshin flare


	_uint iIndex = (_uint)eClass;
	pOwner->On_ChangeToHero(iIndex);

	Play_Voice(pOwner, L"Voice_Start", 1.f);

	return m_eStateType;
}
