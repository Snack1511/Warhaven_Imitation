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
#include "CPath.h"
#include "CPlayer.h"
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

#define Add_WhyCondition(ConditionContainer, strFunctionName, Function)\
ConditionContainer.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3))

#define Add_WhatCondition(ConditionContainer, strFunctionName, Function)\
ConditionContainer.emplace(\
Convert_ToHash(strFunctionName),\
bind(&CTable_Conditions::Function,\
    this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4))

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
    Add_WhyCondition(m_OtherConditions, wstring(L"Check_FarAwayLeader"), Check_FarAwayLeader);
    Add_WhyCondition(m_OtherConditions, wstring(L"Check_PathArrived"), Check_PathArrived);
    Add_WhyCondition(m_OtherConditions, wstring(L"Check_FarAwayRoute"), Check_FarAwayRoute);
    Add_WhyCondition(m_OtherConditions, wstring(L"Check_NearFromRoute"), Check_NearFromRoute);
    Add_WhyCondition(m_OtherConditions, wstring(L"Check_LookEnemy"), Check_LookEnemy);

    Add_WhatCondition(m_WhatConditions, wstring(L"Select_Leader"), Select_Leader);
    Add_WhatCondition(m_WhatConditions, wstring(L"Select_NearEnemy"), Select_NearEnemy);
    Add_WhatCondition(m_WhatConditions, wstring(L"Select_NearRouteEnemy"), Select_NearEnemy);
    Add_WhatCondition(m_WhatConditions, wstring(L"Empty"), EmptyWhatCondition);
    return S_OK;
}

#define Add_Behavior(pBehaviorObject, strBehaviorName, BehaviorType)\
    pBehaviorObject = CBehavior::Create(BehaviorType, this);\
    m_mapAllBehaviors.emplace(Convert_ToHash(strBehaviorName), pBehavior);

HRESULT CTable_Conditions::SetUp_Behaviors()
{
    CBehavior* pBehavior = nullptr; 
    //Add_Behavior(pBehavior, wstring(L"ePatrol"), eBehaviorType::ePatrol);
    Add_Behavior(pBehavior, wstring(L"Follow"), eBehaviorType::eFollow);
    Add_Behavior(pBehavior, wstring(L"Patrol"), eBehaviorType::ePatrol);
    Add_Behavior(pBehavior, wstring(L"Attack"), eBehaviorType::eAttack);
    Add_Behavior(pBehavior, wstring(L"PathNavigation"), eBehaviorType::ePathNavigation);

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

function<void(_bool&, BEHAVIOR_DESC*&, CPlayer*, CAIController*)> CTable_Conditions::Find_WhatCondition(wstring strConditionName)
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
        return bind(&CTable_Conditions::EmptyWhatCondition, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
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
        Make_Dump(string("CTable_Condition"), strMsg);
        return nullptr;
    }
}
#define CHECK_EMPTY(listname) if (listname.empty()) {OutCondition = false; return;}

void CTable_Conditions::Check_FarAwayLeader(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    OutCondition = true;
}

void CTable_Conditions::Check_PathArrived(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    if (!pPlayer->Get_CurPath())
    {
        OutCondition = false;
        return;
    }
    OutCondition = !pPlayer->Get_CurPath()->Is_Arrived();
}

void CTable_Conditions::Check_LookEnemy(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    //플레이어 Look방향 반원
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*>& Enemies = pAIController->Get_NearEnemy();
    CHECK_EMPTY(Enemies);

    for (auto iter = Enemies.begin(); iter != Enemies.end();)
    {
        _float4 vTargetPosition = (*iter)->Get_WorldPos();
        _float4 vDir = (vTargetPosition - MyPositoin).Normalize();
        _float4 vMyLook = pPlayer->Get_LookDir();

        _float DotDir = vMyLook.Dot(vDir);

        if (DotDir < 0.f)
            iter = Enemies.erase(iter);
        else
            ++iter;
    }



    OutCondition = true;

}

void CTable_Conditions::Check_FarAwayRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    OutCondition = false;
    if (!pPlayer->Get_CurPath())
        return;
    _float4 vNearestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
    _float4 vMyPosition = pPlayer->Get_WorldPos();

    if ((vNearestPosition - vMyPosition).Length() > pAIController->Get_Personality()->Get_LimitRouteDistance())
    {
        OutCondition = true;
    }
}

void CTable_Conditions::Check_NearFromRoute(_bool& OutCondition, CPlayer* pPlayer, CAIController* pAIController)
{
    OutCondition = false;

    if (!pPlayer->Get_CurPath())
        return;

    _float4 vLatestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
    _float4 vMyPosition = pPlayer->Get_WorldPos();

    if ((vLatestPosition - vMyPosition).Length() <= pAIController->Get_Personality()->Get_LimitRouteDistance())
    {
        OutCondition = true;
    }
}


void CTable_Conditions::Select_Leader(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    //OutDesc->pAlliesPlayer = pPlayer->Get_Squad()->Get_LeaderPlayer();
    //OutDesc->pAlliesPlayer = PLAYER;

    OutCondition = true;
}



void CTable_Conditions::Select_NearEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*> Enemies = pAIController->Get_NearEnemy();

    CHECK_EMPTY(Enemies);

    Enemies.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });
    OutDesc->pEnemyPlayer = Enemies.front();

    OutCondition = true;

}

void CTable_Conditions::Select_NearAllies(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CPlayer*> Allies = pAIController->Get_NearAllies();
    CHECK_EMPTY(Allies);

    Allies.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    OutDesc->pAlliesPlayer = Allies.front();

    OutCondition = true;

}

void CTable_Conditions::Select_NearTrigger(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    _float4 MyPositoin = pPlayer->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);

    list<CTrigger*> Triggers = pAIController->Get_NearTrigger();
    CHECK_EMPTY(Triggers);

    Triggers.sort([&MyPositoin](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - MyPositoin).Length() > (DestPosition - MyPositoin).Length())
                return true;
            else return false;
        });

    OutDesc->pTriggerPtr = Triggers.front();

    OutCondition = true;

}

void CTable_Conditions::Select_NearRouteEnemy(_bool& OutCondition, BEHAVIOR_DESC*& OutDesc, CPlayer* pPlayer, CAIController* pAIController)
{
    OutCondition = false;
    if (!pPlayer->Get_CurPath())
        return;
    _float4 vNearestPosition = pPlayer->Get_CurPath()->Get_LatestPosition();
    _float4 vMyPosition = pPlayer->Get_WorldPos();
    list<CPlayer*> Enemies = pAIController->Get_NearEnemy();

    CHECK_EMPTY(Enemies);

    Enemies.sort([&vMyPosition](auto& Sour, auto& Dest)
        {
            _float4 SourPosition = Sour->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            _float4 DestPosition = Dest->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS);
            if ((SourPosition - vMyPosition).Length() > (DestPosition - vMyPosition).Length())
                return true;
            else return false;
        });

    _float4 vEnemyPosition = Enemies.front()->Get_WorldPos();

    if ((vNearestPosition - vEnemyPosition).Length() <= pAIController->Get_Personality()->Get_LimitRouteDistance())
    {
        OutCondition = true;
        OutDesc->pEnemyPlayer = Enemies.front();
    }
    else
    {
        OutCondition = false;
        OutDesc->pEnemyPlayer = nullptr;
    }
}
