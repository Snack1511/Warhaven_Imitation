#include "CPersonality_Leader.h"

#include "CBehavior.h"
#include "CTable_Conditions.h"
#include "CAIController.h"

CPersonality_Leader::CPersonality_Leader(CTable_Conditions* pConditionTable)
	:CAIPersonality(pConditionTable)
{
}

CPersonality_Leader::~CPersonality_Leader()
{
}

CPersonality_Leader* CPersonality_Leader::Create(CTable_Conditions* pConditionTable)
{
	CPersonality_Leader* pInstance = new CPersonality_Leader(pConditionTable);
	if (FAILED(pInstance->Initailize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPersonality_Leader");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

HRESULT CPersonality_Leader::Initailize()
{
	//PersonalDesc셋팅
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

	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::eGoToTrigger)] = 1.f;
	/* 거리 */
	m_tPersonalDesc.tPersonalityData.fMinMoveAcc[_uint(eBehaviorType::eGoToTrigger)] = 1.f;

	CBehavior* pBehavior = nullptr;
	/* pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Follow"))->Clone();
	pBehavior->Initialize();
	pBehavior->Add_OtherCondition(wstring(L"Check_FarAwayLeader"));
	pBehavior->Add_WhatCondition(wstring(L"Select_Leader"));
	m_BehaviorList.push_back(pBehavior);*/
	if (FAILED(SetUp_DefaultBehavior()))
	{
		assert(0);
	}

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"GoToTrigger"))->Clone();
	//pBehavior->Add_OtherCondition(wstring(L"Check_EmptyRoute"));
	pBehavior->Add_OtherCondition(wstring(L"Check_Need_Conquer"));
	pBehavior->Add_WhatCondition(wstring(L"Select_RandomConquerTrigger"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(1);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PadenCannonInteract"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_Paden"));
	pBehavior->Add_OtherCondition(wstring(L"Check_Conquer_PadenCannon"));
	pBehavior->Add_WhatCondition(wstring(L"Select_PadenCannonTrigger"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(2);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Revive"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_DeadAllies"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearAllies"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(3);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Combat"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_LookEnemy"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearEnemy"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(4);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Change"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_CombatBehavior"));
	pBehavior->Add_OtherCondition(wstring(L"Check_AbleHero"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(5);
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

