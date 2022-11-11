#include "stdafx.h"
#include "CWindow_Map.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Functor.h"
#include "CStructure.h"

#include "CUtility_Transform.h"

#include "CDrawable_Terrain.h"

#include "Transform.h"

#include "CMesh_Terrain.h"
#include "Easing_Utillity.h"
#include "CGame_Manager_MJ.h"
CWindow_Map::CWindow_Map()
{
    m_CurTerrainData.Initialize();
}

CWindow_Map::~CWindow_Map()
{
    Clear_TupleData(m_arrSaveFilesCombo);
    Clear_TupleData(m_arrObjectGroupId);
    Clear_TupleData(m_arrMeshGroupName);
    Clear_TupleData(m_arrLightTypeCombo);
    Clear_TupleData(m_arrLightGroupCombo);
    Clear_TupleData(m_arrBrushType);
    CGame_Manager_MJ::Destroy_Instance();
}

CWindow_Map* CWindow_Map::Create()
{
    CWindow_Map* pInstance = new CWindow_Map;

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWindow_Map");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
static _int SaveFileIndex = 0;
HRESULT CWindow_Map::Initialize()
{
    m_bEnable = false;
    SetUp_ImGuiDESC(typeid(CWindow_Map).name(), ImVec2(400.f, 600.f), ImGuiWindowFlags_AlwaysAutoResize);


    //파일 위치 설정
    m_MeshRootNode.strFolderPath = "../bin/resources/meshes";
    m_MeshRootNode.strFileName = "Map";
    m_MeshRootNode.strFullPath = "../bin/resources/meshes/Map";
    Read_Folder_ForTree("../bin/resources/meshes/Map", m_MeshRootNode);

    m_strPath = "../Bin/Data/MapData/";
    //콤보어레이 생성
    Ready_FileArray();
    if (!m_arrSaveFilesCombo.empty())
    {
        get<Tuple_Bool>(m_arrSaveFilesCombo[SaveFileIndex]) = true;
    }
    Ready_ObjectGroupID();

    Ready_LightGroup();

    Ready_LightType();

    Ready_TerrainBrushType();
    return S_OK;
}

void CWindow_Map::Tick()
{
    _bool bPicked = false;
    Select_DataControlFlag();

    if (Calculate_Pick())
        bPicked = true;

    if (false == bPicked)
        Control_Object();

    Update_Data();
}

HRESULT CWindow_Map::Render()
{
    ImVec2 vPannelSize = ImVec2(300.f, 300.f);
    ImVec2 vTerrainControlPos = ImVec2(vPannelSize.x, 0.f);
    ImVec2 vDataControlPos = ImVec2(vPannelSize.x, vPannelSize.y);
    ImVec2 vLightControlPos = ImVec2(vPannelSize.x, vPannelSize.y * 2);

    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::Text("MapTool");

    //파일 컨트롤
    Func_FileControl();

    if (nullptr != m_pCurTerrain)
    {
        Create_SubWindow("Terrain_Controller", vTerrainControlPos, vPannelSize, bind(&CWindow_Map::Func_TerrainControl, this));
    }

    //데이타 컨트롤
    if (nullptr != m_pCurSelectGameObject)
    {
        Create_SubWindow("Data_Controller", vDataControlPos, vPannelSize, bind(&CWindow_Map::Func_DataControl, this));
    }

    //라이트 컨트롤
    // 
    //조건 필요.. 
    if (false)
    {
        Create_SubWindow("Light_Controller", vLightControlPos, vPannelSize, bind(&CWindow_Map::Func_LightControl, this));
    }
    __super::End();


    return S_OK;
}

#pragma region static value 파일컨트롤러
static char szSaveNameBuf[MAXCHAR] = "";
static char szMeshGroupNameBuf[MAXCHAR] = "";
static string SaveFilePath = "";
static string LoadFilePath = "";

static int iCurSelectTileIndex = 0;
#pragma endregion

#pragma region 파일 컨트롤러 함수
void CWindow_Map::Func_FileControl()
{
    if(m_bHoverWindow)
        ImGui::Text("Hover");
    else
        ImGui::Text("No_Hover");

    ImVec2 ButtonSize(60.f, 20.f);
    //1. 선택 파일 콤보박스
    if (!m_arrSaveFilesCombo.empty()) 
    {
        SetUp_FilePath(LoadFilePath, get<Tuple_CharPtr>(m_arrSaveFilesCombo[SaveFileIndex]), ".MapData");
        Make_Combo("##Save_Files", m_arrSaveFilesCombo, &SaveFileIndex, bind(&CWindow_Map::EmptyFunction, this));
        DebugData("Debug_LoadPath", LoadFilePath);
    }

    //2. 파일명 입력 창
    SetUp_FilePath(SaveFilePath, szSaveNameBuf);
    ImGui::InputText("##SaveName", szSaveNameBuf, sizeof(char) * MAXCHAR);
    DebugData("Debug_SavePath", SaveFilePath);


    //6. 세이브 / 로드 버튼
    if (ImGui::Button("Save", ButtonSize))
    {
        string SaveFileName = szSaveNameBuf;
        Save_MapData(m_strPath, SaveFileName);
        Update_FileArray();
        //SaveFilePath
        //세이브
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", ButtonSize))
    {
        string strLoadPath = LoadFilePath;
        //"../bin/Data/MapData/Test.MapData"
        Load_MapData(LoadFilePath);
        strcpy_s(szSaveNameBuf, sizeof(char) * MAXCHAR, get<Tuple_CharPtr>(m_arrSaveFilesCombo[SaveFileIndex]));
        Confirm_Data();
        //LoadFilePath
        //로드 버튼
    }
    ImGui::Spacing();
    ImGui::Text("Select PickingGroupID");
    SetUp_CurPickingGroup();
    if (Make_Combo("##PickingGroupID", m_arrObjectGroupId, &m_SelectObjectGroupIDIndex, bind(&CWindow_Map::EmptyFunction, this)))
    {
    }
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("SetUp Terrain", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text("VertX : ");
        ImGui::SameLine();
        _int iTerrainVerticalX = m_CurTerrainData.iNumVerticesX;
        if (ImGui::InputInt("##InputVerticesX", &iTerrainVerticalX))
        {
            iTerrainVerticalX = (iTerrainVerticalX < 0) ? 0 : iTerrainVerticalX;
            m_CurTerrainData.iNumVerticesX = iTerrainVerticalX;
        }
        ImGui::SameLine();
        ImGui::Text("VertZ : ");
        ImGui::SameLine();
        _int iTerrainVerticalZ = m_CurTerrainData.iNumVerticesZ;
        if (ImGui::InputInt("##InputVerticesZ", &iTerrainVerticalZ))
        {
            iTerrainVerticalZ = (iTerrainVerticalZ < 0) ? 0 : iTerrainVerticalZ;
            m_CurTerrainData.iNumVerticesZ = iTerrainVerticalZ;
        }
        ImGui::Text("SelectTileTexture");
        //Make_Combo("##TileTextureCombo", m_arrTileTextureName, &iCurSelectTileIndex, bind(&CWindow_Map::EmptyFunction, this));
        if (ImGui::Button("Generate!"))
        {
            Generate_Terrain();
        }
    }
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Object Grouping", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //그룹 이름
        string DebugMeshGroupName = szMeshGroupNameBuf;
        DebugData("MeshGroupName", DebugMeshGroupName);
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
    if (ImGui::CollapsingHeader("FBX Files List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //파일 탐색 트리노드
        if (ImGui::BeginListBox("##FBX_Files_List", ImVec2(360.f, 200.f)))
        {
            Show_TreeData(m_MeshRootNode);
            ImGui::EndListBox();
        }
        DebugData("CurSelectedMeshFilePath", m_CurSelectedMeshFilePath);
        DebugData("SelectedMeshName", m_CurSelectedMeshName);
        ImGui::Spacing();

        if (ImGui::Button("Add Object"))
        {

            //Add_Object(m_CurSelectedMeshFilePath);
            _int NameIndex = 0;
            for (vector<string>::value_type& Value : m_vecSelectedMeshFilePath)
            {
                if (!m_arrMeshGroupName.empty())
                {
                    Add_Object(get<Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]),
                        Value, m_vecSelectedMeshName[NameIndex]);
                }
                NameIndex++;
            }

        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Object"))
        {
            size_t HashNum = HASHING(string, string(get<Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex])));

            OBJGROUPING::iterator ObjectMapIter = m_ObjectGroupMap.find(HashNum);
            DATAGROUPING::iterator DataMapIter = m_ObjectDataGroupMap.find(HashNum);

            if (ObjectMapIter != m_ObjectGroupMap.end() && DataMapIter != m_ObjectDataGroupMap.end())
            {
                Delete_Object(m_strCurSelectObjectName, ObjectMapIter->second, DataMapIter->second);
            }
        }
    }
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Add Object List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //해당 이름을 가진 탭
        if (ImGui::BeginTabBar("##AddObject TabBar"))
        {
            for (_int i = 0; i < _int(m_arrMeshGroupName.size()); ++i)
            {
                if (ImGui::BeginTabItem(get<Tuple_CharPtr>(m_arrMeshGroupName[i])))
                {
                    //리스트 업데이트
                    if (ImGui::BeginListBox("##TabList", ImVec2(360.f, 200.f)))
                    {
                        m_SelectMeshGroupIndex = i;
                        size_t Hashnum = HASHING(string, string(get<Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex])));
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
            string DebugTab = get<Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]);
            DebugData("SelectTab", DebugTab);
        }
        DebugData("SelectedName", m_strCurSelectObjectName);
    }
    ImGui::Spacing();
    //5. 선택된 오브젝트 이름 텍스트
}

void CWindow_Map::Ready_FileArray()
{
    list<string> FileNameList = Read_Folder_ToStringList(m_strPath.c_str());

    for (list<string>::value_type& Value : FileNameList)
    {
        string FileName = Value;
        FileName += "\0";
        _int FileNameLength = _int(FileName.size()) + 1;
        char* pFileName = new char[260];
        memcpy_s(pFileName, sizeof(char) * 260, FileName.c_str(), sizeof(char) * FileNameLength);
        m_arrSaveFilesCombo.push_back(make_tuple(pFileName, false));
    }
}
void CWindow_Map::Ready_ObjectGroupID()
{
    for (_uint i = 0; i < _uint(GROUP_END); ++i)
    {
        if (nullptr == ArrObjectGroup[i])
            continue;
        else
        {
            char* pFileName = new char[260];
            memcpy_s(pFileName, sizeof(char) * 260, ArrObjectGroup[i], sizeof(char) * 260);
            m_arrObjectGroupId.push_back(make_tuple(pFileName, false));
        }
    }
}

void CWindow_Map::Update_FileArray()
{
    Clear_TupleData(m_arrSaveFilesCombo);

    list<string> FileNameList = Read_Folder_ToStringList(m_strPath.c_str());

    for (list<string>::value_type& Value : FileNameList)
    {
        string FileName = Value;
        FileName += "\0";
        _int FileNameLength = _int(FileName.size()) + 1;
        char* pFileName = new char[260];
        memcpy_s(pFileName, sizeof(char) * 260, FileName.c_str(), sizeof(char) * FileNameLength);
        m_arrSaveFilesCombo.push_back(make_tuple(pFileName, false));
    }

}
void CWindow_Map::SetUp_FilePath(string& strFilePath, char* szData, string strExt)
{
    strFilePath = "";
    strFilePath += m_strPath;
    strFilePath += szData;
    strFilePath += strExt;
}
void CWindow_Map::SetUp_CurPickingGroup()
{
    m_pCurObjectList = &(GAMEINSTANCE->Get_ObjGroup(m_SelectObjectGroupIDIndex));
}

void CWindow_Map::Add_MeshGroup(char* pMeshGroupName)
{
    DataComboArr::iterator MeshGroupIter = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), [&pMeshGroupName](DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<Tuple_CharPtr>(Value)))
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
void CWindow_Map::Delete_MeshGroup(char* pMeshGroupName)
{
    DataComboArr::iterator MeshGroupIter = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), [&pMeshGroupName](DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (MeshGroupIter != m_arrMeshGroupName.end())
    {
        char* pGroupName = get<Tuple_CharPtr>((*MeshGroupIter));

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

void CWindow_Map::Add_Object(string MeshGroup, string Meshpath, string MeshName)
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
void CWindow_Map::Add_Object(string MeshGroup, MTO_DATA& tData)
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
void CWindow_Map::Delete_Object(string MeshName, vector<CGameObject*>& ObjList, vector<MTO_DATA>& DataList)
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

void CWindow_Map::Clear_MeshGroup(char* pMeshGroupName)
{
    DataComboArr::iterator MeshGroupIter = find_if(m_arrMeshGroupName.begin(), m_arrMeshGroupName.end(), [&pMeshGroupName](DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pMeshGroupName, get<Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (MeshGroupIter != m_arrMeshGroupName.end())
    {
        string strObjectGroup = get<Tuple_CharPtr>((*MeshGroupIter));
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
#pragma endregion

#pragma region static value 데이터 컨트롤러
//static 
static bool ObjectLightFlagOpt[4] = { false };
#pragma endregion

void CWindow_Map::Func_DataControl()
{
    //ImGui::Text("Data_Control");

    //z : Scale
    //x : Rotate
    //c : Position
    _bool bPicked = (!m_bHoverWindow && m_bObjectPick);
    if (bPicked)
    {
        ImGui::Text("Use_Picking");
    }
    else
    {
        ImGui::Text("Unuse_Picking");
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

    if (false == bPicked)
    {
        if (ImGui::Button("Confirm"))
        {
            Confirm_Data();
        }
        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Object Matrix(Read-Only)", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Show_ObjectData();
    }
    ImGui::Spacing();

    if (bPicked)
        return;

    if (ImGui::CollapsingHeader("Object Speed", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        Set_ObjectSpeed();
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Light Option", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        if (ImGui::Checkbox("LightOpt_1", &ObjectLightFlagOpt[0]))
        {
        }
        if (ImGui::Checkbox("LightOpt_2", &ObjectLightFlagOpt[1]))
        {
        }
        if (ImGui::Checkbox("LightOpt_3", &ObjectLightFlagOpt[2]))
        {
        }
        if (ImGui::Checkbox("LightOpt_4", &ObjectLightFlagOpt[3]))
        {
        }
    }
}

void CWindow_Map::SetUp_CurSelectObject()
{
    string CurSelectMeshGroup = get<Tuple_CharPtr>(m_arrMeshGroupName[m_SelectMeshGroupIndex]);
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

void CWindow_Map::Confirm_Data()
{
    m_pCurSelectGameObject = nullptr;
    m_pObjTransform = nullptr;
    m_pCurSelectData = nullptr;
    m_eControlType = CONTROL_MOVE;
    m_fTickPerScalingSpeed = 1.f;
    m_fTickPerRotSpeed = 1.f;
    m_fTickPerMoveSpeed = 1.f;
}

void CWindow_Map::Show_ObjectData()
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

void CWindow_Map::Set_ObjectSpeed()
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

void CWindow_Map::Select_DataControlFlag()
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
    if (KEY(V, TAP))
    {
        m_bObjectPick = !m_bObjectPick;
    }
}

void CWindow_Map::Control_Object()
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


void CWindow_Map::Scaling_Object()
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

void CWindow_Map::Rotate_Object()
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

void CWindow_Map::Position_Object()
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

void CWindow_Map::Place_Object()
{
    if (nullptr == m_pObjTransform)
        return;
    m_pObjTransform->Set_World(WORLD_POS, m_OutPos);
}

void CWindow_Map::Change_Object_UpDir()
{
    if (nullptr == m_pObjTransform)
        return;
    
    _vector xNormal = -m_OutNorm.XMLoad();
    _vector xRight = XMVector3Cross( xNormal, XMVectorSet(0.f, 0.f, 1.f, 0.f));
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

void CWindow_Map::Update_Data()
{
    if (nullptr == m_pCurSelectData)
        return;
    if (m_pCurSelectGameObject)
    {
        m_pCurSelectData->vScale = m_pCurSelectGameObject->Get_Transform()->Get_Scale();
        m_pCurSelectData->ObjectStateMatrix = m_pCurSelectGameObject->Get_Transform()->Get_WorldMatrix();
    }
}





#pragma region static value 라이트 컨트롤러 
static int LightGroupIndex = 0;
static string SelectLightGroup = "";
static char szLightGroupName[MAXCHAR] = "";
static char szLightName[MAXCHAR] = "";
static int LightTypeIndex = 0;
static float LightPos[3] = { 0.f };
static float LightDir[3] = { 0.f };
static float LightRange = 0.f;
static float LightDiffuse[3] = { 0.f };
static float LightAmbient[3] = { 0.f };
static float LightSpecular[3] = { 0.f };
static bool LightFlagOpt[4] = { false };
#pragma endregion

#pragma region 라이트 컨트롤함수
void CWindow_Map::Func_LightControl()
{
    ImGui::Text("Light GroupList");
    //1. 빛 그룹 리스트 콤보
    if (!m_arrLightGroupCombo.empty())
    {
        SelectLightGroup = get<Tuple_CharPtr>(m_arrLightGroupCombo[LightGroupIndex]);
    }
    Make_Combo("##Light_GroupList", m_arrLightGroupCombo, &LightGroupIndex, bind(&CWindow_Map::EmptyFunction, this));
    DebugData("Debug_SelectLightGroup", SaveFilePath);

    if (ImGui::InputText("##Light_GroupName", szLightGroupName, sizeof(char) * MAXCHAR))
    {

    }
    if (ImGui::Button("Add Group"))
    {
        Add_LightGroupList(szLightGroupName);
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete Group"))
    {
        Delete_LightGroupList(szLightGroupName);
    }

    //해당 그룹 소속 빛 정보 리스트
    //빛 추가
    if (ImGui::Button("Add Light"))
    {
        Add_Light(szLightName);
    }
    ImGui::SameLine();
    //빛 제거
    if (ImGui::Button("Delete Light"))
    {
        Delete_Light(szLightName);
    }



    if (ImGui::CollapsingHeader("Light Default", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //2. 빛 종류
        ImGui::Text("Light Type");
        Make_Combo("##Light_TypeList", m_arrLightTypeCombo, &LightTypeIndex, bind(&CWindow_Map::EmptyFunction, this));
        ImGui::Spacing();
        //3. 위치
        ImGui::Text("Light Pos");
        if (ImGui::InputFloat3("##Light_Pos", LightPos, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
        }
        ImGui::Spacing();

        //4. 방향
        ImGui::Text("Light Dir");
        if (ImGui::InputFloat3("##Light_Dir", LightDir, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
        {
        }
        ImGui::Spacing();

        //5. 범위
        ImGui::Text("Light Range");
        if (0.f >= LightRange)
        {
            LightRange = 0.f;
        }
        if (ImGui::DragFloat("##Light_Range", &LightRange, 0.1f, 0.0f, 0.f, "%.1f"))
        {

        }
        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Light Color", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //6. 디퓨즈
        ImGui::Text("Light Diffuse");
        if (ImGui::ColorEdit3("##Light_Diffuse", LightDiffuse))
        {
        }
        ImGui::Spacing();

        //7. 엠비언트
        ImGui::Text("Light Ambient");
        if (ImGui::ColorEdit3("##Light_Ambient", LightAmbient))
        {
        }
        ImGui::Spacing();

        //8. 스펙큘러
        ImGui::Text("Light Specular");
        if (ImGui::ColorEdit3("##Light_Specular", LightSpecular))
        {
        }
        ImGui::Spacing();
    }

    //9. 타입플래그
    if (ImGui::CollapsingHeader("Light Option", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        if (ImGui::Checkbox("LightOpt_1", &LightFlagOpt[0]))
        {
        }
        if (ImGui::Checkbox("LightOpt_2", &LightFlagOpt[1]))
        {
        }
        if (ImGui::Checkbox("LightOpt_3", &LightFlagOpt[2]))
        {
        }
        if (ImGui::Checkbox("LightOpt_4", &LightFlagOpt[3]))
        {
        }
    }
}

void CWindow_Map::Ready_LightGroup()
{

}
void CWindow_Map::Ready_LightType()
{
    char* pLightType = new char[260];
    ZeroMemory(pLightType, sizeof(char) * 260);
    memcpy_s(pLightType, sizeof(char) * 260, "DIR", sizeof(char) * 260);
    m_arrLightTypeCombo.push_back(make_tuple(pLightType, false));

    pLightType = new char[260];
    ZeroMemory(pLightType, sizeof(char) * 260);
    memcpy_s(pLightType, sizeof(char) * 260, "POINT", sizeof(char) * 260);
    m_arrLightTypeCombo.push_back(make_tuple(pLightType, false));
}

void CWindow_Map::Add_LightGroupList(char* pLightGroupName)
{
    DataComboArr::iterator LightGroupIter = find_if(m_arrLightGroupCombo.begin(), m_arrLightGroupCombo.end(), [&pLightGroupName](DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pLightGroupName, get<Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (LightGroupIter == m_arrLightGroupCombo.end())
    {
        char* pLightGroup = new char[260];
        ZeroMemory(pLightGroup, sizeof(char) * 260);
        memcpy_s(pLightGroup, sizeof(char) * 260, pLightGroupName, sizeof(char) * 260);
        m_arrLightGroupCombo.push_back(make_tuple(pLightGroup, false));
    }


}
void CWindow_Map::Delete_LightGroupList(char* pLightGroupName)
{
    DataComboArr::iterator LightGroupIter = find_if(m_arrLightGroupCombo.begin(), m_arrLightGroupCombo.end(), [&pLightGroupName](DataComboArr::value_type& Value)
        {
            if (0 == strcmp(pLightGroupName, get<Tuple_CharPtr>(Value)))
                return true;
            else
                return false;
        });

    if (LightGroupIter != m_arrLightGroupCombo.end())
    {
        char* pGroupName = get<Tuple_CharPtr>((*LightGroupIter));
        m_arrLightGroupCombo.erase(LightGroupIter);
        Safe_Delete_Array(pGroupName);
    }
}

void CWindow_Map::Add_Light(char* pLightName)
{
}
void CWindow_Map::Delete_Light(char* pLightName)
{
}
#pragma endregion

#pragma region 파일입출력 함수
void CWindow_Map::Save_MapData(string BasePath, string SaveName)
{
    MAPDATA tMapData;
    ofstream	writeFile;
    if (FAILED(tMapData.SaveData(writeFile, BasePath, SaveName)))
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        return;
    }

    //터레인 데이터 저장
    string TerrainPath = BasePath;
    TerrainPath += "TerrainData/";
    Save_TerrainData(TerrainPath, SaveName);

    //오브젝트 데이터 저장
    string GroupPath = BasePath;
    GroupPath += "ObjectData/";
    Save_ObjectGroup(GroupPath, SaveName);

    //내비 데이터 저장
    string NavPath = BasePath;
    NavPath += "NavData/";
    Save_NavGroup(NavPath, SaveName);

    //라이트 데이터 저장
    string LightPath = BasePath;
    LightPath += "LightData/";
    Save_LightGroup(LightPath, SaveName);

}
void CWindow_Map::Load_MapData(string FilePath)
{
    MAPDATA tMapData;
    ifstream	readFile;
    if (FAILED(tMapData.LoadData(readFile, FilePath)))
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        return;
    }

    Load_TerrainData(CFunctor::To_String(tMapData.TerrainDataPath));
    Load_ObjectGroup(CFunctor::To_String(tMapData.ObjectDataPath));
    Load_NavGroup(CFunctor::To_String(tMapData.NavDataPath));
    Load_LightGroup(CFunctor::To_String(tMapData.LightDataPath));
}

void CWindow_Map::Save_TerrainData(string BasePath, string SaveName)
{
    if (nullptr == m_pCurTerrain)
    {
        Call_MsgBox(TEXT("현재 저장할 터레인 없음"));
        return;
    }
    //지형의 정점 정보들 저장
    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += "_Terrain.dat";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }


    char* szTexturePath = new char[MAX_PATH];
    strcpy_s(szTexturePath, sizeof(char) * MAX_PATH, CFunctor::To_String(m_CurTerrainData.strTileTexturePath).c_str());
    _uint iTilePathPathLength = strlen(szTexturePath) + 1;


    writeFile.write((char*)&iTilePathPathLength, sizeof(_uint));
    writeFile.write((char*)szTexturePath, sizeof(char) * iTilePathPathLength);


    _uint VerticesX = m_CurTerrainData.iNumVerticesX;
    _uint VerticesZ = m_CurTerrainData.iNumVerticesZ;
    writeFile.write((char*)&VerticesX, sizeof(_uint));
    writeFile.write((char*)&VerticesZ, sizeof(_uint));

    _uint iNumVertices = VerticesX * VerticesZ;

    _float3* Vertices = m_CurTerrainData.pCurTerrainVertPos;
    writeFile.write((char*)Vertices, sizeof(_float3) * iNumVertices);

    writeFile.close();

    Safe_Delete_Array(szTexturePath);
}

void CWindow_Map::Load_TerrainData(string FilePath)
{
    Disable_DefaultTerrain();
    if (nullptr != m_pCurTerrain)
    {
        DELETE_GAMEOBJECT(m_pCurTerrain);
        m_pCurTerrain = nullptr;
    }
    CDrawable_Terrain* pTerrain = nullptr;
    wstring strPath = CFunctor::To_Wstring(FilePath);
    pTerrain = CDrawable_Terrain::Create(strPath.c_str());
    if (nullptr == pTerrain)
        assert(0);
    CREATE_GAMEOBJECT(pTerrain, GROUP_DEFAULT);
    m_pCurTerrain = pTerrain;
    MTT_DATA::Terrain_TUPLE tupleData = m_pCurTerrain->Get_TerrainData();
    m_CurTerrainData.Make_Data(tupleData);
    //지형의 정점을 통한 터레인 수정
}

void CWindow_Map::Save_ObjectGroup(string BasePath, string SaveName)
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
        char* pGroupName = get<Tuple_CharPtr>(m_arrMeshGroupName[i]);
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
    //Call_MsgBox(L"Save 성공");
}
void CWindow_Map::Load_ObjectGroup(string FilePath)
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
void CWindow_Map::Save_NavGroup(string BasePath, string SaveName)
{
}
void CWindow_Map::Load_NavGroup(string FilePath)
{
}
void CWindow_Map::Save_LightGroup(string BasePath, string SaveName)
{
}
void CWindow_Map::Load_LightGroup(string FilePath)
{
}
#pragma endregion


#pragma region 터레인 컨트롤 함수
void CWindow_Map::Ready_TerrainBrushType()
{
    Add_Brush("Linear");
    
    Add_Brush("QuadIn");
    Add_Brush("QuadOut");
    Add_Brush("QuadInOut");
    
    Add_Brush("CubicIn");
    Add_Brush("CubicOut");
    Add_Brush("CubicInOut");
    
    Add_Brush("QuarticIn");
    Add_Brush("QuarticOut");
    Add_Brush("QuarticInOut");
    
    Add_Brush("QuinticIn");
    Add_Brush("QuinticOut");
    Add_Brush("QuinticInOut");

    Add_Brush("SinIn");
    Add_Brush("sinfOut");
    Add_Brush("sinfInOut");

    Add_Brush("ExpoIn");
    Add_Brush("ExpoOut");
    Add_Brush("ExpoInOut");

    Add_Brush("CircularIn");
    Add_Brush("CircularOut");
    Add_Brush("CircularInOut");

    Add_Brush("ElasticEaseIn");
    Add_Brush("ElasticEaseOut");
    Add_Brush("ElasticEaseInOut");

    Add_Brush("BounceEaseIn");
    Add_Brush("BounceEaseOut");

}
HRESULT CWindow_Map::Disable_DefaultTerrain()
{
    if (nullptr == m_pDefaultTerrain)
    {
        m_pCurObjectList = &(GAMEINSTANCE->Get_ObjGroup(GROUP_DEFAULT));
        CTerrain* pDefaultTerrain = nullptr;
        for (list<CGameObject*>::value_type& Value : (*m_pCurObjectList))
        {
            m_pDefaultTerrain = dynamic_cast<CTerrain*>(Value);
            if (nullptr != m_pDefaultTerrain)
                break;
        }
        m_pCurObjectList = nullptr;
    }
    m_pDefaultTerrain->Set_Enable(false);
    return S_OK;
}

void CWindow_Map::Func_TerrainControl()
{
    string strBrushOnOffText;
    if (!m_bTerrainPick)
    {
        strBrushOnOffText = "On Brush";
    }
    else
        strBrushOnOffText = "Off Brush";


    if (ImGui::Button(strBrushOnOffText.c_str()))
    {
        m_bObjectPick = false;
        m_bTerrainPick = !m_bTerrainPick;
    }
    ImGui::Spacing();

    _bool bPicked = (!m_bObjectPick);
    ImGui::Text("CurPickedIndex(Ready - Only)");
    ImGui::InputInt3("##CurPickedIndex", (int*)&m_i3PickedIndex, ImGuiInputTextFlags_ReadOnly);
    ImGui::Spacing();

    if (!bPicked)
        return;

    if (ImGui::Checkbox("Activate WireMode", &m_TerrainWireFrame))
    {
        if(m_TerrainWireFrame)
            m_pCurTerrain->Change_ShaderPass(VTXNOR_PASS_NAVIGATION);
        else
            m_pCurTerrain->Change_ShaderPass();

    }

    if (ImGui::Button("Activate VertsNormal"))
    {
        m_pCurTerrain->Update_Normal();
    }
    ImGui::Text("Brush Type");
    Make_Combo("##Brush Combo", m_arrBrushType, &m_iCurSelectTerrainBrush, bind(&CWindow_Map::EmptyFunction, this));
    ImGui::Spacing();

    ImGui::Text("Brush Size");
    ImGui::DragFloat("##BrushSize", &m_fBrushSize, 0.01f, 0.f, 100.f, "%.3f");
    ImGui::Spacing();

    ImGui::Text("Height Increase Value");
    ImGui::DragFloat("##HeightIncrease", &m_fHeightRatio, 0.1f, 0.f, 10.f, "%.3f");
    ImGui::Spacing();

}
void CWindow_Map::Generate_Terrain()
{
    Disable_DefaultTerrain();
    if (nullptr != m_pCurTerrain)
    {
        DELETE_GAMEOBJECT(m_pCurTerrain);
        m_pCurTerrain = nullptr;
    }
    CDrawable_Terrain* pTerrain = CDrawable_Terrain::Create(m_CurTerrainData.iNumVerticesX, m_CurTerrainData.iNumVerticesZ);
    CREATE_GAMEOBJECT(pTerrain, GROUP_DEFAULT);
    m_pCurTerrain = pTerrain;
    MTT_DATA::Terrain_TUPLE TupleData = m_pCurTerrain->Get_TerrainData();
    m_CurTerrainData.Make_Data(TupleData);
}
void CWindow_Map::Increase_Height()
{
    _float4(*func)(_float);
    if (!m_bTerrainPick)
        return;
    if (nullptr != m_pCurTerrain)
    {
        _float3* Verts = m_pCurTerrain->Get_TerrainVerticesPos();
        list<_float3*> VertsList = Select_Vertices();
        for (list<_float3*>::value_type& Value : VertsList)
        {
            (*Value) = Easing_Vertices(Value);
        }
        VertsList.clear();
        //Verts[m_i3PickedIndex._1] = Easing_Vertices(&Verts[m_i3PickedIndex._1]);
        //Verts[m_i3PickedIndex._2] = Easing_Vertices(&Verts[m_i3PickedIndex._2]);
        //Verts[m_i3PickedIndex._3] = Easing_Vertices(&Verts[m_i3PickedIndex._3]);
        m_pCurTerrain->Update_Vertices();
    }
}
void CWindow_Map::Add_Brush(const char* BrushName)
{
    char* szTypeName = new char[MAXCHAR];
    memcpy_s(szTypeName, sizeof(char) * MAXCHAR, BrushName, sizeof(char) * (_int(strlen(BrushName)) + 1));
    m_arrBrushType.push_back(make_tuple(szTypeName, false));

}
list<_float3*> CWindow_Map::Select_Vertices()
{
    _float3* Verts = m_pCurTerrain->Get_TerrainVerticesPos();
    _float fRount = m_fBrushSize * 0.5f;
    _float4 Center = m_OutPos;
    _int VertXNums = m_pCurTerrain->Get_TerrainVerticesX();
    _int VertZNums = m_pCurTerrain->Get_TerrainVerticesZ();

    list<_float3*> VertsList;
    _int IndexWidStart = _int((m_OutPos.x /*+ TerrainPos*/) - fRount);
    IndexWidStart = (0 > IndexWidStart) ? 0 : IndexWidStart;
    IndexWidStart = (VertXNums - 1 < IndexWidStart) ? VertXNums - 1 : IndexWidStart;

    _int IndexWidEnd = _int((m_OutPos.x /*+ TerrainPos*/) + fRount);
    IndexWidEnd = (0 > IndexWidEnd) ? 0 : IndexWidEnd;
    IndexWidEnd = (VertXNums - 1 < IndexWidEnd) ? VertXNums - 1 : IndexWidEnd;

    _int IndexHeightStart = _int((m_OutPos.z /*+ TerrainPos*/) - fRount);
    IndexHeightStart = (0 > IndexHeightStart) ? 0 : IndexHeightStart;
    IndexHeightStart = (VertZNums - 1 < IndexHeightStart) ? VertZNums - 1 : IndexHeightStart;
    
    _int IndexHeightEnd = _int((m_OutPos.z /*+ TerrainPos*/) + fRount);
    IndexHeightEnd = (0 > IndexHeightEnd) ? 0 : IndexHeightEnd;
    IndexHeightEnd = (VertZNums - 1 < IndexHeightEnd) ? VertZNums - 1 : IndexHeightEnd;


    for (_int i = IndexHeightStart; i <= IndexHeightEnd; ++i)
    {
        for (_int j = IndexWidStart; j <= IndexWidEnd; ++j)
        {
            _int Index = i * VertXNums + j;
            if(Check_InBrush(&Verts[Index]))
                VertsList.push_back(&Verts[Index]);
        }
    }


    return VertsList;
}
_bool CWindow_Map::Check_InBrush(_float3* CompVert)
{
    _float fRount = m_fBrushSize * 0.5f;
    _float4 Center = m_OutPos;
    _float XPos = (*CompVert).x;
    _float ZPos = (*CompVert).z;

    //x - a
    _float XDiff = XPos - m_OutPos.x;
    //z - b
    _float ZDiff = ZPos - m_OutPos.z;

    //(x - a)^2 + (z - b)^2  <= r^2 --> 원 내부에 점이 존재
    if (XDiff * XDiff + ZDiff * ZDiff <= fRount * fRount)
        return true;
    else
        return false;
}
_float3 CWindow_Map::Easing_Vertices(_float3* pVertPos)
{
    _float4 vVertPos 
        = _float4(
            pVertPos->x, 
            pVertPos->y, 
            pVertPos->z, 
            1.f);

    _float VertLength = XMVectorGetX(XMVector3Length(vVertPos.XMLoad() - m_OutPos.XMLoad()));
    _float fVertRatio = VertLength / (m_fBrushSize*0.5f);
    _float4 vStartPos = vVertPos;
    _float4 vTargetPos = _float4(vVertPos.x, vVertPos.y + m_fHeightRatio, vVertPos.z, vVertPos.w);
    _float4 vOut;
    CGame_Manager_MJ::Get_Instance()->Easing_Vert(0, &vOut, vStartPos, vTargetPos, 1- fVertRatio);
    _float3 vReturn = _float3(vOut.x, vOut.y, vOut.z);
    return vReturn;
}
_bool CWindow_Map::Calculate_Pick()
{
    _bool bPicked = false;
    if (m_bHoverWindow)
        return bPicked;
    if (nullptr == m_pCurTerrain)
        return bPicked;

    if (KEY(LBUTTON, HOLD))
    {
        if (GAMEINSTANCE->Is_Picked_Mesh(m_pCurTerrain->Get_MeshTerrain(), &m_i3PickedIndex, &m_OutPos, &m_OutNorm))
        {
            if (m_bObjectPick)
            {
                Place_Object();
                Change_Object_UpDir();
            }
            else
                Increase_Height();
            bPicked = true;
        }
    }
    return bPicked;
}
#pragma endregion


#pragma region 기타 기능 함수
void CWindow_Map::Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func)
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
    m_bHoverWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

    func(*this);
    ImGui::End();
}
_bool CWindow_Map::Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction)
{
    _bool bReturn = false;
    if (szDataArr.empty())
        return bReturn;
    if (ImGui::BeginCombo(szLabel, get<Tuple_CharPtr>(szDataArr[(*pCurIndex)])))
    {
        for (_uint i = 0; i < szDataArr.size(); ++i)
        {
            bReturn = get<Tuple_Bool>(szDataArr[i]);
            if (ImGui::Selectable(get<Tuple_CharPtr>(szDataArr[i]), &get<Tuple_Bool>(szDataArr[i])))
            {
                get<Tuple_Bool>(szDataArr[(*pCurIndex)]) = false;
                (*pCurIndex) = i;
                get<Tuple_Bool>(szDataArr[(*pCurIndex)]) = true;
                SelectFunction();
                bReturn = true;

            }
            if (bReturn)
                ImGui::SetItemDefaultFocus();

        }
        ImGui::EndCombo();
    }
    return bReturn;
}
void CWindow_Map::Clear_TupleData(vector<tuple<char*, bool>>& ArrData)
{
    for (_int i = 0; i < _int(ArrData.size()); ++i)
    {
        Safe_Delete_Array(get<Tuple_CharPtr>(ArrData[i]));
    }
    ArrData.clear();
}
void CWindow_Map::DebugData(const char* szTitleName, string& strData)
{
    char szTitle[MAXCHAR] = "";
    strcat_s(szTitle, szTitleName);
    strcat_s(szTitle, " : ");
    ImGui::Text(szTitle);
    ImGui::SameLine();
    char szSaveFilePath[MAX_PATH] = "";
    strcpy_s(szSaveFilePath, strData.c_str());
    strcat_s(szSaveFilePath, "\0");
    ImGui::Text(szSaveFilePath);
}
list<string> CWindow_Map::Read_Folder_ToStringList(const char* pFolderPath)
{
    list<string> PathList;
    for (filesystem::directory_iterator FileIter(pFolderPath);
        FileIter != filesystem::end(FileIter); ++FileIter)
    {
        const filesystem::directory_entry& entry = *FileIter;

        wstring wstrPath = entry.path().relative_path();
        string strFullPath;
        strFullPath.assign(wstrPath.begin(), wstrPath.end());

        _int iFind = (_int)strFullPath.rfind("\\") + 1;
        string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);
        if (!entry.is_directory())
        {
            _int iFindExt = (int)strFileName.rfind(".") + 1;
            string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);
            if (strExtName == "MapData")
            {
                _int iSlash = strFileName.rfind("/") + 1;
                string strOutputName = strFileName.substr(iSlash, strFileName.length() + 1);
                PathList.push_back(CutOut_Ext(strOutputName, strExtName));
            }
        }
    }
    return PathList;
}
void CWindow_Map::Read_Folder_ForTree(const char* pFolderPath, TREE_DATA& tRootTree)
{
    for (filesystem::directory_iterator FileIter(pFolderPath);
        FileIter != filesystem::end(FileIter); ++FileIter)
    {
        const filesystem::directory_entry& entry = *FileIter;

        wstring wstrPath = entry.path().relative_path();
        string strFullPath;
        strFullPath.assign(wstrPath.begin(), wstrPath.end());

        _int iFind = (_int)strFullPath.rfind("\\") + 1;
        string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

        TREE_DATA	tTreeData;
        tTreeData.strFullPath = strFullPath;
        tTreeData.strFileName = strFileName;
        tTreeData.strFolderPath = pFolderPath;
        if (entry.is_directory())
        {
            Read_Folder_ForTree(strFullPath.c_str(), tTreeData);
        }
        else
        {
            _int iFindExt = (int)strFileName.rfind(".") + 1;
            string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);

            if (strExtName == "dat")
                continue;
        }

        tRootTree.vecChildren.push_back(tTreeData);

    }
}
void CWindow_Map::Show_TreeData(TREE_DATA& tTree)
{
    if (!tTree.vecChildren.empty())
    {
        if (ImGui::TreeNode(tTree.strFileName.c_str()))
        {
            for (auto& tTreeData : tTree.vecChildren)
            {
                Show_TreeData(tTreeData);
            }

            ImGui::TreePop();
        }

    }
    else
    {
        _bool bSelected = false;

        if (m_CurSelectedMeshFilePath == tTree.strFullPath)
        {
            bSelected = true;
        }
        for (auto& strPath : m_vecSelectedMeshFilePath)
        {
            if (strPath == tTree.strFullPath)
                bSelected = true;
        }

        if (ImGui::Selectable(tTree.strFileName.c_str(), bSelected))
        {
            m_vecSelectedMeshFilePath.clear();
            m_vecSelectedMeshName.clear();

            string prevFilePath = m_CurSelectedMeshFilePath;
            m_CurSelectedMeshFilePath = tTree.strFullPath;
            string Ext(".fbx");
            m_CurSelectedMeshName = CutOut_Ext(tTree.strFileName, Ext);
            if (KEY(LSHIFT, HOLD))
            {
                // 1. 새로운 이터레이터로 prevPath 위치로 가야함
                filesystem::directory_iterator shiftFileIter(tTree.strFolderPath);
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
}
string CWindow_Map::CutOut_Ext(string& Origin, string& Ext)
{
    string strReturn = Origin;
    size_t ExtLength = Ext.size() + 1;
    strReturn = strReturn.substr(0, strReturn.size() - ExtLength);
    return strReturn;
}

#pragma endregion

#pragma region MAPDATA 멤버함수
void CWindow_Map::MAPDATA::Initialize()
{
    TerrainDataPath = wstring();
    ObjectDataPath = wstring();
    NavDataPath = wstring();
    LightDataPath = wstring();
}
void CWindow_Map::MAPDATA::Make_Path(string BasePath, string DataName)
{
    Initialize();
    TerrainDataPath = CFunctor::To_Wstring(BasePath);
    TerrainDataPath += TEXT("TerrainData/");
    TerrainDataPath += CFunctor::To_Wstring(DataName);
    TerrainDataPath += TEXT("_Terrain.dat");

    ObjectDataPath = CFunctor::To_Wstring(BasePath);
    ObjectDataPath += TEXT("ObjectData/");
    ObjectDataPath += CFunctor::To_Wstring(DataName);
    ObjectDataPath += TEXT(".GroupData");

    NavDataPath = CFunctor::To_Wstring(BasePath);
    NavDataPath += TEXT("NavData/");
    NavDataPath += CFunctor::To_Wstring(DataName);
    NavDataPath += TEXT(".NavData");

    LightDataPath = CFunctor::To_Wstring(BasePath);
    LightDataPath += TEXT("LightData/");
    LightDataPath += CFunctor::To_Wstring(DataName);
    LightDataPath += TEXT(".LightData");
}
HRESULT CWindow_Map::MAPDATA::SaveData(ofstream& rhsWriteFile, string BasePath, string DataName)
{
    Initialize();
    Make_Path(BasePath, DataName);

    string SaveFullPath = BasePath;
    SaveFullPath += DataName;
    SaveFullPath += ".MapData";
    rhsWriteFile.open(SaveFullPath, ios::binary);
    if (!rhsWriteFile.is_open())
    {
        //Call_MsgBox(L"SSave 실패 ??!?!");
        return E_FAIL;
    }

    try {
        if (FAILED(SavePath(rhsWriteFile, TerrainDataPath)))
            throw TerrainDataPath;
        if (FAILED(SavePath(rhsWriteFile, ObjectDataPath)))
            throw ObjectDataPath;
        if (FAILED(SavePath(rhsWriteFile, NavDataPath)))
            throw NavDataPath;
        if (FAILED(SavePath(rhsWriteFile, LightDataPath)))
            throw LightDataPath;
    }
    catch (wstring ErrPath)
    {
        wstring strErrMsg = TEXT("Fail to Save : ") + ErrPath;
        Call_MsgBox(strErrMsg.c_str());
        rhsWriteFile.close();
        return E_FAIL;
    }

    rhsWriteFile.close();
    return S_OK;
}
HRESULT CWindow_Map::MAPDATA::LoadData(ifstream& rhsReadFile, string FilePath)
{
    Initialize();

    string LoadFullPath = FilePath;
    rhsReadFile.open(LoadFullPath, ios::binary);
    if (!rhsReadFile.is_open())
    {
        return E_FAIL;
    }
    try {
        if (FAILED(LoadPath(rhsReadFile, TerrainDataPath)))
            throw TerrainDataPath;
        if (FAILED(LoadPath(rhsReadFile, ObjectDataPath)))
            throw ObjectDataPath;
        if (FAILED(LoadPath(rhsReadFile, NavDataPath)))
            throw NavDataPath;
        if (FAILED(LoadPath(rhsReadFile, LightDataPath)))
            throw LightDataPath;
    }
    catch (wstring ErrPath)
    {
        wstring strErrMsg = TEXT("Fail to Load : ") + ErrPath;
        Call_MsgBox(strErrMsg.c_str());
        rhsReadFile.close();
        return E_FAIL;
    }

    return S_OK;
}
HRESULT CWindow_Map::MAPDATA::SavePath(ofstream& rhsWriteFile, wstring strPath)
{
    string strDataPath = CFunctor::To_String(strPath);
    _int DataPathLength = _int(strDataPath.size()) + 1;
    rhsWriteFile.write((char*)&DataPathLength, sizeof(_int));
    char* szDataPath = new char[DataPathLength];
    strcpy_s(szDataPath, sizeof(char) * DataPathLength, strDataPath.c_str());
    rhsWriteFile.write(szDataPath, sizeof(char) * DataPathLength);

    Safe_Delete_Array(szDataPath);
    if (nullptr != szDataPath)
        return E_FAIL;

    return S_OK;
}
HRESULT CWindow_Map::MAPDATA::LoadPath(ifstream& rhsReadFile, wstring& strPath)
{
    _int DataPathLength = 0;
    rhsReadFile.read((char*)&DataPathLength, sizeof(_int));

    if (0 > DataPathLength)
        return E_FAIL;

    char* szDataPath = new char[DataPathLength];
    rhsReadFile.read(szDataPath, sizeof(char) * DataPathLength);

    string strDataPath = szDataPath;
    Safe_Delete_Array(szDataPath);

    if (nullptr != szDataPath)
        return E_FAIL;

    strPath = CFunctor::To_Wstring(strDataPath);

    return S_OK;
}
#pragma endregion

#pragma region MTO_DATA 멤버함수
void CWindow_Map::tagMapToolObjectData::Initialize()
{
    strMeshName = wstring();
    //strGroupName = wstring();
    strMeshPath = wstring();
    ObjectStateMatrix.Identity();
    vScale = _float4(1.f, 1.f, 1.f, 0.f);
    byteLightFlag = 0;
}
#pragma endregion

void CWindow_Map::tagMapToolTerrainData::Initialize()
{
    strTileTexturePath = wstring();
    iNumVerticesX = 0;
    iNumVerticesZ = 0;
    pCurTerrainVertPos = nullptr;
}

void CWindow_Map::tagMapToolTerrainData::Make_Data(tagMapToolTerrainData::Terrain_TUPLE& tTerrainData)
{
    Initialize();
    strTileTexturePath = get<Tuple_TileTexture>(tTerrainData);
    iNumVerticesX = get<Tuple_VerticesX>(tTerrainData);
    iNumVerticesZ = get<Tuple_VerticesZ>(tTerrainData);
    pCurTerrainVertPos = get<Tuple_TerrainPosPtr>(tTerrainData);
}
