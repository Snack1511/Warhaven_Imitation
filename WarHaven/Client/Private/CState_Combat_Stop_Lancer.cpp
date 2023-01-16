#include "stdafx.h"
#include "CState_Combat_Stop_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "UsefulHeaders.h"

CState_Combat_Stop_Lancer::CState_Combat_Stop_Lancer()
{
}

CState_Combat_Stop_Lancer::~CState_Combat_Stop_Lancer()
{
}

CState_Combat_Stop_Lancer* CState_Combat_Stop_Lancer::Create()
{
    CState_Combat_Stop_Lancer* pInstance = new CState_Combat_Stop_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Stop_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Stop_Lancer::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 11;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_STOP_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 75;

    //Add_KeyFrame(18, 222);
    //Add_KeyFrame(29, 333);

    
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_Combat_Stop_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;
    CFunctor::Play_Sound(L"Effect_HorseStop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS), 1.f);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Stop_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        return AI_STATE_COMBAT_DEAFULT_LANCER;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Stop_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Stop_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}


void CState_Combat_Stop_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 222:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    case 333:
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    default:
        break;
    }
}