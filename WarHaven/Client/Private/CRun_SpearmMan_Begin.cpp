#include "stdafx.h"
#include "CRun_SpearmMan_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_SpearmMan_Begin::CRun_SpearmMan_Begin()
{
}

CRun_SpearmMan_Begin::~CRun_SpearmMan_Begin()
{
}

HRESULT CRun_SpearmMan_Begin::Initialize()
{
	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);
	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);

    m_iStateChangeKeyFrame = 21;

    m_fInterPolationTime = 0.f;


	m_fMyAccel = 10.f;
	m_fAnimSpeed = 4.f;

	m_iStateChangeKeyFrame = 21;

	m_fInterPolationTime = 0.1f;


    return S_OK;
}

void CRun_SpearmMan_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	m_fMaxSpeed = pOwner->Get_Status().fRunBeginSpeed;


	Physics_Setting(m_fMaxSpeed, pOwner, false);

	Add_KeyFrame(5, 0);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_SpearmMan_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //if (CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
    //    return STATE_SPRINT_BEGIN_PLAYER;

	_uint iDirection = Get_Direction();

	Change_Location_Begin(iDirection, pAnimator);

	Move(iDirection, pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CRun_SpearmMan_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_SpearmMan_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(LSHIFT, NONE))
	{
		if (
			KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(D, HOLD)
			)
		{
			return m_eStateType;
		}
	}

    return STATE_END;
}


void CRun_SpearmMan_Begin::Change_Location_Begin(_uint iDirection, CAnimator* pAnimator)
{
	if (!m_bMoveTrigger)
	{
		_int iFrame = pAnimator->Get_CurAnimFrame();

		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];

		pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
		pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);

		pAnimator->Set_CurFrame(iFrame);
	}


	m_bMoveTrigger = true;
}

void CRun_SpearmMan_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
	{
		Physics_Setting(m_fMaxSpeed, pOwner, false);
	}


	default:
		break;
	}


}