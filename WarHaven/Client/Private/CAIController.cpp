#include "CAIController.h"
#include "CPlayer.h"
#include "CUnit.h"
#include "CTrigger.h"
#include "CAIPersonality.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "CBehavior.h"

#include "CPath.h"

CAIController::CAIController(_uint iGroupID)
	: CComponent(iGroupID)
{
}

CAIController::CAIController(const CAIController& rhs)
	: CComponent(rhs)
	, m_pPersonality(rhs.m_pPersonality)
{
}

CAIController::~CAIController()
{
}

CAIController* CAIController::Create(CAIPersonality* pPersonality)
{
	CAIController* pInstance = new CAIController(CP_BEFORE_TRANSFORM);//아니면 CP_NAVIGATION

	pInstance->m_pPersonality = pPersonality;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CAIController");
		return nullptr;
	}

	return pInstance;
}

HRESULT CAIController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAIController::Initialize()
{

	return S_OK;
}

void CAIController::Start()
{
	if (nullptr == m_pOwner)
		assert(0);
	m_pOwnerPlayer = static_cast<CPlayer*>(m_pOwner);
	m_BehaviorList = m_pPersonality->Get_BehaviorList();
	__super::Start();

}

void CAIController::Tick()
{

	Ready_Controller();

	CBehavior* pNextBehavior = nullptr;
	BEHAVIOR_DESC* pBehaviorDesc = nullptr;
	for (auto& Value : m_BehaviorList)
	{
		_bool bPassCondition = false;
		BEHAVIOR_DESC* pCurCheckBehaviorDesc = nullptr;
		pCurCheckBehaviorDesc = Value->Check_Condition(bPassCondition, m_pOwnerPlayer, this);
		if (!bPassCondition)
			continue;
		if (nullptr == pBehaviorDesc)
		{
			pBehaviorDesc = pCurCheckBehaviorDesc;
			pNextBehavior = Value;
		}
		else {
			if (nullptr != pCurCheckBehaviorDesc)
			{
				if (pCurCheckBehaviorDesc->fPriorityWeight > pBehaviorDesc->fPriorityWeight)
				{
					pBehaviorDesc = pCurCheckBehaviorDesc;
					pNextBehavior = Value;
				}
			}
		}
	}

	if (nullptr == pNextBehavior)
	{
		m_pPersonality->Get_Patrol();
	}

	if(m_pCurrentBehavior != pNextBehavior)
	{
		if(nullptr != m_pCurrentBehavior)
			pBehaviorDesc->ePrevType = m_pCurrentBehavior->Get_BehaviorType();
		else
			pBehaviorDesc->ePrevType = eBehaviorType::ePatrol;

		pBehaviorDesc->eCurType = pNextBehavior->Get_BehaviorType();
		m_pCurrentBehavior = pNextBehavior;
		
		m_pOwnerPlayer->On_ChangeBehavior(pBehaviorDesc);

		//m_pOwnerPlayer->Set_TargetPlayer(reinterpret_cast<CPlayer*>(pBehaviorDesc->pAlliesPlayer));
		//m_pOwnerPlayer->Reserve_State(pBehaviorDesc->iStateType);

	}
	//NextBehavior = 조건판단
	//if(CurBehavior ? NextBehavior )
	// 
	// {
		//CurBehavior = NextBehavior;
		//UnitStateUpdate
	//}


}

void CAIController::Late_Tick()
{


	m_NearObjectList.clear();
}

void CAIController::Release()
{
	SAFE_DELETE(m_pCurPath);
}

void CAIController::OnEnable()
{
	__super::OnEnable();
}

void CAIController::OnDisable()
{
	__super::OnDisable();
}

void CAIController::Ready_Controller()
{
	/* 현재 타고 있는 Path 갱신 */

	if (m_pCurPath)
	{
		m_pCurPath->Update_CurrentIndex(m_pOwnerPlayer->Get_WorldPos());
	}

	for (auto& Value : m_NearObjectList)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Value);
		CTrigger* pTrigger = static_cast<CTrigger*>(Value);
		if (nullptr != pTrigger)
		{
			m_NearTriggerList.push_back(pTrigger);
		}
		else if (nullptr != pPlayer)
		{
			if(pPlayer->Get_Team() == m_pOwnerPlayer->Get_Team())
				m_NearAlliesList.push_back(pPlayer);
			else
				m_NearEnemyList.push_back(pPlayer);

		}
	}

}
