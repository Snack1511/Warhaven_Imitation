#include "stdafx.h"
#include "CState_Patrol_Switch_Fiona_CSwitchRtoL.h"

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

CState_Patrol_Switch_Fiona_CSwitchRtoL::CState_Patrol_Switch_Fiona_CSwitchRtoL()
{
}

CState_Patrol_Switch_Fiona_CSwitchRtoL::~CState_Patrol_Switch_Fiona_CSwitchRtoL()
{
}

CState_Patrol_Switch_Fiona_CSwitchRtoL* CState_Patrol_Switch_Fiona_CSwitchRtoL::Create()
{
    CState_Patrol_Switch_Fiona_CSwitchRtoL* pInstance = new CState_Patrol_Switch_Fiona_CSwitchRtoL();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Switch_Fiona_CSwitchRtoL");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Switch_Fiona_CSwitchRtoL::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 43;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_FIONA_SWITCH_R_TO_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 3.5f;

    return S_OK;
}

void CState_Patrol_Switch_Fiona_CSwitchRtoL::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Switch_Fiona_CSwitchRtoL::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATROL_DEFAULT_FIONA_L;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Switch_Fiona_CSwitchRtoL::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Switch_Fiona_CSwitchRtoL::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
