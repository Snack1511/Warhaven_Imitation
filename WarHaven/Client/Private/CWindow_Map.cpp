#include "stdafx.h"
#include "CWindow_Map.h"

CWindow_Map::CWindow_Map()
{
}

CWindow_Map::~CWindow_Map()
{
    Clear_TupleData(m_arrSaveFilesCombo);
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
    m_strPath = "../Bin/Data/MapData/";
    //콤보어레이 생성
    Ready_FileArray();
    if (!m_arrSaveFilesCombo.empty())
    {
        get<Tuple_Bool>(m_arrSaveFilesCombo[SaveFileIndex]) = true;
    }

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

    //char* pFileName = new char[260];
    //memcpy_s(pFileName, sizeof(char) * 260, FileData.cFileName, sizeof(char) * 260);
    //m_arrSaveFilesCombo.push_back(make_tuple(pFileName, false));
}

void CWindow_Map::Ready_MeshGroupName()
{
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

static _int SelectObjectGroupIDIndex = 0;
static _int SelectObjectPrototypeIndex = 0;
static char szObjectGroupBuf[MAXCHAR] = "";
static char szCurSelectObjectNameBuf[MAXCHAR] = "Test.....";

static float ObjectScale[3] = { 0.f };
static float ObjectRotate[3] = { 0.f };
static float ObjectPosition[3] = { 0.f };

static bool ObjectLightFlagOpt[4] = { false };
void CWindow_Map::Func_DataControl()
{
    ImGui::Text("Data_Control");

    //3. 오브젝트 프로토타입 그룹 콤보박스
    if (ImGui::CollapsingHeader("Object Info", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text("Object Group");
        if (Make_Combo("##ObjectGroupID", m_arrObjectGroupId, &SelectObjectGroupIDIndex, bind(&CWindow_Map::EmptyFunction, this)))
        {
            strcpy_s(szObjectGroupBuf, get<Tuple_CharPtr>(m_arrObjectGroupId[SelectObjectGroupIDIndex]));
        }
        ImGui::Spacing();

        ImGui::Text("Mesh Group");
        if (Make_Combo("##ObjectGroupID", m_arrObjectGroupId, &SelectObjectGroupIDIndex, bind(&CWindow_Map::EmptyFunction, this)))
        {
            strcpy_s(szObjectGroupBuf, get<Tuple_CharPtr>(m_arrObjectGroupId[SelectObjectGroupIDIndex]));
        }
        ImGui::Spacing();

        //4. 오브젝트 프로토타입 리스트
        ImGui::Text("Objects");
        if (Make_Combo("##ObjectPrototypeList", m_arrObjectPrototypeName, &SelectObjectPrototypeIndex, bind(&CWindow_Map::EmptyFunction, this)))
        {
            //strcpy_s(szObjectGroupBuf, get<Tuple_CharPtr>(m_arrObjectPrototypeName[SelectObjectPrototypeIndex]));
        }
        ImGui::Spacing();


        ImGui::Text("Add Object List");
        ImGui::Spacing();

        //5. 선택된 오브젝트 이름 텍스트
        ImGui::Text("Name");
        ImGui::Text(szCurSelectObjectNameBuf);
        ImGui::Spacing();
    }

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
    ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    ImGui::Begin(szWindowName, &Open, WindowFlags);
    ImGui::SetWindowPos(ImVec2(ViewSize.x - Pos.x, ViewPos.y + Pos.y));
    ImGui::SetWindowSize(Size);
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
