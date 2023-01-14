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
	//높이 차 별로 안나면 ray 쏴야함
	if (!pOwner->Get_OwnerPlayer()->Get_TargetObject())
		return __super::Tick(pOwner, pAnimator);

	_float4 vOutPos;
	_float fOutDist;

	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vTargetPos = pOwner->Get_OwnerPlayer()->Get_TargetObjPos();

	//높이 차 별로 안나면 ray 쏴야함

	_float4 vRayStartPos = vCurPos;
	vRayStartPos.y += 0.5f;
	_float4 vRayEndPos = vTargetPos;
	vRayEndPos.y += 0.5f;

	_float4 vRayDir = vRayEndPos - vRayStartPos;

	if (GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vRayStartPos, vRayDir, vRayDir.Length()))
	{
		if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame || pAnimator->Is_CurAnimFinished())
		{
			STATE_TYPE eAIPathNavigationState = pOwner->Get_AIState_Type().eAIPathFindDefaultState;
			return eAIPathNavigationState;
		}			

	}


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
