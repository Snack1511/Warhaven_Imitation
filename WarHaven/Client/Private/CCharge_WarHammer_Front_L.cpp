#include "stdafx.h"
#include "CCharge_WarHammer_Front_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CCharge_WarHammer_Front_L::CCharge_WarHammer_Front_L()
{
}

CCharge_WarHammer_Front_L::~CCharge_WarHammer_Front_L()
{
}

CCharge_WarHammer_Front_L* CCharge_WarHammer_Front_L::Create()
{
    CCharge_WarHammer_Front_L* pInstance = new CCharge_WarHammer_Front_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CCharge_WarHammer_Front_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CCharge_WarHammer_Front_L::Initialize()
{

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 26;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CHARGE_FRONT_WARHAMMER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	//Vertical�� ���� Land�� ����
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 4;


	m_eWalkState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eJumpState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eLandState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eFallState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eRunState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eIdleState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eBounceState = STATE_VERTICALATTACK_WARHAMMER_L;
	m_eChargeAttackState = STATE_CHARGEATTACK_FRONT_WARHAMMER_L;

    return __super::Initialize();
}

void CCharge_WarHammer_Front_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CCharge_WarHammer_Front_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CCharge_WarHammer_Front_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CCharge_WarHammer_Front_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CCharge_WarHammer_Front_L::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
