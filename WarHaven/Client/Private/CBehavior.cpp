#include "CBehavior.h"
#include "CPlayer.h"
#include "CTrigger.h"
#include "CAIController.h"
CBehavior::CBehavior(eBehaviorType BehaviorType, CTable_Conditions* pTable)
    : m_eBehaviorType(BehaviorType)
    , m_pConditionTable(pTable)
{

}

CBehavior::CBehavior(const CBehavior& rhs)
    : m_eBehaviorType(rhs.m_eBehaviorType)
    , m_pConditionTable(rhs.m_pConditionTable)
    , m_strDebugBehaviorName(rhs.m_strDebugBehaviorName)
    , Callback_WhatCondition(rhs.Callback_WhatCondition)
    , Callback_OtherCondition(rhs.Callback_OtherCondition)
{
}

CBehavior::~CBehavior()
{
    Release();
}

CBehavior* CBehavior::Create(eBehaviorType BehaviorType, CTable_Conditions* pTable)
{
    CBehavior* pInstance = new CBehavior(BehaviorType, pTable);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CBehavior");
        SAFE_DELETE(pInstance);
    }
    return pInstance;
}

void CBehavior::Release()
{
    SAFE_DELETE(m_pBehaviorDesc);
}

HRESULT CBehavior::Initialize_Prototype()
{


    return S_OK;
}

HRESULT CBehavior::Initialize()
{
    m_pBehaviorDesc = new BEHAVIOR_DESC;
    ZeroMemory(m_pBehaviorDesc, sizeof(BEHAVIOR_DESC));
    return S_OK;
}

HRESULT CBehavior::Start()
{
    return S_OK;
}

void CBehavior::Add_WhatCondition(wstring strWhatConditionName)
{
    if (nullptr == m_pConditionTable)
        return;
    m_strConditionName[_uint(eConditionType::eWhat)].push_back(strWhatConditionName);
    Callback_WhatCondition += m_pConditionTable->Find_WhatCondition(strWhatConditionName);;
}

void CBehavior::Add_OtherCondition(wstring strOtherConditionName)
{
    if (nullptr == m_pConditionTable)
        return;
    m_strConditionName[_uint(eConditionType::eWhen)].push_back(strOtherConditionName);
    Callback_OtherCondition += m_pConditionTable->Find_OtherCondition(strOtherConditionName);
}

void CBehavior::Add_Condition(wstring strConditionName, eConditionType eType)
{
    if (eType == eConditionType::eWhen)
    {
        Add_OtherCondition(strConditionName);
    }
    else if (eType == eConditionType::eWhat)
    {
        Add_WhatCondition(strConditionName);
    }
}

void CBehavior::Add_BehaviorTick(wstring strBehaviorTickName)
{
    if (nullptr == m_pConditionTable)
        return;
    m_strBehaviorTickName.push_back(strBehaviorTickName);
    Callback_BehaviorTick += m_pConditionTable->Find_BehaviorTick(strBehaviorTickName);
}

void CBehavior::Set_Priority(_int iPriorityScore)
{
    m_pBehaviorDesc->iPriorityScore = iPriorityScore;
}

void CBehavior::Clear_Condition(eConditionType eType)
{
    if (eType == eConditionType::eWhen)
    {
        m_strConditionName[_uint(eType)].clear();
        Callback_OtherCondition.Clear();
    }
    else if (eType == eConditionType::eWhat)
    {
        m_strConditionName[_uint(eType)].clear();
        Callback_WhatCondition.Clear();
    }
}

BEHAVIOR_DESC* CBehavior::Check_Condition(_bool& bOut , CPlayer* pPlayer, CAIController* pAIController)
{
    //1. OtherCondition 조사
    _bool bOtherCondition = true;

    if (Callback_OtherCondition.Empty())
        bOtherCondition = false;
    
    Callback_OtherCondition(bOtherCondition, pPlayer, pAIController);

    if (!bOtherCondition)
        return nullptr;

    //2. WhatCondition 조사
    if (Callback_WhatCondition.Empty())
        bOut = false;

    Callback_WhatCondition(bOut, m_pBehaviorDesc, pPlayer, pAIController);

    if (!bOut)
        return nullptr;

    return m_pBehaviorDesc;
}

vector<wstring>& CBehavior::Get_ConditionNames(eConditionType eType)
{
    return m_strConditionName[_uint(eType)];
}

vector<wstring>& CBehavior::Get_BehaviorTickNames()
{
    return m_strBehaviorTickName;
}
