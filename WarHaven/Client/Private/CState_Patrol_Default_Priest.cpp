#include "stdafx.h"
#include "CState_Patrol_Default_Priest.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Patrol_Default_Priest::CState_Patrol_Default_Priest()
{
}

CState_Patrol_Default_Priest::~CState_Patrol_Default_Priest()
{
}

CState_Patrol_Default_Priest* CState_Patrol_Default_Priest::Create()
{
    CState_Patrol_Default_Priest* pInstance = new CState_Patrol_Default_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEFAULT_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 100.f;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    m_iIdleDelay = 30;

    return __super::Initialize();
}

void CState_Patrol_Default_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iRand = random(0, 5);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CUnit* pCurObject = static_cast<CUnit*>(pOwner->Get_CureObject());

    if (pCurObject)
    {
        if(pCurObject->Get_Status().fHP < pCurObject->Get_Status().fMaxHP)
            return AI_STATE_PATROL_CURE_BEGIN_PRIEST;
    }
        

    if (m_iRand != 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            return AI_STATE_PATROL_WALK_PRIEST;
        }
    }
    else
    {
        return m_eStateType;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Patrol_Default_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
