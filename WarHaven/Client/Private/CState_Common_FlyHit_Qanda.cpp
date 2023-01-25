#include "stdafx.h"
#include "CState_Common_FlyHit_Qanda.h"

#include "GameInstance.h"
#include "Physics.h"
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

#include "CUtility_Transform.h"

CState_Common_FlyHit_Qanda::CState_Common_FlyHit_Qanda()
{
}

CState_Common_FlyHit_Qanda::~CState_Common_FlyHit_Qanda()
{
}

CState_Common_FlyHit_Qanda* CState_Common_FlyHit_Qanda::Create()
{
    CState_Common_FlyHit_Qanda* pInstance = new CState_Common_FlyHit_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_FlyHit_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_FlyHit_Qanda::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iFlyHitIndex;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FLYHIT_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.f;

    m_iStateChangeKeyFrame = 99;


    return S_OK;
}

void CState_Common_FlyHit_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */
    if (pData)
    {
        m_tHitInfo = *((HIT_INFO*)(pData));
        __super::Fly_State();
    }

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_FlyHit_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() > m_tHitInfo.iLandKeyFrame)
        return AI_STATE_COMBAT_SHADOWSTEP_QANDA;


    if (pAnimator->Is_CurAnimFinished())    
        return AI_STATE_COMBAT_SHADOWSTEP_QANDA;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_FlyHit_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_FlyHit_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return STATE_END;
}
