#include "stdafx.h"
#include "CState_Combat_GuardBreak_Warrior.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_GuardBreak_Warrior::CState_Combat_GuardBreak_Warrior()
{
}

CState_Combat_GuardBreak_Warrior::~CState_Combat_GuardBreak_Warrior()
{
}

CState_Combat_GuardBreak_Warrior* CState_Combat_GuardBreak_Warrior::Create()
{
    CState_Combat_GuardBreak_Warrior* pInstance = new CState_Combat_GuardBreak_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardBreak_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardBreak_Warrior::Initialize()
{
    m_tHitInfo.eHitType = HIT_TYPE::eUP;
    m_tHitInfo.fKnockBackPower = 1.f;
    m_tHitInfo.fJumpPower = 0.f;
    m_tHitInfo.bGuardBreak = true;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_GUARDBREAK_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 65;


	Add_KeyFrame(26, 1);
	Add_KeyFrame(54, 2);

    return S_OK;
}

void CState_Combat_GuardBreak_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->On_Use(CUnit::SKILL1);

    m_iDirectionRand = STATE_DIRECTION_N;

    pOwner->CallBack_CollisionEnter += bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

    CColorController::COLORDESC tColorDesc;
    ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    tColorDesc.fFadeInStartTime = 0.f;
    tColorDesc.fFadeInTime = 0.1f;
    tColorDesc.fFadeOutStartTime = 1.f;
    tColorDesc.fFadeOutTime = 0.1f;
    tColorDesc.vTargetColor = _float4((80.f / 255.f), (0.f / 255.f), (0.f / 255.f), 0.1f);
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

STATE_TYPE CState_Combat_GuardBreak_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bHit && m_bKeyInputable)
        return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R;

    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        return AI_STATE_COMBAT_DEFAULT_WARRIOR_R;

    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardBreak_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CState::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
    
    pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
}

STATE_TYPE CState_Combat_GuardBreak_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_GuardBreak_Warrior::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
    if (iOtherColType == COL_REDHITBOX_BODY || 
        iOtherColType == COL_REDHITBOX_HEAD ||
        iOtherColType == COL_REDGUARD       ||
        
        iOtherColType == COL_BLUEHITBOX_BODY ||
        iOtherColType == COL_BLUEHITBOX_HEAD ||
        iOtherColType == COL_BLUEGUARD)
        m_bHit = true;
}

void CState_Combat_GuardBreak_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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