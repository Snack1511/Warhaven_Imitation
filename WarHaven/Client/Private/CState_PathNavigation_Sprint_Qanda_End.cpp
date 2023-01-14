#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Qanda_End.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "UsefulHeaders.h"

CState_PathNavigation_Sprint_Qanda_End::CState_PathNavigation_Sprint_Qanda_End()
{
}

CState_PathNavigation_Sprint_Qanda_End::~CState_PathNavigation_Sprint_Qanda_End()
{
}

CState_PathNavigation_Sprint_Qanda_End* CState_PathNavigation_Sprint_Qanda_End::Create()
{
    CState_PathNavigation_Sprint_Qanda_End* pInstance = new CState_PathNavigation_Sprint_Qanda_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Qanda_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Qanda_End::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 33;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTEND_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 30;

    Add_KeyFrame(18, 222);
    Add_KeyFrame(29, 333);

  
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Qanda_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(0.3f, 0.8f);

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    CState_PathNavigation::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fAIDelayTime > m_fRand)
        return AI_STATE_PATHNAVIGATION_DEFAULT_QANDA;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Qanda_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.F;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Qanda_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}


void CState_PathNavigation_Sprint_Qanda_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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