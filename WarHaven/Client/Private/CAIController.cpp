#include "CAIController.h"
#include "CPlayer.h"
#include "CUnit.h"
#include "CTrigger.h"
#include "CAIPersonality.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "CBehavior.h"
#include "Functor.h"
#include "CPath.h"
#include  "CGameSystem.h"
#include  "CCannonBall.h"

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

void CAIController::Early_Tick()
{
	Ready_Controller();

	CBehavior* pNextBehavior = nullptr;
	BEHAVIOR_DESC* pBehaviorDescTemp = nullptr;

	if (m_pPersonality->Is_UpdateList())
	{
		m_BehaviorList = m_pPersonality->Get_BehaviorList();
		m_pPersonality->Complete_Update();
	}

	for (auto& Value : m_BehaviorList)
	{
		_bool bPassCondition = false;
		BEHAVIOR_DESC* pCurCheckBehaviorDesc = nullptr;
		pCurCheckBehaviorDesc = Value->Check_Condition(bPassCondition, m_pOwnerPlayer, this);

		/* false가 return 되었따 */
		if (!bPassCondition)
			continue;

		if (!pCurCheckBehaviorDesc)
		{
			assert(0);
			continue;
		}

		if (!pBehaviorDescTemp)
		{
			pBehaviorDescTemp = pCurCheckBehaviorDesc;
			pNextBehavior = Value;
			continue;
		}

		if (pCurCheckBehaviorDesc->iPriorityScore >= pBehaviorDescTemp->iPriorityScore)
		{
			pBehaviorDescTemp = pCurCheckBehaviorDesc;
			pNextBehavior = Value;
		}
	}

	/* For문 끝 */
	CBehavior* pPatrolBehavior = m_pPersonality->Get_DefaultBehavior();
	if (nullptr == pNextBehavior && nullptr != pPatrolBehavior)
	{
		pNextBehavior = m_pPersonality->Get_DefaultBehavior();
		pBehaviorDescTemp = pNextBehavior->Get_BehaviorDesc();
	}

	if (nullptr == pBehaviorDescTemp)
		return;

	/* 무지성 CHange */
	pBehaviorDescTemp->eCurType = pNextBehavior->Get_BehaviorType();
	m_pOwnerPlayer->On_ChangeBehavior(pBehaviorDescTemp);
	m_pCurrentBehavior = pNextBehavior;

	/* Behavior가 딱 바뀐 시점에는 cout과 시간 갱신 정도만 */
	if (!m_pCurrentBehavior || m_pCurrentBehavior != pNextBehavior)
	{
		/*
		if (nullptr != m_pCurrentBehavior)
			pBehaviorDesc->ePrevType = m_pCurrentBehavior->Get_BehaviorType();
		else
			pBehaviorDesc->ePrevType = eBehaviorType::ePatrol;
			*/

#ifdef _DEBUG
		cout << "CurUnitName - " << CFunctor::To_String(m_pOwnerPlayer->Get_PlayerName()).c_str() << endl;
		cout << "CurBehavior - " << CFunctor::To_String(m_pCurrentBehavior->Get_BehaviorName()).c_str() << endl;
		if(m_pCurPath)
			cout << "CurPath - " << m_pCurPath->Get_PathName().c_str() << endl;
#endif // _DEBUG
		m_fTimeAcc = 0.f;


	}


}

void CAIController::Tick()
{
	//연산량 많으면 빼야됨;;
	if(m_pCurrentBehavior)
		m_pCurrentBehavior->Callback_BehaviorTick(m_pOwnerPlayer, this);
	

}

void CAIController::Late_Tick()
{

	m_NearEnemyList.clear();
	m_NearAlliesList.clear();
	m_NearCannonBallList.clear();
	m_NearObjectList.clear();
}

void CAIController::Release()
{
	//SAFE_DELETE(m_pCurPath);
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
	m_NearAlliesList.clear();
	m_NearEnemyList.clear();
	m_NearCannonBallList.clear();

	for (auto& Value : m_NearObjectList)
	{
		CUnit* pUnit = dynamic_cast<CUnit*>(Value);
		CTrigger* pTrigger = dynamic_cast<CTrigger*>(Value);
		CCannonBall* pCannonBall = dynamic_cast<CCannonBall*>(Value);
		if (nullptr != pTrigger)
		{
			m_NearTriggerList.push_back(pTrigger);
		}
		else if (nullptr != pUnit)
		{
			if(pUnit->Get_OwnerPlayer()->Get_Team() == m_pOwnerPlayer->Get_Team())
				m_NearAlliesList.push_back(pUnit->Get_OwnerPlayer());
			else
				m_NearEnemyList.push_back(pUnit->Get_OwnerPlayer());

		}
		else if (nullptr != pCannonBall)
		{
			if(!pCannonBall->Is_Catch())
				m_NearCannonBallList.push_back(pCannonBall);
		}
	}

	//m_NearTriggerList.sort();
	//m_NearAlliesList.sort();
	//m_NearEnemyList.sort();

}

void CAIController::Set_NewPath(CPath* pPath)
{
	m_pCurPath = pPath;
	m_pCurPath->m_pOwnerController = this;
}

void CAIController::Set_Personality(CAIPersonality* pPersonality)
{
	m_pPersonality = pPersonality;
	m_pCurrentBehavior = nullptr;
	if (pPersonality)
		m_BehaviorList = m_pPersonality->Get_BehaviorList();
	else m_BehaviorList.clear();
}

_bool CAIController::Is_LongTimeRemain(eBehaviorType eBhavior)
{
	if (nullptr == m_pPersonality)
		return false;
	return m_pPersonality->Is_LongTimeRemain(eBhavior);
}

void CAIController::Change_NearPath()
{
	m_pOwnerPlayer->Change_NearPath();
}

void CAIController::Set_BehaviorList(list<CBehavior*>& BehaviorList)
{
	m_BehaviorList = BehaviorList;
}


