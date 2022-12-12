#include "stdafx.h"
#include "CState_Patrol_Switch_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Switch_Fiona::CState_Patrol_Switch_Fiona()
{
}

CState_Patrol_Switch_Fiona::~CState_Patrol_Switch_Fiona()
{
}

HRESULT CState_Patrol_Switch_Fiona::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_Patrol_Switch_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	//if (ePrevType == AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L ||
	//	ePrevType == AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R)
	//{
	//	m_bAttack = true;
	//	m_fAnimSpeed = 5.f;
	//	m_iAINextState = ePrevType;
	//}

	//if (pAnimator->Is_CurAnimFinished())
	//	return m_iAINextState;

//protected:
//	_bool	m_bAttack = false;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Switch_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Switch_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Patrol_Switch_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

