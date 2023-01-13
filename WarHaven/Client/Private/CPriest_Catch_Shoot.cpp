#include "stdafx.h"
#include "CPriest_Catch_Shoot.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CProjectile.h"
#include "CCannonBall.h"


CPriest_Catch_Shoot::CPriest_Catch_Shoot()
{
}

CPriest_Catch_Shoot::~CPriest_Catch_Shoot()
{
}

CPriest_Catch_Shoot* CPriest_Catch_Shoot::Create()
{
    CPriest_Catch_Shoot* pInstance = new CPriest_Catch_Shoot();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CPriest_Catch_Shoot");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CPriest_Catch_Shoot::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 11;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PROJECTILECATCH_SHOOT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)



    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 50;


	m_iStopIndex = 712;
	m_iAttackEndIndex = 712;

	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_JUMP_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);


	Init_AttackState_Priest();
	Init_CommonState_Player();

	Add_KeyFrame(8, 0);

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
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 11; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
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


	m_eWalkState = STATE_WALK_PRIEST;
	m_eJumpState = STATE_JUMP_PRIEST;
	m_eLandState = STATE_JUMP_LAND_PRIEST;
	m_eFallState = STATE_JUMPFALL_PRIEST;
	m_eRunState = STATE_RUN_PRIEST;
	m_eIdleState = STATE_IDLE_PRIEST;
	m_eBounceState = STATE_BOUNCE_PRIEST;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.f;

	Add_KeyFrame(11, 111);

	return __super::Initialize();
}

void CPriest_Catch_Shoot::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CState::Enter(pOwner, pAnimator, ePrevType, pData);

	if (!pOwner->Get_CatchProjectileObject())
		m_bCatchBall = true;

	if (pOwner->Get_CatchedBall())
	{
		ENABLE_GAMEOBJECT(pOwner->Get_CatchedBall());
		pOwner->Get_Status().fDamageMultiplier = 7.f;
	}
		
 
}

STATE_TYPE CPriest_Catch_Shoot::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CPriest_Catch_Shoot::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CPriest_Catch_Shoot::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
   
    if (KEY(Q, AWAY))
        return m_eStateType;

    return STATE_END;
}


void CPriest_Catch_Shoot::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		if (m_bCatchBall)
		{
			pOwner->Get_CatchedBall()->Shoot_CatchedCannon(GAMEINSTANCE->Get_CurCamLook());
			pOwner->Catch_CannonBall(nullptr);
		}
		else
		{
			pOwner->Get_CatchProjectileObject()->Get_Transform()->Set_Right(pOwner->Get_FollowCamLook());
			pOwner->Get_CatchProjectileObject()->Get_Transform()->Make_WorldMatrix();

			pOwner->Get_CatchProjectileObject()->On_ShootProjectile();
			pOwner->Catch_ProjectileObject(nullptr);
		}
		
		break;

	case 111:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Catch_Shoot", pOwner, ZERO_VECTOR);
	default:
		break;
	}
}
