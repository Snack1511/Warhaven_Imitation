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
	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::eCombat)] = 2.f;

	//m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::eGoToTrigger)] = 1.f;
	/* �Ÿ� */
	//m_tPersonalDesc.tPersonalityData.fMinMoveAcc[_uint(eBehaviorType::eGoToTrigger)] = 1.f;

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

	_uint iPriority = 1;

	/* Find Main Path : ���� Path �� �������� �ϴ� �װ� ���� Ÿ��. */
	/* �ٵ� �߰��� �ٲ��? �� ź�ɷ� ó�� �ؾ���.. */
	/* 1. Path �̹� arrived ���� Ȯ��. */
	/* arrived�� */
	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PathFinding"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_ValidPath"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);	
	
	/*pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Gliding"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_GriderTrigger"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearGliderTrigger"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);*/

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Revive"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"EmptyOtherCondition"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearAllies"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	//ĳ���������� �ְ�, ĳ�� ��ó��, ĳ���밡���� ��
	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PadenCannonInteract"))->Clone();
	//pBehavior->Add_OtherCondition(wstring(L"Check_Paden"));
	pBehavior->Add_OtherCondition(wstring(L"Check_InCannonConquerTrigger"));
	pBehavior->Add_OtherCondition(wstring(L"Check_AdjCannon"));
	pBehavior->Add_OtherCondition(wstring(L"Check_UsableCannon"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Combat"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_LookEnemy"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearEnemy"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	

	//pBehavior = m_pConditionTable->Find_Behavior(wstring(L"CatchCannon"))->Clone();
	////��ó�� ��밡 �� �������� ���� ��
	//pBehavior->Add_OtherCondition(wstring(L"EmptyOtherCondition"));
	////�������� Ÿ������ �־��ܤ���
	//pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	//pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	//pBehavior->Initialize();
	//pBehavior->Set_Priority(iPriority++);
	//m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Change"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_CombatBehavior"));
	pBehavior->Add_OtherCondition(wstring(L"Check_AbleHero"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
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

