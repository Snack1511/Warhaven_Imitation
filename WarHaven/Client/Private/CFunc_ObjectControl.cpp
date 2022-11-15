#include "stdafx.h"
#include "CFunc_ObjectControl.h"

#include "CWindow_Map.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "CStructure.h"
#include "Transform.h"
#include "CUtility_Transform.h"

#include "Functor.h"

CFunc_ObjectControl::CFunc_ObjectControl()
{
}

CFunc_ObjectControl::~CFunc_ObjectControl()
{
    Clear_TupleData(m_arrMeshGroupName);
}

CFunc_ObjectControl* CFunc_ObjectControl::Create(CWindow_Map* pMaptool)
{
    CFunc_ObjectControl* pInstance = new CFunc_ObjectControl();
    pInstance->m_pMapTool = pMaptool;
    pInstance->m_pMeshRoot = pMaptool->Get_MeshRoot();
    return pInstance;
}

void CFunc_ObjectControl::Destroy()
{
    delete this;
}
static char szMeshGroupNameBuf[MAXCHAR] = "";
void CFunc_ObjectControl::Func_Grouping()
{
    if (ImGui::CollapsingHeader("Object Grouping", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //그룹 이름
        string DebugMeshGroupName = szMeshGroupNameBuf;
        m_pMapTool->DebugData("MeshGroupName", DebugMeshGroupName);
        if (ImGui::InputText("##GroupNameInput", szMeshGroupNameBuf, sizeof(char) * MAXCHAR))
        {

        }
        if (ImGui::Button("Add MeshGroup"))
        {
            Add_MeshGroup(szMeshGroupNameBuf);
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete MeshGroup"))
        {
            Delete_MeshGroup(szMeshGroupNameBuf);
        }
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_FBXList()
{
    if (ImGui::CollapsingHeader("FBX Files List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //파일 탐색 트리노드
        if (ImGui::BeginListBox("##FBX_Files_List", ImVec2(360.f, 200.f)))
        {
            m_pMapTool->Show_TreeData(m_pMeshRoot, bind(&CFunc_ObjectControl::Routine_MeshSelect, this, placeholders::_1));
            ImGui::EndListBox();
        }
        m_pMapTool->DebugData("CurSelectedMeshFilePath", m_CurSelectedMeshFilePath);
        m_pMapTool->DebugData("SelectedMeshName", m_CurSelectedMeshName);
        ImGui::Spacing();

        if (ImGui::Button("Add Object"))
        {
            Func_AddObject();

        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Object"))
        {
            Func_DeleteOBject();
        }
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_ObjectList()
{
    if (ImGui::CollapsingHeader("Add Object List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //해당 이름을 가진 탭
        if (ImGui::BeginTabBar("##AddObject TabBar"))
        {
            for (_int i = 0; i < _int(m_arrMeshGroupName.size()); ++i)
            {
                if (ImGui::BeginTabItem(get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[i])))
                {
                    //리스트 업데이트
                    if (ImGui::BeginListBox("##TabList", ImVec2(360.f, 200.f)))
                    {
                        m_SelectMeshGroupIndex = i;
                        size_t Hashnum = HASHING(string, string(get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex])));
                        vector<MTO_DATA>& CurGroupData = m_ObjectDataGroupMap[Hashnum];
                        _int Index = 0;
                        for (MTO_DATA& Data : CurGroupData)
                        {
                            _bool bSelected = false;
                            string MeshName = CFunctor::To_String(Data.strMeshName);

                            if (m_strCurSelectObjectName == MeshName)
                            {
                                bSelected = true;
                            }
                            if (m_iCurSelectObjectIndex == Index)
                            {
                                bSelected = true;
                            }
                            if (ImGui::Selectable(MeshName.c_str(), bSelected))
                            {
                                m_strCurSelectObjectName = MeshName;
                                m_iCurSelectObjectIndex = Index;
                                SetUp_CurSelectObject();
                                //선택 
                            }

                            Index++;
                        }
                        ImGui::EndListBox();
                    }
                    ImGui::EndTabItem();
                }

            }

            ImGui::EndTabBar();
        }
        if (ImGui::Button("Clear MeshGroup"))
        {
            Clear_MeshGroup(szMeshGroupNameBuf);
        }
        if (!m_arrMeshGroupName.empty())
        {
            string DebugTab = get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]);
            m_pMapTool->DebugData("SelectTab", DebugTab);
        }
        m_pMapTool->DebugData("SelectedName", m_strCurSelectObjectName);
    }
    ImGui::Spacing();
}

void CFunc_ObjectControl::Func_ObjectControl()
{
    string strPickInfo = "";
    if (!m_pMapTool->Is_CurPickMode(CWindow_Map::PICK_OBJECT))
    {
        strPickInfo = "Use_Picking";
    }
    else
    {
        strPickInfo = "Unuse_Picking";
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
    ImGui::Spacing();

    switch (m_eControlType)
    {
    case CONTROL_SCALING:
        ImGui::Text("MODE : SCALING");
        break;
    case CONTROL_ROTATE:
        ImGui::Text("MODE : ROTATE");
        break;
    case CONTROL_MOVE:
        ImGui::Text("MODE : MOVE");
        break;
    }
    ImGui::Spacing();

    if (ImGui::Button("Confirm"))
    {
        m_pMapTool->Change_CurPickMode(CWindow_Map::PICK_NONE);
        Confirm_Data();
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Object Matrix(Read-Only)", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Show_ObjectData();
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Object Speed", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Set_ObjectSpeed();
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
}

void CFunc_ObjectControl::Func_Picking()
{
    Place_Object();
    Change_Object_UpDir();
}

void CFunc_ObjectControl::Tick_Function()
{
    Select_DataControlFlag();
    Control_Object();
    Update_Data();
}

_bool CFunc_ObjectControl::Is_CurSelectObject()
{
    if (nullptr == m_pCurSelectGameObject)
        return false;
    return true;
}

void CFunc_ObjectControl::SetUp_CurSelectObject()
{
    string CurSelectMeshGroup = get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]);
    size_t HashNum = HASHING(string, CurSelectMeshGroup);

    OBJGROUPING::iterator ObjGroupIter = m_ObjectGroupMap.find(HashNum);
    DATAGROUPING::iterator DataGroupIter = m_ObjectDataGroupMap.find(HashNum);

    OBJVECTOR* pObjGroupArr = nullptr;
    DATAVECTOR* pDataGroupArr = nullptr;

    if (m_ObjectGroupMap.end() != ObjGroupIter)
    {
        pObjGroupArr = &(ObjGroupIter->second);
    }

    if (m_ObjectDataGroupMap.end() != DataGroupIter)
    {
        pDataGroupArr = &(DataGroupIter->second);
    }

    if (nullptr == pObjGroupArr)
        return;
    else
        m_pCurSelectGameObject = (*pObjGroupArr)[m_iCurSelectObjectIndex];

    if (nullptr == pDataGroupArr)
        return;
    else
        m_pCurSelectData = &((*pDataGroupArr)[m_iCurSelectObjectIndex]);

    if (nullptr != m_pCurSelectGameObject)
        m_pObjTransform = m_pCurSelectGameObject->Get_Transform();
    else
    {
        m_pObjTransform = nullptr;
    }
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
}

void CFunc_ObjectControl::Add_MeshGroup(char* pMeshGroupName)
{
    CWindow_Map::DataComboArr::iterator MeshGroupIter 
        = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), 
            [&pMeshGroupName](CWindow_Map::DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<CWindow_Map::Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (MeshGroupIter == m_arrMeshGroupName.end())
    {
        char* pGroupName = new char[260];
        ZeroMemory(pGroupName, sizeof(char) * 260);
        memcpy_s(pGroupName, sizeof(char) * 260, pMeshGroupName, sizeof(char) * 260);
        m_arrMeshGroupName.push_back(make_tuple(pGroupName, false));

        string strObjectGroup = pMeshGroupName;
        size_t HashNum = HASHING(string, strObjectGroup);

        OBJGROUPING::iterator ObjMapIter = m_ObjectGroupMap.find(HashNum);
        if (ObjMapIter == m_ObjectGroupMap.end())
        {
            m_ObjectGroupMap.emplace(HashNum, vector<CGameObject*>());
        }

        DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);
        if (DataMapIter == m_ObjectDataGroupMap.end())
        {
            m_ObjectDataGroupMap.emplace(HashNum, vector<MTO_DATA>());
        }
        Confirm_Data();
    }
}

void CFunc_ObjectControl::Delete_MeshGroup(char* pMeshGroupName)
{
    CWindow_Map::DataComboArr::iterator MeshGroupIter
        = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), 
            [&pMeshGroupName](CWindow_Map::DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<CWindow_Map::Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (MeshGroupIter != m_arrMeshGroupName.end())
    {
        char* pGroupName = get<CWindow_Map::Tuple_CharPtr>((*MeshGroupIter));

        string strObjectGroup = pGroupName;
        size_t HashNum = HASHING(string, strObjectGroup);
        OBJGROUPING::iterator ObjMapIter = m_ObjectGroupMap.find(HashNum);
        DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);
        if (ObjMapIter != m_ObjectGroupMap.end())
        {
            for (list<CGameObject*>::value_type& Value : ObjMapIter->second)
            {
                DELETE_GAMEOBJECT(Value);
            }
            ObjMapIter->second.clear();
            m_ObjectGroupMap.erase(ObjMapIter);
        }
        if (DataMapIter != m_ObjectDataGroupMap.end())
        {
            DataMapIter->second.clear();
            m_ObjectDataGroupMap.erase(DataMapIter);
        }

        m_arrMeshGroupName.erase(MeshGroupIter);
        Safe_Delete_Array(pGroupName);
        m_SelectMeshGroupIndex = (m_SelectMeshGroupIndex <= 1) ? 0 : m_SelectMeshGroupIndex - 1;
        Confirm_Data();
    }
}

void CFunc_ObjectControl::Add_Object(string MeshGroup, string Meshpath, string MeshName)
{
    vector<CGameObject*>* pObjectList = nullptr;
    vector<MTO_DATA>* pDataList = nullptr;
    wstring strObjectGroupName;

    size_t HashNum = HASHING(string, MeshGroup);
    OBJGROUPING::iterator ObjMapIter = m_ObjectGroupMap.find(HashNum);
    DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);
    if (ObjMapIter == m_ObjectGroupMap.end())
    {
        m_ObjectGroupMap.emplace(HashNum, vector<CGameObject*>());
        pObjectList = &(m_ObjectGroupMap[HashNum]);
    }
    else
    {
        pObjectList = &ObjMapIter->second;
    }
    if (DataMapIter == m_ObjectDataGroupMap.end())
    {
        m_ObjectDataGroupMap.emplace(HashNum, vector<MTO_DATA>());
        pDataList = &(m_ObjectDataGroupMap[HashNum]);
    }
    else
    {
        pDataList = &DataMapIter->second;
    }

    if (nullptr == pObjectList)
        assert(0);
    if (nullptr == pDataList)
        assert(0);
    //Meshpath
    wstring strName = CFunctor::To_Wstring(MeshName);
    size_t NameHashNum = HASHING(wstring, strName);
    map<size_t, _int>::iterator CallStackIter = m_ObjNameCallStack.find(NameHashNum);
    if (CallStackIter == m_ObjNameCallStack.end())
    {
        m_ObjNameCallStack.emplace(NameHashNum, 0);
    }

    MTO_DATA tData;
    tData.Initialize();
    tData.strMeshName = strName + wstring(TEXT("_")) + to_wstring(m_ObjNameCallStack[NameHashNum]++);
    //tData.strGroupName = CFunctor::To_Wstring(MeshGroup);
    tData.strMeshPath = CFunctor::To_Wstring(Meshpath);
    tData.ObjectStateMatrix.Identity();

    CStructure* pGameObject = CStructure::Create(tData.strMeshPath);
    if (nullptr == pGameObject)
        assert(0);
    pGameObject->Initialize();
    CREATE_GAMEOBJECT(pGameObject, GROUP_DECORATION);

    pObjectList->push_back(pGameObject);
    pDataList->push_back(tData);
    Confirm_Data();
}

void CFunc_ObjectControl::Add_Object(string MeshGroup, MTO_DATA& tData)
{
    vector<CGameObject*>* pObjectList = nullptr;
    vector<MTO_DATA>* pDataList = nullptr;
    wstring strObjectGroupName;

    size_t HashNum = HASHING(string, MeshGroup);
    OBJGROUPING::iterator ObjMapIter = m_ObjectGroupMap.find(HashNum);
    DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);
    if (ObjMapIter == m_ObjectGroupMap.end())
    {
        m_ObjectGroupMap.emplace(HashNum, vector<CGameObject*>());
        pObjectList = &(m_ObjectGroupMap[HashNum]);
    }
    else
    {
        pObjectList = &ObjMapIter->second;
    }

    if (DataMapIter == m_ObjectDataGroupMap.end())
    {
        m_ObjectDataGroupMap.emplace(HashNum, vector<MTO_DATA>());
        pDataList = &(m_ObjectDataGroupMap[HashNum]);
    }
    else
    {
        pDataList = &DataMapIter->second;
    }

    if (nullptr == pObjectList)
        assert(0);
    if (nullptr == pDataList)
        assert(0);
    //Meshpath
    wstring strName = tData.strMeshName;
    _int iIndexLength = strName.rfind(TEXT("_"), strName.length() + 1);
    strName = strName.substr(0, iIndexLength);
    size_t NameHashNum = HASHING(wstring, strName);
    map<size_t, _int>::iterator CallStackIter = m_ObjNameCallStack.find(NameHashNum);
    if (CallStackIter == m_ObjNameCallStack.end())
    {
        m_ObjNameCallStack.emplace(NameHashNum, 0);
    }
    m_ObjNameCallStack[NameHashNum]++;

    CStructure* pGameObject = CStructure::Create(tData.strMeshPath, tData.vScale, tData.ObjectStateMatrix);
    if (nullptr == pGameObject)
        assert(0);
    pGameObject->Initialize();
    CREATE_GAMEOBJECT(pGameObject, GROUP_DECORATION);

    pObjectList->push_back(pGameObject);
    pDataList->push_back(tData);

    Confirm_Data();
}

void CFunc_ObjectControl::Delete_Object(string MeshName, vector<CGameObject*>& ObjList, vector<MTO_DATA>& DataList)
{
    if (ObjList.size() != DataList.size())
        assert(0);
    if (ObjList.empty())
        return;
    wstring CmpMeshName = CFunctor::To_Wstring(MeshName);
    _int ObjectIndex = 0;
    vector<MTO_DATA>::iterator DataListIter = DataList.begin();
    for (; DataListIter != DataList.end(); ++DataListIter)
    {
        if ((*DataListIter).strMeshName == CmpMeshName)
            break;
        ObjectIndex++;
    }
    if (DataListIter == DataList.end())
        return;

    wstring strName = (*DataListIter).strMeshName;
    _int iIndexLength = strName.rfind(TEXT("_"), strName.length() + 1);
    strName = strName.substr(0, iIndexLength);
    size_t NameHashNum = HASHING(wstring, strName);
    map<size_t, _int>::iterator CallStackIter = m_ObjNameCallStack.find(NameHashNum);
    if (--(CallStackIter->second) == 0)
    {
        m_ObjNameCallStack.erase(CallStackIter);
    }



    DataList.erase(DataListIter);

    if (ObjectIndex >= _int(ObjList.size()))
        assert(0);


    CGameObject* pDelete = ObjList[ObjectIndex];
    vector<CGameObject*>::iterator ObjListIter = ObjList.begin();

    for (_int CompIndex = 0; CompIndex < ObjectIndex; ++CompIndex)
    {
        ObjListIter++;
    }
    ObjList.erase(ObjListIter);
    DELETE_GAMEOBJECT(pDelete);
    Confirm_Data();
}

void CFunc_ObjectControl::Clear_MeshGroup(char* pMeshGroupName)
{
    CWindow_Map::DataComboArr::iterator MeshGroupIter = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), [&pMeshGroupName](CWindow_Map::DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<CWindow_Map::Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (MeshGroupIter != m_arrMeshGroupName.end())
    {
        string strObjectGroup = get<CWindow_Map::Tuple_CharPtr>((*MeshGroupIter));
        size_t HashNum = HASHING(string, strObjectGroup);
        OBJGROUPING::iterator ObjMapIter = m_ObjectGroupMap.find(HashNum);
        DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);
        if (ObjMapIter != m_ObjectGroupMap.end())
        {
            for (list<CGameObject*>::value_type& Value : ObjMapIter->second)
            {
                DELETE_GAMEOBJECT(Value);
            }
            ObjMapIter->second.clear();
            m_ObjectGroupMap.erase(ObjMapIter);
        }
        if (DataMapIter != m_ObjectDataGroupMap.end())
        {
            DataMapIter->second.clear();
            m_ObjectDataGroupMap.erase(DataMapIter);
        }
        Confirm_Data();

    }
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
        string Ext(".fbx");
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

void CFunc_ObjectControl::Func_DeleteOBject()
{
    size_t HashNum = HASHING(string, string(get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex])));

    OBJGROUPING::iterator ObjectMapIter = m_ObjectGroupMap.find(HashNum);
    DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);

    if (ObjectMapIter != m_ObjectGroupMap.end() && DataMapIter != m_ObjectDataGroupMap.end())
    {
        Delete_Object(m_strCurSelectObjectName, ObjectMapIter->second, DataMapIter->second);
    }
}

void CFunc_ObjectControl::Func_AddObject()
{
    _int NameIndex = 0;
    for (vector<string>::value_type& Value : m_vecSelectedMeshFilePath)
    {
        if (!m_arrMeshGroupName.empty())
        {
            Add_Object(get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]),
                Value, m_vecSelectedMeshName[NameIndex]);
        }
        NameIndex++;
    }

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

void CFunc_ObjectControl::Set_ObjectSpeed()
{
    ImGui::Text("Scale : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Scaling Speed", &m_fTickPerScalingSpeed, 0.1f, 10.f, "%.1f");
    ImGui::Text("Rotate : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Rotate Speed", &m_fTickPerRotSpeed, 0.1f, 90.f, "%.1f");
    ImGui::Text("Move : ");
    ImGui::SameLine();
    ImGui::SliderFloat("##Move Speed", &m_fTickPerMoveSpeed, 0.1f, 50.f, "%.1f");
}

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
}

void CFunc_ObjectControl::Save_ObjectGroup(string BasePath, string SaveName)
{
    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".GroupData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }


    //그룹 개수 저장
    _int GroupSize = _int(m_arrMeshGroupName.size());
    writeFile.write((char*)&GroupSize, sizeof(_int));

    _int i = 0;
    for (_int i = 0; i < GroupSize; ++i)
    {
        //Loop~ 그룹 이름 저장
        char* pGroupName = get<CWindow_Map::Tuple_CharPtr>(m_arrMeshGroupName[i]);
        _int GroupNameLength = strlen(pGroupName) + 1;
        writeFile.write((char*)&GroupNameLength, sizeof(_int));
        writeFile.write(pGroupName, sizeof(char) * GroupNameLength);

        //오브젝트 개수 저장
        string strGroupName = pGroupName;
        size_t GroupNameHash = HASHING(string, strGroupName);
        vector<MTO_DATA>* pDataList = &(m_ObjectDataGroupMap.find(GroupNameHash)->second);
        _int DataSize = _int(pDataList->size());
        writeFile.write((char*)&DataSize, sizeof(_int));
        //Loop~ MTO데이터 저장
        for (_int j = 0; j < DataSize; ++j)
        {
            char* pName = new char[MAX_PATH];
            strcpy_s(pName, sizeof(char) * MAX_PATH, CFunctor::To_String((*pDataList)[j].strMeshName).c_str());
            char* pPath = new char[MAX_PATH];
            strcpy_s(pPath, sizeof(char) * MAX_PATH, CFunctor::To_String((*pDataList)[j].strMeshPath).c_str());
            _float4x4 StateMatrix = (*pDataList)[j].ObjectStateMatrix;
            _float4 vScale = (*pDataList)[j].vScale;
            _byte LightFlag = (*pDataList)[j].byteLightFlag;

            //이름 저장
            _int NameLength = strlen(pName) + 1;
            writeFile.write((char*)&NameLength, sizeof(_int));
            writeFile.write(pName, sizeof(char) * NameLength);

            //경로 저장
            _int PathLength = strlen(pPath) + 1;
            writeFile.write((char*)&PathLength, sizeof(_int));
            writeFile.write(pPath, sizeof(char) * PathLength);

            //행렬 저장
            writeFile.write((char*)&StateMatrix, sizeof(_float4x4));

            //스케일 저장
            writeFile.write((char*)&vScale, sizeof(_float4));

            //라이트플래그 저장
            writeFile.write((char*)&LightFlag, sizeof(_byte));

            Safe_Delete_Array(pName);
            Safe_Delete_Array(pPath);
        }
    }
    writeFile.close();
}

void CFunc_ObjectControl::Load_ObjectGroup(string FilePath)
{
    Clear_TupleData(m_arrMeshGroupName);
    for (DATAGROUPING::value_type& Value : m_ObjectDataGroupMap)
    {
        Value.second.clear();
    }
    m_ObjectDataGroupMap.clear();

    for (OBJGROUPING::value_type& MapValue : m_ObjectGroupMap)
    {
        for (OBJVECTOR::value_type& Value : MapValue.second)
        {
            DELETE_GAMEOBJECT(Value);
        }
        MapValue.second.clear();
    }
    m_ObjectGroupMap.clear();

    for (map<size_t, _int>::value_type& Value : m_ObjNameCallStack)
    {
        Value.second = 0;
    }


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
        m_arrMeshGroupName.push_back(make_tuple(pGroupName, false));
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
            tData.strMeshName = CFunctor::To_Wstring(strObjName);

            //경로 저장
            _int ObjectPathLength = 0;
            readFile.read((char*)&ObjectPathLength, sizeof(_int));
            char* pObjectPath = new char[ObjectPathLength];
            readFile.read(pObjectPath, sizeof(char) * ObjectPathLength);
            string strObjPath = pObjectPath;
            tData.strMeshPath = CFunctor::To_Wstring(strObjPath);

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

            Add_Object(strGroupName, tData);

            Safe_Delete_Array(pObjectName);
            Safe_Delete_Array(pObjectPath);
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
#pragma region MTO_DATA 멤버함수
void CFunc_ObjectControl::tagMapToolObjectData::Initialize()
{
    strMeshName = wstring();
    //strGroupName = wstring();
    strMeshPath = wstring();
    ObjectStateMatrix.Identity();
    vScale = _float4(1.f, 1.f, 1.f, 0.f);
    byteLightFlag = 0;
}
#pragma endregion