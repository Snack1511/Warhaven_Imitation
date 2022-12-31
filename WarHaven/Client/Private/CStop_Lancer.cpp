#include "stdafx.h"
#include "CStop_Lancer.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_Lancer::CStop_Lancer()
{
}

CStop_Lancer::~CStop_Lancer()
{
}

CStop_Lancer* CStop_Lancer::Create()
{
	CStop_Lancer* pInstance = new CStop_Lancer();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_Lancer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_Lancer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_iStateChangeKeyFrame = 75;

	m_fAnimSpeed = 2.f;

	m_vecAdjState.push_back(STATE_IDLE_LANCER);
	//m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_LANCER);


	//m_iDirectionAnimIndex[STATE_DIRECTION_E] = 25;

	//m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 26;
	//m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 26;
	//m_iDirectionAnimIndex[STATE_DIRECTION_N] = 26;

	//m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 27;
	//m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 27;
	//m_iDirectionAnimIndex[STATE_DIRECTION_S] = 27;

	//m_iDirectionAnimIndex[STATE_DIRECTION_W] = 28;


	return S_OK;
}

void CStop_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	/* Owner�� Animator Set Idle�� */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

	return __super::Tick(pOwner, pAnimator);
}

void CStop_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CStop_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE)
		)
	{
		return m_eStateType;
	}

	return STATE_END;
}

