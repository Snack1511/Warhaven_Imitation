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
    HRESULT SetUp_BehaviorTick();
    HRESULT SetUp_Behaviors();

public:
    function<void(_bool&, CPlayer*, CAIController*)> Find_OtherCondition(wstring strConditionName);
    function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)> Find_WhatCondition(wstring strConditionName);
    function<void(CPlayer*, CAIController*)> Find_BehaviorTick(wstring strConditionName);
    CBehavior* Find_Behavior(wstring strBehavior);
    vector<wstring>& Get_ConditionNames(_uint iConditionType);
    vector<wstring>& Get_BehaviorTickNames();
    vector<wstring>& Get_BehaviorNames();
private:
    /* 잘못된 조건명 입력 시 들어감ㅇㅇ*/
    void EmptyOtherCondition(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyWhatCondition(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyBehaviorTick(CPlayer* pPlayer, CAIController* pAIController) { }

private:
    void Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_PathArrived(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_LookEnemy(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_FarAwayRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_NearFromRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_DeadAllies(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_AttackBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_AbleHero(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);

    //void Check_Winning(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_EmptyEnemyInTerritory(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_Losing(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_LowHealthPoint(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);

private:
    void  Select_Leader(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearAllies(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearRouteEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_LowHealthEnemy(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
private:
    void Callback_Tick_UpdatePatrol(CPlayer* pPlayer, CAIController* pAIController);
    void Callback_Tick_Check_NaviTime(CPlayer* pPlayer, CAIController* pAIController);
private:
    _bool RemovePlayer(_bool bFlag, list<CPlayer*>& PlayerList, list<CPlayer*>::iterator& rhsIter);
private:
    map<_hashcode, function<void(_bool&, CPlayer*, CAIController*)>> m_OtherConditions;
    map<_hashcode, function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)>> m_WhatConditions;
    map<_hashcode, function<void(CPlayer*, CAIController*)>> m_BehaviorTick;
    map<_hashcode, CBehavior*> m_mapAllBehaviors;
    vector<wstring> m_vecStrConditionName[2];
    vector<wstring> m_vecBehaviorTickName;
    vector<wstring> m_vecBehaviorName;

};
END
