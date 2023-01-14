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


	/* Path Ÿ�� */

	_float4 vDir;
	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	CPath* pCurPath = pOwner->Get_CurPath();

	if (!pCurPath)
		return __super::Tick(pOwner, pAnimator);

	pCurPath->Lock();
	pCurPath->Update_CurrentIndex(vCurPos);

	/* �����ߴ��� �ݵ�� Ȯ�� */
	if (pCurPath->Is_Arrived())
	{
		pCurPath->UnLock();
		return __super::Tick(pOwner, pAnimator);

	}


	vDir = pCurPath->Get_CurDir(vCurPos, false);

	// ���� Path Ÿ���ϴµ� Dir�� Ray���� ���� ������, �ٸ� ���� ����� Release Path�� ã�ƾ� �Ѵ�.

	if (m_fAIDelayTime > 2.f)
	{
		m_fAIDelayTime = 0.f;
		_float4 vRayStartPos = vCurPos;
		vRayStartPos.y += 0.5f;


		if (GAMEINSTANCE->Shoot_RaytoStaticActors(nullptr, nullptr, vRayStartPos, vDir, pCurPath->Get_CurLength(vCurPos) * 0.8f))
		{
			/* ���� main path Ÿ���ִ��Ÿ� main path �׳� �� ź�ɷ� ó���� */
			if (m_pOwner->Get_StartMainPath() == pCurPath)
			{
				m_pOwner->Get_StartMainPath()->Set_Arrived();
				/*���� Path �����ؾ��ϰ� MainPath ����� �ȵǱ� ������ null ó��*/
				m_pOwner->Get_OwnerPlayer()->Set_CurPathNull();
			}

			pCurPath = CGameSystem::Get_Instance()->Clone_RandomReleasePath(vCurPos);

			if (!pCurPath)
			{
				/* Path ��ã������ main path �ٽ� */
				pCurPath = m_pOwner->Get_StartMainPath();
				m_pOwner->Get_OwnerPlayer()->Set_NewPath(pCurPath);
			}
			else
			{
				pOwner->Get_OwnerPlayer()->Set_NewPath(pCurPath);
			}
		}
	}
	

	/* Path ���󰡴� �ڵ� */
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

