#include "stdafx.h"
#include "CRun_WarHammer_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_WarHammer_Begin::CRun_WarHammer_Begin()
{
}

CRun_WarHammer_Begin::~CRun_WarHammer_Begin()
{
}

HRESULT CRun_WarHammer_Begin::Initialize()
{

	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);


    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;


	m_fMyAccel = 10.f;
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 0;

	m_fInterPolationTime = 0.1f;



    return S_OK;
}

void CRun_WarHammer_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_WarHammer_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	_uint iDirection = Get_Direction();

	DoMove(iDirection, pOwner);


	if (iDirection < 8)
		m_iCurDirection = iDirection;


    return __super::Tick(pOwner, pAnimator);
}

void CRun_WarHammer_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	m_pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CRun_WarHammer_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    // 뛰어갈 준비를 한다.
    if (KEY(W, HOLD) ||
        KEY(A, HOLD) ||
        KEY(D, HOLD))
    {

        return m_eStateType;
    }


    return STATE_END;
}


void CRun_WarHammer_Begin::Change_Location_Begin(_uint iDirection, CAnimator* pAnimator)
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

void CRun_WarHammer_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}