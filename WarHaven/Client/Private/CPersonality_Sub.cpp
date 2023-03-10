#include "CPersonality_Sub.h"

#include "CBehavior.h"
#include "CTable_Conditions.h"
#include "CAIController.h"

CPersonality_Sub::CPersonality_Sub(CTable_Conditions* pConditionTable)
	:CAIPersonality(pConditionTable)
{
}

CPersonality_Sub::~CPersonality_Sub()
{
}

CPersonality_Sub* CPersonality_Sub::Create(CTable_Conditions* pConditionTable)
{
	CPersonality_Sub* pInstance = new CPersonality_Sub(pConditionTable);
	if (FAILED(pInstance->Initailize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPersonality_Sub");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

HRESULT CPersonality_Sub::Initailize()
{
	//PersonalDesc셋팅
	m_tPersonalDesc.strPersonalityName = wstring(L"Sub_Personal");
	m_tPersonalDesc.tPersonalityData.fChangeDelayTime = 0.01f;
	m_tPersonalDesc.tPersonalityData.fDelayWeight = 0.0f;
	m_tPersonalDesc.tPersonalityData.fSIghtRadius = 13.0f;
	m_tPersonalDesc.tPersonalityData.fSightRadiusWeight = 0.0f;
	m_tPersonalDesc.tPersonalityData.fCheckedHP = 30.f;
	m_tPersonalDesc.tPersonalityData.fHPWeight = 0.f;
	m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel = 1;
	m_tPersonalDesc.tPersonalityData.eMainPerosnality = eMain_Default;
	m_tPersonalDesc.tPersonalityData.eFightPersonality = eFight_Default;
	m_tPersonalDesc.tPersonalityData.eCoopPersonality = eCoop_Default;
	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::ePatrol)] = 5.f;
	m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBehaviorType::eCombat)] = 1.f;

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

	/* Find Main Path : 메인 Path 다 안탔으면 일단 그거 부터 타기. */
	/* 근데 중간에 바뀌면? 다 탄걸로 처리 해야함.. */
	/* 1. Path 이미 arrived 인지 확인. */
	/* arrived면 */
	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PathFinding"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_ValidPath"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Gliding"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_GlidePath"));
	pBehavior->Add_OtherCondition(wstring(L"Check_GriderTrigger"));
	pBehavior->Add_OtherCondition(wstring(L"Check_Gliding"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearGliderTrigger"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	//캐논점령지에 있고, 캐논 근처에, 캐논사용가능할 때
	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"PadenCannonInteract"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_Paden"));
	pBehavior->Add_OtherCondition(wstring(L"Check_InCannonConquerTrigger"));
	pBehavior->Add_OtherCondition(wstring(L"Check_UsableCannon"));
	pBehavior->Add_WhatCondition(wstring(L"Select_Cannon"));
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

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Revive"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_DeadAllies"));
	pBehavior->Add_WhatCondition(wstring(L"Select_NearAllies"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"CatchCannon"))->Clone();
	//근처에 상대가 쏜 대포알이 있을 때
	pBehavior->Add_OtherCondition(wstring(L"Check_InNearCannonBall"));
	pBehavior->Add_OtherCondition(wstring(L"Check_IsPriest"));
	pBehavior->Add_OtherCondition(wstring(L"Check_IsEnableSkill3"));
	//대포알을 타겟으로 넣어줌ㅇㅇ
	pBehavior->Add_WhatCondition(wstring(L"Select_CannonBall"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);

	pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Change"))->Clone();
	pBehavior->Add_OtherCondition(wstring(L"Check_CombatBehavior"));
	pBehavior->Add_OtherCondition(wstring(L"Check_AbleHero"));
	pBehavior->Add_WhatCondition(wstring(L"EmptyWhatCondition"));
	pBehavior->Add_BehaviorTick(wstring(L"EmptyBehaviorTick"));
	pBehavior->Initialize();
	pBehavior->Set_Priority(iPriority++);
	m_BehaviorList.push_back(pBehavior);


	return S_OK;
}

