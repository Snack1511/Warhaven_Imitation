#include "stdafx.h"
#include "CBounce_Valkyrie.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CBounce_Valkyrie::CBounce_Valkyrie()
{
}

CBounce_Valkyrie::~CBounce_Valkyrie()
{
}

HRESULT CBounce_Valkyrie::Initialize()
{
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 5.f;
	m_fMaxSpeed = 2.f;
	
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

    return S_OK;
}

void CBounce_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


	/* Shake */
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

    /* Owner의 Animator Set Idle로 */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitPos());

	pOwner->Get_PhysicsCom()->Set_Speed(0.f);

	switch (ePrevType)
	{
	case Client::STATE_ATTACK_HORIZONTALUP_VALKYRIE_L:
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L:
	case Client::STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L:


		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 0;    
		m_eStateType = STATE_BOUNCE_VALKYRIE_L;

		break;

	case Client::STATE_SPRINTATTACK_VALKYRIE:
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 37;
		m_eStateType = STATE_BOUNCE_VALKYRIE_L;
		
		break;

	case Client::STATE_ATTACK_HORIZONTALUP_VALKYRIE_R:
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R:
	case Client::STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R:

		m_eAnimType = ANIM_BASE_R;
		m_iAnimIndex = 0;
		m_eStateType = STATE_BOUNCE_VALKYRIE_R;

		break;

	//case Client::STATE_ATTACK_VERTICALCUT:


	//	m_eAnimType = ANIM_ATTACK;
	//	m_iAnimIndex = 19;

	//	break;

	default:
		break;
	}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Sound(L"Effect_Bounce");
}

STATE_TYPE CBounce_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 공격이 닿았을 시     
	*/

	if (pOwner->Is_Weapon_R_Collision())
		return m_eStateType;


    return STATE_END;
}

