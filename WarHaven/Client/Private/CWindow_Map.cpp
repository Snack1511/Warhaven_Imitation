#include "stdafx.h"
#include "CWindow_Map.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Functor.h"
#include "CStructure.h"
CWindow_Map::CWindow_Map()
{
}

CWindow_Map::~CWindow_Map()
{
    Clear_TupleData(m_arrSaveFilesCombo);
    Clear_TupleData(m_arrObjectGroupId);
    Clear_TupleData(m_arrMeshGroupName);
    Clear_TupleData(m_arrLightTypeCombo);
    Clear_TupleData(m_arrLightGroupCombo);
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
    Read_Folder("../bin/resources/meshes/Map", m_MeshRootNode);

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
	return S_OK;
}

void CWindow_Map::Tick()
{

}

static char szSaveNameBuf[MAXCHAR] = "";
static string SaveFilePath = "";
static string LoadFilePath = "";

HRESULT CWindow_Map::Render()
{
    ImVec2 vPannelSize = ImVec2(300.f, 300.f);
    ImVec2 vDataControlPos = ImVec2(vPannelSize.x, 0.f);
    ImVec2 vLightControlPos = ImVec2(vPannelSize.x, vPannelSize.y);

    if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::Text("MapTool");

    //파일 컨트롤
    Func_FileControl();

    //데이타 컨트롤
    Create_SubWindow("Data_Controller", vDataControlPos, vPannelSize, bind(&CWindow_Map::Func_DataControl, this));

    //라이트 컨트롤
    Create_SubWindow("Light_Controller", vLightControlPos, vPannelSize, bind(&CWindow_Map::Func_LightControl, this));
    __super::End();


	return S_OK;
}

static char szMeshGroupNameBuf[MAXCHAR] = "";

void CWindow_Map::Func_FileControl()
{
    ImVec2 ButtonSize(60.f, 20.f);

    //1. 선택 파일 콤보박스
#pragma region 선택 파일 콤보박스
    SetUp_FilePath(LoadFilePath, get<Tuple_CharPtr>(m_arrSaveFilesCombo[SaveFileIndex]));
    Make_Combo("##Save_Files", m_arrSaveFilesCombo, &SaveFileIndex, bind(&CWindow_Map::EmptyFunction, this));
    DebugData("Debug_LoadPath", LoadFilePath);
#pragma endregion


    //2. 파일명 입력 창
#pragma region 파일명 입력
    SetUp_FilePath(SaveFilePath, szSaveNameBuf);
    ImGui::InputText("##SaveName", szSaveNameBuf, sizeof(char) * MAXCHAR);
    DebugData("Debug_SavePath", SaveFilePath);
#pragma endregion


    //6. 세이브 / 로드 버튼
    if (ImGui::Button("Save", ButtonSize))
    {
        //SaveFilePath
        //세이브
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", ButtonSize))
    {
        //LoadFilePath
        //로드 버튼
    }
    ImGui::Spacing();
    ImGui::Text("Select PickingGroupID");
    SetUp_CurObjectlist();
    if (Make_Combo("##ObjectGroupID", m_arrObjectGroupId, &m_SelectObjectGroupIDIndex, bind(&CWindow_Map::EmptyFunction, this)))
    {
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
        ////4. 오브젝트 프로토타입 리스트
    //ImGui::Text("Objects");
    //if (Make_Combo("##ObjectPrototypeList", m_arrObjectPrototypeName, &SelectObjectPrototypeIndex, bind(&CWindow_Map::EmptyFunction, this)))
    //{
    //    //strcpy_s(szObjectGroupBuf, get<Tuple_CharPtr>(m_arrObjectPrototypeName[SelectObjectPrototypeIndex]));
    //}
    //ImGui::Spacing();

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
                        list<MTO_DATA>& CurGroupData = m_ObjectDataGroupMap[Hashnum];
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
    string Path = m_strPath;
    Path += "*";
    if (m_arrSaveFilesCombo.empty())
    {
        WIN32_FIND_DATAA FileData;
        HANDLE hFind = FindFirstFileA(Path.c_str(), &FileData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            DWORD ERR = GetLastError();
            Call_MsgBox(TEXT("안읽힘;;"));
            assert(ERR);

        }
        while (FindNextFileA(hFind, &FileData))
        {
            //. .. 는 시스템폴더라 읽을 필요가 없음
            if (!strcmp(FileData.cFileName, ".")
                || !strcmp(FileData.cFileName, ".."))
                continue;
            char* pFileName = new char[260];
            memcpy_s(pFileName, sizeof(char) * 260, FileData.cFileName, sizeof(char) * 260);
            m_arrSaveFilesCombo.push_back(make_tuple(pFileName, false));
        }
        FindClose(hFind);
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

void CWindow_Map::Ready_MeshGroupName()
{
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
            m_ObjectDataGroupMap.emplace(HashNum, list<MTO_DATA>());
        }
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
    }
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
    }
}

void CWindow_Map::Add_Object(string MeshGroup, string Meshpath, string MeshName)
{
    vector<CGameObject*>* pObjectList = nullptr;
    list<MTO_DATA>* pDataList = nullptr;
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
        m_ObjectDataGroupMap.emplace(HashNum, list<MTO_DATA>());
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
    tData.strMeshName = strName +wstring(TEXT("_")) + to_wstring(m_ObjNameCallStack[NameHashNum]++);
    tData.strGroupName = CFunctor::To_Wstring(MeshGroup);
    tData.strMeshPath = CFunctor::To_Wstring(Meshpath);
    tData.ObjectStateMatrix.Identity();

    CStructure* pGameObject = CStructure::Create(tData.strMeshPath);
    if (nullptr == pGameObject)
        assert(0);
    pGameObject->Initialize();
    CREATE_GAMEOBJECT(pGameObject, GROUP_DECORATION);

    pObjectList->push_back(pGameObject);
    pDataList->push_back(tData);

}

void CWindow_Map::Delete_Object(string MeshName, vector<CGameObject*>& ObjList, list<MTO_DATA>& DataList)
{
    if (ObjList.size() != DataList.size())
        assert(0);
    if (ObjList.empty())
        return;
    wstring CmpMeshName = CFunctor::To_Wstring(MeshName);
    _int ObjectIndex = 0;
    list<MTO_DATA>::iterator DataListIter = DataList.begin();
    for (;DataListIter != DataList.end(); ++DataListIter)
    {
        if ((*DataListIter).strMeshName == CmpMeshName)
            break;
        ObjectIndex++;
    }
    if (DataListIter == DataList.end())
        return;
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
}

void CWindow_Map::Update_FileArray()
{
    string Path = m_strPath;
    Path += "*";
    WIN32_FIND_DATAA FileData;
    HANDLE hFind = FindFirstFileA(Path.c_str(), &FileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        DWORD ERR = GetLastError();
        Call_MsgBox(TEXT("안읽힘;;"));
        assert(ERR);

    }
    while (FindNextFileA(hFind, &FileData))
    {
        //. .. 는 시스템폴더라 읽을 필요가 없음
        if (!strcmp(FileData.cFileName, ".")
            || !strcmp(FileData.cFileName, ".."))
            continue;
        char* pFileName = new char[260];
        memcpy_s(pFileName, sizeof(char) * 260, FileData.cFileName, sizeof(char) * 260);
        DataComboArr::iterator VectorIter = find_if(m_arrSaveFilesCombo.begin(), m_arrSaveFilesCombo.end(), [&pFileName](DataComboArr::value_type Value)
            {
                if (0 == strcmp(get<Tuple_CharPtr>(Value), pFileName))
                    return true;
                else
                    return false;
            });

        if (m_arrSaveFilesCombo.end() == VectorIter)
            m_arrSaveFilesCombo.push_back(make_tuple(pFileName, false));
        else
            continue;
    }
    FindClose(hFind);

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

void CWindow_Map::SetUp_FilePath(string& strFilePath,char* szData)
{
    strFilePath = "";
    strFilePath += m_strPath;
    strFilePath += szData;
}

//static 
//static _int SelectMeshGroupIndex = 0;
//static _int SelectObjectPrototypeIndex = 0;




static float ObjectScale[3] = { 0.f };
static float ObjectRotate[3] = { 0.f };
static float ObjectPosition[3] = { 0.f };

static bool ObjectLightFlagOpt[4] = { false };
void CWindow_Map::SetUp_CurObjectlist()
{
    m_CurObjectList = GAMEINSTANCE->Get_ObjGroup(m_SelectObjectGroupIDIndex);
}
void CWindow_Map::Func_DataControl()
{
    ImGui::Text("Data_Control");

    if (ImGui::CollapsingHeader("Object Value", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text("Scale");
        if (ImGui::DragFloat3("##Object Scale", ObjectScale, 0.1f, 0.f, 0.f, "%.1f"))
        {
        }
        ImGui::Spacing();

        ImGui::Text("Rotate");
        if (360.1 <= ObjectRotate[0])
            ObjectRotate[0] = 0.0f;
        if (-0.1f >= ObjectRotate[0])
            ObjectRotate[0] = 360.0f;

        if (360.1 <= ObjectRotate[1])
            ObjectRotate[1] = 0.0f;
        if (-0.1f >= ObjectRotate[1])
            ObjectRotate[1] = 360.0f;

        if (360.1 <= ObjectRotate[2])
            ObjectRotate[2] = 0.0f;
        if (-0.1f >= ObjectRotate[2])
            ObjectRotate[2] = 360.0f;

        if (ImGui::DragFloat3("##Object Rotate", ObjectRotate, 0.1f, -0.1f, 360.1f, "%.1f"))
        {
            
        }
        ImGui::Spacing();

        ImGui::Text("Position");
        if (ImGui::DragFloat3("##Object Position", ObjectPosition, 0.1f, 0.f, 0.f, "%.1f"))
        {
        }
        ImGui::Spacing();
    }


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
    
    if (ImGui::InputText("##Light_GroupName", szLightGroupName, sizeof(char)* MAXCHAR))
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

    func(*this);
    ImGui::End();
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

void CWindow_Map::Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree)
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
            Read_Folder(strFullPath.c_str(), tTreeData);
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
            m_CurSelectedMeshName = CutOut_Ext(tTree.strFileName);
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

                    if (strExtName == "dat")
                        continue;

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

string CWindow_Map::CutOut_Ext(string& Origin)
{
    string strReturn = Origin;
    strReturn = strReturn.substr(0, strReturn.size() - size_t(4));
    return strReturn;
}
