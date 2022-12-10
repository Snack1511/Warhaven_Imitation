#include "CTable_Conditions.h"
#include "Functor.h"
#include "CAIController.h"
#include "CPlayer.h"
#include "CUnit.h"
#include "GameInstance.h"
#include "Transform.h"
#include "CAIPersonality.h"
#include "CTrigger.h"
#include "CTeamConnector.h"
#include "CSquad.h"
#include "CBehavior.h"
#include "CUser.h"
CTable_Conditions::CTable_Conditions()
{
}

CTable_Conditions::~CTable_Conditions()
{
    Release();
}

CTable_Conditions* CTable_Conditions::Create()
{
    CTable_Conditions* pInstance = new CTable_Conditions();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CTable_Conditions");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CTable_Conditions::Release()
{
    for (auto& elem : m_mapAllBehaviors)
        SAFE_DELETE(elem.second);
}

#define Add_Condition(ConditionContainer, strFunctionName, Function)\
ConditionContainer.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3))

HRESULT CTable_Conditions::Initialize()
{
    if (FAILED(SetUp_Conditions()))
        return E_FAIL;

    if (FAILED(SetUp_Behaviors()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTable_Conditions::SetUp_Conditions()
{
    Add_Condition(m_OtherConditions, wstring(L"Check_FarAwayLeader"), Check_FarAwayLeader);

    Add_Condition(m_WhatConditions, wstring(L"Select_Leader"), Select_Leader);
    return S_OK;
}

#define Add_Behavior(pBehaviorObject, strBehaviorName, BehaviorType)\
    pBehaviorObject = CBehavior::Create(BehaviorType, this);\
    m_mapAllBehaviors.emplace(Convert_ToHash(strBehaviorName), pBehavior);

HRESULT CTable_Conditions::SetUp_Behaviors()
{
    CBehavior* pBehavior = nullptr; 
    Add_Behavior(pBehavior, wstring(L"Follow"), eBehaviorType::eFollow);
    Add_Behavior(pBehavior, wstring(L"Patrol"), eBehaviorType::ePatrol);
    Add_Behavior(pBehavior, wstring(L"Attack"), eBehaviorType::eAttack);

    return S_OK;
}

function<void(_bool&, CPlayer*, CAIController*)> CTable_Conditions::Find_OtherCondition(wstring strConditionName)
{
    try {
        _hashcode hsConditionName = Convert_ToHash(strConditionName);

        auto ConditionIter = m_OtherConditions.find(hsConditionName);

        if (m_OtherConditions.end() == ConditionIter)
            throw strConditionName;
        return ConditionIter->second;
    }
    catch (wstring ConditionName)
    {
        string strMsg = "Not Found : ";
        strMsg += CFunctor::To_String(ConditionName);
        strMsg += " - Find_OtherCondition()";
        Make_Dump(typeid(CTable_Conditions).name(), strMsg);
        return bind(&CTable_Conditions::EmptyOtherCondition, this, placeholders::_1, placeholders::_2, placeholders::_3);
    }
}

function<void(BEHAVIOR_DESC*&, CPlayer*, CAIController*)> CTable_Conditions::Find_WhatCondition(wstring strConditionName)
{
    try {
        _hashcode hsConditionName = Convert_ToHash(strConditionName);

        auto ConditionIter = m_WhatConditions.find(hsConditionName);

        if (m_WhatConditions.end() == ConditionIter)
            throw strConditionName;
        return ConditionIter->second;
    }
    catch (wstring ConditionName)
    {
        string strMsg = "Not Found : ";
        strMsg += CFunctor::To_String(ConditionName);
        strMsg += " - Find_WhatCondition()";
        Make_Dump(typeid(CTable_Conditions).name(), strMsg);
        return bind(&CTable_Conditions::EmptyWhatCondition, this, placeholders::_1, placeholders::_2, placeholders::_3);
    }
}

CBehavior* CTable_Conditions::Find_Behavior(wstring strBehavior)
{
    try {
        _hashcode hsConditionName = Convert_ToHash(strBehavior);

        auto BehaviorIter = m_mapAllBehaviors.find(hsConditionName);

        if (m_mapAllBehaviors.end() == BehaviorIter)
            throw strBehavior;
        return BehaviorIter->second;
    }
    catch (wstring ConditionName)
    {
        string strMsg = "Not Found : ";
        strMsg += CFunctor::To_String(ConditionName);
        strMsg += " - Find_Behavior()";
        Make_Dump(typeid(CTable_Conditions).name(), strMsg);
        return nullptr;
    }
}

void CTable_Conditions::Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    //OutCondition = false;
    //CPlayer* m_pLeader = pPlayer->Get_Squad()->Get_LeaderPlayer();
    //if (m_pLeader)
    //{
    //    if (m_pLeader->Is_Valid())
    //    {
    //        _float4 LeaderPos = m_pLeader->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
    //        _float4 MyPos = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    //        if ((LeaderPos - MyPos).Length() < 3.f);
    //        {
    //            OutCondition = true;
    //        }

    //    }
 
    //}

    OutCondition = true;
}


void CTable_Conditions::Select_Leader(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    //OutDesc->pAlliesPlayer = pPlayer->Get_Squad()->Get_LeaderPlayer();
    OutDesc->pAlliesPlayer = PLAYER;
}

void CTable_Conditions::Select_NearEnemy(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*> Enemies = pAIController->Get_NearEnemy();
    Enemies.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    OutDesc->pTriggerPtr = Enemies.front();
}

void CTable_Conditions::Select_NearAllies(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*> Allies = pAIController->Get_NearAllies();
    Allies.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    OutDesc->pAlliesPlayer = Allies.front();
}

void CTable_Conditions::Select_NearTrigger(BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CTrigger*> Triggers = pAIController->Get_NearTrigger();
    Triggers.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    OutDesc->pEnemyPlayer = Triggers.front();
}



