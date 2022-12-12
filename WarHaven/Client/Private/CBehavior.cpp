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
    Callback_WhatCondition += m_pConditionTable->Find_WhatCondition(strWhatConditionName);;
}

void CBehavior::Add_OtherCondition(wstring strOtherConditionName)
{
    if (nullptr == m_pConditionTable)
        return;

    Callback_OtherCondition += m_pConditionTable->Find_OtherCondition(strOtherConditionName);
}

void CBehavior::Add_BehaviorTick(wstring strBehaviorTickName)
{
    if (nullptr == m_pConditionTable)
        return;

    Callback_BehaviorTick += m_pConditionTable->Find_BehaviorTick(strBehaviorTickName);
}

void CBehavior::Set_Priority(_int iPriorityScore)
{
    m_pBehaviorDesc->iPriorityScore = iPriorityScore;
}

BEHAVIOR_DESC* CBehavior::Check_Condition(_bool& bOut , CPlayer* pPlayer, CAIController* pAIController)
{
    //1. OtherCondition 조사
    _bool bOtherCondition = true;
    Callback_OtherCondition(bOtherCondition, pPlayer, pAIController);

    if (!bOtherCondition)
        return nullptr;

    //2. WhatCondition 조사
    Callback_WhatCondition(bOut, m_pBehaviorDesc, pPlayer, pAIController);

    if (!bOut)
        return nullptr;

    return m_pBehaviorDesc;
}
