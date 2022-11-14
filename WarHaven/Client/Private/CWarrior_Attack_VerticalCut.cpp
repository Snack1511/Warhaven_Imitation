#include "stdafx.h"
#include "CWarrior_Attack_VerticalCut.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_VerticalCut::CWarrior_Attack_VerticalCut()
{
}

CWarrior_Attack_VerticalCut::~CWarrior_Attack_VerticalCut()
{
}

CWarrior_Attack_VerticalCut* CWarrior_Attack_VerticalCut::Create()
{
    CWarrior_Attack_VerticalCut* pInstance = new CWarrior_Attack_VerticalCut();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_VerticalCut::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_VERTICALCUT;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.3f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	Add_KeyFrame(36, 0);
	Add_KeyFrame(31, 1);
	Add_KeyFrame(50, 2);

    return S_OK;
}

void CWarrior_Attack_VerticalCut::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	m_bMoveTrigger = true;

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Attack_VerticalCut::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_PLAYER_R;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_VerticalCut::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

    //Exit에선 무조건 남겨놔야함
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);


}

STATE_TYPE CWarrior_Attack_VerticalCut::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */
   
    if (MOUSE_MOVE(MMS_WHEEL) < 0)
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Attack_VerticalCut::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
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
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	case 2:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, m_bAttackTrigger);
		break;

	default:
		break;
	}

}
