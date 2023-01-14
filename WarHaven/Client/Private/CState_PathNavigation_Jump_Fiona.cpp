#include "stdafx.h"
#include "CState_PathNavigation_Jump_Fiona.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Jump_Fiona::CState_PathNavigation_Jump_Fiona()
{
}

CState_PathNavigation_Jump_Fiona::~CState_PathNavigation_Jump_Fiona()
{
}

HRESULT CState_PathNavigation_Jump_Fiona::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_PathNavigation_Jump_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Jump_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Jump_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_PathNavigation_Jump_Fiona::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}

