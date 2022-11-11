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

void CState::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
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
    if (!m_wstrSoundKey.empty())
    {
        m_fLoopSoundAcc += fDT(0);

        if (m_fLoopSoundAcc > m_fSoundLoopTime)
        {
            m_fLoopSoundAcc = 0.f;
            //CFunctor::Play_Sound(m_wstrSoundKey, m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

        }
    }
   


    STATE_TYPE eType = STATE_END;


   /* m_fTimeAcc += fDT(0);


    if (m_fTimeAcc >= m_fDelayTime)
    {
        if (!m_bExecuted)
        {
            OnExecute(pOwner, pAnimator);
            m_bExecuted = true;
        }

        TickExecute(pOwner, pAnimator);
    }

    if (m_fTimeAcc <= m_fStateChangableTime)
        return eType;*/


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

void CState::Change_Animation(_uint iCurAnimIndex, _uint iChangeIndex, CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iAnimIndex == iCurAnimIndex)
    {
        m_iAnimIndex = iChangeIndex;

        Enter(pOwner, pAnimator, iCurAnimIndex);
    }
}

/* bChange 가 true 이면 Change_Animation 호출 */
STATE_TYPE CState::End_Animation(_uint iChangeIndex, ANIM_TYPE eAnimType, STATE_TYPE eStateType, _bool bStateChange, CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_iAnimIndex == iChangeIndex)
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            if (bStateChange)
            {
                m_eAnimType = eAnimType;
                Change_Animation(m_iAnimIndex, iChangeIndex, pOwner, pAnimator);
            }
            else
                return eStateType;

        }
    }


    return STATE_END;
}
