#include "stdafx.h"
#include "CAI_SandBack_Hit.h"

#include "GameInstance.h"
#include "Physics.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

#include "CUtility_Transform.h"

CAI_SandBack_Hit::CAI_SandBack_Hit()
{
}

CAI_SandBack_Hit::~CAI_SandBack_Hit()
{
}

CAI_SandBack_Hit* CAI_SandBack_Hit::Create()
{
    CAI_SandBack_Hit* pInstance = new CAI_SandBack_Hit();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAI_SandBack_Hit");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAI_SandBack_Hit::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = m_iHitIndex[0];                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_HIT_TEST_ENEMY;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_IDLE_WARRIOR_R_AI_ENEMY);
    

    return S_OK;
}

void CAI_SandBack_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    if (ePrevType == STATE_GUARD_LOOP_WARRIOR_AI_ENEMY ||
        ePrevType == STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY)
        bTestGuard = true;

    if (ePrevType == SANDBAG_STATE_IDLE_WARHAMMER_R)
        bWarHammerIdle = true;

	Test_Attack(ePrevType);


    /* 날 때린놈의 hit info를 받았다. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Hit_State(pOwner);

    /* Owner의 Animator Set Idle로 */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CAI_SandBack_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Get_CurAnimFrame() > 50)
	{
		if (bTestGuard)
			return STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;

		if (bTestAttack)
			return STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;

        if (bWarHammerIdle)
            return SANDBAG_STATE_IDLE_WARHAMMER_R;
	}



    return __super::Tick(pOwner, pAnimator);
}

void CAI_SandBack_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

void CAI_SandBack_Hit::Test_Attack(STATE_TYPE ePrevType)
{
	if (ePrevType == STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY ||
		ePrevType == STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY ||
        ePrevType == STATE_BOUNCE_WARRIOR_L_AI_ENEMY ||
        ePrevType == STATE_BOUNCE_WARRIOR_R_AI_ENEMY )
		bTestAttack = true;
}

STATE_TYPE CAI_SandBack_Hit::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}
