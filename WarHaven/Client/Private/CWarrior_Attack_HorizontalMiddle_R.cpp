#include "stdafx.h"
#include "CWarrior_Attack_HorizontalMiddle_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarrior_Attack_HorizontalMiddle_R::CWarrior_Attack_HorizontalMiddle_R()
{
}

CWarrior_Attack_HorizontalMiddle_R::~CWarrior_Attack_HorizontalMiddle_R()
{
}

CWarrior_Attack_HorizontalMiddle_R* CWarrior_Attack_HorizontalMiddle_R::Create()
{
	CWarrior_Attack_HorizontalMiddle_R* pInstance = new CWarrior_Attack_HorizontalMiddle_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_HorizontalMiddle_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarrior_Attack_HorizontalMiddle_R::Initialize()
{
	/* HIT_INFO */
	/*오른쪽에서 왼쪽으로 휘두르니까 LEFT*/
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;







	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 11;
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
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
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


	m_eWalkState = STATE_WALK_PLAYER_L;
	m_eJumpState = STATE_JUMP_PLAYER_L;
	m_eLandState = STATE_JUMP_LAND_PLAYER_L;
	m_eFallState = STATE_JUMPFALL_PLAYER_L;
	m_eRunState = STATE_RUN_BEGIN_PLAYER_L;
	m_eIdleState = STATE_IDLE_PLAYER_L;
	m_eBounceState = STATE_BOUNCE_PLAYER_R;


	return __super::Initialize();
}

void CWarrior_Attack_HorizontalMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_PLAYER_R);

	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	

}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* Player가 Attack 으로 오는 조건
	1.  LBuutton 을 이용해 공격한다.
	*/
	//if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	//{
	//	return m_eStateType;
	//}

	//return STATE_END;
}

void CWarrior_Attack_HorizontalMiddle_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
