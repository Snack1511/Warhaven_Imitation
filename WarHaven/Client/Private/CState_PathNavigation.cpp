#include "stdafx.h"
#include "CState_PathNavigation.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CTrigger.h"
#include "CPath.h"
#include "CAI_MoveStateUtility.h"

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
	m_vOriPos = pOwner->Get_Transform()->Get_World(WORLD_POS);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fAIDelayTime += fDT(0);


	/* Path 타기 */

	_float4 vDir;
	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	CPath* pCurPath = pOwner->Get_CurPath();

	if (!pCurPath)
		return __super::Tick(pOwner, pAnimator);

	pCurPath->Lock();
	pCurPath->Update_CurrentIndex(vCurPos);

	/* 도착했는지 반드시 확인 */
	if (pCurPath->Is_Arrived())
	{
		pCurPath->UnLock();
		return __super::Tick(pOwner, pAnimator);

	}


	vDir = pCurPath->Get_CurDir(vCurPos, false);

	// 만약 Path 타야하는데 Dir로 Ray쏴서 벽에 막히면, 다른 가장 가까운 Release Path를 찾아야 한다.

	if (m_fAIDelayTime > 2.f)
	{
		m_fAIDelayTime = 0.f;
		_float4 vRayStartPos = vCurPos;
		vRayStartPos.y += 0.5f;


		if (GAMEINSTANCE->Shoot_RaytoStaticActors(nullptr, nullptr, vRayStartPos, vDir, pCurPath->Get_CurLength(vCurPos) * 0.8f))
		{
			/* 만약 main path 타고있던거면 main path 그냥 다 탄걸로 처리해 */
			if (m_pOwner->Get_StartMainPath() == pCurPath)
			{
				m_pOwner->Get_StartMainPath()->Set_Arrived();
				/*현재 Path 갱신해야하고 MainPath 지우면 안되기 때문에 null 처리*/
				m_pOwner->Get_OwnerPlayer()->Set_CurPathNull();
			}

			pCurPath = CGameSystem::Get_Instance()->Clone_RandomReleasePath(vCurPos);

			if (!pCurPath)
			{
				/* Path 못찾았으면 main path 다시 */
				pCurPath = m_pOwner->Get_StartMainPath();
				m_pOwner->Get_OwnerPlayer()->Set_NewPath(pCurPath);
			}
			else
			{
				pOwner->Get_OwnerPlayer()->Set_NewPath(pCurPath);
			}
		}
	}
	

	/* Path 따라가는 코드 */
	vDir = pCurPath->Get_CurDir(vCurPos);

	vDir.y = 0.f;
	pOwner->Get_Transform()->Set_LerpLook(vDir, 0.4f);
	//pOwner->Get_PhysicsCom()->Set_Dir(vDir);
	_float4 vCurDir = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	vCurDir.y = 0.f;
	pOwner->Get_PhysicsCom()->Set_Dir(vCurDir);
	pOwner->Get_PhysicsCom()->Set_Accel(100.f);
	pCurPath->UnLock();

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
 
}

STATE_TYPE CState_PathNavigation::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

