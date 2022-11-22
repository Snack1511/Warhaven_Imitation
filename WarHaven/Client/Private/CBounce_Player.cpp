#include "stdafx.h"
#include "CBounce_Player.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CBounce_Player::CBounce_Player()
{
}

CBounce_Player::~CBounce_Player()
{
}

HRESULT CBounce_Player::Initialize()
{
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 5.f;
	m_fMaxSpeed = 2.f;
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    return S_OK;
}

void CBounce_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	/* Shake */
	pOwner->Shake_Camera(0.3f, 0.25f);

    /* Owner의 Animator Set Idle로 */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitPos());

	pOwner->Get_PhysicsCom()->Set_Speed(0.f);

	switch (ePrevType)
	{
	case Client::STATE_ATTACK_HORIZONTALUP_L:
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_L:
	case Client::STATE_ATTACK_HORIZONTALDOWN_L:


		m_eAnimType = ANIM_ATTACK;     
		m_iAnimIndex = 16;    
		m_eStateType = STATE_BOUNCE_PLAYER_L;

		break;

	case Client::STATE_ATTACK_HORIZONTALUP_R:
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_R:
	case Client::STATE_ATTACK_HORIZONTALDOWN_R:

		m_eAnimType = ANIM_ATTACK;
		m_iAnimIndex = 18;
		m_eStateType = STATE_BOUNCE_PLAYER_R;

		break;
	case Client::STATE_ATTACK_VERTICALCUT:
	case Client::STATE_SPRINTATTACK_PLAYER:

		m_eAnimType = ANIM_ATTACK;
		m_iAnimIndex = 19;

		break;

	default:
		break;
	}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 공격이 닿았을 시     
	*/

	if (pOwner->Is_Weapon_R_Collision())
		return m_eStateType;


    return STATE_END;
}

