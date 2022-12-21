#include "stdafx.h"
#include "CState_Patrol_Default_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Patrol_Default_Archer::CState_Patrol_Default_Archer()
{
}

CState_Patrol_Default_Archer::~CState_Patrol_Default_Archer()
{
}

HRESULT CState_Patrol_Default_Archer::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	
	m_iIdleDelay = 30;


    return S_OK;
}

void CState_Patrol_Default_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_AnimWeaponIndex(0, m_fInterPolationTime, m_fAnimSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Patrol_Default_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

