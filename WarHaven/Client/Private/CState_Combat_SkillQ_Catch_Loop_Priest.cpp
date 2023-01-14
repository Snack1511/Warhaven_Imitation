#include "stdafx.h"
#include "CState_Combat_SkillQ_Catch_Loop_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"
#include "CUnit_Priest.h"


CState_Combat_SkillQ_Catch_Loop_Priest::CState_Combat_SkillQ_Catch_Loop_Priest()
{
}

CState_Combat_SkillQ_Catch_Loop_Priest::~CState_Combat_SkillQ_Catch_Loop_Priest()
{
}

CState_Combat_SkillQ_Catch_Loop_Priest* CState_Combat_SkillQ_Catch_Loop_Priest::Create()
{
    CState_Combat_SkillQ_Catch_Loop_Priest* pInstance = new CState_Combat_SkillQ_Catch_Loop_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Catch_Loop_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_SkillQ_Catch_Loop_Priest::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_CATCH_LOOP_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 999;
    m_fMyMaxLerp = 0.4f;

	return __super::Initialize();
}

void CState_Combat_SkillQ_Catch_Loop_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	pOwner->Enable_GuardCollider(true);

	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Voice(pOwner, L"Voice_Catch", m_fAIDeafultVolume);
}

STATE_TYPE CState_Combat_SkillQ_Catch_Loop_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Get_CatchProjectileObject() || pOwner->Get_CatchedBall())
		return AI_STATE_COMBAT_CATCHING_PRIEST;

    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEFAULT_PRIEST;

    CUnit* pUnit = pOwner->Get_TargetUnit();
    CTransform* pMyTransform = pOwner->Get_Transform();

    _float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
    vLook.y = 0.f;
    pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Catch_Loop_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->On_Use(CUnit::SKILL3);
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(false);

	pOwner->Enable_GuardCollider(false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_Catch_Loop_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Combat_SkillQ_Catch_Loop_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
