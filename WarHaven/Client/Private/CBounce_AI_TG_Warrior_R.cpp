#include "stdafx.h"
#include "CBounce_AI_TG_Warrior_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_AI_TG_Warrior_R::CBounce_AI_TG_Warrior_R()
{
}

CBounce_AI_TG_Warrior_R::~CBounce_AI_TG_Warrior_R()
{
}

CBounce_AI_TG_Warrior_R* CBounce_AI_TG_Warrior_R::Create()
{
    CBounce_AI_TG_Warrior_R* pInstance = new CBounce_AI_TG_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_AI_TG_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_AI_TG_Warrior_R::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_BOUNE_WARRIOR_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(AI_STATE_RUN_WARRIOR_R);
    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_R);

    return S_OK;
}

void CBounce_AI_TG_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == STATE_SWITCH_R_TO_L)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_AI_TG_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_AI_TG_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_AI_TG_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  공격하다 땅에 튕기면
    */

	return __super::Check_Condition(pOwner, pAnimator);

}