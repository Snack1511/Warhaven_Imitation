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
	m_eStateType = STATE_PROJECTILECATCH_SHOOT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)



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
		
	Play_Voice(pOwner, L"Voice_Attack", 1.f);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillQ_Shoot_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_PRIEST;

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
