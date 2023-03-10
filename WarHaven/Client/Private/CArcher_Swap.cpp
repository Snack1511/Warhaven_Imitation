#include "stdafx.h"
#include "CArcher_Swap.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Archer.h"

#include "CDefaultArrow.h"
#include "CPurpleArrow.h"
#include "CSnipeArrow.h"


CArcher_Swap::CArcher_Swap()
{
}

CArcher_Swap::~CArcher_Swap()
{
}

CArcher_Swap* CArcher_Swap::Create()
{
    CArcher_Swap* pInstance = new CArcher_Swap();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Swap");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Swap::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 52;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWAP_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 0;


	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;

	Add_KeyFrame(11, 1);
	Add_KeyFrame(30, 2);


	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;
	
	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 8;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 21;
	
	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 26;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 27;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 28;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 29;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 30;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;

	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 28;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 29;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 35;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 4;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 3;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;

	m_eWalkState = STATE_WALK_ARCHER_R;
	m_eJumpState = STATE_JUMP_ARCHER_R;
	m_eLandState = STATE_JUMP_LAND_ARCHER_R;
	m_eFallState = STATE_JUMPFALL_ARCHER_R;
	m_eRunState = STATE_RUN_ARCHER_R;
	m_eIdleState = STATE_IDLE_ARCHER_R;
	m_eBounceState = STATE_BOUNCE_ARCHER;

	//m_eWalkState = STATE_WALK_ARCHER_R;
	//m_eJumpState = STATE_JUMP_ARCHER_R;
	//m_eLandState = STATE_JUMP_LAND_ARCHER_R;
	//m_eFallState = STATE_JUMPFALL_ARCHER_R;
	//m_eRunState = STATE_RUN_ARCHER_R;
	//m_eIdleState = STATE_IDLE_ARCHER_R;
	//m_eBounceState = STATE_WALK_ARCHER_R;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.25f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 0.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 0.8f;

    return __super::Initialize();
}

void CArcher_Swap::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    CState::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Swap::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
	{
		STATE_TYPE eDeafultState = pOwner->Get_DefaultState();
		return eDeafultState;
	}

	if (m_bKeyInputable)
	{
		if (KEY(LBUTTON, HOLD))
		{
			if (pOwner->Get_SkillTrigger().bSkillQTrigger)
				return STATE_ATTACK_BEGIN_SNIPING_ARCHER;

			else if (pOwner->Get_SkillTrigger().bSkillETrigger)
				return STATE_ATTACK_BEGIN_POISION_ARCHER;

			else
				return STATE_ATTACK_BEGIN_ARCHER;
			
		}
	}



    return __super::Tick(pOwner, pAnimator);
}

void CArcher_Swap::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!m_bAttackTrigger)
		Choice_Arrow(pOwner);
		

    //Exit에선 무조건 남겨놔야함
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Swap::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	// Attack_Begin Sniping, Attack_Begin Poison Check_Condition 쪽에서 전부 처리
    return STATE_END;
}

void CArcher_Swap::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:
		
		m_bAttackTrigger = true;
		Choice_Arrow(pOwner);
		break;

	case 2:
		m_bKeyInputable = true;
		break;


	default:
		break;
	}

}

void CArcher_Swap::Choice_Arrow(CUnit* pOwner)
{
	Play_Sound(L"Effect_SwapArrow");

	if (pOwner->Get_SkillTrigger().bSkillQTrigger)
	{
		static_cast<CUnit_Archer*>(pOwner)->Create_SnipeArrow(); // 스나이핑 화살이 나올 수 있도록 설정
		//static_cast<CUnit_Archer*>(pOwner)->Set_ColorController(MODEL_PART_SKEL);
	}
	else if (pOwner->Get_SkillTrigger().bSkillETrigger)
	{
		static_cast<CUnit_Archer*>(pOwner)->Create_PurpleArrow(); // 포이즌 화살이 나올 수 있도록 설정
	}
	else
	{
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();
	}
		
}