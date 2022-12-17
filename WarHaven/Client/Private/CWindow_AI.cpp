#include "CWindow_AI.h"
#include "CPlayer.h"
#include "Functor.h"
#include "ImGui_Manager.h"
#include "CAIPersonality.h"
#include "CBehavior.h"
#include "CTeamConnector.h"
#include "CSquad.h"
#include "CGameSystem.h"

CWindow_AI::CWindow_AI()
{
}

CWindow_AI::~CWindow_AI()
{
}

CWindow_AI* CWindow_AI::Create()
{
    CWindow_AI* pInstance = new CWindow_AI;
    
    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWindow_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CWindow_AI::On_Enable()
{
    m_pTeamConnector[_uint(eTEAM_TYPE::eRED)] = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED);
    m_pTeamConnector[_uint(eTEAM_TYPE::eBLUE)] = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE);
    m_pVecPlayerInfoName = CGameSystem::Get_Instance()->GetPtr_PlayerInfoNames();
}

HRESULT CWindow_AI::Initialize()
{
    m_bEnable = false;
    SetUp_ImGuiDESC(typeid(CWindow_AI).name(), ImVec2(400.f, 600.f), ImGuiWindowFlags_AlwaysAutoResize);

    return S_OK;
}

void CWindow_AI::Tick()
{
}

HRESULT CWindow_AI::Render()
{
    CImGui_Manager::Get_Instance()->Push_KorFont();

    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::Text("AI_Tool");
    CImGui_Manager::Get_Instance()->On_ToolTip(u8"AI 툴");
    m_vMainWndSize = ImGui::GetWindowSize();
    ImVec2 vGroupSize = ImVec2(m_vMainWndSize.x * 0.45f, 0.f);
    //현재 띄워진 AI플레이어(레드팀, 블루팀)
    //플레이어 추가 제거(레드팀, 블루팀)
    if (ImGui::CollapsingHeader(u8"팀리스트")) 
    {
        On_Grouping(u8"레드 팀",
            vGroupSize,
            bind(&CWindow_AI::ListUp_RedPlayer, this, placeholders::_1));

        ImGui::SameLine();

        On_Grouping(u8"블루 팀",
            vGroupSize,
            bind(&CWindow_AI::ListUp_BluePlayer, this, placeholders::_1));
    }
    if (ImGui::CollapsingHeader(u8"AI 셋팅")) 
    {
        Func_PlayerSetting();
        Func_AISetting();
    }


    __super::End();

    CImGui_Manager::Get_Instance()->Pop_Font();
    return S_OK;
}

void CWindow_AI::On_Grouping(const char* GroupName, const ImVec2& Size, const function<void(const ImVec2&)> func)
{
    ImGui::BeginGroup();
    ImGui::Text(GroupName);
    func(Size);
    ImGui::EndGroup();
}

void CWindow_AI::Func_PlayerSetting()
{
    string strPlayerName = (m_pCurSelectPlayer) ? CFunctor::To_String(m_pCurSelectPlayer->Get_PlayerName()) : u8"N/V";
    Display_Data(u8"플레이어 이름", strPlayerName);

    if (nullptr == m_pCurSelectPlayer)
        return;

    SetUp_SelectPersonality();
}

void CWindow_AI::Func_AISetting()
{
    if (nullptr == m_pCurSelectPlayer)
        return;

    string strPersonalityName;
    
    strPersonalityName = 
        (m_pCurSelectPersonality) ? CFunctor::To_String(m_pCurSelectPersonality->Get_PersonalityName()) : "";
    char szPersonalityName[MAXCHAR] = "";
    strcpy_s(szPersonalityName, strPersonalityName.c_str());

    //이름 설정
    ImGui::Text(u8"AI 이름 : ");
    ImGui::SameLine();
    if (ImGui::InputText("##InputPersonality", szPersonalityName, sizeof(char) * MAXCHAR))
    {
        strPersonalityName = szPersonalityName;
        if(m_pCurSelectPersonality)
            m_pCurSelectPersonality->Set_PersonalityName(CFunctor::To_Wstring(strPersonalityName));
    }
    
    if (ImGui::Button(u8"새 AI생성"))
    {
        m_pCurSelectPersonality = Create_Personality(strPersonalityName);
        m_pCurSelectPlayer->Set_Personality(m_pCurSelectPersonality);
    }
    if (ImGui::Button(u8"기존 AI불러오기"))
    {
        m_pCurSelectPersonality = Load_Personality(strPersonalityName);
        m_pCurSelectPlayer->Set_Personality(m_pCurSelectPersonality);
    }
    if (ImGui::Button(u8"현재 AI저장"))
    {
        Save_Personality(m_pCurSelectPersonality);
    }

    if (nullptr == m_pCurSelectPersonality)
        return;

    //행동 리스트 보기
    ImVec2 vBehaviorSize = ImVec2(m_vMainWndSize.x, 0.f);
    if (ImGui::CollapsingHeader(u8"행동 리스트")) 
    {
        ListUp_Behaviors("##BehaviorList", vBehaviorSize, m_pCurSelectPersonality->Get_BehaviorList());
    }
    //행동조건 설정
    if (ImGui::CollapsingHeader(u8"조건 리스트")) 
    {
        ListUp_BehaviorConditions("##Conditions", vBehaviorSize, m_pCurSelectBehavior);
    }
    //행동 변수설정
    if (ImGui::CollapsingHeader(u8"행동 정의")) 
    {
        Func_ChangeBehavior(m_pCurSelectBehavior);
    }
}

void CWindow_AI::ListUp_RedPlayer(const ImVec2& Size)
{
    ListUp_Player(
        "##List_RedPlayer",
        Size, 
        m_pCurSelectRedPlayer, 
        m_pTeamConnector[_uint(eTEAM_TYPE::eRED)],
        eTEAM_TYPE::eRED);
}

void CWindow_AI::ListUp_BluePlayer(const ImVec2& Size)
{
    ListUp_Player(
        "##List_BluePlayer", 
        Size, 
        m_pCurSelectBluePlayer, 
        m_pTeamConnector[_uint(eTEAM_TYPE::eBLUE)],
        eTEAM_TYPE::eBLUE);
}

void CWindow_AI::ListUp_Player(const char* ListID, const ImVec2& Size, CPlayer*& pCurSelectPlayer, CTeamConnector* TeamConnector, eTEAM_TYPE eTeamType)
{
    if (nullptr == TeamConnector)
    {
        if (ImGui::BeginListBox(ListID, Size))
        {
            ImGui::Text(u8"팀 생성 안됨");
            ImGui::EndListBox();
        }
        if (ImGui::Button(u8"팀 임의 생성"))
        {
            Add_Team(eTeamType);
        }
        return;
    }

    CSquad* pSquad = nullptr;
    if (ImGui::BeginListBox(ListID, Size))
    {
        list<CSquad*>& SquadList = TeamConnector->Get_SquadList();
        _int Index = 0;
        for (auto& SquadValue : SquadList)
        {
            map<_hashcode, CPlayer*>& PlayerMap = SquadValue->Get_AllPlayers();
            _bool bSelectLeader = false;
            CPlayer* pSquadLeader = SquadValue->Get_LeaderPlayer();
            if (PlayerMap.empty())
                continue;
            if(nullptr == pSquadLeader)
                continue;

            string SquadName = CFunctor::To_String(pSquadLeader->Get_PlayerName());
            SquadName += "_Squad";

            if (ImGui::TreeNodeEx(SquadName.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected))
            {
                string strLeaderName = CFunctor::To_String(pSquadLeader->Get_PlayerName());
                if (pCurSelectPlayer == pSquadLeader)
                {
                    pSquad = SquadValue;
                    bSelectLeader = true;
                }
                if (ImGui::Selectable(strLeaderName.c_str(), bSelectLeader))
                {
                    pCurSelectPlayer = pSquadLeader;
                    m_pCurSelectPlayer = pSquadLeader;
                }//리더 먼저 출력

                for (auto& PlayerValue : PlayerMap)
                {
                    if (pSquadLeader == PlayerValue.second)
                        continue;

                    _bool bSelect = false;
                    if (pCurSelectPlayer == PlayerValue.second)
                    {
                        pSquad = SquadValue;
                        bSelect = true;
                    }
                    string strPlayerName = CFunctor::To_String(PlayerValue.second->Get_PlayerName());
                    if (ImGui::Selectable(strPlayerName.c_str(), bSelect))
                    {
                        pCurSelectPlayer = PlayerValue.second;
                        m_pCurSelectPlayer = PlayerValue.second;
                    }
                }//나머지 분대원들 출력
                ImGui::TreePop();
            }
        }
        ImGui::EndListBox();
    }
    string strPreviewName = "";
    if (nullptr == m_pVecPlayerInfoName)
        strPreviewName = "N/A";
    else if (m_pVecPlayerInfoName->empty())
    {
        strPreviewName = "N/V";
        m_iCurPlayerNameIndex = 0;
    }
    else if (m_iCurPlayerNameIndex >= _uint(m_pVecPlayerInfoName->size()))
    {
        m_iCurPlayerNameIndex = _uint(m_pVecPlayerInfoName->size()) - 1;
        strPreviewName = u8"";
        strPreviewName += CFunctor::To_String((*m_pVecPlayerInfoName)[m_iCurPlayerNameIndex]);
    }
    else {
        strPreviewName = u8"";
        strPreviewName += CFunctor::To_String((*m_pVecPlayerInfoName)[m_iCurPlayerNameIndex]);
    }
    if (ImGui::BeginCombo("##SelectPlayerInfoName", strPreviewName.c_str()))
    {
        if (nullptr != m_pVecPlayerInfoName) 
        {
            for (_uint i = 0; i < _uint(m_pVecPlayerInfoName->size()); ++i)
            {
                _bool bSelect = false;
                if (m_iCurPlayerNameIndex == i)
                {
                    bSelect = true;
                }
                string strName = u8"";
                strName += CFunctor::To_String((*m_pVecPlayerInfoName)[i]).c_str();
                if (ImGui::Selectable(strName.c_str(), bSelect))
                {
                    m_iCurPlayerNameIndex = i;
                }
            }
        }
        ImGui::EndCombo();
    }
    wstring strPlayerInfoName = L"";
    if (nullptr == m_pVecPlayerInfoName 
        || m_pVecPlayerInfoName->empty() 
        || m_iCurPlayerNameIndex >= _uint(m_pVecPlayerInfoName->size()))
    {
        strPlayerInfoName = L"";
    }
    else {
        strPlayerInfoName = (*m_pVecPlayerInfoName)[m_iCurPlayerNameIndex];
    }

    if (ImGui::Button(u8"분대장 추가"))
    {
        pCurSelectPlayer = Add_Player(true, strPlayerInfoName, pSquad, eTeamType);
        m_pCurSelectPlayer = pCurSelectPlayer;

    }
    ImGui::SameLine();
    if (ImGui::Button(u8"분대원 추가"))
    {
        pCurSelectPlayer = Add_Player(false, strPlayerInfoName, pSquad, eTeamType);
        m_pCurSelectPlayer = pCurSelectPlayer;
    }
    if (ImGui::Button(u8"삭제"))
    {
        Delete_Player(pCurSelectPlayer, pSquad);
        Clear_SelectPlayerData();
    }
}

void CWindow_AI::ListUp_Behaviors(const char* ListID, const ImVec2& Size, list<CBehavior*>& BehaviorList)
{
    if (ImGui::BeginListBox(ListID, Size))
    {
        for (auto& Value : BehaviorList)
        {
            _bool bSelect = false;
            if (m_pCurSelectBehavior == Value)
            {
                bSelect = true;
            }
            string strBehaviorName = CFunctor::To_String(Value->Get_BehaviorName());
            if (ImGui::Selectable(strBehaviorName.c_str(), bSelect))
            {
                if (bSelect)
                    m_pCurSelectBehavior = nullptr;
                else
                    m_pCurSelectBehavior = Value;
            }
        }
        ImGui::EndListBox();
    }
}

void CWindow_AI::ListUp_BehaviorConditions(const char* ListID, const ImVec2& Size, CBehavior* pBehavior)
{
    if (nullptr == pBehavior)
    {
        if (ImGui::BeginListBox(ListID, Size))
        {
            ImGui::Text(u8"선택된 행동 없음");
            ImGui::EndListBox();
        }
    }
    else
    {
        if (ImGui::BeginListBox(ListID, Size))
        {
            if (/*조건리스트사이즈 == 0*/true)
            {
                ImGui::Text(u8"조건 없음");
            }
            else
            {
                //조건 이름 가져오기..
            }

            ImGui::EndListBox();
        }
    }
    if (ImGui::Button(u8"조건 추가"));
    {
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"조건 제거"))
    {

    }
    if (ImGui::Button(u8"조건 초기화")) 
    {
    }
}

void CWindow_AI::Func_ChangeBehavior(CBehavior* pBehavior)
{
    if (nullptr == m_pCurSelectBehavior)
        return;
}

void CWindow_AI::Display_Data(string strTitle, string strData, const ImVec4& vTitleColor, const ImVec4& vDataColor)
{
    ImGui::TextColored(vTitleColor, strTitle.c_str());
    ImGui::SameLine();
    ImGui::TextColored(vTitleColor, " : ");
    ImGui::SameLine();
    ImGui::TextColored(vDataColor, strData.c_str());
}

void CWindow_AI::Add_Team(eTEAM_TYPE eTeamType)
{
    m_pTeamConnector[(_uint)eTeamType] = CTeamConnector::Create(list<CSquad*>(), eTeamType);
}

CPlayer* CWindow_AI::Add_Player(_bool bLeader, wstring strPlayerInfoName, CSquad* pSquad, eTEAM_TYPE eTeamType)
{
    if (nullptr == m_pTeamConnector[_uint(eTeamType)])
        return nullptr;
    CSquad* pSelectSquad = nullptr;
    if (bLeader)
        pSelectSquad = m_pTeamConnector[_uint(eTeamType)]->Add_Squad();
    else
        pSelectSquad = pSquad;

    if (nullptr == pSelectSquad)
        return nullptr;


    CPlayerInfo* pPlayerInfo = CGameSystem::Get_Instance()->Find_PlayerInfo(Convert_ToHash(strPlayerInfoName));
    if (nullptr == pPlayerInfo)
        return nullptr;
    CPlayer* pPlayer = nullptr;//플레이어 생성
    pPlayer = pPlayerInfo->Make_Player();
    pPlayer->Set_Unit_ReserveState(WARRIOR, STATE_IDLE_WARRIOR_R_AI_ENEMY);
    pSelectSquad->Add_EmptyPlayer(bLeader, pPlayer);
    pPlayer->Get_PlayerInfo()->Set_Squad(pSquad);
    pPlayer->Get_PlayerInfo()->Set_TeamConnector(m_pTeamConnector[_uint(eTeamType)]);
    pPlayer->Get_PlayerInfo()->Set_TeamType(eTeamType);
    //이게 맞..나..?
    return pPlayer;
}

void CWindow_AI::Delete_Player(CPlayer* pSelectPlayer, CSquad* pSquad)
{
    if (nullptr == pSquad)
        return;
    pSquad->Delete_EmptyPlayer(pSelectPlayer);

    //pSelectPlayer->삭제
}

CAIPersonality* CWindow_AI::Create_Personality(string strPersonalityName)
{
    //Personality생성 루틴
    CAIPersonality* pPersonality = nullptr;
    return pPersonality;
}

void CWindow_AI::Save_Personality(CAIPersonality* pCurSelectPersonality)
{
    if (nullptr == pCurSelectPersonality)
        return;
    //pCurSelectPersonality--> Save루틴
}

CAIPersonality* CWindow_AI::Load_Personality(string strLoadName)
{
    //Personality로드 루틴
    CAIPersonality* pPersonality = nullptr;
    return pPersonality;
}

void CWindow_AI::Clear_SelectPlayerData()
{
    m_pCurSelectRedPlayer = nullptr;
    m_pCurSelectBluePlayer = nullptr;
    m_pCurSelectPlayer = nullptr;
    m_pCurSelectPersonality = nullptr;
    m_pCurSelectBehavior = nullptr;
}

void CWindow_AI::SetUp_SelectPersonality()
{
    if(nullptr != m_pCurSelectPlayer)
        m_pCurSelectPersonality = m_pCurSelectPlayer->Get_Personality();
}

void CWindow_AI::Update_Personality()
{
    if (nullptr == m_pCurSelectPlayer)
        return;
    if (nullptr == m_pCurSelectPersonality)
        return;

    m_pCurSelectPlayer->Set_Personality(m_pCurSelectPersonality);
}

