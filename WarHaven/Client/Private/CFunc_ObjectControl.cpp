#include "stdafx.h"
#include "CFunc_ObjectControl.h"

#include "CWindow_Map.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "CStructure.h"
#include "Transform.h"
#include "CUtility_Transform.h"

#include "Functor.h"
#include "CStructure_Instance.h"
#include "ImGui_Manager.h"

CFunc_ObjectControl::CFunc_ObjectControl()
{
}

CFunc_ObjectControl::~CFunc_ObjectControl()
{
}

CFunc_ObjectControl* CFunc_ObjectControl::Create(CWindow_Map* pMaptool)
{
    CFunc_ObjectControl* pInstance = new CFunc_ObjectControl();
    pInstance->m_pMapTool = pMaptool;
    pInstance->m_pMeshRoot = pMaptool->Get_MeshRoot();
    pInstance->SetUp_ColliderType();
    pInstance->SetUp_LODLevel();
    pInstance->m_matPickedAnchor.Identity();
    return pInstance;
}

void CFunc_ObjectControl::Destroy()
{
    delete this;
}
static char szMeshGroupNameBuf[MAXCHAR] = "";
void CFunc_ObjectControl::Func_Grouping()
{
    if (ImGui::CollapsingHeader(u8"그룹 설정", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //그룹 이름
        string DebugMeshGroupName = szMeshGroupNameBuf;
        m_pMapTool->DebugData(u8"오브젝트 그룹 명", DebugMeshGroupName);
        m_pMapTool->DebugData(u8"선택된 메시 명", m_CurSelectedMeshName);
        ImGui::InputText("##GroupNameInput", szMeshGroupNameBuf, sizeof(char) * MAXCHAR);
        if (ImGui::Button(u8"그룹 추가"))
        {
            Add_ObjectGroup(szMeshGroupNameBuf);
        }
        ImGui::SameLine();
        if (ImGui::Button(u8"그룹 제거"))
        {
            Delete_ObjectGroup(szMeshGroupNameBuf);
        }

        if (ImGui::Button(u8"그룹 내 같은 오브젝트 전체 제거"))
        {
            Clear_SameNameInGroup(
                get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]), m_iCurSelectObjecNametIndex);
            m_pCurSelectObjectGroup = nullptr;
            m_pCurSelectDataGroup = nullptr;
            Confirm_Data();
            m_iCurSelectObjecNametIndex = 0;
        }//그룹내에 같은 오브젝트들 삭제
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"현재 그룹에서 동일 이름을 가진 오브젝트 제거");

        ImGui::Checkbox("GroupControl", &m_bGroupControl);
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"그룹 단위 컨트롤 시 선택");
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_FBXList()
{
    if (ImGui::CollapsingHeader(u8"FBX 리스트", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //파일 탐색 트리노드
        if (ImGui::BeginListBox("##FBX_Files_List", ImVec2(360.f, 200.f)))
        {
            m_pMapTool->Show_TreeData(m_pMeshRoot, bind(&CFunc_ObjectControl::Routine_MeshSelect, this, placeholders::_1));
            ImGui::EndListBox();
        }
        m_pMapTool->DebugData(u8"선택된 메시 경로", m_CurSelectedMeshFilePath);
        m_pMapTool->DebugData(u8"선택된 메시 명", m_CurSelectedMeshName);
        ImGui::Spacing();

        if (ImGui::Button(u8"오브젝트 추가"))
        {
            Func_AddObject();
            SetUp_CurSelectObject();

        }
        ImGui::SameLine();
        if (ImGui::Button(u8"오브젝트 제거"))
        {
            Func_DeleteOBject();
            SetUp_CurSelectObject();
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        if (ImGui::Button(u8"HLOD 추가"))
        {
            Add_HLOD();
        }
        ImGui::SameLine();
        if (ImGui::Button(u8"HLOD 비우기"))
        {
            for (auto& elem : m_pHLODList)
            {
                DELETE_GAMEOBJECT(elem);
            }
            m_pHLODList.clear();
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"F7입력시 켜고 끌 수 있음");
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_ObjectList()
{
   

    if (ImGui::CollapsingHeader(u8"그룹핑된 리스트", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        if (m_pCurSelectGroupingNameArr && ((_uint)m_pCurSelectGroupingNameArr->size() > 10000))
        {
            m_pCurSelectGroupingNameArr = nullptr;
            return;
        }


        //해당 이름을 가진 탭
        if (ImGui::BeginTabBar("##AddObject TabBar"))
        {
            for (_int i = 0; i < _int(m_GroupingInfo.size()); ++i)
            {
                if (ImGui::BeginTabItem(get<Tuple_GroupName>(m_GroupingInfo[i]).c_str()))
                {
                    //리스트 업데이트
                    ImGui::Text(u8"그룹 리스트");
                    if (ImGui::BeginListBox("##TabGroupList", ImVec2(360.f, 200.f)))
                    {
                        m_SelectObjectGroupIndex = i;
                        OBJECTGROUPINGMAP::iterator NamingGroup;
                        string CurSelectGroupName = string(get< Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]));
                        size_t Hashnum = Convert_ToHash(CurSelectGroupName);//HASHING(string, CurSelectGroupName);
                        _bool bFindGropingName = Find_ObjectGroupingName(Hashnum, NamingGroup);
                        if (bFindGropingName)
                        {
                            m_pCurSelectGroupingNameArr = (&NamingGroup->second);
                        }
                        if (nullptr != m_pCurSelectGroupingNameArr)
                        {
                            for (_uint ObjectNameIndex = 0; ObjectNameIndex < _uint(m_pCurSelectGroupingNameArr->size()); ++ObjectNameIndex)
                            {
                                _bool bSelected = false;
                                if (m_iCurSelectObjecNametIndex == ObjectNameIndex)
                                {
                                    bSelected = true;
                                }
                                if (ImGui::Selectable(get<0>((*m_pCurSelectGroupingNameArr)[ObjectNameIndex]).c_str(), bSelected))
                                {
                                    m_strCurSelectObjectName = get<0>((*m_pCurSelectGroupingNameArr)[ObjectNameIndex]);
                                    m_iCurSelectObjecNametIndex = ObjectNameIndex;
                                    ObjectMap::iterator ObjectMapIter;
                                    DataMap::iterator DataMapIter;
                                    Find_ObjectDatas(m_strCurSelectObjectName, ObjectMapIter, DataMapIter);
                                    if (m_ObjectNamingGroupMap.end() != ObjectMapIter)
                                        m_pCurSelectObjectGroup = &(ObjectMapIter->second);
                                    else
                                        m_pCurSelectObjectGroup = nullptr;

                                    if (m_DataNamingGroupMap.end() != DataMapIter)
                                        m_pCurSelectDataGroup = &(DataMapIter->second);
                                    else
                                        m_pCurSelectDataGroup = nullptr;

                                    //SetUp_CurSelectObject();
                                    //선택 
                                }
                            }
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }

       

        if (nullptr != m_pCurSelectGroupingNameArr)
        {
            


            if (!(*m_pCurSelectGroupingNameArr).empty())
            {
                if((*m_pCurSelectGroupingNameArr).size() <= m_iCurSelectObjecNametIndex)
                    m_iCurSelectObjecNametIndex = 0;

                string ListBoxTitleData = u8"그룹명 : ";
                ListBoxTitleData += get<0>((*m_pCurSelectGroupingNameArr)[m_iCurSelectObjecNametIndex]).c_str();
                ImGui::Text(ListBoxTitleData.c_str());
                if (ImGui::BeginListBox("##TabObjectList", ImVec2(360.f, 200.f)))
                {
                    if (!get<1>((*m_pCurSelectGroupingNameArr)[m_iCurSelectObjecNametIndex]).empty())
                    {
                        list<_int>& ObjectList = get<1>((*m_pCurSelectGroupingNameArr)[m_iCurSelectObjecNametIndex]);
                        for (list<_int>::value_type& Value : ObjectList)
                        {
                            _bool bSelect = false;
                            if (m_iCurSelectObjectIndex == Value)
                            {
                                bSelect = true;
                            }
                            if (ImGui::Selectable(to_string(Value).c_str(), bSelect))
                            {
                                m_iCurSelectObjectIndex = Value;
                                SetUp_CurSelectObject();
                            }
                        }
                    }
                    ImGui::EndListBox();
                }
            }
        }

    }



    if (ImGui::CollapsingHeader(u8"그룹단위 복사"))
    {
        char szFromGroup[MAXCHAR] = "";
        strcpy_s(szFromGroup, strFromeGroup.c_str());

        char szToGroup[MAXCHAR] = "";
        strcpy_s(szToGroup, strToGroup.c_str());

        ImGui::Text("From : ");
        ImGui::SameLine();
        if (ImGui::InputText("##InputFromGroup", szFromGroup, sizeof(char) * MAXCHAR))
        {
            strFromeGroup = szFromGroup;
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"복사할 원본 그룹");

        ImGui::Text("To : ");
        ImGui::SameLine();
        if(ImGui::InputText("##InputToGroup", szToGroup, sizeof(char)*MAXCHAR))
        {
            strToGroup = szToGroup;
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"복사할 목표 그룹");

        if (ImGui::Button("Clone"))
        {
            Clone_Group();
        }//새로 생성
    }
    ImGui::Spacing(); 
    if (ImGui::CollapsingHeader(u8"그룹단위 컨트롤"))
    {
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"앵커위치를 무조건 설정해 줘야함\n앵커를 기준으로 모든 정보가 변경",u8"필독", false);
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"== 사용법 ==\n1. 앵커 위치 설정\n2. 정보 변경\n3. Update 버튼 클릭", u8"도움말", false);
        string strPreviewData = "";
        if (m_GroupingInfo.empty())
        {
            strPreviewData = u8"그룹 없음";
        }
        else
        {
            strPreviewData = get<Tuple_GroupName>(m_GroupingInfo[m_iSelectedControlGroup]);
        }
        if (ImGui::BeginCombo("##ControlGroupSelectCombo", strPreviewData.c_str()))
        {
            for (_uint i = 0; i < m_GroupingInfo.size(); ++i)
            {
                _bool bSelect = false;
                if (m_iSelectedControlGroup == i)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<Tuple_GroupName>(m_GroupingInfo[i]).c_str(), bSelect))
                {
                    if (!m_ObjectOriginMatrixlist.empty())
                    {
                        ObjectNameTupleArr& PrevSelectObjectVector = Get_TupleArr(get<Tuple_GroupName>(m_GroupingInfo[m_iSelectedControlGroup]).c_str());
                        for (auto& Value : PrevSelectObjectVector)
                        {
                            size_t NameHash = Convert_ToHash(get<Tuple_ObjectName>(Value));
                            ObjectArr& rhsObject = m_ObjectNamingGroupMap[NameHash];
                            for (auto& ObjectListElem : get<Tuple_IndexList>(Value))
                            {
                                _int Index = ObjectListElem;
                                SetUp_Matrix(rhsObject[Index], m_ObjectOriginMatrixlist.front());
                                m_ObjectOriginMatrixlist.pop_front();
                            }
                        }
                    }
                    m_iSelectedControlGroup = i;
                    m_ObjectOriginMatrixlist.clear();
                    ObjectNameTupleArr& SelectObjectVector = Get_TupleArr(get<Tuple_GroupName>(m_GroupingInfo[i]).c_str());
                    for (auto& Value : SelectObjectVector)
                    {
                        size_t NameHash = Convert_ToHash(get<Tuple_ObjectName>(Value));
                        ObjectArr& rhsObject = m_ObjectNamingGroupMap[NameHash];
                        for (auto& ObjectListElem : get<Tuple_IndexList>(Value))
                        {
                            _int Index = ObjectListElem;

                            _float4x4 Matrix = rhsObject[Index]->Get_Transform()->Get_WorldMatrix();
                            m_ObjectOriginMatrixlist.push_back(Matrix);
                        }
                    }
                }
            }

            ImGui::EndCombo();
        }
        string strPickInfo = "";
        if (!m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_ANCHOR))
        {
            strPickInfo = "Use_Picking";
        }
        else
        {
            strPickInfo = "Unuse_Picking";
        }
        if (ImGui::Button(strPickInfo.c_str()))
        {
            if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_ANCHOR))
            {
                m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_NONE);
            }
            else
            {
                m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_ANCHOR);
            }
        }

        Show_GroupMatrix();
        static _float ImGuiGroupMoveDragSpeed = 0.01f;
        if (ImGui::CollapsingHeader("Group Move"))
        {
            CImGui_Manager::Get_Instance()->On_ToolTip(u8"모든 오브젝트들이 앵커를 부모로 삼아 이동");
            ImGui::Text("MoveValue : ");
            ImGui::SameLine();
            ImGui::SliderFloat("##ImGuiGroupMoveDragSpeed", &ImGuiGroupMoveDragSpeed, 0.01f, 50.f, "%.2f");

            m_AnchorPos = m_matPickedAnchor.XMLoad().r[3];
            ImGui::Text("X : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputXPos", &m_AnchorPos.x, ImGuiGroupMoveDragSpeed))
            {
                _matrix TmpMat = m_matPickedAnchor.XMLoad();
                TmpMat.r[3] = m_AnchorPos.XMLoad();
                m_matPickedAnchor = TmpMat;
                Update_GroupMatrixForAnchor();
            }
            ImGui::Text("Y : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputYPos", &m_AnchorPos.y, ImGuiGroupMoveDragSpeed))
            {
                _matrix TmpMat = m_matPickedAnchor.XMLoad();
                TmpMat.r[3] = m_AnchorPos.XMLoad();
                m_matPickedAnchor = TmpMat;
                Update_GroupMatrixForAnchor();
            }
            ImGui::Text("Z : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputZPos", &m_AnchorPos.z, ImGuiGroupMoveDragSpeed))
            {
                _matrix TmpMat = m_matPickedAnchor.XMLoad();
                TmpMat.r[3] = m_AnchorPos.XMLoad();
                m_matPickedAnchor = TmpMat;
                Update_GroupMatrixForAnchor();
            }
        }
        static _float ImGuiGroupRotateDragSpeed = 0.01f;
        if (ImGui::CollapsingHeader("Group Rotate"))
        {
            CImGui_Manager::Get_Instance()->On_ToolTip(u8"모든 오브젝트들이 앵커를 부모로 삼아 공전");
            ImGui::Text("RotateValue : ");
            ImGui::SameLine();
            ImGui::SliderFloat("##ImGuiGroupRotateDragSpeed", &ImGuiGroupRotateDragSpeed, 0.01f, 50.f, "%.2f");

            ImGui::Text("X : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputXAngle", &m_AnchorRot.x, ImGuiGroupRotateDragSpeed))
            {
                if (m_AnchorRot.x >= 360.f)
                {
                    _int INTLOWER = _int(m_AnchorRot.x * 10000);
                    INTLOWER %= 3600000;
                    m_AnchorRot.x = _float(INTLOWER) / 10000;
                }
                if (m_AnchorRot.x < 0.f)
                {
                    m_AnchorRot.x = 360.f + m_AnchorRot.x;
                }
                _float4x4 InitMat = XMMatrixIdentity();
                _float4 Scale = _float4(
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[0])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[1])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[2])),
                    0.f
                );
                CUtility_Transform::Turn_ByAngle(m_matPickedAnchor, _float4(1.f, 0.f, 0.f, 0.f), m_AnchorRot.x);
                _matrix TmpMat = InitMat.XMLoad();
                TmpMat.r[0] = XMVector3Normalize(TmpMat.r[0]) * Scale.x;
                TmpMat.r[1] = XMVector3Normalize(TmpMat.r[1]) * Scale.y;
                TmpMat.r[2] = XMVector3Normalize(TmpMat.r[2]) * Scale.z;
                TmpMat.r[3] = m_matPickedAnchor.XMLoad().r[3];
                m_matPickedAnchor = TmpMat;


                Update_GroupMatrixForAnchor();
            }
            ImGui::Text("Y : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputYAngle", &m_AnchorRot.y), ImGuiGroupRotateDragSpeed)
            {
                if (m_AnchorRot.y >= 360.f)
                {
                    _int INTLOWER = _int(m_AnchorRot.y * 10000);
                    INTLOWER %= 3600000;
                    m_AnchorRot.y = _float(INTLOWER) / 10000;
                }
                if (m_AnchorRot.y < 0.f)
                {
                    m_AnchorRot.y = 360.f + m_AnchorRot.y;
                }
                _float4x4 InitMat = XMMatrixIdentity();
                _float4 Scale = _float4(
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[0])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[1])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[2])),
                    0.f
                );
                CUtility_Transform::Turn_ByAngle(InitMat, _float4(0.f, 1.f, 0.f, 0.f), m_AnchorRot.y);
                _matrix TmpMat = InitMat.XMLoad();
                TmpMat.r[0] = XMVector3Normalize(TmpMat.r[0]) * Scale.x;
                TmpMat.r[1] = XMVector3Normalize(TmpMat.r[1]) * Scale.y;
                TmpMat.r[2] = XMVector3Normalize(TmpMat.r[2]) * Scale.z;
                TmpMat.r[3] = m_matPickedAnchor.XMLoad().r[3];
                m_matPickedAnchor = TmpMat;

                Update_GroupMatrixForAnchor();
            }
            ImGui::Text("Z : ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##InputZAngle", &m_AnchorRot.z, ImGuiGroupRotateDragSpeed))
            {
                if (m_AnchorRot.z >= 360.f)
                {
                    _int INTLOWER = _int(m_AnchorRot.z * 10000);
                    INTLOWER %= 3600000;
                    m_AnchorRot.z = _float(INTLOWER) / 10000;
                }
                if (m_AnchorRot.z < 0.f)
                {
                    m_AnchorRot.z = 360.f + m_AnchorRot.z;
                }
                _float4x4 InitMat = XMMatrixIdentity();
                _float4 Scale = _float4(
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[0])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[1])),
                    XMVectorGetX(XMVector3Length(m_matPickedAnchor.XMLoad().r[2])),
                    0.f
                );
                CUtility_Transform::Turn_ByAngle(m_matPickedAnchor, _float4(0.f, 0.f, 1.f, 0.f), m_AnchorRot.z);
                _matrix TmpMat = InitMat.XMLoad();
                TmpMat.r[0] = XMVector3Normalize(TmpMat.r[0]) * Scale.x;
                TmpMat.r[1] = XMVector3Normalize(TmpMat.r[1]) * Scale.y;
                TmpMat.r[2] = XMVector3Normalize(TmpMat.r[2]) * Scale.z;
                TmpMat.r[3] = m_matPickedAnchor.XMLoad().r[3];
                m_matPickedAnchor = TmpMat;

                Update_GroupMatrixForAnchor();
            }
        }
        if (ImGui::Button("Group Update!"))
        {
            Update_GroupObject();
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"모든 정보가 변경되었다면 해당 버튼을 눌러 모든 오브젝트한태 적용");
        //Confirm()
    }
    ImGui::Spacing();


    ImGui::Spacing();
    ImGui::Spacing();
    if (!m_GroupingInfo.empty())
    {
        if (m_GroupingInfo.size() <= m_SelectObjectGroupIndex)
        {
            m_SelectObjectGroupIndex = 0;
        }
        string DebugTab = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
        m_pMapTool->DebugData("SelectTab", DebugTab);
    }
    string DebugSelectObject = m_strCurSelectObjectName + to_string(m_iCurSelectObjecNametIndex);
    m_pMapTool->DebugData("SelectedName", DebugSelectObject);
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_SelectedObject_NameBase()
{
    if (ImGui::CollapsingHeader(u8"같은 이름의 오브젝트들", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        m_pMapTool->DebugData(u8"선택된 메시 명", m_CurSelectedMeshName);
        if (ImGui::BeginListBox("##AddObjectList", ImVec2(360.f, 200.f)))
        {
            if (nullptr != m_pCurSelectObjectGroup)
            {
                if (_uint(m_pCurSelectObjectGroup->size()) > 10000)
                {
                    m_pCurSelectObjectGroup = nullptr;
                    return;
                }

                for (_uint i = 0; i < _uint(m_pCurSelectObjectGroup->size()); ++i)
                {
                    if ((*m_pCurSelectObjectGroup)[i] != nullptr && (*m_pCurSelectObjectGroup)[i]->Is_Valid())
                    {
                        _bool bSelect = false;
                        if (m_iCurSelectObjectIndex == i)
                        {
                            bSelect = true;
                        }
                        if (ImGui::Selectable(to_string(i).c_str(), bSelect))
                        {
                            m_iCurSelectObjectIndex = i;
                            SetUp_CurSelectObject();
                        }
                    }
                }
            }
            ImGui::EndListBox();
        }
        if (ImGui::Button(u8"해당 오브젝트 전체 제거"))
        {
            if (!m_GroupingInfo.empty() && _uint(m_GroupingInfo.size()) > m_SelectObjectGroupIndex)
            {
                Clear_SameNameObject(get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]), m_iCurSelectObjecNametIndex);
                m_pCurSelectObjectGroup = nullptr;
                m_pCurSelectDataGroup = nullptr;
                Confirm_Data();
                m_iCurSelectObjecNametIndex = 0;
            }
        }//같은 이름의 모든 오브젝트 삭제
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"전체 그룹에서 동일 이름을 가진 오브젝트 제거");
        ImGui::Spacing();

        if (ImGui::Button(u8"병합"))
        {
            string CurSelectGroupName = string(get< Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]));
            size_t Hashnum = Convert_ToHash(CurSelectGroupName);
            OBJECTGROUPINGMAP::iterator NamingGroup;
            Find_ObjectGroupingName(Hashnum, NamingGroup);
            string strSelectObjectName = get<0>(NamingGroup->second[m_iCurSelectObjecNametIndex]);
            Merge_Object(strSelectObjectName);
            Confirm_Data();
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"같은 이름의 오브젝트들 병합");
        ImGui::SameLine();
        if (ImGui::Button(u8"분할"))
        {
            string CurSelectGroupName = string(get< Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]));
            size_t Hashnum = Convert_ToHash(CurSelectGroupName);
            OBJECTGROUPINGMAP::iterator NamingGroup;
            Find_ObjectGroupingName(Hashnum, NamingGroup);
            string strSelectObjectName = get<0>(NamingGroup->second[m_iCurSelectObjecNametIndex]);
            Split_Object(strSelectObjectName);
            Confirm_Data();
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"병합된 오브젝트와 같은 이름의 오브젝트들로 분할");

        if (ImGui::Button(u8"전체 병합"))
        {
            Merge_All();
        }
        ImGui::SameLine();
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"모든 오브젝트들을 이름별로 묶어서 병합");
        if (ImGui::Button(u8"전체 분할"))
        {
            Split_All();
        }
        CImGui_Manager::Get_Instance()->On_ToolTip(u8"병합된 오브젝트들을 이름별로 분할");
        ImGui::Spacing();
    }

}


void CFunc_ObjectControl::Func_ObjectControl()
{
    string strPickInfo = "";
    if (!m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_OBJECT))
    {
        strPickInfo = "Use_Placing";
    }
    else
    {
        strPickInfo = "Unuse_Placing";
    }
    if (ImGui::Button(strPickInfo.c_str()))
    {
        if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_OBJECT))
        {
            m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_NONE);
        }
        else
        {
            m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_OBJECT);
        }
    }

    if (!m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_CLONE))
    {
        strPickInfo = "Use_Cloning";
    }
    else
    {
        strPickInfo = "Unuse_Cloning";
    }
    if (ImGui::Button(strPickInfo.c_str()))
    {
        if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_CLONE))
        {
            m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_NONE);
            m_eCloneDir = CLONE_NONE;
        }
        else
        {
            m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_CLONE);
        }
    }
    ImGui::Spacing();
    if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_CLONE)) 
    {
        ImGui::Text(CloneTypeDesc[m_eCloneDir]);
        if (ImGui::BeginCombo("##CloneDIR", CloneTypeDesc[m_eCloneDir]))
        {
            for (_int i = 0; i < CLONE_END; ++i) {
                _bool bSelect = false;
                if (m_eCloneDir == i)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(CloneTypeDesc[i], bSelect))
                {
                    m_eCloneDir = CLONEDIR(i);
                    m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_CLONE);
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::Spacing();
    Show_ControlInfo("MODE : SCALING", "MODE : ROTATE", "MODE : MOVE");
    CImGui_Manager::Get_Instance()->On_ToolTip(u8"Z : Move\nX :Rotate\nC : Scaling", u8"조작 키", false);
    ImGui::Spacing();

    if (ImGui::Button("Confirm"))
    {
        m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_NONE);
        Confirm_Data();
    }
    CImGui_Manager::Get_Instance()->On_ToolTip(u8"클릭시 선택해제");
    ImGui::Spacing();
    if (ImGui::Button("Clone"))
    {
        Clone_SamePosition();
    }
    CImGui_Manager::Get_Instance()->On_ToolTip(u8"클릭시 해당 위치에 복사");
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Object Matrix(Read-Only)", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Show_ObjectData();
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Object Speed", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Set_ControlSpeed(&m_fTickPerMoveSpeed, &m_fTickPerRotSpeed, &m_fTickPerScalingSpeed);
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Light Option", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        if (ImGui::Checkbox("LightOpt_1", &m_ObjectLightFlagOpt[0]))
        {
        }
        if (ImGui::Checkbox("LightOpt_2", &m_ObjectLightFlagOpt[1]))
        {
        }
        if (ImGui::Checkbox("LightOpt_3", &m_ObjectLightFlagOpt[2]))
        {
        }
        if (ImGui::Checkbox("LightOpt_4", &m_ObjectLightFlagOpt[3]))
        {
        }
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Collider Control", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text("Select ColliderType");
        if (ImGui::BeginCombo("##Select ColliderType", get<0>(m_listColliderType[m_ColliderType]).c_str()))
        {
            for (vector<tuple<string, _uint>>::value_type& Value : m_listColliderType)
            {
                _bool bSelect = false;
                if (get<1>(Value) == m_ColliderType)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_ColliderType = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
        ImGui::Text("Select LODLEVEL");
        string strPreviewLODLevel = (m_LODLevel >= 1) ? get<0>(m_listLODLevel[m_LODLevel - 1]) : "";
        if (ImGui::BeginCombo("##Select LOD Level", strPreviewLODLevel.c_str()))
        {
            for (vector<tuple<string, _uint>>::value_type& Value : m_listLODLevel)
            {
                _bool bSelect = false;
                if (get<1>(Value) == m_LODLevel)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_LODLevel = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
        CStructure* pStructure = static_cast<CStructure*>(m_pCurSelectGameObject);
        if (nullptr == pStructure)
            assert(0);
        if (ImGui::Button("Generate!"))
        {
            if (m_ColliderType == _uint(CStructure::ePhysXEnum::eBOX))
            {
                pStructure->Make_PhysXCollier_Box();
            }
            else
            {
                pStructure->Make_PhysXCollider(CStructure::ePhysXEnum(m_ColliderType), m_LODLevel);
            }
            m_iCurSelectBoxIndex = 0;
        }
        _uint BoxCount = pStructure->Get_BoxCount();
        if (ImGui::BeginCombo("Box Collider", to_string(m_iCurSelectBoxIndex).c_str()))
        {
            for (_uint i = 0; i < BoxCount; ++i)
            {
                _bool bSelect = false;
                if (m_iCurSelectBoxIndex == i)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(to_string(i).c_str(), bSelect))
                {
                    m_iCurSelectBoxIndex = i;
                }
            }
            ImGui::EndCombo();
        }
        if (BoxCount > 0) 
        {
            string strDebug = to_string(m_iCurSelectBoxIndex);
            m_pMapTool->DebugData("Cur Select Box", strDebug);
            ImGui::Text("Box Position");
            
            vector<_float4>& ColPoses = pStructure->Get_ColliderPoses();
            vector<_float4>& ColSize = pStructure->Get_ColliderScales();
            vector<_float4>& ColAngles = pStructure->Get_ColliderAngles();

            float vPos[3] = {0};
            if (!ColPoses.empty())
            {
                vPos[0] = ColPoses[m_iCurSelectBoxIndex].x;
                vPos[1] = ColPoses[m_iCurSelectBoxIndex].y;
                vPos[2] = ColPoses[m_iCurSelectBoxIndex].z;
            }

            float vRot[3] = {0};
            if (!ColAngles.empty()) 
            {
                vRot[0] = ColAngles[m_iCurSelectBoxIndex].x;
                vRot[1] = ColAngles[m_iCurSelectBoxIndex].y;
                vRot[2] = ColAngles[m_iCurSelectBoxIndex].z;
            }

            float vScal[3] = {0};
            if (!ColSize.empty()) 
            {
                vScal[0] = ColSize[m_iCurSelectBoxIndex].x;
                vScal[1] = ColSize[m_iCurSelectBoxIndex].y;
                vScal[2] = ColSize[m_iCurSelectBoxIndex].z;
            }
            if (ImGui::InputFloat3("##Box Pos", vPos))
            {
                if (!ColPoses.empty())
                {
                    pStructure->RePosition_Box(m_iCurSelectBoxIndex, _float4(vPos[0], vPos[1], vPos[2], 1.f));
                }

            }
            ImGui::Text("Box Size");
            if (ImGui::InputFloat3("##Box Scal", vScal))
            {
                if (!ColSize.empty())
                {
                    pStructure->ReScale_Box(m_iCurSelectBoxIndex, _float4(vScal[0], vScal[1], vScal[2], 1.f));
                }
            }
            ImGui::Text("Box Rotate");
            if (ImGui::InputFloat3("##Box Rot", vRot))
            {
                if (!ColAngles.empty())
                {
                    pStructure->Rotate_Box(m_iCurSelectBoxIndex, _float4(vRot[0], vRot[1], vRot[2], 1.f));
                }
            }
        }
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_Picking()
{
    if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_OBJECT))
    {
        Place_Object();
        Change_Object_UpDir();
    }
    else if(m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_ANCHOR))
    {
        Pick_Anchor();
    }
}

void CFunc_ObjectControl::Func_SetUpCollider()
{
    if (m_GroupingInfo.empty())
        return;

    if (m_SelectObjectGroupIndex >= m_GroupingInfo.size())
        return;

    if (ImGui::CollapsingHeader("GroupCollider"))
    {
        ImGui::Text("Select ColliderType");
        if (ImGui::BeginCombo("##Select ColliderType", get<0>(m_listColliderType[m_ColliderType]).c_str()))
        {
            for (vector<tuple<string, _uint>>::value_type& Value : m_listColliderType)
            {
                _bool bSelect = false;
                if (get<1>(Value) == m_ColliderType)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_ColliderType = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
        ImGui::Text("Select LODLEVEL");
        string strPreviewLODLevel = (m_LODLevel >= 1) ? get<0>(m_listLODLevel[m_LODLevel - 1]) : "";
        if (ImGui::BeginCombo("##Select LOD Level", strPreviewLODLevel.c_str()))
        {
            for (vector<tuple<string, _uint>>::value_type& Value : m_listLODLevel)
            {
                _bool bSelect = false;
                if (get<1>(Value) == m_LODLevel)
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_LODLevel = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();

        if (ImGui::Button("Bake Group"))
        {
            size_t GroupHash = Convert_ToHash(get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]));//HASHING(string, get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]));
            OBJECTGROUPINGMAP::iterator GroupIter;
            _bool bFindGroupingName = Find_ObjectGroupingName(GroupHash, GroupIter);

            if (!bFindGroupingName)
                return;
            
            for (ObjectNameTupleArr::value_type Value: GroupIter->second)
            {
                string ObjectName = get<0>(Value);
                size_t NameHash = Convert_ToHash(ObjectName);
                vector<CGameObject*>& CurGroupData = m_ObjectNamingGroupMap[NameHash];

                if (CurGroupData.empty())
                    return;
                if (m_ColliderType == _uint(CStructure::ePhysXEnum::eBOX))
                {
                    for (vector<CGameObject*>::value_type& Value : CurGroupData)
                    {
                        static_cast<CStructure*>(Value)->Make_PhysXCollier_Box();
                    }
                }
                else
                {
                    for (vector<CGameObject*>::value_type& Value : CurGroupData)
                    {
                        static_cast<CStructure*>(Value)->Make_PhysXCollider(CStructure::ePhysXEnum(m_ColliderType), m_LODLevel);
                    }
                }
            }

            
        }

        if (ImGui::Button("Bake All"))
        {
            for (ObjectMap::value_type& MapValue : m_ObjectNamingGroupMap)
            {
                for(ObjectArr::value_type& Value : MapValue.second)
                {
                    if (m_ColliderType == _uint(CStructure::ePhysXEnum::eBOX))
                    {
                        static_cast<CStructure*>(Value)->Make_PhysXCollier_Box();
                    }
                    else
                    {

                        static_cast<CStructure*>(Value)->Make_PhysXCollider(CStructure::ePhysXEnum(m_ColliderType), m_LODLevel);
                    }
                }
            }
        }

    }
}

void CFunc_ObjectControl::Tick_Function()
{
    if (KEY(F7, TAP))
    {
        if (m_bHLOD_HIDE)
        {
            m_bHLOD_HIDE = false;
            for(auto& elem : m_pHLODList)
            {
                ENABLE_GAMEOBJECT(elem);
            }
        }
        else
        {
            m_bHLOD_HIDE = true;
            for (auto& elem : m_pHLODList)
            {
                DISABLE_GAMEOBJECT(elem);
            }
        }
    }

    if (false == m_pMapTool->Is_PickTerrain()) 
    {
        if (KEY(LBUTTON, TAP))
        {
            Pick_inOjbect();
        }
    }

    Select_DataControlFlag();
    //if (m_bGroupControl)
    //    Control_Group();
    //else
    //    Control_Object();
    Control_Object();
    Update_Data();
}

_bool CFunc_ObjectControl::Is_CurSelectObject()
{
    if (nullptr == m_pCurSelectGameObject)
        return false;
    return true;
}

void CFunc_ObjectControl::Confirm_Group()
{
    //m_pObjGroup = nullptr;
    //m_pDataGroup = nullptr;
}
#include "Model.h"
void CFunc_ObjectControl::SetUp_CurSelectObject()
{



    if (nullptr != m_pCurSelectObjectGroup
        && _uint(m_pCurSelectObjectGroup->size()) > m_iCurSelectObjectIndex)
    {
        if (nullptr != m_pCurSelectGameObject) {
            GET_COMPONENT_FROM(m_pCurSelectGameObject, CModel)->Set_RimLightFlag(_float4(1, 0, 0, 0));
            GET_COMPONENT_FROM(m_pCurSelectGameObject, CModel)->Set_OutlineFlag(_float4(1, 0, 0, 0));
        }
        m_pCurSelectGameObject = (*m_pCurSelectObjectGroup)[m_iCurSelectObjectIndex];

        if (nullptr != m_pCurSelectGameObject) {
            GET_COMPONENT_FROM(m_pCurSelectGameObject, CModel)->Set_RimLightFlag(_float4(1, 0.2f, 0.2f, 0.5f));
            GET_COMPONENT_FROM(m_pCurSelectGameObject, CModel)->Set_OutlineFlag(_float4(1, 0.1f, 0, 1));
        }
    }
    else
        m_pCurSelectGameObject = nullptr;

    if (nullptr != m_pCurSelectDataGroup 
        && _uint(m_pCurSelectDataGroup->size()) > m_iCurSelectObjectIndex)
        m_pCurSelectData = &((*m_pCurSelectDataGroup)[m_iCurSelectObjectIndex]);
    else
        m_pCurSelectData = nullptr;

    if (nullptr != m_pCurSelectGameObject)
        m_pObjTransform = m_pCurSelectGameObject->Get_Transform();
    else
        m_pObjTransform = nullptr;

    
}

void CFunc_ObjectControl::Show_GroupMatrix()
{
    _float* pRight = (_float*)(&m_matPickedAnchor._11);
    _float* pUp = (_float*)(&m_matPickedAnchor._21);
    _float* pLook = (_float*)(&m_matPickedAnchor._31);
    _float* pPosition = (_float*)(&m_matPickedAnchor._41);
    ImGui::InputFloat4("##GroupRight", pRight, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::InputFloat4("##GroupUp", pUp, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::InputFloat4("##GroupLook", pLook, "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::InputFloat4("##GroupPosition", pPosition, "%.2f", ImGuiInputTextFlags_ReadOnly);
    //if (nullptr == m_pCurSelectData)
    //    return;
    //else
    //{
    //    //_float* pRight = (_float*)(&m_matGroup._11);
    //    //_float* pUp = (_float*)(&m_matGroup._21);
    //    //_float* pLook = (_float*)(&m_matGroup._31);
    //    //_float* pPosition = (_float*)(&m_matGroup._41);

    //}
}

void CFunc_ObjectControl::Confirm_Data()
{
    m_pCurSelectGameObject = nullptr;
    m_pObjTransform = nullptr;
    m_pCurSelectData = nullptr;
    m_eControlType = CONTROL_MOVE;
    m_fTickPerScalingSpeed = 1.f;
    m_fTickPerRotSpeed = 1.f;
    m_fTickPerMoveSpeed = 1.f;
    m_iCurSelectBoxIndex = 0;
}

void CFunc_ObjectControl::Add_ObjectGroup(char* pObjectGroupName)
{
    GroupingArr::iterator GroupInfoIter;
    string strGroupName = pObjectGroupName;
    size_t HashNum = 0;
    _bool bFind = Find_ObjectGroupInfo(strGroupName, GroupInfoIter, HashNum);
    if (!bFind)
    {
        m_GroupingInfo.push_back(make_tuple(strGroupName, true));
        OBJECTGROUPINGMAP::iterator GroupingMapIter;
        _bool bGroupNameFind = Find_ObjectGroupingName(HashNum, GroupingMapIter);
        if (!bGroupNameFind)
        {
            m_ObjectNameGroupingMap.emplace(HashNum, ObjectNameTupleArr());
        }
    }
    Confirm_Data();
}

void CFunc_ObjectControl::Delete_ObjectGroup(char* pObjectGroupName)
{
    GroupingArr::iterator GroupInfoIter;
    string strGroupName = pObjectGroupName;
    size_t HashNum = 0;
    _bool bFind = Find_ObjectGroupInfo(strGroupName, GroupInfoIter, HashNum);
    if (bFind)
    {
        string strDeleteObjectGroupName = get<Tuple_GroupName>(*GroupInfoIter);
        m_GroupingInfo.erase(GroupInfoIter);
        OBJECTGROUPINGMAP::iterator GroupingMapIter;
        _bool bGroupNameFind = Find_ObjectGroupingName(HashNum, GroupingMapIter);
        if (bGroupNameFind)
        {
            //그룹핑된 오브젝트 이름 탐색
            //해당이름을 키값으로 가지는 오브젝트 데이터 그룹을 탐색
            //해당 이름을 키값으로 가지는 오브젝트 그룹을 탐색
            for (ObjectNameTupleArr::value_type& SearchObjectNameTuple : GroupingMapIter->second)
            {
                string SearchObjectName = get<0>(SearchObjectNameTuple);
                list<_int>& rhsIndexList = get<1>(SearchObjectNameTuple);
                Delete_ObjectNamingMap(SearchObjectName, rhsIndexList);
            }
            
            GroupingMapIter->second.clear();
            m_ObjectNameGroupingMap.erase(GroupingMapIter);
            m_SelectObjectGroupIndex--;
            if (m_SelectObjectGroupIndex <= 0)
            {
                m_SelectObjectGroupIndex = 0;
            }
        }

    }
    Confirm_Data();
}
CStructure* CFunc_ObjectControl::Add_ObjectNamingMap(string GroupName, string Meshpath, string ObjectName)
{
    size_t HashNum = 0;
    GroupingArr::iterator GroupingIter;
    _bool bFindGroupName = Find_ObjectGroupInfo(GroupName, GroupingIter, HashNum);

    OBJECTGROUPINGMAP::iterator SelectGroupIter;
    _bool bFindGroup = Find_ObjectGroupingName(HashNum, SelectGroupIter);

    list<_int>* pIndexList = nullptr;
    if (bFindGroup)
    {
        ObjectNameTupleArr::iterator NameIter = find_if(SelectGroupIter->second.begin(), SelectGroupIter->second.end(),
            [&ObjectName](ObjectNameTupleArr::value_type& Value)
            {
                if (ObjectName == get<Tuple_ObjectName>(Value))
                    return true;
                else
                    return false;
            }
        );
        if (SelectGroupIter->second.end() == NameIter)
        {
            SelectGroupIter->second.push_back(make_tuple(ObjectName, list<_int>()));
            pIndexList = &(get<Tuple_IndexList>(SelectGroupIter->second.back()));
        }
        else
        {
            pIndexList = &(get<Tuple_IndexList>(*NameIter));
        }

    }
    else
    {
        return nullptr;
    }

    if (nullptr == pIndexList)
        return nullptr;

    ObjectMap::iterator ObjMapIter;
    DataMap::iterator DataMapIter;

    if (false == Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter))
    {
        if (m_ObjectNamingGroupMap.end() == ObjMapIter)
            m_ObjectNamingGroupMap.emplace(Convert_ToHash(ObjectName), ObjectArr());
        if (m_DataNamingGroupMap.end() == DataMapIter)
            m_DataNamingGroupMap.emplace(Convert_ToHash(ObjectName), DataArr());
        Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter);
    }


    CStructure* pStructure = Add_Object(ObjectName, ObjMapIter->second, Meshpath);
    Add_Data(ObjectName, DataMapIter->second, Meshpath);

    pIndexList->push_back(_int(ObjMapIter->second.size()) - 1);
    
    m_pCurSelectObjectGroup = &ObjMapIter->second;
    m_iCurSelectObjectIndex = (_int(ObjMapIter->second.size()) <= 1) ? 0 : _int(ObjMapIter->second.size()) - 1;
    SetUp_CurSelectObject();


    return pStructure;
}

CStructure* CFunc_ObjectControl::Add_ObjectNamingMap(string GroupName, MTO_DATA& tData)
{
    CStructure* pStructure = nullptr;
    size_t HashNum = 0;
    GroupingArr::iterator GroupingIter;
    _bool bFindGroupName = Find_ObjectGroupInfo(GroupName, GroupingIter, HashNum);

    OBJECTGROUPINGMAP::iterator SelectGroupIter;
    _bool bFindGroup = Find_ObjectGroupingName(HashNum, SelectGroupIter);

    string ObjectName = CFunctor::To_String(tData.strObejctName);

    list<_int>* pIndexList = nullptr;
    if (bFindGroup)
    {
        ObjectNameTupleArr::iterator NameIter = find_if(SelectGroupIter->second.begin(), SelectGroupIter->second.end(),
            [&ObjectName](ObjectNameTupleArr::value_type& Value)
            {
                if (ObjectName == get<0>(Value))
                    return true;
                else
                    return false;
            }
        );
        if (SelectGroupIter->second.end() == NameIter)
        {
            SelectGroupIter->second.push_back(make_tuple(ObjectName, list<_int>()));
            pIndexList = &(get<1>(SelectGroupIter->second.back()));
        }
        else
        {
            pIndexList = &(get<1>(*NameIter));
        }

    }
    else
    {
        return nullptr;
    }
    if (nullptr == pIndexList)
        return nullptr;
    ObjectMap::iterator ObjMapIter;
    DataMap::iterator DataMapIter;

    if (false == Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter))
    {
        if (m_ObjectNamingGroupMap.end() == ObjMapIter)
            m_ObjectNamingGroupMap.emplace(Convert_ToHash(ObjectName), ObjectArr());
        if (m_DataNamingGroupMap.end() == DataMapIter)
            m_DataNamingGroupMap.emplace(Convert_ToHash(ObjectName), DataArr());
        Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter);
    }
    pStructure = Add_Object(ObjMapIter->second, tData);
    Add_Data(DataMapIter->second, tData);
    m_pCurSelectData = &DataMapIter->second.back();
    pIndexList->push_back(_int(ObjMapIter->second.size()) - 1);
    m_iCurSelectObjectIndex = _int(ObjMapIter->second.size()) - 1;
    SetUp_CurSelectObject();

    return pStructure;
}
CStructure* CFunc_ObjectControl::Add_ObjectNamingMap(MTO_DATA& tData) 
{
    CStructure* pStructure = nullptr;
    string ObjectName = CFunctor::To_String(tData.strObejctName);
    ObjectMap::iterator ObjMapIter;
    DataMap::iterator DataMapIter;

    if (false == Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter))
    {
        if (m_ObjectNamingGroupMap.end() == ObjMapIter)
            m_ObjectNamingGroupMap.emplace(Convert_ToHash(ObjectName), ObjectArr());
        if (m_DataNamingGroupMap.end() == DataMapIter)
            m_DataNamingGroupMap.emplace(Convert_ToHash(ObjectName), DataArr());
        Find_ObjectDatas(ObjectName, ObjMapIter, DataMapIter);
    }
    pStructure = Add_Object(ObjMapIter->second, tData);
    Add_Data(DataMapIter->second, tData);
    return pStructure;
}

CStructure* CFunc_ObjectControl::Add_Object(string ObjectName, vector<CGameObject*>& rhsObjectArr, string MeshPath)
{

    wstring strMeshPath = CFunctor::To_Wstring(MeshPath);
    CStructure* pStructure = CStructure::Create(strMeshPath);
    if (nullptr == pStructure)
        assert(0);
    pStructure->Initialize();
    CREATE_GAMEOBJECT(pStructure, GROUP_DECORATION);
    rhsObjectArr.push_back(pStructure);
    return pStructure;
}

CFunc_ObjectControl::MTO_DATA CFunc_ObjectControl::Add_Data(string ObjectName, vector<MTO_DATA>& rhsDataArr, string MeshPath)
{
    MTO_DATA tData;
    tData.Initialize();
    tData.bIgnoreFlag = false;
    tData.strMeshPath = CFunctor::To_Wstring(MeshPath);
    tData.strObejctName = CFunctor::To_Wstring(ObjectName);
    tData.ObejectIndex = _int(rhsDataArr.size());
    rhsDataArr.push_back(tData);
    return tData;
}

CStructure* CFunc_ObjectControl::Add_Object(vector<CGameObject*>& rhsObjectArr, MTO_DATA& tData) 
{
    wstring MeshPath = tData.strMeshPath;
    _float4 vScale = tData.vScale;
    _float4x4 matWorld = tData.ObjectStateMatrix;
    CStructure* pStructure = CStructure::Create(MeshPath, vScale, matWorld);
    if (nullptr == pStructure)
        assert(0);
    pStructure->Initialize();
    CREATE_GAMEOBJECT(pStructure, GROUP_DECORATION);
    rhsObjectArr.push_back(pStructure);
    return pStructure;
}
CFunc_ObjectControl::MTO_DATA CFunc_ObjectControl::Add_Data(vector<MTO_DATA>& rhsDataArr, MTO_DATA& tData) 
{
    rhsDataArr.push_back(tData);
    return tData;
}

CStructure* CFunc_ObjectControl::Merge_Object(string ObjectName) 
{
    size_t HashNum = Convert_ToHash(ObjectName);

    map<size_t, CStructure*>::iterator MergeIter = m_MergeMap.find(HashNum);
    if (m_MergeMap.end() != MergeIter)
        return MergeIter->second;

    ObjectMap::iterator ObjectIter;
    DataMap::iterator DataIter;
    Find_ObjectDatas(ObjectName, ObjectIter, DataIter);

    for (ObjectArr::value_type& Value  : ObjectIter->second)
    {
        if (nullptr != Value)
            DISABLE_GAMEOBJECT(Value);
    }

    vector<_int>ValidList;
    _int Index = 0;
    for (DataArr::value_type& Value : DataIter->second)
    {
        if (Value.bIgnoreFlag == false)
        {
            ValidList.push_back(Index);
        }
        Index++;
    }

    wstring strMeshPath;
    _int InstanceCount = ValidList.size();
    VTXINSTANCE* pInstance = new VTXINSTANCE[InstanceCount];
    ZeroMemory(pInstance, sizeof(VTXINSTANCE)* InstanceCount);
    for (_uint i = 0; i < InstanceCount; ++i)
    {
        strMeshPath = DataIter->second[ValidList[i]].strMeshPath;
        _float4x4 matInstanceWorld = DataIter->second[ValidList[i]].ObjectStateMatrix;
        _float4 vScale = _float4(1.f, 1.f, 1.f, 0.f);//= DataIter->second[i].vScale;
        XMStoreFloat4(&pInstance[i].vRight, matInstanceWorld.XMLoad().r[0] * vScale.x);
        XMStoreFloat4(&pInstance[i].vUp, matInstanceWorld.XMLoad().r[1] * vScale.y);
        XMStoreFloat4(&pInstance[i].vLook, matInstanceWorld.XMLoad().r[2] * vScale.z);
        XMStoreFloat4(&pInstance[i].vTranslation, matInstanceWorld.XMLoad().r[3]);
    }

    CStructure_Instance* pInstanceStructure = CStructure_Instance::Create(strMeshPath, InstanceCount,pInstance);
    pInstanceStructure->Initialize();
    CREATE_GAMEOBJECT(pInstanceStructure, GROUP_DECORATION);
    m_MergeMap.emplace(HashNum, pInstanceStructure);
    Safe_Delete_Array(pInstance);
    return pInstanceStructure;
}
void CFunc_ObjectControl::Split_Object(string ObjectName) 
{
    size_t HashNum = Convert_ToHash(ObjectName);

    map<size_t, CStructure*>::iterator MergeIter = m_MergeMap.find(HashNum);
    if (m_MergeMap.end() == MergeIter)
        return;
    DELETE_GAMEOBJECT(MergeIter->second);
    m_MergeMap.erase(MergeIter);


    ObjectMap::iterator ObjectIter;
    DataMap::iterator DataIter;
    Find_ObjectDatas(ObjectName, ObjectIter, DataIter);

    for (ObjectArr::value_type& Value : ObjectIter->second)
    {
        if(nullptr != Value)
            ENABLE_GAMEOBJECT(Value);
    }
}

void CFunc_ObjectControl::Merge_All() 
{
    for (auto& elem : m_ObjectNamingGroupMap)
    {
        for (auto& elemArr : elem.second)
        {
            if (nullptr != elemArr)
                DISABLE_GAMEOBJECT(elemArr);
        }
    }

    for (auto& elem : m_DataNamingGroupMap)
    {
        vector<_int>ValidList;
        _int Index = 0;
        for (DataArr::value_type& Value : elem.second)
        {
            if (Value.bIgnoreFlag == false)
            {
                ValidList.push_back(Index);
            }
            Index++;
        }
        wstring strMeshPath;
        _int InstanceCount = ValidList.size();
        VTXINSTANCE* pInstance = new VTXINSTANCE[InstanceCount];
        ZeroMemory(pInstance, sizeof(VTXINSTANCE) * InstanceCount);
        for (_uint i = 0; i < InstanceCount; ++i)
        {
            strMeshPath = elem.second[ValidList[i]].strMeshPath;
            _float4x4 matInstanceWorld = elem.second[ValidList[i]].ObjectStateMatrix;
            _float4 vScale = _float4(1.f, 1.f, 1.f, 0.f);//= elem.second[i].vScale;
            XMStoreFloat4(&pInstance[i].vRight, matInstanceWorld.XMLoad().r[0] * vScale.x);
            XMStoreFloat4(&pInstance[i].vUp, matInstanceWorld.XMLoad().r[1] * vScale.y);
            XMStoreFloat4(&pInstance[i].vLook, matInstanceWorld.XMLoad().r[2] * vScale.z);
            XMStoreFloat4(&pInstance[i].vTranslation, matInstanceWorld.XMLoad().r[3]);
        }

        CStructure_Instance* pInstanceStructure = CStructure_Instance::Create(strMeshPath, InstanceCount, pInstance);

        if (!pInstanceStructure)
        {
            Safe_Delete_Array(pInstance);
            continue;
        }

        pInstanceStructure->Initialize();
        CREATE_GAMEOBJECT(pInstanceStructure, GROUP_DECORATION);
        m_MergeMap.emplace(elem.first, pInstanceStructure);
        Safe_Delete_Array(pInstance);
    }
}
void CFunc_ObjectControl::Split_All() 
{
    for (auto& elem : m_MergeMap)
    {
        DELETE_GAMEOBJECT(elem.second);
    }
    m_MergeMap.clear();

    for (auto& elem : m_ObjectNamingGroupMap)
    {
        for (auto& elemArr : elem.second)
        {
            if (nullptr != elemArr)
                ENABLE_GAMEOBJECT(elemArr);
        }
    }

}

void CFunc_ObjectControl::Clear_SameNameObject(string ObjectGroupName, _int NameIndexInGroup)
{
    size_t GroupHash = Convert_ToHash(ObjectGroupName);
    OBJECTGROUPINGMAP::iterator ObjectGroupIter;
    _bool bFindGroupingName = Find_ObjectGroupingName(GroupHash, ObjectGroupIter);
    if (bFindGroupingName)
    {
        string ObjectName = get<0>(ObjectGroupIter->second[NameIndexInGroup]);
        ObjectMap::iterator ObjectIter;
        DataMap::iterator DataIter;
        Find_ObjectDatas(ObjectName, ObjectIter, DataIter);

        for (ObjectArr::value_type& ObjValue : ObjectIter->second)
        {
            if (nullptr != ObjValue)
                DELETE_GAMEOBJECT(ObjValue);
        }

        ObjectIter->second.clear();
        DataIter->second.clear();
        m_ObjectNamingGroupMap.erase(ObjectIter);
        m_DataNamingGroupMap.erase(DataIter);

        OBJECTGROUPINGMAP::iterator GroupingIter = m_ObjectNameGroupingMap.begin();
        for (; GroupingIter != m_ObjectNameGroupingMap.end(); ++GroupingIter)
        {
            ObjectNameTupleArr::iterator GroupArrIter = GroupingIter->second.begin();
            for (; GroupArrIter != GroupingIter->second.end(); ++GroupArrIter)
            {
                if (get<0>((*GroupArrIter)) == ObjectName)
                {
                    break;
                }
            }
            if (GroupArrIter != GroupingIter->second.end())
            {
                GroupingIter->second.erase(GroupArrIter);
            }

        }

    }
}

//void CFunc_ObjectControl::Clear_ObjectGroup(string ObjectGroupName)
//{
//    size_t GroupHash = Convert_ToHash(ObjectGroupName);
//    OBJECTGROUPINGMAP::iterator ObjectGroupIter;
//    _bool bFindGroupingName = Find_ObjectGroupingName(GroupHash, ObjectGroupIter);
//    if (bFindGroupingName)
//    {
//        for (ObjectNameTupleArr::value_type& Value : ObjectGroupIter->second)
//        {
//            Delete_ObjectNamingMap(get<0>(Value), get<1>(Value));
//        }
//        ObjectGroupIter->second.clear();
//        Confirm_Data();
//        
//    }
//}
void CFunc_ObjectControl::Clear_SameNameInGroup(string ObjectGroupName, _int NameIndexInGroup)
{
    size_t GroupHash = Convert_ToHash(ObjectGroupName);
    OBJECTGROUPINGMAP::iterator ObjectGroupIter;
    _bool bFindGroupingName = Find_ObjectGroupingName(GroupHash, ObjectGroupIter);
    if (bFindGroupingName)
    {
        string ObjectName = get<Tuple_ObjectName>(ObjectGroupIter->second[NameIndexInGroup]);
        list<_int> ObjectIndexList = get<Tuple_IndexList>(ObjectGroupIter->second[NameIndexInGroup]);
        Delete_ObjectNamingMap(ObjectName, ObjectIndexList);

        ObjectNameTupleArr::iterator GroupArrIter = ObjectGroupIter->second.begin();
        for (; GroupArrIter != ObjectGroupIter->second.end(); ++GroupArrIter)
        {
            if (get<0>((*GroupArrIter)) == ObjectName)
            {
                break;
            }
        }
        if (GroupArrIter != ObjectGroupIter->second.end())
        {
            ObjectGroupIter->second.erase(GroupArrIter);
        }


    }
}

void CFunc_ObjectControl::Func_PickStart()
{
    if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_CLONE))
    {
        Place_Clone();
    }
}

void CFunc_ObjectControl::Func_PickEnd()
{
}

void CFunc_ObjectControl::Routine_MeshSelect(void* tTreeNode)
{
    _bool bSelected = false;
    CWindow_Map::TREE_DATA* pTreeNode = reinterpret_cast<CWindow_Map::TREE_DATA*>(tTreeNode);
    if (nullptr == pTreeNode)
        return;
    if (m_CurSelectedMeshFilePath == (*pTreeNode).strFullPath)
    {
        bSelected = true;
    }
    for (auto& strPath : m_vecSelectedMeshFilePath)
    {
        if (strPath == (*pTreeNode).strFullPath)
            bSelected = true;
    }

    if (ImGui::Selectable((*pTreeNode).strFileName.c_str(), bSelected))
    {
        m_vecSelectedMeshFilePath.clear();
        m_vecSelectedMeshName.clear();

        string prevFilePath = m_CurSelectedMeshFilePath;
        m_CurSelectedMeshFilePath = (*pTreeNode).strFullPath;
        string Ext("fbx");
        m_CurSelectedMeshName = m_pMapTool->CutOut_Ext((*pTreeNode).strFileName, Ext);
        if (KEY(LSHIFT, HOLD))
        {
            // 1. 새로운 이터레이터로 prevPath 위치로 가야함
            filesystem::directory_iterator shiftFileIter((*pTreeNode).strFolderPath);
            string strshiftFullPath;

            wstring wstrPath = shiftFileIter->path().relative_path();
            strshiftFullPath = CFunctor::To_String(wstrPath);

            while (strshiftFullPath != prevFilePath)
            {
                shiftFileIter++;
                wstrPath = shiftFileIter->path().relative_path();
                strshiftFullPath = CFunctor::To_String(wstrPath);
            }

            // 2. prevPath부터 curPath까지 모두 선택

            for (; strshiftFullPath != m_CurSelectedMeshFilePath; ++shiftFileIter)
            {
                if (shiftFileIter == filesystem::end(shiftFileIter))
                    break;



                const filesystem::directory_entry& shiftentry = *shiftFileIter;

                wstring wstrshiftPath = shiftentry.path().relative_path();
                strshiftFullPath = strshiftFullPath.assign(wstrshiftPath.begin(), wstrshiftPath.end());

                _int iFind = (_int)strshiftFullPath.rfind("\\") + 1;
                string strFileName = strshiftFullPath.substr(iFind, strshiftFullPath.length() - iFind);

                _int iFindExt = (int)strshiftFullPath.rfind(".") + 1;
                string strExtName = strshiftFullPath.substr(iFindExt, strshiftFullPath.length() - iFindExt);

                //if (strExtName == "dat")
                    //continue;

                m_vecSelectedMeshFilePath.push_back(strshiftFullPath);
                m_vecSelectedMeshName.push_back(strFileName);
            }

        }
        else
        {
            m_vecSelectedMeshFilePath.push_back(m_CurSelectedMeshFilePath);
            m_vecSelectedMeshName.push_back(m_CurSelectedMeshName);
        }


    }
}

void CFunc_ObjectControl::Func_AddObject()
{
    _int NameIndex = 0;
    for (vector<string>::value_type& Value : m_vecSelectedMeshFilePath)
    {
        if (!m_GroupingInfo.empty())
        {
            string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
            Add_ObjectNamingMap(strGroupName, Value, m_vecSelectedMeshName[NameIndex]);
        }
        NameIndex++;
    }

}

void CFunc_ObjectControl::Func_DeleteOBject()
{
    string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
    size_t HashNum = Convert_ToHash( strGroupName);
    //그룹정보에서 제거

    string CurSelectName = string();

    OBJECTGROUPINGMAP::iterator GroupingIter;
    _bool bFindGroup = Find_ObjectGroupingName(HashNum, GroupingIter);
    if (bFindGroup)
    {
        _int SelectIndex = m_iCurSelectObjecNametIndex;
        _int Index = 0;
        ObjectNameTupleArr::iterator GroupingNameIter = GroupingIter->second.begin();
        for (; GroupingNameIter != GroupingIter->second.end(); ++GroupingNameIter)
        {
            if (Index == SelectIndex)
                break;
            else
            {
                Index++;
            }
        }

        if (GroupingNameIter != GroupingIter->second.end())
        {
            CurSelectName = get<Tuple_ObjectName>((*GroupingNameIter));
            get<Tuple_IndexList>(*GroupingNameIter).remove(m_iCurSelectObjectIndex);
            if (get<Tuple_IndexList>(*GroupingNameIter).empty())
            {
                GroupingIter->second.erase(GroupingNameIter);
            }
            //GroupingIter->second.erase(GroupingNameIter);
        }
        else
        {
            return;
        }

    }

    ObjectMap::iterator ObjectMapIter;
    DataMap::iterator DataMapIter;
    Find_ObjectDatas(CurSelectName, ObjectMapIter, DataMapIter);

    //이름을 통해 
    if (ObjectMapIter == m_ObjectNamingGroupMap.end())
        assert(0);
    if (DataMapIter == m_DataNamingGroupMap.end())
        assert(0);


    Delete_Object(ObjectMapIter->second, m_iCurSelectObjectIndex);
    Delete_Data(DataMapIter->second, m_iCurSelectObjectIndex);
    m_iCurSelectObjectIndex = 0;
}



void CFunc_ObjectControl::Show_ObjectData()
{
    if (nullptr == m_pCurSelectData)
        return;
    else
    {
        _float* pRight = (_float*)(&m_pCurSelectData->ObjectStateMatrix._11);
        _float* pUp = (_float*)(&m_pCurSelectData->ObjectStateMatrix._21);
        _float* pLook = (_float*)(&m_pCurSelectData->ObjectStateMatrix._31);
        _float* pPosition = (_float*)(&m_pCurSelectData->ObjectStateMatrix._41);
        ImGui::InputFloat4("##ObjectRight", pRight, "%.2f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat4("##ObjectUp", pUp, "%.2f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat4("##ObjectLook", pLook, "%.2f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat4("##ObjectPosition", pPosition, "%.2f", ImGuiInputTextFlags_ReadOnly);
    }
}

void CFunc_ObjectControl::Set_ControlSpeed(_float* fMoveSpeed, _float* fRotateSpeed, _float* fScaleSpeed)
{
    ImGui::Text("Scale : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Group Scaling Speed", fScaleSpeed, 0.1f, 10.f, "%.1f");
    ImGui::Text("Rotate : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Group Rotate Speed", fRotateSpeed, 0.1f, 90.f, "%.1f");
    ImGui::Text("Move : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Group Move Speed", fMoveSpeed, 0.1f, 50.f, "%.1f");
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    Show_ControlInfo("MODE : SCALING", "MODE : ROTATE", "MODE : MOVE");
    Show_ControlInfo(
        u8"X축 : Insert/Delete\nY축 : End/Home\nZ축 : PageUp/Down", 
        u8"X축 : Home/End\nY축 : Delete/PageDown\nZ축 : Insert/PageUp",
        u8"X축 : Insert/Delete\nY축 : Home/End\nZ축 : PageUp/Down");
}

//void CFunc_ObjectControl::Control_Group()
//{
//    switch (m_eControlType)
//    {
//    case CONTROL_SCALING:
//        Scaling_Group();
//        break;
//    case CONTROL_ROTATE:
//        Rotate_Group();
//        break;
//    case CONTROL_MOVE:
//        Position_Group();
//        break;
//    }
//}

void CFunc_ObjectControl::Select_DataControlFlag()
{
    if (KEY(Z, TAP))
    {
        m_eControlType = CONTROL_MOVE;
    }
    if (KEY(X, TAP))
    {
        m_eControlType = CONTROL_ROTATE;
    }
    if (KEY(C, TAP))
    {
        m_eControlType = CONTROL_SCALING;
    }
}

void CFunc_ObjectControl::Control_Object()
{
    switch (m_eControlType)
    {
    case CONTROL_SCALING:
        Scaling_Object();
        break;
    case CONTROL_ROTATE:
        Rotate_Object();
        break;
    case CONTROL_MOVE:
        Position_Object();
        break;
    }
}

//void CFunc_ObjectControl::Scaling_Group()
//{
//    if (nullptr == m_pObjTransform)
//        return;
//    _float XScale = XMVectorGetX(XMVector3Length(m_matGroup.XMLoad().r[0]));
//    _float YScale = XMVectorGetX(XMVector3Length(m_matGroup.XMLoad().r[1]));
//    _float ZScale = XMVectorGetX(XMVector3Length(m_matGroup.XMLoad().r[2]));
//    _float4 ScaleValue = _float4(XScale, YScale, ZScale, 0.f);
//    //RightDir
//    if (KEY(INSERTKEY, HOLD))
//    {
//        ScaleValue.x += m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//    if (KEY(DELETEKEY, HOLD))
//    {
//        ScaleValue.x -= m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//
//    //UpDir
//    if (KEY(HOMEKEY, HOLD))
//    {
//        ScaleValue.y += m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//    if (KEY(ENDKEY, HOLD))
//    {
//        ScaleValue.y -= m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//
//    //LookDir
//    if (KEY(PAGEUP, HOLD))
//    {
//        ScaleValue.z += m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//    if (KEY(PAGEDOWN, HOLD))
//    {
//        ScaleValue.z -= m_fTickPerGroupScalingSpeed * fDT(0);
//    }
//    m_matGroup.XMLoad().r[0] = XMVector3Normalize(m_matGroup.XMLoad().r[0]) * ScaleValue.x;
//    m_matGroup.XMLoad().r[1] = XMVector3Normalize(m_matGroup.XMLoad().r[1]) * ScaleValue.y;
//    m_matGroup.XMLoad().r[2] = XMVector3Normalize(m_matGroup.XMLoad().r[2]) * ScaleValue.z;
//}
//
//void CFunc_ObjectControl::Rotate_Group()
//{
//    if (nullptr == m_pObjTransform)
//        return;
//    //RightAxis
//    _float4 Right = m_matGroup.XMLoad().r[0];
//    if (KEY(HOMEKEY, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Right, m_fTickPerGroupRotSpeed * fDT(0));
//    }
//    if (KEY(ENDKEY, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Right, -m_fTickPerGroupRotSpeed * fDT(0));
//    }
//
//    //UpAxis    
//    _float4 Up = m_matGroup.XMLoad().r[1];
//    if (KEY(DELETEKEY, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Up, -m_fTickPerGroupRotSpeed * fDT(0));
//    }
//    if (KEY(PAGEDOWN, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Up, m_fTickPerGroupRotSpeed * fDT(0));
//    }
//
//    //LookAxis
//    _float4 Look = m_matGroup.XMLoad().r[2];
//    if (KEY(INSERTKEY, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Look, -m_fTickPerGroupRotSpeed * fDT(0));
//    }
//    if (KEY(PAGEUP, HOLD))
//    {
//        CUtility_Transform::Turn_ByAngle(m_matGroup, Look, m_fTickPerGroupRotSpeed * fDT(0));
//    }
//}
//
//void CFunc_ObjectControl::Position_Group()
//{
//    if (nullptr == m_pObjTransform)
//        return;
//    _float4 PosValue = m_matGroup.XMLoad().r[3];
//    //RightDir
//    if (KEY(INSERTKEY, HOLD))
//    {
//        PosValue.x += m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//    if (KEY(DELETEKEY, HOLD))
//    {
//        PosValue.x -= m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//
//    //UpDir
//    if (KEY(HOMEKEY, HOLD))
//    {
//        PosValue.y -= m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//    if (KEY(ENDKEY, HOLD))
//    {
//        PosValue.y += m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//
//    //LookDir
//    if (KEY(PAGEUP, HOLD))
//    {
//        PosValue.z += m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//    if (KEY(PAGEDOWN, HOLD))
//    {
//        PosValue.z -= m_fTickPerGroupMoveSpeed * fDT(0);
//    }
//
//    m_matGroup.XMLoad().r[3] = PosValue.XMLoad();
//}

void CFunc_ObjectControl::Delete_ObjectNamingMap(string strSearchObejctName, list<_int>& IndexList)
{
    ObjectMap::iterator ObjectMapIter;
    DataMap::iterator DataMapIter;
    //CollisionMap::iterator CollisionMapIter;
    
    Find_ObjectDatas(strSearchObejctName, ObjectMapIter, DataMapIter);

    Delete_Object(ObjectMapIter, IndexList);
    Delete_Data(DataMapIter, IndexList);
    //Delete_Collision(CollisionMapIter);
}

void CFunc_ObjectControl::Scaling_Object()
{
    if (nullptr == m_pObjTransform)
        return;
    _float4 ScaleValue = m_pObjTransform->Get_Scale();
    //RightDir
    if (KEY(INSERTKEY, HOLD))
    {
        ScaleValue.x += m_fTickPerScalingSpeed * fDT(0);
    }
    if (KEY(DELETEKEY, HOLD))
    {
        ScaleValue.x -= m_fTickPerScalingSpeed * fDT(0);
    }

    //UpDir
    if (KEY(HOMEKEY, HOLD))
    {
        ScaleValue.y += m_fTickPerScalingSpeed * fDT(0);
    }
    if (KEY(ENDKEY, HOLD))
    {
        ScaleValue.y -= m_fTickPerScalingSpeed * fDT(0);
    }

    //LookDir
    if (KEY(PAGEUP, HOLD))
    {
        ScaleValue.z += m_fTickPerScalingSpeed * fDT(0);
    }
    if (KEY(PAGEDOWN, HOLD))
    {
        ScaleValue.z -= m_fTickPerScalingSpeed * fDT(0);
    }

    m_pCurSelectGameObject->Get_Transform()->Set_Scale(ScaleValue);
}

void CFunc_ObjectControl::Rotate_Object()
{
    if (nullptr == m_pObjTransform)
        return;
    //RightAxis
    if (KEY(HOMEKEY, HOLD))
    {
        _float4 Right = m_pObjTransform->Get_World(WORLD_RIGHT);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Right, m_fTickPerRotSpeed * fDT(0));
    }
    if (KEY(ENDKEY, HOLD))
    {
        _float4 Right = m_pObjTransform->Get_World(WORLD_RIGHT);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Right, -m_fTickPerRotSpeed * fDT(0));
    }

    //UpAxis
    if (KEY(DELETEKEY, HOLD))
    {
        _float4 Up = m_pObjTransform->Get_World(WORLD_UP);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Up, -m_fTickPerRotSpeed * fDT(0));
    }
    if (KEY(PAGEDOWN, HOLD))
    {
        _float4 Up = m_pObjTransform->Get_World(WORLD_UP);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Up, m_fTickPerRotSpeed * fDT(0));
    }

    //LookAxis
    if (KEY(INSERTKEY, HOLD))
    {
        _float4 Look = m_pObjTransform->Get_World(WORLD_LOOK);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Look, -m_fTickPerRotSpeed * fDT(0));
    }
    if (KEY(PAGEUP, HOLD))
    {
        _float4 Look = m_pObjTransform->Get_World(WORLD_LOOK);

        CUtility_Transform::Turn_ByAngle(m_pObjTransform, Look, m_fTickPerRotSpeed * fDT(0));
    }
}

void CFunc_ObjectControl::Delete_Object(map<size_t, vector<CGameObject*>>::iterator& ObjectIter, list<_int> IndexList)
{
    if (ObjectIter != m_ObjectNamingGroupMap.end())
    {
        for(list<_int>::value_type& Value : IndexList)
        {
            CGameObject* pGameObject = ObjectIter->second[Value];

            if (pGameObject)
                DELETE_GAMEOBJECT(pGameObject);

            ObjectIter->second[Value] = nullptr;

        }

        for (ObjectArr::iterator ObjArrIter = ObjectIter->second.begin();  ObjArrIter != ObjectIter->second.end();)
        {
            if ((*ObjArrIter) == nullptr)
            {
                ObjArrIter = ObjectIter->second.erase(ObjArrIter);
            }
            else
            {
                ObjArrIter++;
            }
        }
        if (ObjectIter->second.empty())
        {
            ObjectIter->second.clear();
            m_ObjectNamingGroupMap.erase(ObjectIter);
        }
    }

}
void CFunc_ObjectControl::Delete_Data(map<size_t, vector<MTO_DATA>>::iterator& DataIter, list<_int> IndexList)
{
    if (DataIter != m_DataNamingGroupMap.end())
    {
        IndexList.sort();

        for (_uint i = 0; i < _uint(DataIter->second.size()); ++i)
        {
            if (!IndexList.empty() && i == IndexList.front())
            {
                IndexList.pop_front();
                DataIter->second[i].Initialize();
                continue;
            }
        }
        if (!DataIter->second.empty())
        {
            _bool bAllIgnore = true;
            for (_uint i = 0; i < _uint(DataIter->second.size()); ++i)
            {
                if (DataIter->second[i].bIgnoreFlag == false)
                {
                    bAllIgnore = false;
                    break;
                }
            }
            if (bAllIgnore) 
            {
                DataIter->second.clear();
                m_DataNamingGroupMap.erase(DataIter);
            }
        }
    }


}

void CFunc_ObjectControl::Delete_Object(vector<CGameObject*>& rhsObjectGroup, _int TargetIndex) 
{
    _int SelectIndex = TargetIndex;
    _int Index = 0;
    ObjectArr::iterator SelectIter = rhsObjectGroup.begin();
    for(; SelectIter != rhsObjectGroup.end(); ++SelectIter)
    {
        if (SelectIndex == Index)
        {
            break;
        }
        else
        {
            Index++;
        }
    }
    CStructure* pStructure = nullptr;
    if (rhsObjectGroup.end() != SelectIter)
    {
        pStructure = dynamic_cast<CStructure*>((*SelectIter));
        //rhsObjectGroup.erase(SelectIter);
        (*SelectIter) = nullptr;
        DELETE_GAMEOBJECT(pStructure);
    }

}
void CFunc_ObjectControl::Delete_Data(vector<MTO_DATA>& rhsDataGroup, _int TargetIndex)
{
    _int SelectIndex = TargetIndex;
    _int Index = 0;
    DataArr::iterator SelectIter = rhsDataGroup.begin();

    for (; SelectIter != rhsDataGroup.end(); ++SelectIter)
    {
        if (SelectIndex == Index)
        {
            break;
        }
        else
        {
            Index++;
        }
    }
        /*find_if(rhsDataGroup.begin(), rhsDataGroup.end(), [&SelectIndex, &Index]()
        {
            if (SelectIndex == Index)
                return true;
            else
            {
                Index++;
                return false;
            }
        });*/

    if (rhsDataGroup.end() != SelectIter)
    {
        //rhsDataGroup.erase(SelectIter);
        (*SelectIter).Initialize();
    }

    for (_uint i = 0; i < _uint(rhsDataGroup.size()); ++i)
    {
        rhsDataGroup[i].ObejectIndex = i;
    }
}

void CFunc_ObjectControl::Position_Object()
{
    if (nullptr == m_pObjTransform)
        return;
    _float4 PosValue = m_pObjTransform->Get_World(WORLD_POS);
    //RightDir
    if (KEY(INSERTKEY, HOLD))
    {
        PosValue.x += m_fTickPerMoveSpeed * fDT(0);
    }
    if (KEY(DELETEKEY, HOLD))
    {
        PosValue.x -= m_fTickPerMoveSpeed * fDT(0);
    }

    //UpDir
    if (KEY(HOMEKEY, HOLD))
    {
        PosValue.y -= m_fTickPerMoveSpeed * fDT(0);
    }
    if (KEY(ENDKEY, HOLD))
    {
        PosValue.y += m_fTickPerMoveSpeed * fDT(0);
    }

    //LookDir
    if (KEY(PAGEUP, HOLD))
    {
        PosValue.z += m_fTickPerMoveSpeed * fDT(0);
    }
    if (KEY(PAGEDOWN, HOLD))
    {
        PosValue.z -= m_fTickPerMoveSpeed * fDT(0);
    }

    m_pCurSelectGameObject->Get_Transform()->Set_World(WORLD_POS, PosValue);
}

void CFunc_ObjectControl::Place_Object()
{
    if (nullptr == m_pObjTransform)
        return;
    _float4 OutPos = get<CWindow_Map::PICK_OUTPOS>(m_pMapTool->Get_PickData());
    m_pObjTransform->Set_World(WORLD_POS, OutPos);
}

void CFunc_ObjectControl::Change_Object_UpDir()
{
    if (nullptr == m_pObjTransform)
        return;

    _float4 OutNorm = get<CWindow_Map::PICK_OUTNORM>(m_pMapTool->Get_PickData());
    _vector xNormal = -OutNorm.XMLoad();
    _vector xRight = XMVector3Cross(xNormal, XMVectorSet(0.f, 0.f, 1.f, 0.f));
    _vector xLook = XMVector3Cross(xRight, xNormal);
    _float4 Scale = m_pObjTransform->Get_Scale();

    _float4 vRight;
    _float4 vUp;
    _float4 vLook;
    XMStoreFloat4(&vRight, xRight);
    XMStoreFloat4(&vUp, xNormal);
    XMStoreFloat4(&vLook, xLook);

    m_pObjTransform->Set_World(WORLD_RIGHT, vRight);
    m_pObjTransform->Set_World(WORLD_UP, vUp);
    m_pObjTransform->Set_World(WORLD_LOOK, vLook);
    m_pObjTransform->Set_Scale(Scale);
}

void CFunc_ObjectControl::Update_Data()
{
    if (nullptr == m_pCurSelectData)
        return;
    if (m_pCurSelectGameObject)
    {
        m_pCurSelectData->vScale = m_pCurSelectGameObject->Get_Transform()->Get_Scale();
        m_pCurSelectData->ObjectStateMatrix = m_pCurSelectGameObject->Get_Transform()->Get_WorldMatrix();
    }

    if (KEY(L, TAP))
    {
        Func_DeleteOBject();
    }

    if (m_bGroupControl)
    {
        Update_Group();
    }
}

void CFunc_ObjectControl::Update_Group()
{

}

void CFunc_ObjectControl::Save_Data(string BasePath, string SaveName)
{
    Save_ObjectData(BasePath, SaveName);
    Save_ObjectGroup(BasePath, SaveName);
    Save_ObjectSplit(BasePath, SaveName);
    Save_ObjectMerge(BasePath, SaveName);
}

void CFunc_ObjectControl::Save_ObjectData(string BasePath, string SaveName) 
{
    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".ObjectData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }
    string SaveGroupFullPath = BasePath;
    SaveGroupFullPath += "GroupData/";
    SaveGroupFullPath += SaveName;
    SaveGroupFullPath += ".GroupData";

    string SaveSplitFullPath = BasePath;
    SaveSplitFullPath += "SplitData/";
    SaveSplitFullPath += SaveName;
    SaveSplitFullPath += ".SplitData";

    string SaveMergeFullPath = BasePath;
    SaveMergeFullPath += "MergeData/";
    SaveMergeFullPath += SaveName;
    SaveMergeFullPath += ".MergeData";

    _uint GroupPathLength = _uint(SaveGroupFullPath.size()) + 1;
    char szGroupFullPath[MAX_PATH] = "";
    strcpy_s(szGroupFullPath, SaveGroupFullPath.c_str());
    writeFile.write((char*)&GroupPathLength, sizeof(_uint));
    writeFile.write(szGroupFullPath, sizeof(char) * GroupPathLength);

    _uint SplitPathLength = _uint(SaveSplitFullPath.size()) + 1;
    char szSplitFullPath[MAX_PATH] = "";
    strcpy_s(szSplitFullPath, SaveSplitFullPath.c_str());
    writeFile.write((char*)&SplitPathLength, sizeof(_uint));
    writeFile.write(szSplitFullPath, sizeof(char) * SplitPathLength);


    _uint MergePathLength = _uint(SaveMergeFullPath.size()) + 1;
    char szMergeFullPath[MAX_PATH] = "";
    strcpy_s(szMergeFullPath, SaveMergeFullPath.c_str());
    writeFile.write((char*)&MergePathLength, sizeof(_uint));
    writeFile.write(szMergeFullPath, sizeof(char) * MergePathLength);
}
void CFunc_ObjectControl::Save_ObjectGroup(string BasePath, string SaveName)
{
    string SaveFullPath = BasePath;
    SaveFullPath += "GroupData/";
    SaveFullPath += SaveName;
    SaveFullPath += ".GroupData";

    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }


    map<size_t, vector<_int>> SyncData;
    for (ObjectMap::value_type& ValueMap : m_ObjectNamingGroupMap)
    {
        SyncData.emplace(ValueMap.first, vector<_int>());
        _int IndexPadding = 0;
        for (ObjectArr::value_type& Value : ValueMap.second)
        {
            SyncData[ValueMap.first].push_back(IndexPadding);
            if (Value == nullptr)
            {
                IndexPadding++;
            }

        }
    }

    _uint GroupLength = _uint(m_GroupingInfo.size());
    writeFile.write((char*)&GroupLength, sizeof(_uint));
    for (_uint i = 0; i < GroupLength; ++i) 
    {
        string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[i]);
        _uint GroupNameLength = _uint(get<Tuple_GroupName>(m_GroupingInfo[i]).length()) + 1;
        char GroupName[MAXCHAR] = "";
        strcpy_s(GroupName, strGroupName.c_str());
        writeFile.write((char*)&GroupNameLength, sizeof(_uint));
        writeFile.write(GroupName, sizeof(char) * GroupNameLength);

        size_t HashNum = Convert_ToHash(strGroupName);

        OBJECTGROUPINGMAP::iterator MapGroupIter;
        Find_ObjectGroupingName(HashNum, MapGroupIter);

        _uint NameGroupLength = _uint(MapGroupIter->second.size());
        writeFile.write((char*)&NameGroupLength, sizeof(_uint));

        for (ObjectNameTupleArr::value_type& NameValue : MapGroupIter->second)
        {
            string strObjectName = get<0>(NameValue);
            _int NameLenght = _int(strObjectName.length()) + 1;
            char ObjectName[MAXCHAR] = "";
            strcpy_s(ObjectName, strObjectName.c_str());

            writeFile.write((char*)&NameLenght, sizeof(_int));
            writeFile.write(ObjectName, sizeof(char) * NameLenght);

            _uint IndexListLength = _uint(get<1>(NameValue).size());
            writeFile.write((char*) & IndexListLength, sizeof(_uint));
            size_t NameHash = Convert_ToHash(get<0>(NameValue));
            for (list<_int>::value_type& value : get<1>(NameValue))
            {
                  _int Padding = SyncData[NameHash][value];
                _int SaveIndex = value - Padding;
                writeFile.write((char*)&SaveIndex, sizeof(_uint));
            }
        }
    }
}

void CFunc_ObjectControl::Show_ControlInfo(string ScaleInfo, string RotateInfo, string MoveInfo)
{

    switch (m_eControlType)
    {
    case CONTROL_SCALING:
        ImGui::Text(ScaleInfo.c_str());
        break;
    case CONTROL_ROTATE:
        ImGui::Text(RotateInfo.c_str());
        break;
    case CONTROL_MOVE:
        ImGui::Text(MoveInfo.c_str());
        break;
    }
}

void CFunc_ObjectControl::Save_ObjectSplit(string BasePath, string SaveName)
{
    string SaveFullPath = BasePath;
    SaveFullPath += "SplitData/";
    SaveFullPath += SaveName;
    SaveFullPath += ".SplitData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }


    //전체 그룹 순회
    _uint MapLength = _uint(m_DataNamingGroupMap.size());
    writeFile.write((char*)&MapLength, sizeof(_uint));
    for (DataMap::value_type& DataArrValue : m_DataNamingGroupMap)
    {
        list<_int>ValidList;
        _int Index = 0;
        for (DataArr::value_type& Value : DataArrValue.second)
        {
            if (Value.bIgnoreFlag == false)
            {
                ValidList.push_back(Index);
            }
            Index++;
        }

        if (ValidList.empty())
            continue;


        string strName = CFunctor::To_String(DataArrValue.second[ValidList.front()].strObejctName);
        _int NameLength = _int(strName.length()) + 1;
        char ObjectName[MAXCHAR] = "";
        strcpy_s(ObjectName, strName.c_str());
        writeFile.write((char*)&NameLength, sizeof(_int));
        writeFile.write(ObjectName, sizeof(char) * NameLength);

        string MeshPath = CFunctor::To_String(DataArrValue.second[ValidList.front()].strMeshPath);
        _int PathLength = _int(MeshPath.length()) + 1;
        char szMeshPath[MAX_PATH] = "";
        strcpy_s(szMeshPath, MeshPath.c_str());

        writeFile.write((char*)&PathLength, sizeof(_int));
        writeFile.write(szMeshPath, sizeof(char) * PathLength);

        _uint DataLength = _uint(ValidList.size());
        writeFile.write((char*)&DataLength, sizeof(_uint));
        for (list<_int>::value_type Value : ValidList)
        {
            writeFile.write((char*)&DataArrValue.second[Value].ObjectStateMatrix, sizeof(_float4x4));
            writeFile.write((char*)&DataArrValue.second[Value].vScale, sizeof(_float4));
            writeFile.write((char*)&DataArrValue.second[Value].ObejectIndex, sizeof(_int));
            writeFile.write((char*)&DataArrValue.second[Value].byteLightFlag, sizeof(_byte));
        }
    }
    writeFile.close();
}

void CFunc_ObjectControl::Clear_AllDatas()
{
    m_GroupingInfo.clear();
    for (OBJECTGROUPINGMAP::value_type& GroupingValue : m_ObjectNameGroupingMap)
    {
        GroupingValue.second.clear();
    }
    m_ObjectNameGroupingMap.clear();

    for (ObjectMap::value_type& Value : m_ObjectNamingGroupMap)
    {
        for (ObjectArr::value_type& ObjValue : Value.second)
        {
            if(nullptr != ObjValue)
                DELETE_GAMEOBJECT(ObjValue);
        }
        Value.second.clear();
    }
    m_ObjectNamingGroupMap.clear();


    for (DataMap::value_type& Value : m_DataNamingGroupMap)
    {
        Value.second.clear();
    }
    m_DataNamingGroupMap.clear();
    m_iCurSelectObjecNametIndex = 0;
    m_iCurSelectObjectIndex = 0;
    m_iSelectedControlGroup = 0;
}

void CFunc_ObjectControl::Save_ObjectMerge(string BasePath, string SaveName)
{
    string SaveFullPath = BasePath;
    SaveFullPath += "MergeData/";
    SaveFullPath += SaveName;
    SaveFullPath += ".MergeData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }
    
    //이름 길이 저장
    //이름 저장
    //개수 저장
    //인스턴싱 정보 생성
    //정보 저장

    _uint GroupMapSize = _uint(m_DataNamingGroupMap.size());
    writeFile.write((char*)&GroupMapSize, sizeof(_uint));

    for (DataMap::value_type& DataArrValue : m_DataNamingGroupMap)
    {
        list<_int>ValidList;
        _int Index = 0;
        for (DataArr::value_type& Value : DataArrValue.second)
        {
            if (Value.bIgnoreFlag == false)
            {
                ValidList.push_back(Index);
            }
            Index++;
        }
        if (ValidList.empty())
            continue;
        string strName = CFunctor::To_String(DataArrValue.second[ValidList.front()].strObejctName);
        _int NameLength = _int(strName.length()) + 1;
        char ObjectName[MAXCHAR] = "";
        strcpy_s(ObjectName, strName.c_str());
        writeFile.write((char*)&NameLength, sizeof(_int));
        writeFile.write(ObjectName, sizeof(char) * NameLength);

        string MeshPath = CFunctor::To_String(DataArrValue.second[ValidList.front()].strMeshPath);
        _int PathLength = _int(MeshPath.length()) + 1;
        char szMeshPath[MAX_PATH] = "";
        strcpy_s(szMeshPath, MeshPath.c_str());

        writeFile.write((char*)&PathLength, sizeof(_int));
        writeFile.write(szMeshPath, sizeof(char) * PathLength);

        _uint iInstanceNums = _uint(ValidList.size());
        VTXINSTANCE* pInstance = new VTXINSTANCE[iInstanceNums];
        ZeroMemory(pInstance, sizeof(VTXINSTANCE) * iInstanceNums);
        list<_int>::iterator ValidIter = ValidList.begin();
        for (_uint i = 0; i < iInstanceNums; ++i)
        {
            _float4x4 matInstanceWorld = DataArrValue.second[(*ValidIter)].ObjectStateMatrix;
            _float4 vScale = _float4(1.f, 1.f, 1.f, 0.f);//= DataIter->second[i].vScale;
            XMStoreFloat4(&pInstance[i].vRight, matInstanceWorld.XMLoad().r[0] * vScale.x);
            XMStoreFloat4(&pInstance[i].vUp, matInstanceWorld.XMLoad().r[1] * vScale.y);
            XMStoreFloat4(&pInstance[i].vLook, matInstanceWorld.XMLoad().r[2] * vScale.z);
            XMStoreFloat4(&pInstance[i].vTranslation, matInstanceWorld.XMLoad().r[3]);
            ValidIter++;
        }

        writeFile.write((char*)&iInstanceNums, sizeof(_uint));
        writeFile.write((char*)pInstance, sizeof(VTXINSTANCE)* iInstanceNums);
        Safe_Delete_Array(pInstance);
    }

    writeFile.close();
}
void CFunc_ObjectControl::Load_ObjectData(string FilePath, string& GroupFilePath, string& SplitFilePath, string& MergeFilePath)
{
    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }


    _uint GroupFilePathLength = 0;
    readFile.read((char*)&GroupFilePathLength, sizeof(_uint));
    char GroupPath[MAX_PATH] = "";
    readFile.read(GroupPath, sizeof(char) * GroupFilePathLength);
    GroupFilePath = GroupPath;

    _uint SplitFilePathLength = 0;
    readFile.read((char*)&SplitFilePathLength, sizeof(_uint));
    char SplitPath[MAX_PATH] = "";
    readFile.read(SplitPath, sizeof(char) * SplitFilePathLength);
    SplitFilePath = SplitPath;

    _uint MergeFilePathLength = 0;
    readFile.read((char*)&MergeFilePathLength, sizeof(_uint));
    char MergePath[MAX_PATH] = "";
    readFile.read(MergePath, sizeof(char) * MergeFilePathLength);
    MergeFilePath = MergePath;

    readFile.close();
}
void CFunc_ObjectControl::Load_ObjectGroup(string FilePath) 
{
    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }






    _uint GroupLength = 0;
    readFile.read((char*)&GroupLength, sizeof(_uint));

    for (_uint i = 0; i < GroupLength; ++i)
    {
        _uint GroupNameLength = 0;
        char GroupName[MAXCHAR] = "";
        readFile.read((char*)&GroupNameLength, sizeof(_uint));
        readFile.read(GroupName, sizeof(char) * GroupNameLength);

        Add_ObjectGroup(GroupName);

        string strGroupName = GroupName;

        size_t HashNum = Convert_ToHash(strGroupName);

        OBJECTGROUPINGMAP::iterator MapGroupIter;
        Find_ObjectGroupingName(HashNum, MapGroupIter);

        _uint NameGroupLength = 0;
        readFile.read((char*)&NameGroupLength, sizeof(_uint));

        for (_uint j = 0; j < NameGroupLength; ++j)
        {
            _int NameLenght = 0;
            readFile.read((char*)&NameLenght, sizeof(_int));
            char ObjectName[MAXCHAR] = "";
            readFile.read(ObjectName, sizeof(char) * NameLenght);

            string strObjectName = ObjectName;

            _uint IndexListLength = 0;
            readFile.read((char*)&IndexListLength, sizeof(_uint));
            list<_int> IndexList;
            for (_uint k = 0;  k < IndexListLength; ++k)
            {
                _uint SavedIndex = 0;
                readFile.read((char*)&SavedIndex, sizeof(_uint));
                IndexList.push_back(SavedIndex);
            }

            MapGroupIter->second.push_back(make_tuple(strObjectName, IndexList));
        }
    }
}
void CFunc_ObjectControl::Load_ObjectGroup_Temp(string FilePath)
{
    Clear_AllDatas();

    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }

    _int GroupSize = 0;
    //그룹 개수 저장
    readFile.read((char*)&GroupSize, sizeof(_int));
    for (_int i = 0; i < GroupSize; ++i)
    {
        //Loop~ 그룹 이름 저장
        _int ObjectGroupNameLength = 0;
        readFile.read((char*)&ObjectGroupNameLength, sizeof(_int));

        char* pGroupName = new char[ObjectGroupNameLength];
        readFile.read(pGroupName, sizeof(char) * ObjectGroupNameLength);
        Add_ObjectGroup(pGroupName);

        string strGroupName = pGroupName;
        //오브젝트 개수 저장
        _int ObjectCount = 0;
        readFile.read((char*)&ObjectCount, sizeof(_int));
        for (_int j = 0; j < ObjectCount; ++j)
        {
            MTO_DATA tData;
            //Loop~ MTO데이터 저장
            //이름 저장
            _int ObjectNameLength = 0;
            readFile.read((char*)&ObjectNameLength, sizeof(_int));
            char* pObjectName = new char[ObjectNameLength];
            readFile.read(pObjectName, sizeof(char) * ObjectNameLength);
            string strObjName = pObjectName;
            tData.strObejctName = CFunctor::To_Wstring(strObjName);
            //_int Pos = tData.strObejctName.rfind(_T("_"));
            //tData.strObejctName = tData.strObejctName.substr(0, Pos);


            //경로 저장
            _int ObjectPathLength = 0;
            readFile.read((char*)&ObjectPathLength, sizeof(_int));
            char* pObjectPath = new char[ObjectPathLength];
            readFile.read(pObjectPath, sizeof(char) * ObjectPathLength);
            string strObjPath = pObjectPath;
            tData.strMeshPath = CFunctor::To_Wstring(strObjPath);
            _int PosStart = tData.strMeshPath.rfind(_T("\\")) + 1;
            tData.strObejctName = tData.strMeshPath.substr(PosStart, tData.strMeshPath.length());

            _int PosEnd = tData.strObejctName.rfind(_T("."));
            tData.strObejctName = tData.strObejctName.substr(0, PosEnd);
            //행렬 저장
            _float4x4 StateMatrix;
            readFile.read((char*)&StateMatrix, sizeof(_float4x4));
            tData.ObjectStateMatrix = StateMatrix;

            //스케일 저장
            _float4 vScale;
            readFile.read((char*)&vScale, sizeof(_float4));
            tData.vScale = vScale;

            //라이트플래그 저장
            _byte LightFlag = 0;
            readFile.read((char*)&LightFlag, sizeof(_byte));
            tData.byteLightFlag = LightFlag;

            CStructure* pStructure = 
                Add_ObjectNamingMap(strGroupName, tData);

            //콜라이더 타입
            _uint ColType = 0;
            readFile.read((char*)&ColType, sizeof(_uint));

            //LOD
            _uint LODType = 0;
            readFile.read((char*)&LODType, sizeof(_uint));
            pStructure->Make_PhysXCollider(CStructure::ePhysXEnum(ColType), LODType);

            //박스콜라이더 수
            _uint BoxCount = 0;
            readFile.read((char*)&BoxCount, sizeof(_uint));

            for (_uint i = 0; i < BoxCount; ++i)
            {
                pStructure->Make_PhysXCollier_Box();
            }
            for (_uint i = 0; i < BoxCount; ++i)
            {
                _float4 vPosition = _float4(0.f, 0.f, 0.f, 1.f);
                _float4 vAngle = _float4(0.f, 0.f, 0.f, 0.f);
                _float4 vScale = _float4(0.f, 0.f, 0.f, 0.f);

                readFile.read((char*)&vPosition, sizeof(_float4));
                readFile.read((char*)&vAngle, sizeof(_float4));
                readFile.read((char*)&vScale, sizeof(_float4));

                pStructure->RePosition_Box(i, vPosition);
                pStructure->ReScale_Box(i, vScale);
                pStructure->Rotate_Box(i, vAngle);
            }

            //박스콜라이더 정보 저장

            Safe_Delete_Array(pObjectName);
            Safe_Delete_Array(pObjectPath);
        }
        Safe_Delete_Array(pGroupName);

    }
    readFile.close();
}
void CFunc_ObjectControl::Load_ObjectSplit(string FilePath) 
{
    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }

    _uint MapLength = 0;
    readFile.read((char*)&MapLength, sizeof(_uint));
    for (_uint i = 0; i < MapLength; ++i)
    {
        _int NameLength = 0;
        readFile.read((char*)&NameLength, sizeof(_int));
        char ObjectName[MAXCHAR] = "";
        readFile.read(ObjectName, sizeof(char) * NameLength);
        string strName = ObjectName;

        _int PathLength = 0;
        readFile.read((char*)&PathLength, sizeof(_int));
        char szMeshPath[MAX_PATH] = "";
        readFile.read(szMeshPath, sizeof(char) * PathLength);
        string MeshPath = szMeshPath;


        _uint DataLength = 0;
        readFile.read((char*)&DataLength, sizeof(_uint));

        wstring strObjName = CFunctor::To_Wstring(strName);
        wstring strPath = CFunctor::To_Wstring(MeshPath);
        for (_uint j = 0; j < DataLength; ++j)
        {
            MTO_DATA tData;
            tData.Initialize();
            tData.bIgnoreFlag = false;
            tData.strObejctName = strObjName;
            tData.strMeshPath = strPath;
            readFile.read((char*)&tData.ObjectStateMatrix, sizeof(_float4x4));
            readFile.read((char*)&tData.vScale, sizeof(_float4));
            readFile.read((char*)&tData.ObejectIndex, sizeof(_int));
            readFile.read((char*)&tData.byteLightFlag, sizeof(_byte));

            CStructure* pStructure = Add_ObjectNamingMap(tData);
        }
    }
    readFile.close();
}

void CFunc_ObjectControl::Clear_TupleData(vector<tuple<char*, bool>>& ArrData)
{
    for (_int i = 0; i < _int(ArrData.size()); ++i)
    {
        Safe_Delete_Array(get<0>(ArrData[i]));
    }
    ArrData.clear();
}

void CFunc_ObjectControl::Clone_Group()
{
    OBJECTGROUPINGMAP::iterator FromIter;
    OBJECTGROUPINGMAP::iterator ToIter;
    if (false == Find_ObjectGroupingName(Convert_ToHash(strFromeGroup), FromIter))
    {
        Call_MsgBox(_T("Not Found Group_From"));
        return;
    }//FromFind

    if (false == Find_ObjectGroupingName(Convert_ToHash(strToGroup), ToIter))
    {
        Call_MsgBox(_T("Not Found Group_To"));
        return;
    }//ToFind

    ObjectNameTupleArr& FromeArr = Get_TupleArr(strFromeGroup);

    for (auto& FromValue : FromeArr)
    {
        size_t NameHash = Convert_ToHash(get<0>(FromValue));
        DataArr& rhsDatas = m_DataNamingGroupMap[NameHash];
        for (auto& ObjectListElem : get<1>(FromValue))
        {
            _int Index = ObjectListElem;
            MTO_DATA OriginData = rhsDatas[Index];

            Add_ObjectNamingMap(strToGroup, OriginData);
        }
    }
}

CFunc_ObjectControl::ObjectNameTupleArr& CFunc_ObjectControl::Get_TupleArr(string GroupName)
{
    OBJECTGROUPINGMAP::iterator GroupIter;
    if (false == Find_ObjectGroupingName(Convert_ToHash(GroupName), GroupIter))
    {
        return m_ObjectNameGroupingMap.begin()->second;
    }
    return GroupIter->second;
    // // O: 여기에 return 문을 삽입합니다.
}

void CFunc_ObjectControl::Pick_inOjbect()
{
    if (nullptr == m_pObjTransform)
        return;

    if (m_pMapTool->Is_HoveredWindow())
        return;

    list<CGameObject*>& ObjectList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);
    _float4 OutPos;
    _float4 OutNorm;
    if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_OBJECT))
    {
        if (GAMEINSTANCE->Is_Picked(ObjectList, &OutPos, &OutNorm))
        {
            m_pObjTransform->Set_World(WORLD_POS, OutPos);
            m_pObjTransform->Make_WorldMatrix();
        }
    }
    else if (m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_CLONE))
    {
        if (GAMEINSTANCE->Is_Picked(ObjectList, &OutPos, &OutNorm))
        {

            MTO_DATA tData = (*m_pCurSelectData);
            _matrix WorldMat = tData.ObjectStateMatrix.XMLoad();
            WorldMat.r[3] = OutPos.XMLoad();
            tData.ObjectStateMatrix = WorldMat;
            string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
            Add_ObjectNamingMap(strGroupName, tData);
        }
    }        

}

void CFunc_ObjectControl::Pick_Anchor()
{
    _float4 OutPos = get<CWindow_Map::PICK_OUTPOS>(m_pMapTool->Get_PickData());
    memcpy_s(&m_matPickedAnchor._41, sizeof(_float4), &OutPos, sizeof(_float4));
}

void CFunc_ObjectControl::SetUp_Matrix(CGameObject* pGameObject, _float4x4 Matrix)
{
    _float4 vRight = Matrix.XMLoad().r[0];
    _float4 vUp = Matrix.XMLoad().r[1];
    _float4 vLook = Matrix.XMLoad().r[2];
    _float4 vPos = Matrix.XMLoad().r[3];
    pGameObject->Get_Transform()->Set_World(WORLD_RIGHT, vRight);
    pGameObject->Get_Transform()->Set_World(WORLD_UP, vUp);
    pGameObject->Get_Transform()->Set_World(WORLD_LOOK, vLook);
    pGameObject->Get_Transform()->Set_World(WORLD_POS, vPos);

}
void CFunc_ObjectControl::Update_GroupMatrixForAnchor()
{
    if (m_ObjectOriginMatrixlist.empty())
        return;
    ObjectNameTupleArr& SelectObjectVector = Get_TupleArr(get<Tuple_ObjectName>(m_GroupingInfo[m_iSelectedControlGroup]).c_str());
    list<_float4x4>::iterator OriginMatIter = m_ObjectOriginMatrixlist.begin();
    for (auto& Value : SelectObjectVector)
    {
        size_t NameHash = Convert_ToHash(get<0>(Value));
        ObjectArr& rhsObject = m_ObjectNamingGroupMap[NameHash];
        for (auto& ObjectListElem : get<1>(Value))
        {
            _int Index = ObjectListElem;
            _float4x4 GroupingMat = (*OriginMatIter);
            GroupingMat = GroupingMat.XMLoad() * m_matPickedAnchor.XMLoad();
            SetUp_Matrix(rhsObject[Index], GroupingMat);
            OriginMatIter++;
        }
    }
}
void CFunc_ObjectControl::Update_GroupObject() 
{
    if (m_ObjectOriginMatrixlist.empty())
    {
        return;
    }
    ObjectNameTupleArr& SelectObjectVector = Get_TupleArr(get<Tuple_ObjectName>(m_GroupingInfo[m_iSelectedControlGroup]).c_str());
    list<_float4x4>::iterator OriginMatIter = m_ObjectOriginMatrixlist.begin();
    for (auto& Value : SelectObjectVector)
    {
        size_t NameHash = Convert_ToHash(get<Tuple_ObjectName>(Value));
        DataArr& rhsDatas = m_DataNamingGroupMap[NameHash];
        for (auto& ObjectListElem : get<Tuple_IndexList>(Value))
        {
            _int Index = ObjectListElem;
            _float4x4 GroupingMat = (*OriginMatIter);
            GroupingMat = GroupingMat.XMLoad() * m_matPickedAnchor.XMLoad();

            _float4 vRight = rhsDatas[Index].ObjectStateMatrix.XMLoad().r[0];
            _float4 vUp = rhsDatas[Index].ObjectStateMatrix.XMLoad().r[1];
            _float4 vLook = rhsDatas[Index].ObjectStateMatrix.XMLoad().r[2];
            _float4 vPos = rhsDatas[Index].ObjectStateMatrix.XMLoad().r[3];
            
            _float4 Scale = _float4(
                vRight.Length(),
                vUp.Length(),
                vLook.Length(),
                0.f
            );

            vRight = GroupingMat.XMLoad().r[0];
            vUp = GroupingMat.XMLoad().r[1];
            vLook = GroupingMat.XMLoad().r[2];
            vPos = GroupingMat.XMLoad().r[3];

            vRight = vRight.Normalize() * Scale.x;
            vUp = vUp.Normalize() * Scale.y;
            vLook = vLook.Normalize() * Scale.z;

            _matrix Mat = XMMatrixIdentity();
            Mat.r[0] = vRight.XMLoad();
            Mat.r[1] = vUp.XMLoad();
            Mat.r[2] = vLook.XMLoad();
            Mat.r[3] = vPos.XMLoad();

            GroupingMat = Mat;
            rhsDatas[Index].vScale = Scale;
            rhsDatas[Index].ObjectStateMatrix = GroupingMat;

            OriginMatIter++;
        }
    }
    m_ObjectOriginMatrixlist.clear();
    m_iSelectedControlGroup = 0;
}
void CFunc_ObjectControl::SetUp_ColliderType()
{
    m_listColliderType.push_back(make_tuple(string("Convex"), _uint(CStructure::ePhysXEnum::eCONVEX)));
    m_listColliderType.push_back(make_tuple(string("Triangle"), _uint(CStructure::ePhysXEnum::eTRIANGLE)));
    m_listColliderType.push_back(make_tuple(string("Box"), _uint(CStructure::ePhysXEnum::eBOX)));
}
void CFunc_ObjectControl::SetUp_LODLevel()
{
   m_listLODLevel.push_back(make_tuple(string("Level_1"), 1));
   m_listLODLevel.push_back(make_tuple(string("Level_2"), 2));
   m_listLODLevel.push_back(make_tuple(string("Level_3"),3));
}

_bool CFunc_ObjectControl::Find_ObjectGroupingName(size_t GroupNameHashNum, map<size_t, vector<tuple<string, list<_int>>>>::iterator& OutGroupingName)
{
    OutGroupingName = m_ObjectNameGroupingMap.find(GroupNameHashNum);
    if (OutGroupingName == m_ObjectNameGroupingMap.end())
        return false;
    else
        return true;
}

_bool CFunc_ObjectControl::Find_ObjectDatas(string strObjectName, map<size_t, vector<CGameObject*>>::iterator& OutObjectIter, map<size_t, vector<MTO_DATA>>::iterator& OutDataIter)
{
    size_t HashNum = Convert_ToHash( strObjectName);
    OutObjectIter = m_ObjectNamingGroupMap.find(HashNum);
    OutDataIter = m_DataNamingGroupMap.find(HashNum);

    if (OutObjectIter == m_ObjectNamingGroupMap.end())
        return false;
    if (OutDataIter == m_DataNamingGroupMap.end())
        return false;

    return true;
}

void CFunc_ObjectControl::Place_Clone()
{
    if (m_eCloneDir == CLONE_END)
        assert(0);
    if (nullptr == m_pObjTransform || nullptr == m_pCurSelectGameObject)
    {
        m_eCloneDir = CLONE_NONE;
        return;
    }
    switch(m_eCloneDir)
    {
    case CLONE_RIGHT:
        m_vCompDir = m_pObjTransform->Get_World(WORLD_RIGHT);
        break;
    case CLONE_UP:
        m_vCompDir = m_pObjTransform->Get_World(WORLD_UP);
        break;
    case CLONE_LOOK:
        m_vCompDir = m_pObjTransform->Get_World(WORLD_LOOK);
        break;
    }


    _float4 OutPos = get<CWindow_Map::PICK_OUTPOS>(m_pMapTool->Get_PickData());
    MTO_DATA tData = (*m_pCurSelectData);
    Clone_Object(OutPos, m_vCompDir, tData);
    //_matrix WorldMat = tData.ObjectStateMatrix.XMLoad();
    //WorldMat.r[3] = OutPos.XMLoad();
    //tData.ObjectStateMatrix = WorldMat;
    //string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
    //Add_ObjectNamingMap(strGroupName, tData);

}

void CFunc_ObjectControl::Clone_SamePosition()
{
    if (nullptr == m_pObjTransform || nullptr == m_pCurSelectGameObject || nullptr == m_pCurSelectData)
    {
        m_eCloneDir = CLONE_NONE;
        return;
    }
    _float4 ObjectPosition = m_pObjTransform->Get_World(WORLD_POS);
    MTO_DATA tData = (*m_pCurSelectData);
    Clone_Object(ObjectPosition, _float4(0.f, 1.f, 0.f, 0.f), tData);
    //_matrix WorldMat = tData.ObjectStateMatrix.XMLoad();
    //WorldMat.r[3] = ObjectPosition.XMLoad();
    //tData.ObjectStateMatrix = WorldMat;
    //string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
    //Add_ObjectNamingMap(strGroupName, tData);
}

CStructure* CFunc_ObjectControl::Clone_Object(_float4 vObjectPosition, _float4 vObjectCompDir, MTO_DATA tData)
{
    CStructure* pReturnStructure = nullptr;

    _float4 CloningPosition = vObjectPosition;
    MTO_DATA CloningData = tData;

    _matrix WorldMat = CloningData.ObjectStateMatrix.XMLoad();
    WorldMat.r[3] = CloningPosition.XMLoad();
    CloningData.ObjectStateMatrix = WorldMat;

    string strGroupName = get<Tuple_GroupName>(m_GroupingInfo[m_SelectObjectGroupIndex]);
    pReturnStructure = Add_ObjectNamingMap(strGroupName, CloningData);

    return pReturnStructure;
}

void CFunc_ObjectControl::Add_HLOD()
{

    _int NameIndex = 0;
    for (vector<string>::value_type& Value : m_vecSelectedMeshFilePath)
    {
        wstring strMeshPath = CFunctor::To_Wstring(Value);
        CStructure* pStructure = CStructure::Create(strMeshPath);
        if (nullptr == pStructure)
            assert(0);
        pStructure->Initialize();
        CREATE_GAMEOBJECT(pStructure, GROUP_DEFAULT);
        m_pHLODList.push_back(pStructure);
        NameIndex++;
    }
}

void CFunc_ObjectControl::Load_Data(string FilePath)
{
    Confirm_Data();

    if(bTest)
        Load_ObjectGroup_Temp(FilePath);
    else
    {
        Clear_AllDatas();
        string strGroupPath;
        string strSplitPath;
        string strMergePath;
        Load_ObjectData(FilePath, strGroupPath, strSplitPath, strMergePath);
        Load_ObjectGroup(strGroupPath);
        Load_ObjectSplit(strSplitPath);
    }

    map<size_t, list<_int>> Padding;
    for(auto& elem : m_ObjectNameGroupingMap)
    {
        for (auto& elem2 : elem.second)
        {
            size_t hash = Convert_ToHash(get<0>(elem2));
            map<size_t, list<_int>>::iterator PaddingIter = Padding.find(hash);
            if (PaddingIter == Padding.end())
            {
                Padding.emplace(hash, list<_int>());
                PaddingIter = Padding.find(hash);
            }
            for (auto& elem3 : get<1>(elem2))
            {
                PaddingIter->second.push_back(elem3);
            }
            PaddingIter->second.sort();
        }
    }

    for (auto& elem : Padding)
    {
        ObjectMap::iterator ObjIter = m_ObjectNamingGroupMap.find(elem.first);
        DataMap::iterator DataIter = m_DataNamingGroupMap.find(elem.first);
        if (ObjIter == m_ObjectNamingGroupMap.end())
            continue;
        if (DataIter == m_DataNamingGroupMap.end())
            continue;
        ObjectArr& ObjArr = ObjIter->second;
        DataArr& DataArr = DataIter->second;

        ObjectArr::iterator ObjArrIter = ObjArr.begin();
        DataArr::iterator DataArrIter = DataArr.begin();
        
        if (ObjArrIter == ObjArr.end())
            continue;
        if (DataArrIter == DataArr.end())
            continue;
        
        _int Index = 0;
        _bool bInsert = false;

        if (elem.second.empty())
            continue;

        for (list<_int>::iterator iter = elem.second.begin(); iter != elem.second.end(); ++iter)
        {
            if(bInsert)
            {
                iter--;
                bInsert = false;
            }
            if ((*iter) != Index)
            {
                MTO_DATA tData;
                tData.Initialize();
                ObjArrIter = ObjArr.emplace(ObjArrIter, nullptr);
                DataArrIter = DataArr.emplace(DataArrIter, tData);
                
                Index++;
                bInsert = true;
            }
            else {
                Index++;
                ObjArrIter++;
                DataArrIter++;
            }
        }
    }

}



_bool CFunc_ObjectControl::Find_ObjectGroupInfo(string strGroupName, vector<tuple<string, bool>>::iterator& OutGroupingIter, size_t& OutHashNum)
{
    OutHashNum = Convert_ToHash( strGroupName);
    OutGroupingIter
        = find_if(m_GroupingInfo.begin(), m_GroupingInfo.end(),
            [&strGroupName](GroupingArr::value_type& Value)
            {
                if (get<Tuple_GroupName>(Value) == strGroupName)
                    return true;
                else
                    return false;
            });

    if (OutGroupingIter == m_GroupingInfo.end())
        return false;
    return true;
}
#pragma region MTO_DATA 멤버함수
void CFunc_ObjectControl::tagMapToolObjectData::Initialize()
{
    bIgnoreFlag = true;
    //strGroupName = wstring();
    strObejctName = wstring();
    strMeshPath = wstring();
    ObjectStateMatrix.Identity();
    ObejectIndex = 0;
    vScale = _float4(1.f, 1.f, 1.f, 0.f);
    byteLightFlag = 0;
}
#pragma endregion

void CFunc_ObjectControl::tagMapToolObjectMergeData::Initialize()
{
    _float4x4 ObjectStateMatrix;
    ObjectStateMatrix.Identity();
    _float4 vScale = _float4(1.f, 1.f,1.f, 0.f);
    _int ObejectIndex = 0;
    _byte byteLightFlag = 0;
}

void CFunc_ObjectControl::tagMapToolCollisionData::Initialize()
{
    strObejctName = wstring(_T(""));
    iCollisionType = 0;
    iLODType = 3;
    iBoxNums = 0;
    pBoxScaleInfo = nullptr;
    pBoxRotInfo = nullptr;
    pBoxTranslateInfo = nullptr;
}
