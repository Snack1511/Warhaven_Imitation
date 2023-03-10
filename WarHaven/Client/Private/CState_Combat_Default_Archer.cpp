#include "stdafx.h"
#include "CState_Combat_Default_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Archer.h"

#include "CUser.h"

CState_Combat_Default_Archer::CState_Combat_Default_Archer()
{
}

CState_Combat_Default_Archer::~CState_Combat_Default_Archer()
{
}

HRESULT CState_Combat_Default_Archer::Initialize()
{
	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;
	m_fAIMyLength = 2.5f;

    return __super::Initialize();
}

void CState_Combat_Default_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == m_eStateType)
		m_fInterPolationTime = 0.f;

	m_iRand = random(0, 7);
	m_iDirectionRand = random(0, 7);
	m_fRand = frandom(0.2f, 0.5f);

	
	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if (pTargetUnit)
	{
		if (fabs(Get_TargetLook_Length(pOwner)) > m_fAIMyLength * 2.5f && pTargetUnit->Get_CurState() != AI_STATE_COMMON_CHANGE_HERO)
			Set_Direction_Front_AI(m_iDirectionRand);

		else
			Set_Direction_Back_AI(m_iDirectionRand);
	}
	else
		Set_Direction_Back_AI(m_iDirectionRand);



	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iStateChangeKeyFrame = 15;



	Physics_Setting_AI(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if(!pTargetUnit)
		return __super::Tick(pOwner, pAnimator);

	if (pTargetUnit->Get_Status().fHP <= 0.f)
	{
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;


		return __super::Tick(pOwner, pAnimator);
	}



	_uint iFrame = pAnimator->Get_CurAnimFrame();

	m_fTimeAcc += fDT(0);


	// 공격 코드


	if (fabs(Get_TargetLook_Length(pOwner)) > m_fAIMyLength)
	{

		switch (m_iRand)
		{
		case 0:
		case 1:
		case 2:


			if (m_fTimeAcc > m_fRand)
			{
				if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
				{
					pOwner->Get_SkillTrigger().bSkillQTrigger = false;
					return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;
				}

				else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
				{
					pOwner->Get_SkillTrigger().bSkillETrigger = false;
					return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;
				}

				else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
				{
					return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;
				}
				else
					return m_eStateType;
			}
			else 
			{
				if(pAnimator->Is_CurAnimFinished())
					return m_eStateType;
			}


			break;


		case 6:
		case 7:
		case 3:
		case 4:
		case 5:
			
			if(pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
				return m_eStateType;

		default:
			break;
		}
	}
	else
	{
		STATE_TYPE eStateType = Near_Enemy(pOwner, pAnimator);;
			
		if (eStateType != STATE_END)
			return eStateType;
	}

	DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Combat_Default_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Default_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}


STATE_TYPE CState_Combat_Default_Archer::Near_Enemy(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eArrowStateType = Choose_Arrow(pOwner);

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();


	if (!pTargetUnit)
	{
		if (pTargetUnit->Get_Status().fHP <= 40.f || m_iRand == 7)
		{
			if (pTargetUnit->Get_Status().fHP > 0.f)
			{
				if (eArrowStateType != STATE_END)
					return eArrowStateType;
			}

		}
	}

	switch (m_iRand)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	
		return AI_STATE_COMBAT_GUARDLOOP_ARCHER;

	case 4:
	case 5:
	case 6:

		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;

		break;

	default:

		if (eArrowStateType != STATE_END)
			return eArrowStateType;

		break;
	}

	return STATE_END;
}

STATE_TYPE CState_Combat_Default_Archer::Choose_Arrow(CUnit* pOwner)
{
	if (pOwner->Can_Use(CUnit::SKILL1))
		return AI_STATE_COMBAT_ATTACK_SWING_ARCHER;

	if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
	{
		pOwner->Get_SkillTrigger().bSkillQTrigger = false;
		return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;
	}

	else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
	{
		pOwner->Get_SkillTrigger().bSkillETrigger = false;
		return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;
	}

	else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
		return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;


	return STATE_END;
}