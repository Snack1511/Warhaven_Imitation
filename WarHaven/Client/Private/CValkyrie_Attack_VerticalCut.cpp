#include "stdafx.h"
#include "CValkyrie_Attack_VerticalCut.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CValkyrie_Attack_VerticalCut::CValkyrie_Attack_VerticalCut()
{
}

CValkyrie_Attack_VerticalCut::~CValkyrie_Attack_VerticalCut()
{
}

CValkyrie_Attack_VerticalCut* CValkyrie_Attack_VerticalCut::Create()
{
    CValkyrie_Attack_VerticalCut* pInstance = new CValkyrie_Attack_VerticalCut();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CValkyrie_Attack_VerticalCut::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_VERTICALCUT_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 57;
    
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);


	//Add_KeyFrame(36, 0);

	m_iStopIndex = 29;
	m_iAttackEndIndex = 40;

	Add_KeyFrame(m_iStopIndex, 0);
	Add_KeyFrame(37, 1);

	//Vertical은 전부 Land로 맞춤
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
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 12; // 제자리
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;


	m_eWalkState = STATE_WALK_VALKYRIE_L;
	m_eJumpState = STATE_JUMP_VALKYRIE_L;
	m_eLandState = STATE_JUMP_LAND_VALKYRIE_L;
	m_eFallState = STATE_JUMPFALL_VALKYRIE_L;
	m_eRunState = STATE_RUN_VALKYRIE_L;
	m_eIdleState = STATE_IDLE_VALKYRIE_L;
	m_eBounceState = STATE_BOUNCE_VALKYRIE_R;



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

void CValkyrie_Attack_VerticalCut::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_Attack_VerticalCut::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Set_BounceState(STATE_BOUNCE_VALKYRIE_R);

    return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Attack_VerticalCut::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

    //Exit에선 무조건 남겨놔야함
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CValkyrie_Attack_VerticalCut::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */
   
    if (MOUSE_MOVE(MMS_WHEEL) < 0)
        return m_eStateType;

    return STATE_END;
}

void CValkyrie_Attack_VerticalCut::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	default:
		break;
	}

}
