#include "stdafx.h"
#include "CSprintAttack_WarHammer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_WarHammer::CSprintAttack_WarHammer()
{
}

CSprintAttack_WarHammer::~CSprintAttack_WarHammer()
{
}

CSprintAttack_WarHammer* CSprintAttack_WarHammer::Create()
{
    CSprintAttack_WarHammer* pInstance = new CSprintAttack_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 33;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	Add_KeyFrame(23, 0);
	Add_KeyFrame(32, 1);


    return S_OK;
}

void CSprintAttack_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->TurnOn_TrailEffect(true);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();


	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((230.f / 255.f), (150.f / 255.f), (40.f / 255.f), 0.f);
	tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 24; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(tColorDesc);




	//마찰 조절하기
	//*주의 : 사용하고나면 Exit에서 반드시 1로 되돌려주기


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bTrigger)
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Set_Accel(m_fMyAccel);
	}

	if (m_bAttackTrigger)
	{
		// 공격 진입
		if (pOwner->Is_Weapon_R_Collision())
		{
			_float4 vHitPos = pOwner->Get_HitPos();
			_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

			//발쪽이면
			if (vHitPos.y <= vPos.y + 0.1f)
			{
				pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);
				//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
				CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), pOwner->Get_HitMatrix());
			}


			//else
			//	return STATE_BOUNCE_WARHAMMER_R;

			m_bAttackTrigger = false;
		}

	}
	
	Follow_MouseLook(pOwner);
	pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));


    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->TurnOn_TrailEffect(false);

	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

    /* 할거없음 */
}

STATE_TYPE CSprintAttack_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}

void	CSprintAttack_WarHammer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
		//Attack Begin
	case 0:
	{
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		m_bAttackTrigger = true;
	}

	//Attack Done
	break;
	case 1:
	{
		pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);

		cout << "Attack End " << endl;
		m_bAttackTrigger = false;
		m_bTrigger = false;
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.7f;
		pOwner->TurnOn_TrailEffect(false);
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	}


	default:
		break;
	}
}
