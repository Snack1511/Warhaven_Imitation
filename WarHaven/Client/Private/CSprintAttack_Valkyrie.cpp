#include "stdafx.h"
#include "CSprintAttack_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_Valkyrie::CSprintAttack_Valkyrie()
{
}

CSprintAttack_Valkyrie::~CSprintAttack_Valkyrie()
{
}

CSprintAttack_Valkyrie* CSprintAttack_Valkyrie::Create()
{
    CSprintAttack_Valkyrie* pInstance = new CSprintAttack_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Valkyrie::Initialize()
{
	m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 19;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 70;

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	Add_KeyFrame(20, 0);
	Add_KeyFrame(32, 1);


    return S_OK;
}

void CSprintAttack_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->TurnOn_TrailEffect(true);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintAttackSpeed);
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

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);




	//마찰 조절하기
	//*주의 : 사용하고나면 Exit에서 반드시 1로 되돌려주기


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	Follow_MouseLook(pOwner);
	pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));


    return __super::Tick(pOwner, pAnimator);
}

void CSprintAttack_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->TurnOn_TrailEffect(false);

	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

    /* 할거없음 */
}

STATE_TYPE CSprintAttack_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}

void	CSprintAttack_Valkyrie::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	switch (iSequence)
	{
		//Attack Begin
	case 0:
	{
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.15f;

		cout << "Attack Begin " << endl;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		m_bAttackTrigger = true;
	}

	//Attack Done
	break;
	case 1:
	{
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
		pMyPhysicsCom->Set_SpeedasMax();

		pOwner->Shake_Camera(pOwner->Get_Status().fCamPower, pOwner->Get_Status().fCamTime);


		cout << "Attack End " << endl;
		m_bAttackTrigger = false;
		m_bTrigger = false;
		

		pOwner->TurnOn_TrailEffect(false);
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	}


	default:
		break;
	}
}
