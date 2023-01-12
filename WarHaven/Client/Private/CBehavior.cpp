#include "CBehavior.h"
#include "CPlayer.h"
#include "CTrigger.h"
#include "CAIController.h"
#include "Functor.h"
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

CBehavior* CBehavior::Load(wstring strSaveName, CTable_Conditions* pTable)
{
    string strPath = "../bin/Data/GameSystem/Behavior/";
    strPath += CFunctor::To_String(strSaveName);
    strPath += ".bin";

    ifstream readFile;
    readFile.open(strPath, ios::binary);

    if (!readFile.is_open())
    {
        wstring strData = strSaveName;
        strData += L"_Load Failed";
        Call_MsgBox(strData.c_str());
        return nullptr;
    }


    //eBehaviorType
    _uint iBehaviorType = 0;
    readFile.read((char*)&iBehaviorType, sizeof(_uint));

    //Priority
    _uint iPriority = 0;
    readFile.read((char*)&iPriority, sizeof(_uint));

    CBehavior* pBehavior = CBehavior::Create(eBehaviorType(iBehaviorType), pTable);
    pBehavior->Initialize();
    pBehavior->Set_Priority(iPriority);

    //OtherCondition
    pBehavior->Load_Functions(readFile, bind(&CBehavior::Add_OtherCondition, pBehavior, placeholders::_1));

    //WhatCondition
    pBehavior->Load_Functions(readFile, bind(&CBehavior::Add_WhatCondition, pBehavior, placeholders::_1));

    //BehaviorTick
    pBehavior->Load_Functions(readFile, bind(&CBehavior::Add_BehaviorTick, pBehavior, placeholders::_1));

    _uint iFind = strSaveName.rfind(L"_");
    wstring strDebugName = strSaveName.substr(iFind + 1, strSaveName.length());
    pBehavior->Set_BehaviorName(strDebugName);

    readFile.close();

    return pBehavior;
}

void CBehavior::Save(wstring wstrSaveName)
{
    ofstream writeFile;
    //D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Data\GameSystem\Behavior
    string strSaveName = CFunctor::To_String(wstrSaveName);

    string strPath = "../bin/Data/GameSystem/Behavior/";
    strPath += strSaveName;
    strPath += ".bin";
    writeFile.open(strPath, ios::binary);

    if (!writeFile.is_open())
    {
        wstring strData = CFunctor::To_Wstring(strSaveName);
        strData += L"_Save Failed";
        Call_MsgBox(strData.c_str());
        return;
    }

    //eBehaviorType
    _uint iBehaviorType = _uint(m_eBehaviorType);
    writeFile.write((char*)&iBehaviorType, sizeof(_uint));


    //Priority
    _uint iPriority = Get_Priority();
    writeFile.write((char*)&iPriority, sizeof(_uint));

    //OtherCondition
    Save_Functions(writeFile, m_strConditionName[_uint(eConditionType::eWhen)]);

    //WhatCondition
    Save_Functions(writeFile, m_strConditionName[_uint(eConditionType::eWhat)]);

    //BehaviorTick
    Save_Functions(writeFile, m_strBehaviorTickName);
    writeFile.close();
}

void CBehavior::Save_Functions(ofstream& rhsWriteFile, vector<wstring>& rhsDatas)
{
    _uint iFunctionSize = _uint(rhsDatas.size());
    rhsWriteFile.write((char*)&iFunctionSize, sizeof(_uint));
    for (auto& ConditionName : rhsDatas)
    {
        string strName = CFunctor::To_String(ConditionName);

        _uint iNameLength = _uint(strName.length()) + 1;
        rhsWriteFile.write((char*)&iNameLength, sizeof(_uint));

        char szName[MAXCHAR] = "";
        strcat_s(szName, sizeof(char) * MAXCHAR, strName.c_str());
        strcat_s(szName, sizeof(char) * MAXCHAR, "\0");
        rhsWriteFile.write(szName, sizeof(char) * iNameLength);
    }
}

void CBehavior::Load_Functions(ifstream& rhsReadFile, const function<void(wstring)>& Func)
{
    _uint OtherConditionSize = 0;
    rhsReadFile.read((char*)&OtherConditionSize, sizeof(_uint));
    for (_uint i = 0; i < OtherConditionSize; ++i)
    {
        _uint iLenght = 0;
        rhsReadFile.read((char*)&iLenght, sizeof(_uint));
        char szName[MAXCHAR] = "";
        rhsReadFile.read(szName, sizeof(char) * iLenght);
        string strName = szName;

        Func(CFunctor::To_Wstring(strName));
    }
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
    if (strWhatConditionName == L"")
        return;
    m_strConditionName[_uint(eConditionType::eWhat)].push_back(strWhatConditionName);
    Callback_WhatCondition += m_pConditionTable->Find_WhatCondition(strWhatConditionName);
}

void CBehavior::Add_OtherCondition(wstring strOtherConditionName)
{
    if (nullptr == m_pConditionTable)
        return;
    if (strOtherConditionName == L"")
        return;
    m_strConditionName[_uint(eConditionType::eWhen)].push_back(strOtherConditionName);
    Callback_OtherCondition += m_pConditionTable->Find_OtherCondition(strOtherConditionName);
}

void CBehavior::Add_CallBack_Tick(wstring strCallbackTickName)
{
    if (nullptr == m_pConditionTable)
        return;
    if (strCallbackTickName == L"")
        return;
    m_strConditionName[_uint(eConditionType::eTick)].push_back(strCallbackTickName);
    Callback_BehaviorTick += m_pConditionTable->Find_BehaviorTick(strCallbackTickName);
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
    else
    {
        Add_CallBack_Tick(strConditionName);
    }
}

void CBehavior::Add_BehaviorTick(wstring strBehaviorTickName)
{
    if (nullptr == m_pConditionTable)
        return;
    if (strBehaviorTickName == L"")
        return;
    m_strBehaviorTickName.push_back(strBehaviorTickName);
    Callback_BehaviorTick += m_pConditionTable->Find_BehaviorTick(strBehaviorTickName);
}

void CBehavior::Set_Priority(_int iPriorityScore)
{
    m_pBehaviorDesc->iPriorityScore = iPriorityScore;
}

_uint CBehavior::Get_Priority()
{
    if (m_pBehaviorDesc)
        return  m_pBehaviorDesc->iPriorityScore;
    else return 0;
}

void CBehavior::Delete_WhatCondition(wstring strWhatConditionName)
{
    vector<wstring> TmpVector;
    Callback_WhatCondition.Clear();
    for (auto& NameValue : m_strConditionName[_uint(eConditionType::eWhat)])
    {
        if (strWhatConditionName != NameValue)
        {
            Callback_WhatCondition += m_pConditionTable->Find_WhatCondition(NameValue);
            TmpVector.push_back(NameValue);
        }
    }
    TmpVector.swap(m_strConditionName[_uint(eConditionType::eWhat)]);
}

void CBehavior::Delete_OtherCondition(wstring strOtherConditionName)
{
    vector<wstring> TmpVector;
    Callback_OtherCondition.Clear();
    for (auto& NameValue : m_strConditionName[_uint(eConditionType::eWhen)])
    {
        if (strOtherConditionName != NameValue)
        {
            Callback_OtherCondition += m_pConditionTable->Find_OtherCondition(NameValue);
            TmpVector.push_back(NameValue);
        }
    }
    TmpVector.swap(m_strConditionName[_uint(eConditionType::eWhen)]);
}

void CBehavior::Delete_CallBack_Tick(wstring strBehaviorTickName)
{
    vector<wstring> TmpVector;
    Callback_BehaviorTick.Clear();
    for (auto& NameValue : m_strConditionName[_uint(eConditionType::eTick)])
    {
        if (strBehaviorTickName != NameValue)
        {
            Callback_BehaviorTick += m_pConditionTable->Find_BehaviorTick(NameValue);
            TmpVector.push_back(NameValue);
        }
    }
    TmpVector.swap(m_strConditionName[_uint(eConditionType::eTick)]);
}

void CBehavior::Delete_Condition(wstring strConditionName, eConditionType eType)
{
    if (eType == eConditionType::eWhen)
    {
        Delete_OtherCondition(strConditionName);
    }
    else if (eType == eConditionType::eWhat)
    {
        Delete_WhatCondition(strConditionName);
    }    
    else
    {
        Delete_CallBack_Tick(strConditionName);
    }
}

void CBehavior::Clear_Condition(eConditionType eType)
{
    m_strConditionName[_uint(eType)].clear();
    if (eType == eConditionType::eWhen)
    {
        Callback_OtherCondition.Clear();
    }
    else if (eType == eConditionType::eWhat)
    {
        Callback_WhatCondition.Clear();
    }
    else
    {
        Callback_BehaviorTick.Clear();
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
