#include "stdafx.h"
#include "CBounce_SpearMan.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_SpearMan::CBounce_SpearMan()
{
}

CBounce_SpearMan::~CBounce_SpearMan()
{
}

CBounce_SpearMan* CBounce_SpearMan::Create()
{
    CBounce_SpearMan* pInstance = new CBounce_SpearMan();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_SpearMan");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_SpearMan::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_SPEARMAN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 50;

	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);

    m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);


	m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CBounce_SpearMan::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_SpearMan::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_SpearMan::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBounce_SpearMan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  공격하다 땅에 튕기면
    */

	if (pOwner->Is_Weapon_R_Collision())
		return m_eStateType;

    return STATE_END;
}