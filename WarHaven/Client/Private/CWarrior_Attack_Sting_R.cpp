#include "stdafx.h"
#include "CWarrior_Attack_Sting_R.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_R::CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R::~CWarrior_Attack_Sting_R()
{
}

CWarrior_Attack_Sting_R* CWarrior_Attack_Sting_R::Create()
{
    CWarrior_Attack_Sting_R* pInstance = new CWarrior_Attack_Sting_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_R::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 13;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

	Add_KeyFrame(40, 0);
	Add_KeyFrame(38, 1);
	Add_KeyFrame(50, 2);


    return S_OK;
}

void CWarrior_Attack_Sting_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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
	m_tColorDesc.iEndKeyFrame = 50; // 프레임 맞춰놓음

	
	GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_Sting_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PLAYER_R;

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_L;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWarrior_Attack_Sting_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */

   
    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;

    return STATE_END;
}


void CWarrior_Attack_Sting_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:

		m_bMoveTrigger = false;
		pOwner->Get_PhysicsCom()->Set_MaxSpeed(3.f);
		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		pOwner->Set_DirAsLook();
		break;

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
