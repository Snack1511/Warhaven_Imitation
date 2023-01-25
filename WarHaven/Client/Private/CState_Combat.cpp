#include "stdafx.h"
#include "CState_Combat.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat::CState_Combat()
{
}

CState_Combat::~CState_Combat()
{
}

HRESULT CState_Combat::Initialize()
{

    return S_OK;
}

void CState_Combat::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_pCurrentTargetUnit = pOwner->Get_TargetUnit();


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Combat::Straight_Move(CUnit* pOwner)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	if (!m_vAIRandLook.Is_Zero())
		pMyTransform->Set_LerpLook(m_vAIRandLook, m_fMyMaxLerp);
	pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);

	_float4 vOwnerLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	vOwnerLook.y = 0.f;

	pMyPhysicsCom->Set_Dir(vOwnerLook);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);
}

void CState_Combat::Set_StraightLook(CUnit* pOwner)
{
	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if (pTargetUnit)
	{
		m_vAIRandLook = pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
		m_vAIRandLook.Normalize();
	}
	else
		m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));

	m_vAIRandLook.y = 0.f;
}
