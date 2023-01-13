#include "stdafx.h"
#include "CWarrior_Attack_Sting_R.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_R::CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R::~CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R* CWarrior_Attack_Sting_R::Create()
{
    CWarrior_Attack_Sting_R* pInstance = new CWarrior_Attack_Sting_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_R::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;
	//m_tHitInfo.bSting = true;



	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 13;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
	m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


	m_iStopIndex = 40;
	m_iAttackEndIndex = 50;

	Add_KeyFrame(38, 0);
	Add_KeyFrame(50, 1);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 17;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 10;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 30;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 30;


	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;
	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 45;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 38;



	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // ���ڸ�
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // �ǹ̾��°� ä��� (0�̸� ������ �س���)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 12;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;


	

	m_eWalkState = STATE_WALK_PLAYER_R;
	m_eJumpState = STATE_JUMP_PLAYER_R;
	m_eLandState = STATE_JUMP_LAND_PLAYER_R;
	m_eFallState = STATE_JUMPFALL_PLAYER_R;
	m_eRunState = STATE_RUN_PLAYER_R;
	m_eIdleState = STATE_IDLE_PLAYER_R;
	m_eBounceState = STATE_BOUNCE_PLAYER_R;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;


	return __super::Initialize();
}

void CWarrior_Attack_Sting_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PLAYER_R);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_Sting_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_Sting_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1. CTRL + LBuutton �� �̿��� �����Ѵ�.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;

    return STATE_END;
}


void CWarrior_Attack_Sting_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 0:
		Play_Sound(L"Effect_Sting_Warrior", CHANNEL_EFFECTS, 1.f);
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}
