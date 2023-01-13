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
class CTeamConnector;
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
    function<void(CPlayer*, CAIController*)> Find_BehaviorTick(wstring strConditionName);
    CBehavior* Find_Behavior(wstring strBehavior);
    vector<wstring>& Get_ConditionNames(_uint iConditionType);
    vector<wstring>& Get_BehaviorTickNames();
    vector<wstring>& Get_BehaviorNames();

private:
    /* �߸��� ���Ǹ� �Է� �� ������*/
    void EmptyOtherCondition(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyWhatCondition(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController) { OutCondition = true; }
    void EmptyBehaviorTick(CPlayer* pPlayer, CAIController* pAIController) { }

private:

#pragma region �÷��̾� üũ
    void Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_LookEnemy(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_DeadAllies(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion �÷��̾� üũ



#pragma region �� üũ
    void Check_Paden(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Hwara(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Need_Conquer(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Conquer_Respawn(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Conquer_MainPoint(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Conquer_PadenCannon(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_Conquer_HwaraFinal(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion �� üũ

#pragma region ĳ����üũ

#pragma endregion ĳ����üũ

#pragma region ��Ÿ�� üũ

#pragma endregion ��Ÿ�� üũ

#pragma region ���غ�� üũ
    void Check_PatrolBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_GotoTriggerBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_PadenCannonInteractBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_ReviveBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_CombatBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    void Check_ChangeBehavior(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion ���غ�� üũ

#pragma region �÷��̾� ���� üũ
    void Check_AbleHero(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion �÷��̾� ���� üũ
    //void Check_Winning(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_EmptyEnemyInTerritory(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_Losing(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);
    //void Check_LowHealthPoint(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController);

private:
#pragma region �÷��̾� ����
    void  Select_Leader(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_NearAllies(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_MainPlayer(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion �÷��̾� ����

#pragma region Ʈ���� ����
    void  Select_ConquerTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_PadenCannonTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_RespawnTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_MainTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    void  Select_HwaraFinalTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_JumpTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
#pragma endregion Ʈ���� ����


    //void  Select_NearRouteEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
    //void  Select_LowHealthEnemy(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController);
private:
    _bool RemovePlayer(_bool bFlag, list<CPlayer*>& PlayerList, list<CPlayer*>::iterator& rhsIter);
    _bool Check_Behavior(CBehavior* pBehavior, eBehaviorType eType);
    _bool Check_Level(LEVEL_TYPE_CLIENT eLevelType);
    _bool Check_Team(CTeamConnector* pTeamConnector, eTEAM_TYPE eTeam);
private:
    map<_hashcode, function<void(_bool&, CPlayer*, CAIController*)>> m_OtherConditions;
    map<_hashcode, function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)>> m_WhatConditions;
    map<_hashcode, function<void(CPlayer*, CAIController*)>> m_BehaviorTick;
    map<_hashcode, CBehavior*> m_mapAllBehaviors;
    vector<wstring> m_vecStrConditionName[_uint(eBehaviorConditionType::eCount)];
    vector<wstring> m_vecBehaviorTickName;
    vector<wstring> m_vecBehaviorName;

};
END
