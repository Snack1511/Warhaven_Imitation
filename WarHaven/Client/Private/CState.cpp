#include "stdafx.h"
#include "CState.h"

#include "CAnimator.h"
#include "CState_Manager.h"

#include "GameInstance.h"
#include "CUser.h"
#include "Functor.h"
#include "Transform.h"
#include "CUnit.h"
CState::CState()
{
}

CState::~CState()
{
}

void CState::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType)
{
    CUser::Get_Instance()->Clear_KeyCommands();
    m_fTimeAcc = 0.f;
    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, m_fInterPolationTime);
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);
    m_bExecuted = false;
}

STATE_TYPE CState::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //if (!m_wstrSoundKey.empty())
    //{
    //    m_fLoopSoundAcc += fDT(0);

    //    if (m_fLoopSoundAcc > m_fSoundLoopTime)
    //    {
    //        m_fLoopSoundAcc = 0.f;
    //        //CFunctor::Play_Sound(m_wstrSoundKey, m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

    //    }
    //}

    Check_KeyFrameEvent(pOwner, pAnimator);

    STATE_TYPE eType = STATE_END;

    if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
        return eType;

    for (auto& elem : m_vecAdjState)
    {
        eType = CState_Manager::Get_Instance()->Get_State(elem)->Check_Condition(pOwner, pAnimator);

        if (eType != STATE_END)
            break;
    }


    return eType;
}

void CState::Re_Enter(CUnit* pOwner, CAnimator* pAnimator, _float fInterpolationTime, _float fAnimSpeed)
{

    if (fInterpolationTime >= 0.f)
        m_fInterPolationTime = fInterpolationTime;
    
    if (fAnimSpeed >= 0.f)
        m_fAnimSpeed = fAnimSpeed;

    Enter(pOwner, pAnimator, m_eStateType);
}


void CState::Check_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator)
{
    _uint iCurKeyFrame = pAnimator->Get_CurAnimFrame();
    
    _uint iSize = (_uint)m_vecKeyFrameEvent.size();
    for (_uint i = 0; i < iSize; ++i)
    {
        if (iCurKeyFrame >= m_vecKeyFrameEvent[i].iKeyFrame)
        {
            if (m_vecKeyFrameEvent[i].bExecuted)
                continue;

            On_KeyFrameEvent(pOwner, pAnimator, m_vecKeyFrameEvent[i], i);
            m_vecKeyFrameEvent[i].bExecuted = true;
        }
    }
}

void CState::Add_KeyFrame(_uint iKeyFrameIndex, _uint eEventType)
{
    if (eEventType >= KEYFRAME_EVENT::EVENT_END)
        return;

    KEYFRAME_EVENT  tEvent;
    tEvent.iKeyFrame = iKeyFrameIndex;
    tEvent.eEventType = (KEYFRAME_EVENT::EVENT_TYPE)eEventType;
    
    m_vecKeyFrameEvent.push_back(tEvent);
    
}
