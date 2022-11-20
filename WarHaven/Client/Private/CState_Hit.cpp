#include "stdafx.h"
#include "CState_Hit.h"

#include "UsefulHeaders.h"

CState_Hit::CState_Hit()
{
}

CState_Hit::~CState_Hit()
{
}

HRESULT CState_Hit::Initialize()
{
    m_fInterPolationTime = 0.1f;
    return S_OK;
}

void CState_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
    /* 날 때린놈의 hit info를 받았다. */
    m_tHitInfo = *((HIT_INFO*)(pData));

    if (!m_tHitInfo.vDir.Is_Zero())
        pOwner->Get_PhysicsCom()->Set_Dir(m_tHitInfo.vDir);

    if (!m_tHitInfo.vLook.Is_Zero())
        pOwner->Get_Transform()->Set_LerpLook(m_tHitInfo.vLook, 0.3f);

    if (m_tHitInfo.fJumpPower > 0.f)
        pOwner->Get_PhysicsCom()->Set_Jump(m_tHitInfo.fJumpPower);

    if (m_tHitInfo.fKnockBackPower > 0.f)
        pOwner->Get_PhysicsCom()->Set_Speed(m_tHitInfo.fKnockBackPower);

    

    /* 방향 조정 */
    if (m_tHitInfo.bFace)
    {
        if (m_tHitInfo.eHitType == HIT_TYPE::eLEFT)
            m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
            
        else  if (m_tHitInfo.eHitType == HIT_TYPE::eRIGHT)
            m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
    }



    /* 실제 처리 */
    switch (m_tHitInfo.eHitType)
    {
        /* 내가 기울어지는 방향대로 애니메이션 처리 */
    case HIT_TYPE::eLEFT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitLeftIndex;

        break;

    case HIT_TYPE::eRIGHT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitRightIndex;

        break;

    default:
        break;
    }

    __super::Enter(pOwner, pAnimator, ePrevStateType);
}

STATE_TYPE CState_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{



    return __super::Tick(pOwner, pAnimator);
}

void CState_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
}
