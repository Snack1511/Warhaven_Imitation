#include "stdafx.h"
#include "CRun_AI_TG_Warrior_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_AI_TG_Warrior_Begin::CRun_AI_TG_Warrior_Begin()
{
}

CRun_AI_TG_Warrior_Begin::~CRun_AI_TG_Warrior_Begin()
{
}

HRESULT CRun_AI_TG_Warrior_Begin::Initialize()
{
	m_vecAdjState.push_back(AI_STATE_GUARD_BEGIN_WARRIOR);

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;


	Add_KeyFrame(9, 0);

	m_fMyAccel = 10.f;
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 21;

	m_fInterPolationTime = 0.1f;

	m_fAIMyLength = 1.5f;

    return S_OK;
}

void CRun_AI_TG_Warrior_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;
	m_fMaxSpeed = pOwner->Get_Status().fRunBeginSpeed;

	if (0 != rand() % 3)
		m_bAIMove = true;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_AI_TG_Warrior_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	//CUnit* pUnit = pOwner->Get_TargetUnit();

	//pOwner->Set_LookToTarget();

	//_float fLength = Move_Direction_Loop_AI(pOwner);

	if (Get_Length(pOwner) < m_fAIMyLength)
		return m_iAINextState;
	

	DoMove_AI(pOwner, pAnimator);


    return __super::Tick(pOwner, pAnimator);
}

void CRun_AI_TG_Warrior_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_AI_TG_Warrior_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    //// 뛰어갈 준비를 한다.
    //if (KEY(W, HOLD) ||
    //    KEY(A, HOLD) ||
    //    KEY(S, HOLD) ||
    //    KEY(D, HOLD))
    //{

    //    return m_eStateType;
    //}


    return STATE_END;
}


void CRun_AI_TG_Warrior_Begin::Change_Location_Begin(_uint iDirection, CAnimator* pAnimator)
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

void CRun_AI_TG_Warrior_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS);
		break;
	}

}