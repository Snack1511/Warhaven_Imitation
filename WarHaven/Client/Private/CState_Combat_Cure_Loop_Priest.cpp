#include "stdafx.h"
#include "CState_Combat_Cure_Loop_Priest.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Priest.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"
#include "CUI_UnitHUD.h"
#include "CUI_UnitHP.h"

CState_Combat_Cure_Loop_Priest::CState_Combat_Cure_Loop_Priest()
{
}

CState_Combat_Cure_Loop_Priest::~CState_Combat_Cure_Loop_Priest()
{
}

CState_Combat_Cure_Loop_Priest* CState_Combat_Cure_Loop_Priest::Create()
{
	CState_Combat_Cure_Loop_Priest* pInstance = new CState_Combat_Cure_Loop_Priest();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Cure_Loop_Priest");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CState_Combat_Cure_Loop_Priest::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 6;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_CURE_LOOP_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 1.f;

	//enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
	m_iStateChangeKeyFrame = 99;

	m_fMaxTime = 0.5f;

	return S_OK;
}

void CState_Combat_Cure_Loop_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	if (m_eStateType == ePrevStateType)
		m_fInterPolationTime = 0.f;

	static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(true);

	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed;

	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

void CState_Combat_Cure_Loop_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

	static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(false);
	
	if(m_pTargetObject)
		static_cast<CUnit*>(m_pTargetObject)->Get_OwnerHUD()->Get_UnitHP()->SetActive_HealBlur(false);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_Cure_Loop_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pTargetUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	if (!pTargetUnit)
		return AI_STATE_COMBAT_CURE_END_PRIEST;

	
	if(pTargetUnit->Get_Status().fHP >= pTargetUnit->Get_Status().fMaxHP)
		return AI_STATE_COMBAT_CURE_END_PRIEST;


	if(!pOwner->Get_SameNearObejct())
		static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(true);


	// Ÿ�� ������ ���� �ް� �ִ� ���
	pTargetUnit->Get_OwnerHUD()->Get_UnitHP()->SetActive_HealBlur(true);
	m_pTargetObject = pTargetUnit;

	_float fLength = (pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS)).Length();


	if (fabs(fLength) > pOwner->Get_MaxDistance())
		return AI_STATE_COMBAT_CURE_END_PRIEST;

	m_fTimeAcc += fDT(0);
	

	_float4 vTargetLook = pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	vTargetLook.y = 0.f;

	pOwner->Get_Transform()->Set_LerpLook(vTargetLook, 0.4f);

	if (m_fTimeAcc > m_fMaxTime)
	{
		// Ǯ�ǰ� �ƴϸ�
		if (pTargetUnit->Get_Status().fHP < pTargetUnit->Get_Status().fMaxHP)
		{
			_float fPlusHp = 15.f;

			// UI ǥ��
			pTargetUnit->Get_Status().fHP += fPlusHp; // fPlusHp

			if(pOwner->Get_OwnerPlayer()->Get_Gauge() < 100.f - 3.f)
				pOwner->Get_OwnerPlayer()->Get_Gauge() += 3.f;

			// Ǯ�Ǹ� �Ѿ��
			if (pTargetUnit->Get_Status().fHP > pTargetUnit->Get_Status().fMaxHP)
				pTargetUnit->Get_Status().fHP = pTargetUnit->Get_Status().fMaxHP;

		}
		
		m_fTimeAcc = 0.f;
	}


	return __super::Tick(pOwner, pAnimator);
}

