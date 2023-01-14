#include "stdafx.h"
#include "CState_Combat_SkillQ_Shoot_Priest.h"

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


CState_Combat_SkillQ_Shoot_Priest::CState_Combat_SkillQ_Shoot_Priest()
{
}

CState_Combat_SkillQ_Shoot_Priest::~CState_Combat_SkillQ_Shoot_Priest()
{
}

CState_Combat_SkillQ_Shoot_Priest* CState_Combat_SkillQ_Shoot_Priest::Create()
{
    CState_Combat_SkillQ_Shoot_Priest* pInstance = new CState_Combat_SkillQ_Shoot_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Shoot_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_SkillQ_Shoot_Priest::Initialize()
{

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 11;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_CATCH_SHOOT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)



	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 50;

	Add_KeyFrame(8, 0);
	
	return __super::Initialize();
}

void CState_Combat_SkillQ_Shoot_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

	if (!pOwner->Get_CatchProjectileObject())
		m_bCatchBall = true;

	if (pOwner->Get_CatchedBall())
	{
		ENABLE_GAMEOBJECT(pOwner->Get_CatchedBall());
		pOwner->Get_Status().fDamageMultiplier = 7.f;
	}
		
	Play_Voice(pOwner, L"Voice_Attack", m_fAIDelayTime);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Shoot_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return pOwner->Get_DefaultState();

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Shoot_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_Shoot_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Combat_SkillQ_Shoot_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	CUnit* pTargetUnit = pOwner->Get_TargetUnit();
	_float4 vLook = ZERO_VECTOR;

	switch (iSequence)
	{
	case 0:
		if (m_bCatchBall)
		{
			CUnit* pTargetUnit = pOwner->Get_TargetUnit();

			if(!pTargetUnit)
				pOwner->Get_CatchedBall()->Shoot_CatchedCannon(pOwner->Get_FollowCamLook());
			else
			{
				m_tHitInfo.bFly = true;
				m_tHitInfo.iLandKeyFrame = 100;
				m_tHitInfo.fJumpPower = 6.f;
				m_tHitInfo.bGuardBreak = true;

				vLook = pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
				pOwner->Get_CatchedBall()->Shoot_CatchedCannon(vLook.Normalize());
			}

			pOwner->Catch_CannonBall(nullptr);
		}
		else
		{
			if (pTargetUnit)
				vLook = pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
			else
				vLook = pOwner->Get_FollowCamLook();

			vLook.Normalize();

			pOwner->Get_CatchProjectileObject()->Get_Transform()->Set_Right(vLook);
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
