#include "stdafx.h"
#include "CWarrior_GuardBreak.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CWarrior_GuardBreak::CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak::~CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak* CWarrior_GuardBreak::Create()
{
    CWarrior_GuardBreak* pInstance = new CWarrior_GuardBreak();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_GuardBreak::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_GUARDBREAK;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 65;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


	Add_KeyFrame(26, 1);
	Add_KeyFrame(54, 2);

    return S_OK;
}

void CWarrior_GuardBreak::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->On_Use(CUnit::SKILL2);

    /* Owner의 Animator Set Idle로 */
    m_fMaxSpeed = pOwner->Get_Status().fSprintAttackSpeed;
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWarrior_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  스킬버튼 을 이용해 공격한다.
    */

    if (!pOwner->Can_Use(CUnit::SKILL2))
        return STATE_END;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::E)
        return m_eStateType;


    return STATE_END;
}


void CWarrior_GuardBreak::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1:
        pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;


	case 2:
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}


}