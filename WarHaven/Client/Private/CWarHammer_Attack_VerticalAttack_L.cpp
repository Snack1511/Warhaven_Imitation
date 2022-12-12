#include "stdafx.h"
#include "CWarHammer_Attack_VerticalAttack_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarHammer_Attack_VerticalAttack_L::CWarHammer_Attack_VerticalAttack_L()
{
}

CWarHammer_Attack_VerticalAttack_L::~CWarHammer_Attack_VerticalAttack_L()
{
}

CWarHammer_Attack_VerticalAttack_L* CWarHammer_Attack_VerticalAttack_L::Create()
{
    CWarHammer_Attack_VerticalAttack_L* pInstance = new CWarHammer_Attack_VerticalAttack_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarHammer_Attack_VerticalAttack_L::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_VERTICALATTACK_WARHAMMER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	//Vertical�� ���� Land�� ����
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 4;

    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUN_WARHAMMER_R);


	m_eWalkState = STATE_WALK_WARHAMMER_R;
	m_eJumpState = STATE_JUMP_WARHAMMER_R;
	m_eLandState = STATE_JUMP_LAND_WARHAMMER_R;
	m_eFallState = STATE_JUMPFALL_WARHAMMER_R;
	m_eRunState = STATE_RUN_WARHAMMER_R;
	m_eIdleState = STATE_IDLE_WARHAMMER_R;
	m_eBounceState = STATE_BOUNCE_WARHAMMER_L;

    m_fDamagePumping = 1.5f;

    return __super::Initialize();
}

void CWarHammer_Attack_VerticalAttack_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    pOwner->Set_BounceState(STATE_BOUNCE_WARHAMMER_L);


    if (ePrevType == STATE_SWITCH_R_TO_L)
    {
        m_fAnimSpeed = 2.5f;
    }
    else
        m_fAnimSpeed = 2.7f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_Attack_VerticalAttack_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */

    //Exit���� ������ ���ܳ�����
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarHammer_Attack_VerticalAttack_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
  
	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarHammer_Attack_VerticalAttack_L::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
