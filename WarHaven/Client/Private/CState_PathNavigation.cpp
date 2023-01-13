#include "stdafx.h"
#include "CState_PathNavigation.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

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

	// 만약에 계속 낀다면 이 로직 한번 사용해보세요.
	if (m_fAIDelayTime > 2.f)
	{
		//_float fLength = m_vOriPos.Length() - pOwner->Get_Transform()->Get_World(WORLD_POS).Length();

		//if (fabs(fLength) < 1.2f)
		//	return m_eWalkState;
	}


	/* 따라가면 대 */
	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir;

	CPath* pCurPath = pOwner->Get_CurPath();

	if (!pCurPath)
	{
		assert(0);
		return STATE_END;
	}

	if(pOwner->Get_OwnerPlayer()->Is_FindRoute()) //(!pOwner->Get_CurRoute().empty()) 
	{
		_float4 vDestination;
		if (pOwner->Get_CurRoute().empty())
		{
			pOwner->Get_OwnerPlayer()->Set_IsFindRoute(false);
			vDestination = vCurPos;
			_float4 vDiffPositon = (vDestination - vCurPos);
			vDir = vDiffPositon.Normalize();
		}
		else 
		{

			vDestination = pOwner->Get_CurRoute().front();
			vDestination.y = vCurPos.y;
			_float4 vDiffPositon = (vDestination - vCurPos);
			if (vDiffPositon.Length()
				<= m_pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * fDT(0))
			{
				pOwner->Get_CurRoute().pop_front();
#ifdef _DEBUG
				pOwner->Get_OwnerPlayer()->Add_DebugObject(vDestination);
#endif
			}
			vDir = vDiffPositon.Normalize();
		}
	}
	else 
	{
		//vDir = pCurPath->Get_CurDir(pOwner->Get_Transform()->Get_World(WORLD_POS));

		//pCurPath->Update_CurrentIndex(vCurPos);
		
	}


	CCell* pCurCell = pOwner->Get_NaviCom()->Get_CurCell(vCurPos, CGameSystem::Get_Instance()->Get_CellLayer());
	if (pCurCell && pCurCell->Check_Attribute(CELL_WALL))
	{
		m_iRand = RUN_STATE_JUMP;
	}


	pOwner->Get_Transform()->Set_LerpLook(vDir, 0.4f);
	pOwner->Get_PhysicsCom()->Set_Dir(vDir);
	pOwner->Get_PhysicsCom()->Set_Accel(100.f);

	

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
 
}

STATE_TYPE CState_PathNavigation::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

