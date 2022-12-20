#include "CPersonality_Default.h"

#include "CBehavior.h"
#include "CTable_Conditions.h"
#include "CAIController.h"

CPersonality_Default::CPersonality_Default(CTable_Conditions* pConditionTable)
	:CAIPersonality(pConditionTable)
{
}

CPersonality_Default::~CPersonality_Default()
{
}

CPersonality_Default* CPersonality_Default::Create(CTable_Conditions* pConditionTable)
{
	CPersonality_Default* pInstance = new CPersonality_Default(pConditionTable);
	if (FAILED(pInstance->Initailize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPersonality_Default");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

HRESULT CPersonality_Default::Initailize()
{
	//PersonalDesc����
	m_tPersonalDesc.strPersonalityName = wstring(L"Default_Personal");
	m_tPersonalDesc.tPersonalityData.fChangeDelayTime = 0.01f;
	m_tPersonalDesc.tPersonalityData.fDelayWeight = 0.0f;
	m_tPersonalDesc.tPersonalityData.fSIghtRadius = 10.0f;
	m_tPersonalDesc.tPersonalityData.fSightRadiusWeight = 0.0f;
	m_tPersonalDesc.tPersonalityData.fCheckedHP = 30.f;
	m_tPersonalDesc.tPersonalityData.fHPWeight = 0.f;
	m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel = 1;
	m_tPersonalDesc.tPersonalityData.eMainPerosnality = eMain_Default;
	m_tPersonalDesc.tPersonalityData.eFightPersonality = eFight_Default;
	m_tPersonalDesc.tPersonalityData.eCoopPersonality = eCoop_Default;
	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::ePatrol)] = 5.f;

	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::ePathNavigation)] = 1.f;
	/* �Ÿ� */
	m_tPersonalDesc.tPersonalityData.fMinMoveAcc[_uint(eBehaviorType::ePathNavigation)] = 1.f;

	CBehavior* pBehavior = nullptr;
	/* pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Follow"))->Clone();
	pBehavior->Initialize();
	pBehavior->Add_OtherCondition(wstring(L"Check_FarAwayLeader"));
	pBehavior->Add_WhatCondition(wstring(L"Select_Leader"));
	m_BehaviorList.push_back(pBehavior);*/
	if (FAILED(SetUp_PatrolBehavior()))
	{
		assert(0);
	}

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PathNavigation"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_PathArrived"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"Callback_Tick_Check_NaviTime"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(1);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Resurrect"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_DeadAllies"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearAllies"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(2);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Attack"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_LookEnemy"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearEnemy"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(3);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Change"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_AbleHero"));
	pBehavior->Add_OtherCondition(wstring(L"Check_AttackBehavior"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(4);
	m_BehaviorList.push_back(pBehavior);





	//m_BehaviorList.push_back(pBehavior);
// 
	//CBehavior* pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Attack"))->Clone();
	//pBehavior->Initialize();
	//pBehavior->Add_OtherCondition(wstring(L"Check_FarAwayLeader"));
	//pBehavior->Add_WhatCondition(wstring(L"Select_Leader"));
	//pBehavior->SetUp_StateType(STATE_IDLE_WARRIOR_L_AI_ENEMY);
	//m_BehaviorList.push_back(pBehavior);


	return S_OK;
}

