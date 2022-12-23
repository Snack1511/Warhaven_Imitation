#include "stdafx.h"
#include "CTeamConnector.h"
#include "UsefulHeaders.h"
#include "CSquad.h"

#include "CTrigger_Paden.h"

CTeamConnector::CTeamConnector()
{
}

CTeamConnector::~CTeamConnector()
{
    Release();
}

CTeamConnector* CTeamConnector::Create(list<CSquad*> SquadList)
{
    CTeamConnector* pInstance = new CTeamConnector;

    pInstance->m_SquadList = SquadList;

    if (pInstance->Initialize())
    {
        Call_MsgBox(L"TeamConnector에 스쿼드가 부족함 ");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

CTeamConnector* CTeamConnector::Create(list<CSquad*> SquadList, eTEAM_TYPE eTeamType)
{
    CTeamConnector* pInstance = new CTeamConnector;

    pInstance->m_SquadList = SquadList;
    pInstance->m_eTeamType = eTeamType;
    return pInstance;
}

HRESULT CTeamConnector::Initialize()
{
    if (m_SquadList.size() < 2)
    {
        return E_FAIL;
    }

    for (auto& elem : m_SquadList)
    {
        elem->m_pMyTeam = this;
        if (FAILED(elem->Initialize()))
            return E_FAIL;

        for (auto& pPlayer : elem->m_mapPlayers)
        {
            if (pPlayer.second->IsMainPlayer())
                m_bIsMainPlayerTeam = true;
        }
    }

    return S_OK;
}

void CTeamConnector::SetUp_TeamType()
{
   for (auto& elem : m_SquadList)
   {
       for (auto& pPlayer : elem->m_mapPlayers)
       {
           if (m_bIsMainPlayerTeam)
           {
           }
           else
           {
               pPlayer.second->Set_OutlineType(CPlayer::eENEMY);
           }

           if (m_eTeamType == eTEAM_TYPE::eBLUE)
                pPlayer.second->SetUp_UnitColliders(true);
           else
               pPlayer.second->SetUp_UnitColliders(false);



           pPlayer.second->Get_PlayerInfo()->Set_TeamType(m_eTeamType);
           pPlayer.second->Get_PlayerInfo()->Set_Squad(elem);
           pPlayer.second->Get_PlayerInfo()->Set_TeamConnector(this);
       }
   }
   

}

CPlayer* CTeamConnector::Find_Player(wstring wstrName)
{
    CPlayer* pPlayer = nullptr;

    for (auto& elem : m_SquadList)
    {
        pPlayer = elem->Find_Player(wstrName);
    }

    return pPlayer;
}

void CTeamConnector::Add_Trigger(CTrigger* pTrigger)
{
    m_OurTriggers.push_back(pTrigger);

    CTrigger_Paden::ePADEN_TRIGGER_TYPE eType = static_cast<CTrigger_Paden*>(pTrigger)->Get_TriggerType();

    if (eType != CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART)
    {
        m_bHasTrigger[(_uint)eType - 1] = true;
    }
}

void CTeamConnector::Erase_Trigger(string strTriggerKey)
{

    for (auto iter = m_OurTriggers.begin(); iter != m_OurTriggers.end();)
    {
        if (static_cast<CTrigger_Paden*>(*iter)->Get_TriggerName() == strTriggerKey)
        {
            CTrigger_Paden::ePADEN_TRIGGER_TYPE eType = static_cast<CTrigger_Paden*>(*iter)->Get_TriggerType();

            if (eType != CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART)
            {
                m_bHasTrigger[(_uint)eType - 1] = true;
            }

            iter = m_OurTriggers.erase(iter);
            break;
        }
        ++iter;
    }
}

_float4 CTeamConnector::Find_RespawnPosition_Start()
{
    return static_cast<CTrigger_Paden*>(m_OurTriggers.front())->Get_RespawnPosition();
}

_float4 CTeamConnector::Find_RespawnPosition(string strTriggerKey)
{
    _float4 vPosition = ZERO_VECTOR;
    for (auto& elem : m_OurTriggers)
    {
        if (static_cast<CTrigger_Paden*>(elem)->Get_TriggerName() == strTriggerKey)
        {
            vPosition = static_cast<CTrigger_Paden*>(elem)->Get_RespawnPosition();
            break;
        }
    }



    return vPosition;
}

_bool CTeamConnector::Minus_Score()
{
    --m_iScore;

#ifdef _DEBUG
    string strName = "BLUE";
    if (m_eTeamType == eTEAM_TYPE::eRED)
        strName = "RED";
    cout << strName << " Score : " << m_iScore << endl;
#endif // _DEBUG


    if (m_iScore == 0)
    {
        return false;
    }

    return true;
}

HRESULT CTeamConnector::On_EnterPaden()
{
    m_iScore = m_iMaxScore;



    return S_OK;
}

CSquad* CTeamConnector::Add_Squad()
{
    CSquad* pSquad = CSquad::Create(this);
    m_SquadList.push_back(pSquad);
    return pSquad;
}

void CTeamConnector::Release()
{
    for (auto& elem : m_SquadList)
        SAFE_DELETE(elem);
}
