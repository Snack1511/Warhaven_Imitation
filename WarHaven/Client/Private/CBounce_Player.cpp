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

    return S_OK;
}

void CBounce_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitPos());
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());

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
	_uint iDirection = Get_Direction();  
	
	if(iDirection == STATE_DIRECTION_N || iDirection == STATE_DIRECTION_S)
		Move(iDirection, pOwner);
            
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CBounce_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. ������ ����� ��     
	*/

	if (pOwner->Is_Weapon_R_Collision())
		return m_eStateType;


    return STATE_END;
}

