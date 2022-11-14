#include "stdafx.h"
#include "CWarrior_Attack_HorizontalDown.h"

#include "CUtility_Transform.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWarrior_Attack_HorizontalDown::CWarrior_Attack_HorizontalDown()
{
}

CWarrior_Attack_HorizontalDown::~CWarrior_Attack_HorizontalDown()
{
}

HRESULT CWarrior_Attack_HorizontalDown::Initialize()
{

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 50;

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 10.f;
	m_fMaxSpeed = 0.8f;

	Add_KeyFrame(30, 0);
	Add_KeyFrame(39, 1);
	Add_KeyFrame(50, 2);

    return S_OK;
}

void CWarrior_Attack_HorizontalDown::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalDown::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bMoveTrigger)
		Move(Get_Direction(), pOwner);

	if (m_bAttackTrigger)
	{
		// 공격 진입
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_R;

	}

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalDown::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
}

STATE_TYPE CWarrior_Attack_HorizontalDown::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    1. 아래로 공격 시  
	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		_float fDot = CUtility_Transform::Get_LookRotateAngle(GAMEINSTANCE->Get_CurCamLook());

		if (fDot < 0.f && fabs(fDot) < 0.994f)
		{
			return m_eStateType;
		}
	}
    return STATE_END;
}

void CWarrior_Attack_HorizontalDown::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		m_bMoveTrigger = false;
		pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);
		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		pOwner->Set_DirAsLook();
		break;

	case 1:
		m_bAttackTrigger = true;
		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 3.f;
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

