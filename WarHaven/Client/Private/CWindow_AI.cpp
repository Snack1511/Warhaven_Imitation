#include "CWindow_AI.h"
#include "CPlayer.h"
#include "Functor.h"
#include "ImGui_Manager.h"
#include "CAIPersonality.h"
#include "CBehavior.h"
#include "CTeamConnector.h"
#include "CSquad.h"
#include "CGameSystem.h"
#include "CTable_Conditions.h"
#include "GameInstance.h"
#include "CPath.h"
#include "CDebugObject.h"
#include "Transform.h"
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
#ifndef _DEBUG
    ShowCursor(true);
#endif
    m_pTeamConnector[_uint(eTEAM_TYPE::eRED)] = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eRED);
    m_pTeamConnector[_uint(eTEAM_TYPE::eBLUE)] = CGameSystem::Get_Instance()->Get_Team(eTEAM_TYPE::eBLUE);
    m_pVecPlayerInfoName = CGameSystem::Get_Instance()->GetPtr_PlayerInfoNames();
    m_pTableCondition = CGameSystem::Get_Instance()->Get_BXTable();
}

void CWindow_AI::On_Disable()
{
#ifndef _DEBUG
    ShowCursor(false);
#endif
}

HRESULT CWindow_AI::Initialize()
{
    m_bEnable = false;
    SetUp_ImGuiDESC(typeid(CWindow_AI).name(), ImVec2(400.f, 600.f), ImGuiWindowFlags_AlwaysAutoResize);

    return S_OK;
}

void CWindow_AI::Tick()
{
    if (KEY(I, HOLD))
    {
        if (KEY(LBUTTON, TAP))
        {
            if (m_pCurSelectPlayer)
            {
                _float4 vPickPos;
                _float4 vPickNorm;
                CGameObject* pGameObject;

                //충돌체 피킹 위치 물어보기
                if (GAMEINSTANCE->Is_Picked_OutObject(GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION), &vPickPos, &vPickNorm, &pGameObject))
                {
                    m_pCurSelectPlayer->Make_BestRoute(vPickPos);
                    Set_DebugRoute(m_pCurSelectPlayer->Get_CurRoute());
                }
            }
        }
    }
}

HRESULT CWindow_AI::Render()
{
    CImGui_Manager::Get_Instance()->Push_KorFont();

    if (FAILED(__super::Begin()))
        return E_FAIL;

    m_bHoverWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
        || ImGui::IsAnyItemHovered();

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
    string strPlayerName = (m_pCurSelectPlayer) ? CFunctor::To_String_UTF8(m_pCurSelectPlayer->Get_PlayerName()) : u8"N/V";
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

    CBehavior* pBehavior = m_pCurSelectPlayer->Get_CurBehavior();
    CPath* pPath = m_pCurSelectPlayer->Get_CurPath();
    string strBehaviorName;
    string strPathName;

    if (pBehavior) {
        strBehaviorName = CFunctor::To_String(pBehavior->Get_BehaviorName());
    }
    else
    {
        strBehaviorName = u8"행동 없음";
    }
    if (pPath)
    {
        strPathName = pPath->Get_PathName();
    }
    Display_Data(string(u8"현재 행동"), strBehaviorName.c_str());

    Display_Data(string(u8"현재 패스"), strPathName.c_str());
    
    if (ImGui::CollapsingHeader(u8"경로찾기 테스트"))
    {
        if (nullptr == m_pDebugDestination)
        {
            if (ImGui::Button(u8"도착지 보이기"))
            {
                Set_DebugDestination(pPath->Get_vecPositions()[0]);
            }
        }
        else
        {
            if (ImGui::Button(u8"도착지 숨기기"))
            {
                Clear_DebugDestination();
            }
        }
        if (m_listRouteDebug.empty())
        {
            if (ImGui::Button(u8"경로 보이기"))
            {
                Set_DebugRoute(m_pCurSelectPlayer->Get_CurRoute());
            }
        }
        else
        {
            if (ImGui::Button(u8"경로 숨기기"))
            {
                Clear_DebugRoute();
            }
        }
        if (m_listNodeDebug.empty())
        {
            if (ImGui::Button(u8"실제 노드 보이기"))
            {
                Set_DebugNode(m_pCurSelectPlayer->Get_CurRoute());
            }
        }
        else
        {
            if (ImGui::Button(u8"실제 노드 숨기기"))
            {
                Clear_DebugNode();
            }
        }
    }

    //행동 리스트 보기
    ImVec2 vBehaviorSize = ImVec2(m_vMainWndSize.x, 0.f);
    if (ImGui::CollapsingHeader(u8"행동 리스트")) 
    {
        ListUp_Behaviors("##BehaviorList", vBehaviorSize, m_pCurSelectPersonality->Get_BehaviorList());
    }
    //행동조건 설정
    if (nullptr != m_pCurSelectBehavior)
    {
        Create_SubWindow(u8"행동 수치 설정", ImVec2(0.f, 0.f), vBehaviorSize, bind(&CWindow_AI::Func_ChangeBehavior, this));
        Create_SubWindow(u8"조건 설정", ImVec2(0.f, 0.f), vBehaviorSize, bind(&CWindow_AI::Func_ChangeBehaviorCondition, this));
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

            string SquadName = CFunctor::To_String_UTF8(pSquadLeader->Get_PlayerName());
            SquadName += "_Squad";

            if (ImGui::TreeNodeEx(SquadName.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected))
            {
                string strLeaderName = CFunctor::To_String_UTF8(pSquadLeader->Get_PlayerName());
                if (pCurSelectPlayer == pSquadLeader)
                {
                    pSquad = SquadValue;
                    bSelectLeader = true;
                }
                if (ImGui::Selectable(strLeaderName.c_str(), bSelectLeader))
                {
                    pCurSelectPlayer = pSquadLeader;
                    m_pCurSelectPlayer = pSquadLeader;
                    GAMEINSTANCE->Change_Camera(pCurSelectPlayer->Get_PlayerInfo()->Get_LookAtCamName());
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
                    string strPlayerName = CFunctor::To_String_UTF8(PlayerValue.second->Get_PlayerName());
                    if (ImGui::Selectable(strPlayerName.c_str(), bSelect))
                    {
                        pCurSelectPlayer = PlayerValue.second;
                        m_pCurSelectPlayer = PlayerValue.second;
                        GAMEINSTANCE->Change_Camera(pCurSelectPlayer->Get_PlayerInfo()->Get_LookAtCamName());
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
        strPreviewName = CFunctor::To_String_UTF8((*m_pVecPlayerInfoName)[m_iCurPlayerNameIndex]);
    }
    else {
        strPreviewName = CFunctor::To_String_UTF8((*m_pVecPlayerInfoName)[m_iCurPlayerNameIndex]);
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
                string strName = CFunctor::To_String_UTF8((*m_pVecPlayerInfoName)[i]);
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
    if (nullptr == m_pCurSelectPersonality) 
    {
        ImGui::Text(u8"Personality없음");
        return;
    }
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
            if (ImGui::Selectable(strBehaviorName.c_str(), bSelect, ImGuiSelectableFlags_AllowDoubleClick))
            {
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    m_pCurSelectPersonality->Delete_Behavior(m_pCurSelectBehavior);
                    m_pCurSelectBehavior = nullptr;
                    m_strCurSelectWhenCondition = L"";
                    m_strCurSelectWhatCondition = L"";
                    m_pCurSelectPersonality->Sort_BehaviorWithPriority();
                    break;
                }
                else {
                    if (bSelect)
                    {
                        m_pCurSelectBehavior = nullptr;
                    }
                    else
                        m_pCurSelectBehavior = Value;

                }


                //더블 클릭시 행동 제거..
            }
        }
        ImGui::EndListBox();
    }

    //행동 테이블에서 행동 추가
    vector<wstring>& rhsBehaviorNames = m_pTableCondition->Get_BehaviorNames();
    string StrConditionTableListName = ListID;
    StrConditionTableListName += "_Table";
    ImGui::Text(u8"행동테이블");
    if (ImGui::BeginListBox(StrConditionTableListName.c_str(), Size))
    {
        if (rhsBehaviorNames.empty())
        {
            ImGui::Text(u8"행동 없음");
        }
        else
        {
            for (auto& Name : rhsBehaviorNames)
            {
                if (ImGui::Selectable(CFunctor::To_String(Name).c_str()))
                {
                    CBehavior* pBehavior = m_pCurSelectPersonality->Add_Behavior(Name);
                    m_pCurSelectPersonality->Sort_BehaviorWithPriority();
                    m_pCurSelectBehavior = pBehavior;
                }
            }
            //조건 이름 가져오기..

        }
        ImGui::EndListBox();
    }
    if (ImGui::Button(u8"행동 초기화"))
    {
        m_pCurSelectPersonality->Clear_Behavior();
    }
}



void CWindow_AI::ListUp_BehaviorConditions(const char* szListName, const char* ListID, const ImVec2& Size, CBehavior* pBehavior, wstring& rhsConditionName, _uint iConditionType)
{
    ImGui::Text(szListName);
    if (nullptr == m_pTableCondition) 
    {
        ImGui::Text(u8"조건 테이블 못찾음");
        return;
    }
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
        vector<wstring>& rhsConditionNameVector = pBehavior->Get_ConditionNames(eBehaviorConditionType(iConditionType));
        if (ImGui::BeginListBox(ListID, Size))
        {
            if (rhsConditionNameVector.empty())
            {
                ImGui::Text(u8"조건 없음");
            }
            else
            {
                for (auto& Name : rhsConditionNameVector)
                {
                    _bool bSelect = false;
                    if (rhsConditionName == Name)
                        bSelect = true;

                    if (ImGui::Selectable(CFunctor::To_String(Name).c_str(), bSelect))
                    {
                        rhsConditionName = Name;
                        m_pCurSelectBehavior->Delete_Condition(rhsConditionName, eBehaviorConditionType(iConditionType));
                    }
                    //m_CurSelectIncludeCondition.strConditionName = rhsConditionName;
                    //lstrcpy(m_CurSelectIncludeCondition.szConditionName, Name.c_str());
                    //m_CurSelectIncludeCondition.iConditionType = iConditionType;
                    //m_CurSelectIncludeCondition.pBehavior = pBehavior;
                    //On_DragTarget("Include_Condition", &m_CurSelectIncludeCondition, sizeof(DraggingData), CFunctor::To_String(rhsConditionName).c_str());
                }
                //조건 이름 가져오기..

            }
            On_DropTarget("Exclude_Condition", sizeof(DraggingData), bind(&CWindow_AI::Add_Condition, this, placeholders::_1));

            ImGui::EndListBox();
        }
    }
    
    vector<wstring>& rhsConditionTable = m_pTableCondition->Get_ConditionNames(iConditionType);
    string StrConditionTableListName = ListID;
    StrConditionTableListName += "_ConditionTable";
    ImGui::Text(u8"조건테이블");
    if (ImGui::BeginListBox(StrConditionTableListName.c_str(), Size))
    {
        if (rhsConditionTable.empty())
        {
            ImGui::Text(u8"조건 없음");
        }
        else
        {
            for (auto& Name : rhsConditionTable)
            {
                if (ImGui::Selectable(CFunctor::To_String(Name).c_str()))
                {
                    m_pCurSelectBehavior->Add_Condition(Name, eBehaviorConditionType(iConditionType));
                }
                //lstrcpy(m_CurSelectIncludeCondition.szConditionName, Name.c_str());
                //m_CurSelectExcludeCondition.iConditionType = iConditionType;
                //m_CurSelectExcludeCondition.pBehavior = pBehavior;
                //On_DragTarget("Exclude_Condition", &m_CurSelectExcludeCondition, sizeof(DraggingData), CFunctor::To_String(Name).c_str());
            }
            //조건 이름 가져오기..

        }
        ImGui::EndListBox();
    }
    if (ImGui::Button(u8"조건 초기화")) 
    {
        pBehavior->Clear_Condition(eBehaviorConditionType(iConditionType));
    }
}

void CWindow_AI::Func_ChangeBehavior()
{
    if (nullptr == m_pCurSelectPersonality)
        return;
    if (nullptr == m_pCurSelectBehavior)
        return;

    _int iPriority = m_pCurSelectBehavior->Get_Priority();
    ImGui::Text(u8"현재 행동의 우선순위 : ");
    if (ImGui::InputInt("##InputPriority", &iPriority))
    {
        if (iPriority < 0)
            iPriority = 0;

        m_pCurSelectBehavior->Set_Priority(iPriority);
        m_pCurSelectPersonality->Sort_BehaviorWithPriority();
    }
}

void CWindow_AI::Func_ChangeBehaviorCondition()
{
    //비해비어 조건 변경
    if (nullptr == m_pCurSelectBehavior)
        return;
    ImVec2 vBehaviorSize = ImVec2(m_vMainWndSize.x, 0.f);
    if (ImGui::CollapsingHeader(u8"조건 리스트"))
    {
        ListUp_BehaviorConditions("WhenCondition", "##WhenConditions",
            vBehaviorSize, m_pCurSelectBehavior,
            m_strCurSelectWhenCondition,
            _uint(eBehaviorConditionType::eWhen));

        ListUp_BehaviorConditions("WhatCondition", "##WhatConditions",
            vBehaviorSize, m_pCurSelectBehavior,
            m_strCurSelectWhatCondition, 
            _uint(eBehaviorConditionType::eWhat));

        ListUp_BehaviorConditions("BehaviorTick", "##BehaviorTick",
            vBehaviorSize, m_pCurSelectBehavior,
            m_strCurSelectBehaviorTick,
            _uint(eBehaviorConditionType::eTick));
    }

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
    if (nullptr == m_pTableCondition)
        return nullptr;
    CAIPersonality* pPersonality = CAIPersonality::Create(m_pTableCondition);

    return pPersonality;
}

void CWindow_AI::Save_Personality(CAIPersonality* pCurSelectPersonality)
{
    if (nullptr == pCurSelectPersonality)
        return;
    pCurSelectPersonality->Save();
    //pCurSelectPersonality--> Save루틴
}

CAIPersonality* CWindow_AI::Load_Personality(string strLoadName)
{
    //Personality로드 루틴
    CAIPersonality* pPersonality = CAIPersonality::Create(m_pTableCondition);
    pPersonality->Load(CFunctor::To_Wstring(strLoadName));
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
void CWindow_AI::Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_AI&)> func)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImVec2 ViewPos(main_viewport->WorkPos);
    ImVec2 ViewSize(main_viewport->WorkSize);
    bool Open = true;
    ImGuiWindowFlags WindowFlags
        = ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar
        | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar
        | ImGuiWindowFlags_AlwaysAutoResize;

    //ImGui::SetNextWindowPos(ImVec2(ViewSize.x - Pos.x, ViewPos.y + Pos.y));
    //ImGui::SetNextWindowSize(Size);

    ImGui::Begin(szWindowName, &Open, WindowFlags);
    //&& ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    func(*this);
    ImGui::End();
}

void CWindow_AI::On_DragTarget(const char* DataType, void* TargetData, size_t size, const char* DataToolTip)
{
    ImGuiDragDropFlags src_flags = 0;
    src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
    src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
    //src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
    if (ImGui::BeginDragDropSource(src_flags))
    {
        if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            ImGui::Text(DataToolTip);
        ImGui::SetDragDropPayload(DataType, TargetData, size);
        ImGui::EndDragDropSource();
    }
}

void CWindow_AI::On_DropTarget(const char* DataType, size_t size, const function<void(const ImGuiPayload*)>& func)
{
    ImGui::Selectable("+##EmptySelectable");
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DataType, target_flags);
        if (payload)
        {
            func(payload);
        }
        ImGui::EndDragDropTarget();
    }
}

void CWindow_AI::Add_Condition(const ImGuiPayload* pPayload)
{
    if (nullptr == pPayload)
        return;

    if (nullptr == pPayload->Data)
        return;


    DraggingData _DraggingData;// = reinterpret_cast<TupleCondition*>(pPayload->Data);


    memcpy_s(&_DraggingData, sizeof(DraggingData), pPayload->Data, sizeof(DraggingData));

    //wstring strConditionName = _DraggingData.strConditionName;
    wstring strConditionName = _DraggingData.szConditionName;
    _uint iConditionType = _DraggingData.iConditionType;
    CBehavior* pBehavior = _DraggingData.pBehavior;

    if (nullptr == pBehavior)
        return;

    pBehavior->Add_Condition(strConditionName, eBehaviorConditionType(iConditionType));

}



void CWindow_AI::Set_DebugDestination(_float4 vPosition)
{
    PxTransform tTransform;
    ZeroMemory(&tTransform, sizeof(PxTransform));
    tTransform.p.x = vPosition.x;
    tTransform.p.y = vPosition.y;
    tTransform.p.z = vPosition.z;
    m_pDebugDestination = CDebugObject::Create(tTransform);
    CREATE_GAMEOBJECT(m_pDebugDestination, GROUP_PHYSX);
}

void CWindow_AI::Set_DebugRoute(list<_float4> vPosList)
{
    _float4 vPrevPos;
    _float4 vCurPos;
    _bool bFirst = true;
    for (auto Pos : vPosList)
    {
        vCurPos = Pos;
        if (bFirst)
        {
            vPrevPos = vCurPos;
            bFirst = false;
            continue;
        }
        _float4 vDir = (vCurPos - vPrevPos);
        _float fLength = vDir.Length();
        _float4 vLinePos = (vCurPos + vPrevPos) * 0.5f;

        _float4 vScale = _float4(0.2f, 0.2f, fLength);

        CDebugObject* pDebugLine = CDebugObject::Create(vLinePos, vScale);
        pDebugLine->Initialize();
        pDebugLine->Set_Blue();
        pDebugLine->Get_Transform()->Set_Look(vDir);
        CREATE_GAMEOBJECT(pDebugLine, GROUP_PROP);
        m_listRouteDebug.push_back(pDebugLine);

        vPrevPos = vCurPos;
    }
}

void CWindow_AI::Set_DebugNode(list<_float4> vPosList)
{
    for (auto Pos : vPosList)
    {
        PxTransform tTransform;
        ZeroMemory(&tTransform, sizeof(PxTransform));
        tTransform.p.x = Pos.x;
        tTransform.p.y = Pos.y;
        tTransform.p.z = Pos.z;
        m_listNodeDebug.push_back(
            CDebugObject::Create(tTransform)
        );
        CREATE_GAMEOBJECT(m_listNodeDebug.back(), GROUP_PHYSX);
    }
}

void CWindow_AI::Clear_DebugDestination()
{
    DELETE_GAMEOBJECT(m_pDebugDestination);
    m_pDebugDestination = nullptr;
}

void CWindow_AI::Clear_DebugRoute()
{
    for (auto& Debug : m_listRouteDebug)
    {
        DELETE_GAMEOBJECT(Debug);
    }
    m_listRouteDebug.clear();
}

void CWindow_AI::Clear_DebugNode()
{
    for (auto& Debug : m_listNodeDebug)
    {
        DELETE_GAMEOBJECT(Debug);
    }
    m_listNodeDebug.clear();
}


