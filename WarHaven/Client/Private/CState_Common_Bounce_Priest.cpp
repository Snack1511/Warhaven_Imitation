#include "stdafx.h"
#include "CState_Common_Bounce_Priest.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_Bounce_Priest::CState_Common_Bounce_Priest()
{
}

CState_Common_Bounce_Priest::~CState_Common_Bounce_Priest()
{
}

CState_Common_Bounce_Priest* CState_Common_Bounce_Priest::Create()
{
    CState_Common_Bounce_Priest* pInstance = new CState_Common_Bounce_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Bounce_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Bounce_Priest::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_BOUNCE_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    return S_OK;
}

void CState_Common_Bounce_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEFAULT_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_Common_Bounce_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  공격하다 땅에 튕기면
    */

	return __super::Check_Condition(pOwner, pAnimator);

}