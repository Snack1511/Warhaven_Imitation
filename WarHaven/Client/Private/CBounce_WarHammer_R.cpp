#include "stdafx.h"
#include "CBounce_WarHammer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_WarHammer_R::CBounce_WarHammer_R()
{
}

CBounce_WarHammer_R::~CBounce_WarHammer_R()
{
}

CBounce_WarHammer_R* CBounce_WarHammer_R::Create()
{
    CBounce_WarHammer_R* pInstance = new CBounce_WarHammer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_WarHammer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_WarHammer_R::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);

    return S_OK;
}

void CBounce_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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

STATE_TYPE CBounce_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Attack 으로 오는 조건
    1.  공격하다 땅에 튕기면
    */

	return __super::Check_Condition(pOwner, pAnimator);

}