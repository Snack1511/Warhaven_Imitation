#include "stdafx.h"
#include "CState_Victory.h"

#include "GameInstance.h"
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

CState_Victory::CState_Victory()
{
}

CState_Victory::~CState_Victory()
{
}

CState_Victory* CState_Victory::Create()
{
    CState_Victory* pInstance = new CState_Victory();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Victory");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Victory::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 37;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_VICTORY;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fAnimSpeed = 2.f;

    return S_OK;
}

void CState_Victory::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Victory::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CState_Victory::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Victory::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
