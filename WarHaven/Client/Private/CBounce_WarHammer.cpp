#include "stdafx.h"
#include "CBounce_WarHammer.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CEffects_Factory.h"

CBounce_WarHammer::CBounce_WarHammer()
{
}

CBounce_WarHammer::~CBounce_WarHammer()
{
}

HRESULT CBounce_WarHammer::Initialize()
{
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 5.f;
	m_fMaxSpeed = 2.f;
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

    return S_OK;
}

void CBounce_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	pOwner->Get_Status().fCamPower = 0.4f;
	pOwner->Get_Status().fCamTime = 0.25f;

	/* Shake */
	pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

    /* Owner의 Animator Set Idle로 */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitPos());
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), pOwner->Get_HitMatrix());
	switch (ePrevType)
	{
	case Client::STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L:
	case Client::STATE_CHARGEATTACK_WARHAMMER_L:
	case Client::STATE_ATTACK_STING_WARHAMMER_L:

		m_iStateChangeKeyFrame = 74;
		m_eAnimType = ANIM_BASE_L;     
		m_iAnimIndex = 0;    
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

	case Client::STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R:
	case Client::STATE_CHARGEATTACK_WARHAMMER_R:
	case Client::STATE_ATTACK_STING_WARHAMMER_R:

		m_iStateChangeKeyFrame = 60;
		m_eAnimType = ANIM_BASE_R;
		m_iAnimIndex = 10;
		m_eStateType = STATE_BOUNCE_WARHAMMER_R;

		break;

		// RF
	case Client::STATE_VERTICALATTACK_WARHAMMER_R:
	case Client::STATE_CHARGEATTACK_FRONT_WARHAMMER_R:

		m_iStateChangeKeyFrame = 50;
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 38;
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

		//LF
	case Client::STATE_SPRINTATTACK_WARHAMMER:
	case Client::STATE_VERTICALATTACK_WARHAMMER_L:
	case Client::STATE_CHARGEATTACK_FRONT_WARHAMMER_L:
	
		m_iStateChangeKeyFrame = 50;
		m_eAnimType = ANIM_BASE_L;
		m_iAnimIndex = 38;
		m_eStateType = STATE_BOUNCE_WARHAMMER_L;

		break;

	default:
		break;
	}

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
            
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Walk로 오는 조건
    1. 공격이 닿았을 시     
	*/

	if (pOwner->Is_Weapon_R_Collision())
		return m_eStateType;


    return STATE_END;
}

