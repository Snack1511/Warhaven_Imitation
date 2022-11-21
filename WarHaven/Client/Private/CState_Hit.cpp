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

    m_iFallHitIndex = 0;
    m_iFlyHitIndex = 1;

    m_iGroggyIndex[GROGGY_STATE_E] = 3;
    m_iGroggyIndex[GROGGY_STATE_N] = 4;
    m_iGroggyIndex[GROGGY_STATE_S] = 5;
    m_iGroggyIndex[GROGGY_STATE_W] = 6;
    m_iGroggyIndex[GROGGY_STATE_PLACE] = 7;

    m_iGuardIndex[GUARD_STATE_F] = 8;
    m_iGuardIndex[GUARD_STATE_L] = 9;
    m_iGuardIndex[GUARD_STATE_R] = 10;
    m_iGuardIndex[GUARD_STATE_TOP] = 11;

    m_iHitIndex[HIT_STATE_E] = 12;
    m_iHitIndex[HIT_STATE_N] = 13;
    m_iHitIndex[HIT_STATE_S] = 14;
    m_iHitIndex[HIT_STATE_W] = 15;

    m_iHitStabIndex[HIT_STATE_E] = 16;
    m_iHitStabIndex[HIT_STATE_N] = 17;
    m_iHitStabIndex[HIT_STATE_S] = 18;
    m_iHitStabIndex[HIT_STATE_W] = 19;


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

    

    // if(ePrevStateType == STATE_HIT)


    /* 방향 조정 */
    if (m_tHitInfo.bFace)
    {
        if (m_tHitInfo.eHitType == HIT_TYPE::eLEFT)
            m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;
            
        else  if (m_tHitInfo.eHitType == HIT_TYPE::eRIGHT)
            m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
    }


    if (m_tHitInfo.bFace)
    {
        if (m_tHitInfo.eHitType == HIT_TYPE::eUP)
            m_tHitInfo.eHitType = HIT_TYPE::eDOWN;

        else  if (m_tHitInfo.eHitType == HIT_TYPE::eDOWN)
            m_tHitInfo.eHitType = HIT_TYPE::eUP;
    }


    // 가드 브레이크 시전 중
    if (m_tHitInfo.bGuardBreak)
    {
        // 가드 중에 가드 브레이크를 맞은 경우
    }


    /* 상대가 찌르기를 시전했을 경우 */
    if(m_tHitInfo.bSting)
    {
        if (!m_iHitStabIndex[HIT_STATE_N])
            m_iAnimIndex = m_iHitStabIndex[HIT_STATE_N];

        else
        {
            if (m_tHitInfo.eHitType == HIT_TYPE::eUP)
            {
                m_eAnimType = ANIM_HIT;
                m_iAnimIndex = m_iHitIndex[HIT_STATE_S];
            }
            else if (m_tHitInfo.eHitType == HIT_TYPE::eDOWN)
            {
                m_eAnimType = ANIM_HIT;
                m_iAnimIndex = m_iHitIndex[HIT_STATE_N];
            }
        }
            m_iAnimIndex = m_iHitIndex[HIT_STATE_N];

        __super::Enter(pOwner, pAnimator, ePrevStateType);
        return;
    }


    /* 실제 처리 */
    switch (m_tHitInfo.eHitType)
    {
        /* 내가 기울어지는 방향대로 애니메이션 처리 */
    case HIT_TYPE::eLEFT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_W];

        break;

    case HIT_TYPE::eRIGHT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_E];

        break;

    case HIT_TYPE::eUP:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_S];

        break;

    case HIT_TYPE::eDOWN:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_N];

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

void CState_Hit::Face_Change(_uint iDest, _uint iSour)
{
    if (m_tHitInfo.bFace)
    {
        if (m_tHitInfo.eHitType == HIT_TYPE(iDest))
            m_tHitInfo.eHitType = HIT_TYPE(iSour);

        else  if (m_tHitInfo.eHitType == HIT_TYPE(iSour))
            m_tHitInfo.eHitType = HIT_TYPE(iDest);
    }
}
