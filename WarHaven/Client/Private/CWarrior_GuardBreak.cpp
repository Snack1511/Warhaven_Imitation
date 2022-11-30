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
    m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
    m_tHitInfo.fKnockBackPower = 1.f;
    m_tHitInfo.fJumpPower = 0.f;
    m_tHitInfo.bGuardBreak = true;

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
    pOwner->On_Use(CUnit::SKILL1);

    pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


    CColorController::COLORDESC tColorDesc;
    ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    tColorDesc.fFadeInStartTime = 0.f;
    tColorDesc.fFadeInTime = 0.1f;
    tColorDesc.fFadeOutStartTime = 1.f;
    tColorDesc.fFadeOutTime = 0.1f;
    tColorDesc.vTargetColor = _float4((230.f / 255.f), (10.f / 255.f), (10.f / 255.f), 0.1f);
    //tColorDesc.vTargetColor *= 1.1f;
    tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    tColorDesc.iStartKeyFrame = 2;
    tColorDesc.iEndKeyFrame = 54; // 프레임 맞춰놓음

    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


   
    

    /* Owner의 Animator Set Idle로 */
    m_fMaxSpeed = pOwner->Get_Status().fSprintAttackSpeed;
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bKeyInputable)
    {
        if (KEY(LBUTTON, TAP))
            m_bKeyInput = true;
    }

    if (m_bHit && m_bKeyInput)
    {
        return STATE_ATTACK_HORIZONTALDOWN_L;
    }

    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
    
    pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
}

STATE_TYPE CWarrior_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  스킬버튼 을 이용해 공격한다.
    */

    if (!pOwner->Can_Use(CUnit::SKILL1))
        return STATE_END;

    if (KEY(E, TAP))
        return m_eStateType;


    return STATE_END;
}

void CWarrior_GuardBreak::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
    if (iOtherColType == COL_ENEMYHITBOX_BODY || 
        iOtherColType == COL_ENEMYHITBOX_HEAD ||
        iOtherColType == COL_ENEMYGUARD)
        m_bHit = true;
}

void CWarrior_GuardBreak::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1:
        m_bKeyInputable = true;
        pOwner->Get_PhysicsCom()->Set_SpeedasMax();
        pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);

		break;


	case 2:
        pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
		break;

	default:
		break;
	}


}