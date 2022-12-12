#include "stdafx.h"
#include "CState_Patrol_Switch_Warrior_CSwitchLtoR.h"

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

CState_Patrol_Switch_Warrior_CSwitchLtoR::CState_Patrol_Switch_Warrior_CSwitchLtoR()
{
}

CState_Patrol_Switch_Warrior_CSwitchLtoR::~CState_Patrol_Switch_Warrior_CSwitchLtoR()
{
}

CState_Patrol_Switch_Warrior_CSwitchLtoR* CState_Patrol_Switch_Warrior_CSwitchLtoR::Create()
{
    CState_Patrol_Switch_Warrior_CSwitchLtoR* pInstance = new CState_Patrol_Switch_Warrior_CSwitchLtoR();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Switch_Warrior_CSwitchLtoR");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Switch_Warrior_CSwitchLtoR::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 27;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_WARRIOR_SWITCH_L_TO_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.5f;


    return S_OK;
}

void CState_Patrol_Switch_Warrior_CSwitchLtoR::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Switch_Warrior_CSwitchLtoR::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATROL_DEAFULT_WARRIOR_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Switch_Warrior_CSwitchLtoR::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Switch_Warrior_CSwitchLtoR::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
