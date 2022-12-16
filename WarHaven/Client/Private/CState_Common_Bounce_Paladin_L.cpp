#include "stdafx.h"
#include "CState_Common_Bounce_Paladin_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Common_Bounce_Paladin_L::CState_Common_Bounce_Paladin_L()
{
}

CState_Common_Bounce_Paladin_L::~CState_Common_Bounce_Paladin_L()
{
}

CState_Common_Bounce_Paladin_L* CState_Common_Bounce_Paladin_L::Create()
{
    CState_Common_Bounce_Paladin_L* pInstance = new CState_Common_Bounce_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Bounce_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Bounce_Paladin_L::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_BOUNCE_PALADIN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    return S_OK;
}

void CState_Common_Bounce_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Bounce_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEFAULT_PALADIN_L;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Bounce_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_Common_Bounce_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}