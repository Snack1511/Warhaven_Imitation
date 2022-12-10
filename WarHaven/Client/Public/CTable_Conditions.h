#pragma once
#include "Client_Defines.h"


/*
	* 목적과 목표 선택시의 조건들..
	* 얘는 싱글톤이 나아보임
*/
BEGIN(Engine)
class CGameObject;
END
BEGIN(Client)
class CAIController;
class CAIPersonality;
class CPlayer;
class CBehavior;
class CTable_Conditions
{
    friend class CGameSystem;

private:
	CTable_Conditions();
	virtual ~CTable_Conditions();

public:
    static CTable_Conditions* Create();

public:
    HRESULT Initialize();
    void Release();

public:
    HRESULT SetUp_Conditions();
    HRESULT SetUp_Behaviors();

public:
    function<void(_bool&, CPlayer*, CAIController*)> Find_OtherCondition(wstring strConditionName);
    function<void(BEHAVIOR_DESC*&, CPlayer*, CAIController*)> Find_WhatCondition(wstring strConditionName);
    CBehavior* Find_Behavior(wstring strBehavior);

private:
    /* 잘못된 조건명 입력 시 들어감ㅇㅇ*/
    void EmptyOtherCondition(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyWhatCondition(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController) { }

private:
    void Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_Winning(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_Losing(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_LowHealthPoint(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);

private:
    void  Select_Leader(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_NearEnemy(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_LowHealthEnemy(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_NearTrigger(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);

private:
    map<_hashcode, function<void(_bool&, CPlayer*, CAIController*)>> m_OtherConditions;
    map<_hashcode, function<void(BEHAVIOR_DESC*&, CPlayer*, CAIController*)>> m_WhatConditions;
    map<_hashcode, CBehavior*> m_mapAllBehaviors;

};
END
