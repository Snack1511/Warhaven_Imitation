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
	//PersonalDesc¼ÂÆÃ
	m_tPersonalDesc.strPersonalityName = wstring(L"Default_Personal");
	m_tPersonalDesc.fChangeDelayTime = 0.01f;
	m_tPersonalDesc.fDelayWeight = 0.0f;
	m_tPersonalDesc.fSIghtRadius = 3.0f;
	m_tPersonalDesc.fSightRadiusWeight = 0.0f;
	m_tPersonalDesc.fCheckedHP = 30.f;
	m_tPersonalDesc.fHPWeight = 0.f;
	m_tPersonalDesc.iMainPersonalityLevel = 1;
	m_tPersonalDesc.eMainPerosnality = eMain_Default;
	m_tPersonalDesc.eFightPersonality = eFight_Default;
	m_tPersonalDesc.eCoopPersonality = eCoop_Default;

	CBehavior* pBehavior = m_pConditionTable->Find_Behavior(wstring(L"Follow"))->Clone();
	pBehavior->Initialize();
	pBehavior->Add_OtherCondition(wstring(L"Check_FarAwayLeader"));
	pBehavior->Add_WhatCondition(wstring(L"Select_Leader"));
	pBehavior->SetUp_StateType(STATE_IDLE_WARRIOR_L_AI_ENEMY);
	m_BehaviorList.push_back(pBehavior);
	return S_OK;
}

