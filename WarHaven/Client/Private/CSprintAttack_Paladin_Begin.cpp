#include "stdafx.h"
#include "CSprintAttack_Paladin_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CColorController.h"

CSprintAttack_Paladin_Begin::CSprintAttack_Paladin_Begin()
{
}

CSprintAttack_Paladin_Begin::~CSprintAttack_Paladin_Begin()
{
}

CSprintAttack_Paladin_Begin* CSprintAttack_Paladin_Begin::Create()
{
    CSprintAttack_Paladin_Begin* pInstance = new CSprintAttack_Paladin_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Paladin_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Paladin_Begin::Initialize()
{
    m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 16;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINTATTACK_BEGIN_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    
    m_vecAdjState.push_back(STATE_SPRINTATTACK_PALADIN);


    return S_OK;
}

void CSprintAttack_Paladin_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//임시
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = pOwner->Get_FollowCamLook();
	vCamLook.y = 0.f;

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamLook);

	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 0.7f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprintAttack_Paladin_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	pMyPhysicsCom->Set_Accel(m_fMyAccel);

    Follow_MouseLook(pOwner);
    pMyPhysicsCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Paladin_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprintAttack_Paladin_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
    1. 스프린트중에 공격 버튼을 누르면
    */

	if (KEY(LBUTTON, TAP))
		return m_eStateType;


    return STATE_END;
}
