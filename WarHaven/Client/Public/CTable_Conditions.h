#pragma once
#include "Client_Defines.h"


/*
	* ������ ��ǥ ���ý��� ���ǵ�..
	* ��� �̱����� ���ƺ���
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
    function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)> Find_WhatCondition(wstring strConditionName);
    CBehavior* Find_Behavior(wstring strBehavior);

private:
    /* �߸��� ���Ǹ� �Է� �� ������*/
    void EmptyOtherCondition(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyWhatCondition(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }

private:
    void Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_PathArrived(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_LookEnemy(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_FarAwayRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_NearFromRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);

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
    _bool RemovePlayer(_bool bFlag, list<CPlayer*>& PlayerList, list<CPlayer*>::iterator& rhsIter);
private:
    map<_hashcode, function<void(_bool&, CPlayer*, CAIController*)>> m_OtherConditions;
    map<_hashcode, function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)>> m_WhatConditions;
    map<_hashcode, CBehavior*> m_mapAllBehaviors;

};
END
