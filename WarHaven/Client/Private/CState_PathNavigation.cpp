#include "stdafx.h"
#include "CState_PathNavigation.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

CState_PathNavigation::CState_PathNavigation()
{
}

CState_PathNavigation::~CState_PathNavigation()
{
}

HRESULT CState_PathNavigation::Initialize()
{
	if(m_eJumpFallStateType != STATE_END)
		m_vecAdjState.push_back(m_eJumpFallStateType);

    return S_OK;
}

void CState_PathNavigation::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	m_fAIDelayTime += fDT(0);

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

void CState_PathNavigation::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
 
}

STATE_TYPE CState_PathNavigation::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

