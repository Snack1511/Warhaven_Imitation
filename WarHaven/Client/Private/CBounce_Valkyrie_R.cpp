#include "stdafx.h"
#include "CBounce_Valkyrie_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Valkyrie_R::CBounce_Valkyrie_R()
{
}

CBounce_Valkyrie_R::~CBounce_Valkyrie_R()
{
}

CBounce_Valkyrie_R* CBounce_Valkyrie_R::Create()
{
    CBounce_Valkyrie_R* pInstance = new CBounce_Valkyrie_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Valkyrie_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Valkyrie_R::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_VALKYRIE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);

    return S_OK;
}

void CBounce_Valkyrie_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_Valkyrie_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Valkyrie_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_Valkyrie_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  공격하다 땅에 튕기면
    */

	return __super::Check_Condition(pOwner, pAnimator);

}