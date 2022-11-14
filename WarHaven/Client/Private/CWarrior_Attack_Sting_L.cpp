#include "stdafx.h"
#include "CWarrior_Attack_Sting_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_Sting_L::CWarrior_Attack_Sting_L()
{
}

CWarrior_Attack_Sting_L::~CWarrior_Attack_Sting_L()
{
}

CWarrior_Attack_Sting_L* CWarrior_Attack_Sting_L::Create()
{
    CWarrior_Attack_Sting_L* pInstance = new CWarrior_Attack_Sting_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Sting_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_Sting_L::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 12;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_PLAYER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.3f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
	
	Add_KeyFrame(40, 0);
	Add_KeyFrame(38, 1);
	Add_KeyFrame(50, 2);

    return S_OK;
}

void CWarrior_Attack_Sting_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_Sting_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PLAYER_L;

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_L;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_Sting_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWarrior_Attack_Sting_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */

    if (MOUSE_MOVE(MMS_WHEEL) > 0)
        return m_eStateType;


    return STATE_END;
}

void		CWarrior_Attack_Sting_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
