#include "stdafx.h"
#include "CPriest_Catch_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CUnit_Priest.h"


CPriest_Catch_Begin::CPriest_Catch_Begin()
{
}

CPriest_Catch_Begin::~CPriest_Catch_Begin()
{
}

CPriest_Catch_Begin* CPriest_Catch_Begin::Create()
{
    CPriest_Catch_Begin* pInstance = new CPriest_Catch_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CPriest_Catch_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CPriest_Catch_Begin::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PROJECTILECATCH_BEGIN_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)



    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 0;


	m_iStopIndex = 712;
	m_iAttackEndIndex = 712;

	m_vecAdjState.push_back(STATE_PROJECTILECATCH_LOOP_PRIEST);

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 16;
	m_iLandLeftIndex = 16;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 24;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;



	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 11; // ���ڸ�
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // �ǹ̾��°� ä��� (0�̸� ������ �س���)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	m_eWalkState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eJumpState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eLandState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eFallState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eRunState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eIdleState = STATE_PROJECTILECATCH_LOOP_PRIEST;
	m_eBounceState = STATE_PROJECTILECATCH_LOOP_PRIEST;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.f;

	m_bSmootMoveLoop = true;
	m_bLoopAction = true;

	return __super::Initialize();
}

void CPriest_Catch_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    CState::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Sound(L"Effect_Catch_Priest");
}

STATE_TYPE CPriest_Catch_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc > 0.5f)
		return STATE_PROJECTILECATCH_LOOP_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CPriest_Catch_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPriest_Catch_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Can_Use(CUnit::SKILL3))
		return STATE_END;

    if (KEY(Q, HOLD))
        return m_eStateType;

    return STATE_END;
}


void CPriest_Catch_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
