#include "stdafx.h"
#include "CState_PathNavigation_Jump_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Jump_Warrior::CState_PathNavigation_Jump_Warrior()
{
}

CState_PathNavigation_Jump_Warrior::~CState_PathNavigation_Jump_Warrior()
{
}

HRESULT CState_PathNavigation_Jump_Warrior::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.05f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_PathNavigation_Jump_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	DoMove_AI_NoTarget(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Jump_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Jump_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

