#include "stdafx.h"
#include "CWarrior_Attack_HorizontalUp.h"

#include "CUtility_Transform.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWarrior_Attack_HorizontalUp::CWarrior_Attack_HorizontalUp()
{
}

CWarrior_Attack_HorizontalUp::~CWarrior_Attack_HorizontalUp()
{
}

HRESULT CWarrior_Attack_HorizontalUp::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;
	m_iStateChangeKeyFrame = 75;



	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 10.f;
	m_fMaxSpeed = 0.8f;

	m_iStopIndex = 38;
	m_iAttackEndIndex = 58;

	Add_KeyFrame(43, 1);
	Add_KeyFrame(m_iAttackEndIndex, 2);



	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    return __super::Initialize();
}

void CWarrior_Attack_HorizontalUp::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */

  //  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalUp::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalUp::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);

}

STATE_TYPE CWarrior_Attack_HorizontalUp::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 위로 공격할 시 
	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		_float fDot = CUtility_Transform::Get_LookRotateAngle(GAMEINSTANCE->Get_CurCamLook());

		if (fDot > 0.f && fDot < 0.994f)
		{
				return m_eStateType;
		}
	}
	


    return STATE_END;
}

void CWarrior_Attack_HorizontalUp::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{
	case 1:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}

