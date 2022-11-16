#include "stdafx.h"
#include "CWarrior_Attack_Sting_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_L::CWarrior_Attack_Sting_L()
{
}

CWarrior_Attack_Sting_L::~CWarrior_Attack_Sting_L()
{
}

CWarrior_Attack_Sting_L* CWarrior_Attack_Sting_L::Create()
{
    CWarrior_Attack_Sting_L* pInstance = new CWarrior_Attack_Sting_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_L::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 12;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 80;
    
	m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_iStopIndex = 38;
	m_iAttackEndIndex = 50;
	
	Add_KeyFrame(38, 1);
	Add_KeyFrame(50, 2);

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_L;
	m_iIdle_Index = 3;

	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 9;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 1;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 18;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 22;

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


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 37;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 30;



	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // ���ڸ�
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // �ǹ̾��°� ä��� (0�̸� ������ �س���)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 8;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 7;


	m_eWalkState = STATE_WALK_PLAYER_R;
	m_eJumpState = STATE_JUMP_PLAYER_R;
	m_eLandState = STATE_JUMP_LAND_PLAYER_R;
	m_eFallState = STATE_JUMPFALL_PLAYER_R;
	m_eRunState = STATE_RUN_PLAYER_R;
	m_eIdleState = STATE_IDLE_PLAYER_R;


	return __super::Initialize();
}

void CWarrior_Attack_Sting_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	CColorController::COLORDESC m_tColorDesc;
	ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

	m_tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	m_tColorDesc.fFadeInStartTime = 0.f;
	m_tColorDesc.fFadeInTime = 0.1f;
	m_tColorDesc.fFadeOutStartTime = 0.8f;
	m_tColorDesc.fFadeOutTime = 0.1f;

	m_tColorDesc.vTargetColor = _float4((230.f / 255.f), (150.f / 255.f), (40.f / 255.f), 0.f);

	m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	m_tColorDesc.iStartKeyFrame = 20;
	m_tColorDesc.iEndKeyFrame = 50; // ������ �������


	GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_Sting_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_L;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

}

STATE_TYPE CWarrior_Attack_Sting_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1. CTRL + LBuutton �� �̿��� �����Ѵ�.
    */

    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;


    return STATE_END;
}

void		CWarrior_Attack_Sting_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 1:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}
