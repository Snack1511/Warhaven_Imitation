#include "stdafx.h"
#include "CState_Defeat.h"

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

CState_Defeat::CState_Defeat()
{
}

CState_Defeat::~CState_Defeat()
{
}

CState_Defeat* CState_Defeat::Create()
{
    CState_Defeat* pInstance = new CState_Defeat();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Defeat");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Defeat::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_DEFEAT;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_fAnimSpeed = 2.f;

    return S_OK;
}

void CState_Defeat::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Defeat::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CState_Defeat::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CState_Defeat::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
