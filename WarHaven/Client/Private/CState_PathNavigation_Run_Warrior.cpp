#include "stdafx.h"
#include "CState_PathNavigation_Run_Warrior.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation_Run_Warrior::CState_PathNavigation_Run_Warrior()
{
}

CState_PathNavigation_Run_Warrior::~CState_PathNavigation_Run_Warrior()
{
}

HRESULT CState_PathNavigation_Run_Warrior::Initialize()
{
	__super::Initialize();

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.f;
	

    return S_OK;
}

void CState_PathNavigation_Run_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{	
	m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

	m_iStateChangeKeyFrame = 25;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Run_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	CPath* pCurPath = pOwner->Get_CurPath();

	if (!pCurPath)
	{
		assert(0);
		return STATE_END;
	}

	/* 따라가면 대 */
	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	_float4 vDir = pCurPath->Get_CurDir(pOwner->Get_Transform()->Get_World(WORLD_POS));

	pOwner->Get_Transform()->Set_LerpLook(vDir, 0.4f);
	pOwner->Get_PhysicsCom()->Set_Dir(vDir);
	pOwner->Get_PhysicsCom()->Set_Accel(100.f);


	pCurPath->Update_CurrentIndex(vCurPos);

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Run_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_PathNavigation_Run_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

