#include "stdafx.h"
#include "CWarrior_Attack_VerticalCut.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_VerticalCut::CWarrior_Attack_VerticalCut()
{
}

CWarrior_Attack_VerticalCut::~CWarrior_Attack_VerticalCut()
{
}

CWarrior_Attack_VerticalCut* CWarrior_Attack_VerticalCut::Create()
{
    CWarrior_Attack_VerticalCut* pInstance = new CWarrior_Attack_VerticalCut();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_VerticalCut::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 14;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_VERTICALCUT;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.3f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	Add_KeyFrame(31, 1);
	Add_KeyFrame(50, 2);

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iStopIndex = 30;
	m_iIdle_Index = 3;
	m_iLandIndex = 9;
	m_iJumpFallIndex = 1;

	m_iRunAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunAnimIndex[STATE_DIRECTION_W] = 22;

	m_iWalkAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkAnimIndex[STATE_DIRECTION_W] = 37;
	m_iWalkAnimIndex[STATE_DIRECTION_E] = 30;

	m_iJumpAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpAnimIndex[STATE_DIRECTION_NW] = 4; // ���ڸ�

	m_iJumpAnimIndex[STATE_DIRECTION_NE] = 99; // �ǹ̾��°� ä��� (0�̸� ������ �س���)
	m_iJumpAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpAnimIndex[STATE_DIRECTION_SE] = 99;

	m_eWalkState = STATE_WALK_PLAYER_L;
	m_eJumpState = STATE_JUMP_PLAYER_L;
	m_eLandState = STATE_JUMP_LAND_PLAYER_L;
	m_eFallState = STATE_JUMPFALL_PLAYER_L;
	m_eRunState = STATE_RUN_PLAYER_L;
	m_eIdleState = STATE_IDLE_PLAYER_L;

    return S_OK;
}

void CWarrior_Attack_VerticalCut::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_bMoveTrigger = true;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_VerticalCut::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_R;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_VerticalCut::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */

    //Exit���� ������ ���ܳ�����
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_VerticalCut::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1. CTRL + LBuutton �� �̿��� �����Ѵ�.
    */
   
    if (MOUSE_MOVE(MMS_WHEEL) < 0)
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Attack_VerticalCut::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	default:
		break;
	}

}
