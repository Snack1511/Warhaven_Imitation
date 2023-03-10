#include "stdafx.h"
#include "CBounce_AI_TG_Warrior.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CBounce_AI_TG_Warrior::CBounce_AI_TG_Warrior()
{
}

CBounce_AI_TG_Warrior::~CBounce_AI_TG_Warrior()
{
}

HRESULT CBounce_AI_TG_Warrior::Initialize()
{
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 5.f;
	m_fMaxSpeed = 2.f;
	m_vecAdjState.push_back(AI_STATE_GUARD_BEGIN_WARRIOR);

    return S_OK;
}

void CBounce_AI_TG_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


	/* Shake */
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

    /* Owner?? Animator Set Idle?? */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitPos());

	pOwner->Get_PhysicsCom()->Set_Speed(0.f);


	switch (ePrevType)
	{
	case Client::AI_STATE_ATTACK_HORIZONTALMIDDLE_L:
	//case Client::STATE_ATTACK_HORIZONTALMIDDLE_L:
//	case Client::STATE_ATTACK_HORIZONTALDOWN_L:


		m_eAnimType = ANIM_ATTACK;     
		m_iAnimIndex = 16;    
		m_eStateType = AI_STATE_BOUNE_WARRIOR_L;

		break;

	case Client::STATE_SPRINTATTACK_PLAYER:
		m_eAnimType = ANIM_ATTACK;
		m_iAnimIndex = 17;
		break;

	//case Client::STATE_ATTACK_HORIZONTALUP_R:
	case Client::AI_STATE_ATTACK_HORIZONTALMIDDLE_R:
//	case Client::STATE_ATTACK_HORIZONTALDOWN_R:

		m_eAnimType = ANIM_ATTACK;
		m_iAnimIndex = 18;
		m_eStateType = AI_STATE_BOUNE_WARRIOR_R;

		break;

	//case Client::STATE_ATTACK_VERTICALCUT:


	//	m_eAnimType = ANIM_ATTACK;
	//	m_iAnimIndex = 19;

	//	break;

	default:
		break;
	}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_AI_TG_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_AI_TG_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ?Ұž??? */
}

STATE_TYPE CBounce_AI_TG_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player?? Walk?? ???? ????
    1. ?????? ?????? ??     
	*/

	//if (pOwner->Is_Weapon_R_Collision())
	//	return m_eStateType;


    return STATE_END;
}

