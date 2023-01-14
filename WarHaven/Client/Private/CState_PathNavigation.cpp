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
	if (!m_pOwner->Get_OwnerPlayer()->Get_TargetObject())
		return __super::Tick(pOwner, pAnimator);

	m_fAIDelayTime += fDT(0);

	// 만약에 계속 낀다면 이 로직 한번 사용해보세요.
	if (m_fAIDelayTime > 2.f)
	{
		//_float fLength = m_vOriPos.Length() - pOwner->Get_Transform()->Get_World(WORLD_POS).Length();

		//if (fabs(fLength) < 1.2f)
		//	return m_eWalkState;
	}

//#ifdef _DEBUG
	if (KEY(K, TAP))
	{
		pOwner->Get_CurRoute().clear();
		_float4 vTargetPos = PLAYER->Get_Transform()->Get_World(WORLD_POS);
		pOwner->Get_OwnerPlayer()->Make_BestRoute(vTargetPos);

	}
//#endif

	_float4 vTargetPos = m_pOwner->Get_OwnerPlayer()->Get_TargetObjPos();

	if (vTargetPos.Is_Zero())
	{
		assert(0);
		return __super::Tick(pOwner, pAnimator);

	}

	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir = vTargetPos - vCurPos;

	_bool bFindRoute = true;

	{
		_float fDiffY = fabsf(vTargetPos.y - vCurPos.y);

		if (fDiffY < m_fMaxY)
		{
			//높이 차 별로 안나면 ray 쏴야함
			_float4 vOutPos;
			_float fOutDist;

			_float4 vRayStartPos = vCurPos;
			vRayStartPos.y += 0.5f;
			_float4 vRayEndPos = vTargetPos;
			vRayEndPos.y += 0.5f;

			_float4 vRayDir = vRayEndPos - vRayStartPos;

			if (!GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vRayStartPos, vRayDir, vRayDir.Length()))
			{

				if (pOwner->Get_OwnerPlayer()->Is_KeepRay())
				{
					eBehaviorType eBehaviortype = pOwner->Get_OwnerPlayer()->Get_BehaviorDesc()->eCurType;

					STATE_TYPE ePrevStateType = STATE_END;

					switch (eBehaviortype)
					{
					case Client::eBehaviorType::ePatrol:
						ePrevStateType = pOwner->Get_AIState_Type().eAIPatrolDefaultState;
						break;


					case Client::eBehaviorType::ePadenCannonInteract:
						ePrevStateType = pOwner->Get_AIState_Type().eAICannonDefaultState;
						break;

					case Client::eBehaviorType::eRevive:
						ePrevStateType = pOwner->Get_AIState_Type().eAIReviveDefaultState;
						break;

					case Client::eBehaviorType::eCombat:
						ePrevStateType = pOwner->Get_AIState_Type().eAICommbatDefaultState;
						break;

					}

					if (ePrevStateType != STATE_END)
						return ePrevStateType;
				}
				else
				{
					bFindRoute = false;
				}
				

			}
		}
	}

	if (bFindRoute)
	{
		_float4 vDestination;
		/* empty면 일단 루트를 만들으라고 해야대*/
		if (pOwner->Get_CurRoute().empty())
		{
			pOwner->Get_OwnerPlayer()->Make_BestRoute(vTargetPos);
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

				if (pOwner->Get_CurRoute().empty())
					pOwner->Get_OwnerPlayer()->Set_IsFindRoute(false);
#ifdef _DEBUG
				pOwner->Get_OwnerPlayer()->Add_DebugObject(vDestination);
#endif
			}
			vDir = vDiffPositon.Normalize();
		}
	}
	



	CCellLayer* pTemp = nullptr;

	CCell* pCurCell = pOwner->Get_NaviCom()->Get_CurCell(vCurPos, CGameSystem::Get_Instance()->Get_CellLayer(), &pTemp);
	if (pCurCell && pCurCell->Check_Attribute(CELL_WALL))
	{
		m_iRand = RUN_STATE_JUMP;
	}

	vDir.y = 0.f;
	pOwner->Get_Transform()->Set_LerpLook(vDir, 0.4f);
	//pOwner->Get_PhysicsCom()->Set_Dir(vDir);
	_float4 vCurDir = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	vCurDir.y = 0.f;
	pOwner->Get_PhysicsCom()->Set_Dir(vCurDir);
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

