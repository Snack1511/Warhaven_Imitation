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

BEHAVIOR_DESC* CBehavior::Check_Condition(_bool& bOut , CPlayer* pPlayer, CAIController* pAIController)
{
    Callback_OtherCondition(bOut, pPlayer, pAIController);
    if (bOut)
    {
        Callback_WhatCondition(m_pBehaviorDesc, pPlayer, pAIController);

        return m_pBehaviorDesc;
    }
    else
        return nullptr;
}

void CBehavior::SetUp_StateType(_uint iStateType)
{
    if (nullptr == m_pBehaviorDesc)
        assert(0);

    //m_pBehaviorDesc->iStateType = iStateType;
}
