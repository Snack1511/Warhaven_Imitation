#include "stdafx.h"
#include "CState_Combat_Default_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_Default_Archer::CState_Combat_Default_Archer()
{
}

CState_Combat_Default_Archer::~CState_Combat_Default_Archer()
{
}

HRESULT CState_Combat_Default_Archer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 1.2f;
	m_fMyAccel = 100.f;


    return S_OK;
}

void CState_Combat_Default_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == m_eStateType)
		m_fInterPolationTime = 0.f;

	m_iRand = random(0, 7);
	m_iDirectionRand = random(0, 7);
	m_fRand = frandom(1.f, 3.f);

	Set_Direction_Back_AI(m_iDirectionRand);
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

	m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_iStateChangeKeyFrame = 15;

	m_fAIMyLength = 5.2f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eAttackType = Near_Enemy(pOwner, false);

	if (eAttackType != STATE_END)
		return eAttackType;

	
	_uint iFrame = pAnimator->Get_CurAnimFrame();

	m_fTimeAcc += fDT(0);


	// 공격 코드
	if (m_fTimeAcc < m_fRand)
	{

		if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
		{

			switch (m_iRand)
			{
			case 0:
			case 1:

				if (pOwner->Can_Use(CUnit::SKILL3))
				{
					pOwner->Get_SkillTrigger().bSkillQTrigger = true;
					pOwner->Get_SkillTrigger().bSkillETrigger = false;
					return AI_STATE_COMBAT_STATE_SWAP_ARCHER;
				}
				else if (pOwner->Can_Use(CUnit::SKILL2))
				{
					pOwner->Get_SkillTrigger().bSkillQTrigger = false;
					pOwner->Get_SkillTrigger().bSkillETrigger = true;
					return AI_STATE_COMBAT_STATE_SWAP_ARCHER;
				}
				else
					return m_eStateType;

				break;

			case 2:
			case 3:


				if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
					return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;

				else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
					return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;

				else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
					return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;
				else
					return m_eStateType;

				break;

			case 4:
			case 5:
			case 6:
			case 7:

				eAttackType = Near_Enemy(pOwner, true);

				if (eAttackType == STATE_END)
					return m_eStateType;

				return eAttackType;

			default:
				break;
			}
		}
		else
		{
		if(	m_iRand == 4 ||
			m_iRand == 5 ||
			m_iRand == 6 ||
			m_iRand == 7)

			eAttackType = Near_Enemy(pOwner, true);

			if (eAttackType == STATE_END)
				return m_eStateType;

			return eAttackType;
		}
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
    return STATE_END;
}


STATE_TYPE CState_Combat_Default_Archer::Near_Enemy(CUnit* pOwner, _bool bUseAdjSkill)
{
	if (Get_TargetLook_Length(pOwner) < m_fAIMyLength / 2.2f)
	{

		if (pOwner->Can_Use(CUnit::SKILL1))
			return AI_STATE_COMBAT_ATTACK_SWING_ARCHER;

		else if (m_iRand >= 3)
			return AI_STATE_COMBAT_GUARDLOOP_ARCHER;

		else if (m_iRand >= 6)
			return m_eStateType;

		else
		{
			if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
				return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;

			else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
				return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;

			else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
				return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;
		}
	
	}
	else
	{
		if (!bUseAdjSkill)
			return STATE_END;

		if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
			return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;

		else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
			return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;

		else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
			return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;

	}

	return STATE_END;
}

