#include "stdafx.h"
#include "CLancer_Attack_Default.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CLancer_Attack_Default::CLancer_Attack_Default()
{
}

CLancer_Attack_Default::~CLancer_Attack_Default()
{
}

CLancer_Attack_Default* CLancer_Attack_Default::Create()
{
    CLancer_Attack_Default* pInstance = new CLancer_Attack_Default();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLancer_Attack_Default");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CLancer_Attack_Default::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;


	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_DEAFULT_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fDamagePumping = 2.5f;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 130;

	m_vecAdjState.push_back(STATE_IDLE_LANCER);
	m_vecAdjState.push_back(STATE_RUN_LANCER);

	m_iStopIndex = 777;
	m_iAttackEndIndex = 50;

	Add_KeyFrame(46, 0);
	Add_KeyFrame(55, 1);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 9;
	m_iLandRightIndex = 16;
	m_iLandLeftIndex = 16;
	m_iJumpFallRightIndex = 7;
	m_iJumpFallLeftIndex = 7;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 5;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 5;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 5;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 5;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 5;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 5;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 5;



	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 7; // ���ڸ�
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 7; // �ǹ̾��°� ä��� (0�̸� ������ �س���)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 7;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 7;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 7;


	

	m_eWalkState = STATE_RUN_LANCER;
	m_eJumpState = STATE_JUMP_LANCER;
	m_eLandState = STATE_RUN_LANCER;
	m_eFallState = STATE_JUMPFALL_LANCER;
	m_eRunState = STATE_RUN_LANCER;
	m_eIdleState = STATE_IDLE_LANCER;
	m_eBounceState = STATE_BOUNCE_LANCER;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 2.f;

	m_bStraight = true;
	m_bSmootMoveLoop = true;
	m_bCam = false;
	m_bOnlyAnimBase_R = true;

	return __super::Initialize();
}

void CLancer_Attack_Default::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

	pOwner->Set_BounceState(STATE_BOUNCE_LANCER);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CLancer_Attack_Default::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

    return __super::Tick(pOwner, pAnimator);
}

void CLancer_Attack_Default::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CLancer_Attack_Default::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
   
    if (KEY(LBUTTON, TAP))
        return m_eStateType;

    return STATE_END;
}


void CLancer_Attack_Default::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Play_Voice(pOwner, L"Voice_Attack", 1.f);

		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}
