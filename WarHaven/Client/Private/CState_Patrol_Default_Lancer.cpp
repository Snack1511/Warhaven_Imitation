#include "stdafx.h"
#include "CState_Patrol_Default_Lancer.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Patrol_Default_Lancer::CState_Patrol_Default_Lancer()
{
}

CState_Patrol_Default_Lancer::~CState_Patrol_Default_Lancer()
{
}

CState_Patrol_Default_Lancer* CState_Patrol_Default_Lancer::Create()
{
    CState_Patrol_Default_Lancer* pInstance = new CState_Patrol_Default_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEFAULT_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 100.f;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    return __super::Initialize();
}

void CState_Patrol_Default_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (m_eStateType == ePrevType)
        m_fInterPolationTime = 0.f;
    else
        m_fInterPolationTime = 0.1f;

    m_iRand = random(0, 5);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
    {
        if(pAnimator->Is_CurAnimFinished())
            return AI_STATE_PATROL_WALK_LANCER;
    }
    else
    {
        if (pAnimator->Is_CurAnimFinished())
            return m_eStateType;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Patrol_Default_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
