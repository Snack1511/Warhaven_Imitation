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

#include "CCamera_FixedAngle.h"

#include "CStructure_Instance.h"

#include "CFunc_ObjectControl.h"
#include "CLight.h"
CWindow_Map::CWindow_Map()
{
    m_CurTerrainData.Initialize();
}

CWindow_Map::~CWindow_Map()
{
    m_pObjectController->Destroy();

    Clear_TupleData(m_arrSaveFilesCombo);
    Clear_TupleData(m_arrObjectGroupId);
    Clear_TupleData(m_arrLightTypeCombo);
    Clear_TupleData(m_arrBrushType);
    Clear_TupleData(m_arrTileTextureName);

    for (INSTANCEGROUPING::value_type& Value : m_InstanceMap)
    {
        for (INSTANCEVECTOR::value_type& VectorValue : Value.second)
        {
            Safe_Delete_Array(get<0>(VectorValue).ArrInstanceVTX);
        }
        Value.second.clear();
    }
    m_InstanceMap.clear();

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

    Functions_Maptool();
    //파일 위치 설정
    m_MeshRootNode.strFolderPath = "../bin/resources/meshes";
    m_MeshRootNode.strFileName = "Map";
    m_MeshRootNode.strFullPath = "../bin/resources/meshes/Map";
    Read_Folder_ForTree("../bin/resources/meshes/Map", m_MeshRootNode);

    m_tInstanceMeshDataRoot.strFolderPath = "../bin/resources/Meshes/Map";
    m_tInstanceMeshDataRoot.strFileName = "InstancingObjects";
    m_tInstanceMeshDataRoot.strFullPath = "../bin/resources/Meshes/Map/InstancingObjects";
    Read_Folder_ForTree("../bin/resources/Meshes/Map/InstancingObjects", m_tInstanceMeshDataRoot);

    m_strPath = "../Bin/Data/MapData/";
    //콤보어레이 생성
    Ready_FileArray();
    if (!m_arrSaveFilesCombo.empty())
    {
        get<Tuple_Bool>(m_arrSaveFilesCombo[SaveFileIndex]) = true;
    }
    Ready_ObjectGroupID();

    Ready_LightType();

    Ready_TerrainBrushType();

    if(FAILED(SetUp_Cameras()))
        return E_FAIL;

    m_OutDatas = make_tuple(_float4(0.f, 0.f, 0.f, 1.f), _float4(0.f, 0.f, 0.f, 1.f), _float4(0.f, 0.f, 0.f, 0.f));
    return S_OK;
}

void CWindow_Map::Tick()
{

    _bool bPicked = false;
    Select_Camera();

    if (m_bTerrainPick)
    {
        if(nullptr != m_pCurTerrain)
            SetUp_PickMesh(m_pCurTerrain->Get_MeshTerrain());
        if (Calculate_Pick())
            bPicked = true;
    }



    if (false == bPicked)
        m_pObjectController->Tick_Function();

    //if (m_bLightControl)
    //    Update_Light();
}

HRESULT CWindow_Map::Render()
{
    ImVec2 vPannelSize = ImVec2(300.f, 300.f);
    ImVec2 vTerrainControlPos = ImVec2(vPannelSize.x, 0.f);
    ImVec2 vInstnaceObjectControlPos = ImVec2(vPannelSize.x, vPannelSize.y);
    ImVec2 vBrushSettingWindowPos = ImVec2(vPannelSize.x, vPannelSize.y * 2);
    ImVec2 vDataControlPos = ImVec2(vPannelSize.x, vPannelSize.y * 3);
    ImVec2 vLightControlPos = ImVec2(vPannelSize.x, vPannelSize.y * 4);

    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::Text("MapTool");

    //파일 컨트롤
    Func_FileControl();

    if (nullptr != m_pCurTerrain)
    {
        Create_SubWindow("Terrain_Controller", vTerrainControlPos, vPannelSize, bind(&CWindow_Map::Func_TerrainControl, this));
        Create_SubWindow("InstanceStucture_Controller", vInstnaceObjectControlPos, vPannelSize, bind(&CWindow_Map::Func_InstanceObjectControl, this));
        Create_SubWindow("Brush Setting", vBrushSettingWindowPos, vPannelSize, bind(&CWindow_Map::Set_BrushInform, this));

    }



    //데이타 컨트롤
    if (m_pObjectController->Is_CurSelectObject())
    {
        Create_SubWindow("Data_Controller", vDataControlPos, vPannelSize, bind(&CFunc_ObjectControl::Func_ObjectControl, m_pObjectController));
    }

    //라이트 컨트롤
    // 
    //조건 필요.. 

    ImGui::Checkbox("Enable LightControl", &m_bLightControl);
    if (m_bLightControl)
    {
        if (-1 == m_iLightPadding)
            m_iLightPadding = GAMEINSTANCE->Get_LightSize();
        Create_SubWindow("Light_Controller", vLightControlPos, vPannelSize, bind(&CWindow_Map::Func_LightControl, this));
    }
    __super::End();


    return S_OK;
}

void CWindow_Map::Change_CurPickMode(PICKINGTYPE eType)
{
    m_ePickingType = eType;
}

_bool CWindow_Map::Is_CurPickMode(PICKINGTYPE eType)
{
    return (m_ePickingType == eType);
}

CWindow_Map::PICKDATA CWindow_Map::Get_PickData()
{
    
    return m_OutDatas;
}

void CWindow_Map::SetUp_PickMesh(CMesh* pMesh)
{
    m_PickTargetMesh = pMesh;
}

#pragma region static value 파일컨트롤러
static char szSaveNameBuf[MAXCHAR] = "";

static string SaveFilePath = "";
static string LoadFilePath = "";

static int iCurSelectTileIndex = 0;
#pragma endregion

#pragma region 파일 컨트롤러 함수
HRESULT CWindow_Map::Functions_Maptool()
{
    m_pObjectController = CFunc_ObjectControl::Create(this);
    return S_OK;
}
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
        Load_MapData(LoadFilePath);
        strcpy_s(szSaveNameBuf, sizeof(char) * MAXCHAR, get<Tuple_CharPtr>(m_arrSaveFilesCombo[SaveFileIndex]));
        m_pObjectController->Confirm_Data();
        //LoadFilePath
        //로드 버튼
    }
    ImGui::Spacing();
    ImGui::Text("Select PickingGroupID");
    SetUp_CurPickingGroup();
    Make_Combo("##PickingGroupID", m_arrObjectGroupId, &m_SelectObjectGroupIDIndex, bind(&CWindow_Map::EmptyFunction, this));
    ImGui::Spacing();
    if(ImGui::Checkbox("Terrain Pick", &m_bTerrainPick))
    {
        //m_bTerrainPick = true;
    }
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
        if (ImGui::Button("Generate!"))
        {
            Generate_Terrain();
        }
    }
    ImGui::Spacing();
    m_pObjectController->Func_Grouping();
    m_pObjectController->Func_FBXList();
    m_pObjectController->Func_ObjectList();
    m_pObjectController->Func_SetUpCollider();


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

#pragma endregion




#pragma region static value 라이트 컨트롤러 

static float LightPos[3] = { 0.f };
static float LightDir[3] = { 0.f };
static float LightRange = 0.f;
static float LightRandomRange = 0.f;
static float LightOffset[3] = { 0.f };
static float LightDiffuse[3] = { 0.f };
static float LightAmbient[3] = { 0.f };
static float LightSpecular[3] = { 0.f };
static bool LightFlagOpt[4] = { false };
static _float fIncreaseScale = 1.0f;
#pragma endregion

#pragma region 라이트 컨트롤함수
void CWindow_Map::Func_LightControl()
{
    if (ImGui::BeginListBox("##LightListBox"))
    {
        for (_uint i = 0; i < m_LightDescs.size(); ++i)
        {
            _bool bSelect = false;
            if (i == m_iCurSelectLight)
            {
                bSelect = true;
            }
            if (ImGui::Selectable(to_string(i).c_str(), bSelect))
            {
                m_iCurSelectLight = i;

                
            }
        }
        ImGui::EndListBox();
    }

    if (!m_LightDescs.empty() && _uint(m_LightDescs.size()) > m_iCurSelectLight)
    {
        
        LIGHTDESC tData = get<1>(m_LightDescs[m_iCurSelectLight]);

        memcpy_s(&m_iLightTypeIndex, sizeof(_int), &tData.eType, sizeof(_int));

        memcpy_s(LightPos, sizeof(float) * 3, &tData.vPosition, sizeof(float) * 3);
        memcpy_s(LightDir, sizeof(float) * 3, &tData.vDirection, sizeof(float) * 3);
        memcpy_s(&LightRange, sizeof(float), &tData.fRange, sizeof(float));
        memcpy_s(&LightRandomRange, sizeof(float), &tData.fRandomRange, sizeof(float));

        memcpy_s(LightDiffuse, sizeof(float) * 3, &tData.vDiffuse, sizeof(float) * 3);
        memcpy_s(LightAmbient, sizeof(float) * 3, &tData.vAmbient, sizeof(float) * 3);
        memcpy_s(LightSpecular, sizeof(float) * 3, &tData.vSpecular, sizeof(float) * 3);
        
    }
    //해당 그룹 소속 빛 정보 리스트
    //빛 추가
    if (ImGui::Button("Add Light"))
    {
        Add_Light();
    }
    ImGui::SameLine();
    //빛 제거
    if (ImGui::Button("Delete Light"))
    {
        Delete_Light();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clone Light"))
    {
        Clone_Light();
    }

    ImGui::Text("CurSelectIndex : ");
    ImGui::SameLine();
    ImGui::Text(to_string(m_iCurSelectLight).c_str());

    if (ImGui::CollapsingHeader("Light Default", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text("Light Tag");
        char szTagName[MAXCHAR] = "";
        if (m_iCurSelectLight >= 0 && m_iCurSelectLight < _int(m_LightDescs.size()))
        {
            string strTag = get<0>(m_LightDescs[m_iCurSelectLight]);
            strcpy_s(szTagName, strTag.c_str());
        }
        if (ImGui::InputText("##Input Tag", szTagName, sizeof(char) * MAXCHAR))
        {
            if (m_iCurSelectLight >= 0 && m_iCurSelectLight < _int(m_LightDescs.size()))
            {
                get<0>(m_LightDescs[m_iCurSelectLight]) = szTagName;
            }
        }



        //2. 빛 종류
        ImGui::Text("Light Type");
        Make_Combo("##Light_TypeList", m_arrLightTypeCombo, &m_iLightTypeIndex, bind(&CWindow_Map::Set_LightType, this));
        ImGui::Spacing();

        ImGui::Text("Increase Value");
        ImGui::SliderFloat("##SliderIncreaseValue", &fIncreaseScale, 0.001f, 100.f, "%.3f");
        ImGui::InputFloat("##InputIncreaseValue", &fIncreaseScale);
        ImGui::Spacing();
        //3. 위치
        ImGui::Text("Light Pos");
        if (ImGui::DragFloat3("##Light_Pos", LightPos, fIncreaseScale))
        {
            Set_LightPos(LightPos);
        }
        ImGui::Spacing();

        //4. 방향
        ImGui::Text("Light Dir");
        if (ImGui::DragFloat3("##Light_Dir", LightDir, fIncreaseScale))
        {
            Set_LightDir(LightDir);
        }
        ImGui::Spacing();

        //5. 범위
        ImGui::Text("Light Range");
        if (0.f >= LightRange)
        {
            LightRange = 0.f;
        }
        if (ImGui::DragFloat("##Light_Range", &LightRange, fIncreaseScale, 0.0f, 0.f, "%.3f"))
        {
            Set_LightRange(LightRange);
        }
        if (ImGui::CollapsingHeader("RandomRange"))
        {

            ImGui::Text("Light RandomRange");
            if (0.f >= LightRandomRange)
            {
                LightRandomRange = 0.f;
            }
            if (ImGui::DragFloat("##Light RandomRange", &LightRandomRange, fIncreaseScale, 0.0f, 0.f, "%.3f"))
            {
                Set_LightRandomRange(LightRandomRange);
            }
        }
        ImGui::Spacing();

    }

    if (ImGui::CollapsingHeader("Light Color", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow))
    {
        //6. 디퓨즈
        ImGui::Text("Light Diffuse");
        if (ImGui::ColorEdit3("##Light_Diffuse", LightDiffuse))
        {
            Set_LightDifColor(LightDiffuse);
        }
        ImGui::Spacing();

        //7. 엠비언트
        ImGui::Text("Light Ambient");
        if (ImGui::ColorEdit3("##Light_Ambient", LightAmbient))
        {
            Set_LightAmbColor(LightAmbient);
        }
        ImGui::Spacing();

        //8. 스펙큘러
        ImGui::Text("Light Specular");
        if (ImGui::ColorEdit3("##Light_Specular", LightSpecular))
        {
            Set_LightSpecColor(LightSpecular);
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

    //pLightType = new char[260];
    //ZeroMemory(pLightType, sizeof(char) * 260);
    //memcpy_s(pLightType, sizeof(char) * 260, "TARGET", sizeof(char) * 260);
    //m_arrLightTypeCombo.push_back(make_tuple(pLightType, false));
}

void CWindow_Map::Add_Light()
{
    LIGHTDESC tLightDesc;
    ZeroMemory(&tLightDesc, sizeof(LIGHTDESC));
    tLightDesc.fLightTime = 99999999.f;
    m_LightDescs.push_back(make_tuple(string("New Light"), tLightDesc));

    if (FAILED(GAMEINSTANCE->Add_Light(tLightDesc)))
    {
        assert(0);
    }
    m_iCurSelectLight++;
}
void CWindow_Map::Delete_Light()
{
    _int LightIndex = m_iCurSelectLight;
    _int CmpIndex = 0;
    vector<tuple<string, LIGHTDESC>>::iterator iter = m_LightDescs.begin();
    for (; iter != m_LightDescs.end(); ++iter)
    {
        if (CmpIndex == LightIndex)
        {
            break;
        }
        CmpIndex++;
    }
    m_LightDescs.erase(iter);
    GAMEINSTANCE->Remove_Light(LightIndex + m_iLightPadding);
    if (_int(m_LightDescs.size()) <= m_iCurSelectLight)
        m_iCurSelectLight =(m_LightDescs.empty()) ? 0 :  _int(m_LightDescs.size()) - 1;
}
void CWindow_Map::Clone_Light()
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    LIGHTDESC tLightDesc;
    memcpy(&tLightDesc, &get<1>(m_LightDescs[m_iCurSelectLight]), sizeof(LIGHTDESC));
    m_LightDescs.push_back(make_tuple(string("New Light"), tLightDesc));
    if (FAILED(GAMEINSTANCE->Add_Light(tLightDesc)))
    {
        assert(0);
    }
    m_iCurSelectLight++;
}

void CWindow_Map::Set_LightTag(string strTag)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    get<0>(m_LightDescs[m_iCurSelectLight]) = strTag;
}

void CWindow_Map::Set_LightType()
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    get<1>(m_LightDescs[m_iCurSelectLight]).eType = LIGHTDESC::TYPE(m_iLightTypeIndex);
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if(pLight != nullptr)
        pLight->Get_LightDesc_Modify().eType = LIGHTDESC::TYPE(m_iLightTypeIndex);


}
void CWindow_Map::Set_LightPos(float* PosArr)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vPosition, PosArr, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vPosition.w = 1.f;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vPosition = get<1>(m_LightDescs[m_iCurSelectLight]).vPosition;
}
void CWindow_Map::Set_LightOffset(float* OffsetArr)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vOffset, OffsetArr, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vOffset.w = 1.f;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vOffset = get<1>(m_LightDescs[m_iCurSelectLight]).vOffset;
}
void CWindow_Map::Set_LightDir(float* DirArr)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vDirection, DirArr, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vDirection.w = 0.f;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vDirection = get<1>(m_LightDescs[m_iCurSelectLight]).vDirection;
}
void CWindow_Map::Set_LightRange(float fRange)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

    get<1>(m_LightDescs[m_iCurSelectLight]).fRange = fRange;
    get<1>(m_LightDescs[m_iCurSelectLight]).fOriginRange = fRange;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
    {
        pLight->Get_LightDesc_Modify().fRange = get<1>(m_LightDescs[m_iCurSelectLight]).fRange;
        pLight->Get_LightDesc_Modify().fOriginRange = get<1>(m_LightDescs[m_iCurSelectLight]).fOriginRange;
    }
}

void CWindow_Map::Set_LightRandomRange(float fRandomRange)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

    get<1>(m_LightDescs[m_iCurSelectLight]).fRandomRange = fRandomRange;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().fRandomRange = get<1>(m_LightDescs[m_iCurSelectLight]).fRandomRange;
}
void CWindow_Map::Set_LightDifColor(float* ColorValue)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vDiffuse, ColorValue, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vDiffuse.w = 1.f;
    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vDiffuse = get<1>(m_LightDescs[m_iCurSelectLight]).vDiffuse;
}
void CWindow_Map::Set_LightAmbColor(float* ColorValue)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vAmbient, ColorValue, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vAmbient.w = 1.f;

    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vAmbient = get<1>(m_LightDescs[m_iCurSelectLight]).vAmbient;
}
void CWindow_Map::Set_LightSpecColor(float* ColorValue)
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;
    memcpy(&get<1>(m_LightDescs[m_iCurSelectLight]).vSpecular, ColorValue, sizeof(_float3));
    get<1>(m_LightDescs[m_iCurSelectLight]).vSpecular.w = 1.f;


    CLight* pLight = GAMEINSTANCE->Get_Light(m_iCurSelectLight + m_iLightPadding);
    if (pLight != nullptr)
        pLight->Get_LightDesc_Modify().vSpecular = get<1>(m_LightDescs[m_iCurSelectLight]).vSpecular;
}
void CWindow_Map::Set_LightOption()
{
    if (m_iCurSelectLight < 0 || m_iCurSelectLight >= _int(m_LightDescs.size()))
        return;

}
void CWindow_Map::Update_Light()
{
    GAMEINSTANCE->Clear_Lights();
    for (auto& elem : m_LightDescs)
    {
        if (FAILED(GAMEINSTANCE->Add_Light(get<1>(elem))))
        {
            assert(0);
        }
    }
}
#pragma endregion

#pragma region 파일입출력 함수
void CWindow_Map::Save_MapData(string BasePath, string SaveName)
{
    MAPDATA     tMapData;
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
    m_pObjectController->Save_Data(GroupPath, SaveName);

    string InstancePath = BasePath;
    InstancePath += "InstanceData/";
    Save_InstanceData(InstancePath, SaveName);

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
    m_pObjectController->Load_Data(CFunctor::To_String(tMapData.ObjectDataPath));
    Load_InstanceData(CFunctor::To_String(tMapData.InstanceDataPath));
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
    _uint iTilePathPathLength = _uint(strlen(szTexturePath))+ 1;


    writeFile.write((char*)&iTilePathPathLength, sizeof(_uint));
    writeFile.write((char*)szTexturePath, sizeof(char) * iTilePathPathLength);


    _uint VerticesX = m_CurTerrainData.iNumVerticesX;
    _uint VerticesZ = m_CurTerrainData.iNumVerticesZ;
    writeFile.write((char*)&VerticesX, sizeof(_uint));
    writeFile.write((char*)&VerticesZ, sizeof(_uint));

    _uint iNumVertices = VerticesX * VerticesZ;

    _float3* Vertices = m_CurTerrainData.pCurTerrainVertPos;
    writeFile.write((char*)Vertices, sizeof(_float3) * iNumVertices);

    _float4* Colors = m_CurTerrainData.pCurTerrainColor;
    writeFile.write((char*)Colors, sizeof(_float4) * iNumVertices);

    _int IndexBG = m_iBGIndex;
    _int IndexSour = m_iSourIndex;
    _int IndexDest = m_iDestIndex;
    writeFile.write((char*)&IndexBG, sizeof(_int));
    writeFile.write((char*)&IndexSour, sizeof(_int));
    writeFile.write((char*)&IndexDest, sizeof(_int));

    _float4x4 mat_Terrain = m_pCurTerrain->Get_Transform()->Get_WorldMatrix();
    writeFile.write((char*)&mat_Terrain, sizeof(_float4x4));

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
    pTerrain = CDrawable_Terrain::Create(strPath.c_str(), true);
    if (nullptr == pTerrain)
        assert(0);
    CREATE_GAMEOBJECT(pTerrain, GROUP_DEFAULT);
    m_pCurTerrain = pTerrain;
    MTT_DATA::Terrain_TUPLE tupleData = m_pCurTerrain->Get_TerrainData();
    m_CurTerrainData.Make_Data(tupleData);

    _uint3 Indices = _uint3(0, 0, 0);
    Indices = m_pCurTerrain->Get_TerrainTexIndex();
    m_iBGIndex = Indices._1;
    m_iSourIndex = Indices._2;
    m_iDestIndex = Indices._3;
    //지형의 정점을 통한 터레인 수정
}

void CWindow_Map::Save_InstanceData(string BasePath, string SaveName)
{
    string SplitBasePath = BasePath;
    SplitBasePath += "SplitData/";
    Save_SpliteData(SplitBasePath, SaveName);


    string MergeBasePath = BasePath;
    MergeBasePath += "MergeData/";
    Save_MergeData(MergeBasePath, SaveName);



    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".InstanceData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }
    string strSplitFullPath = SplitBasePath;
    strSplitFullPath += SaveName;
    strSplitFullPath += ".InstanceSplitData";

    string strMergeFullPath = MergeBasePath;
    strMergeFullPath += SaveName;
    strMergeFullPath += ".InstanceMergeData";

    _int SplitPathLength = _int(strSplitFullPath.length()) + 1;
    writeFile.write((char*)&SplitPathLength, sizeof(_int));
    char strPath[MAXCHAR];
    strcpy_s(strPath, sizeof(char) * MAXCHAR, strSplitFullPath.c_str());
    writeFile.write(strPath, sizeof(char)* SplitPathLength);

    _int MergePathLength = _int(strMergeFullPath.length()) + 1;
    writeFile.write((char*)&MergePathLength, sizeof(_int));
    strcpy_s(strPath, sizeof(char) * MAXCHAR, strMergeFullPath.c_str());
    writeFile.write(strPath, sizeof(char) * MergePathLength);

    //Call_MsgBox(L"Save 성공");
}
void CWindow_Map::Save_SpliteData(string BasePath, string SaveName)
{
    //.InstanceSplitData

    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".InstanceSplitData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }

    //그룹 개수 저장
    _int GroupSize = _int(m_strArrInstanceMeshName.size());
    writeFile.write((char*)&GroupSize, sizeof(_int));
    for (_int i = 0; i < GroupSize; ++i)
    {
        INSTANCEVECTOR& InstanceVector = m_InstanceMap[Convert_ToHash( m_strArrInstanceMeshName[i])];
        _int InstanceSize = _int(InstanceVector.size());
        writeFile.write((char*)&InstanceSize, sizeof(_int));
        for (_int j = 0; j < InstanceSize; ++j)
        {
            MTINSTANCE_DATA& Data = get<0>(InstanceVector[j]);
            Data.Save(writeFile);
        }
    }
    writeFile.close();
}
void CWindow_Map::Save_MergeData(string BasePath, string SaveName)
{
    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".InstanceMergeData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }
    _int GroupSize = _int(m_InstanceMap.size());
    writeFile.write((char*) & GroupSize, sizeof(_int));

    for (INSTANCEGROUPING::value_type& InstanceGroupValue : m_InstanceMap)
    {
        wstring strGroupname = get<0>(InstanceGroupValue.second.front()).strInstanceGorupName;
        wstring strPath = get<0>(InstanceGroupValue.second.front()).strMeshPath;
        _float4 IsntancePos = get<0>(InstanceGroupValue.second.front()).InstancePosition;
        _int TotalInstanceNums = 0;
        for (INSTANCEVECTOR::value_type& Value : InstanceGroupValue.second)
        {
            TotalInstanceNums += get<0>(Value).iInstanceNums;
            DISABLE_GAMEOBJECT(get<1>(Value));
        }

        _int PrevNumInstance = 0;
        VTXINSTANCE* pInstanceVtx = new VTXINSTANCE[TotalInstanceNums];
        for (INSTANCEVECTOR::value_type& Value : InstanceGroupValue.second)
        {
            memcpy(pInstanceVtx + PrevNumInstance
                , get<0>(Value).ArrInstanceVTX
                , sizeof(VTXINSTANCE) * get<0>(Value).iInstanceNums);
            PrevNumInstance += get<0>(Value).iInstanceNums;
        }

        PrevNumInstance = 0;
        for (INSTANCEVECTOR::value_type& Value : InstanceGroupValue.second)
        {
            for (_int i = 0; i < _int(get<0>(Value).iInstanceNums); ++i)
            {
                pInstanceVtx[i + PrevNumInstance].vTranslation.x += get<0>(Value).InstancePosition.x;
                pInstanceVtx[i + PrevNumInstance].vTranslation.y += get<0>(Value).InstancePosition.y;
                pInstanceVtx[i + PrevNumInstance].vTranslation.z += get<0>(Value).InstancePosition.z;
            }
            PrevNumInstance += get<0>(Value).iInstanceNums;
        }

        _int PathLength = _int(strPath.length()) + 1;
        char Path[MAX_PATH];
        strcpy_s(Path, sizeof(char) * MAX_PATH, CFunctor::To_String(strPath).c_str());
        
        writeFile.write((char*)&PathLength, sizeof(_int));
        writeFile.write(Path, sizeof(char)*PathLength);
        writeFile.write((char*)&TotalInstanceNums, sizeof(_int));
        writeFile.write((char*)pInstanceVtx, sizeof(VTXINSTANCE) * TotalInstanceNums);

        Safe_Delete_Array(pInstanceVtx);
    }
    writeFile.close();
}
void CWindow_Map::Load_InstanceData(string FilePath)
{
    for (INSTANCEGROUPING::value_type& Value : m_InstanceMap)
    {
        for (INSTANCEVECTOR::value_type& VectorValue : Value.second)
        {
            Safe_Delete_Array(get<0>(VectorValue).ArrInstanceVTX);
            DELETE_GAMEOBJECT(get<1>(VectorValue));
        }
        Value.second.clear();
    }
    m_InstanceMap.clear();
    m_strArrInstanceMeshName.clear();
    m_iCurSelectInstanceNameIndex = 0;
    m_iCurSelectInstanceObjectIndex = 0;

    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }
    char strPath[MAXCHAR];
    _int SplitLength = 0;
    readFile.read((char*)&SplitLength, sizeof(_int));
    readFile.read(strPath, sizeof(char) * SplitLength);
    readFile.close();

    LoadFullPath = strPath;
    readFile.open(LoadFullPath, ios::binary);
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
        _int InstanceSize = 0;
        readFile.read((char*)&InstanceSize, sizeof(_int));
        for (_int j = 0; j < InstanceSize; ++j)
        {
            MTINSTANCE_DATA Data;
            Data.Load(readFile);
            string strGroupName = CFunctor::To_String(Data.strInstanceGorupName);
            size_t HashNums = Convert_ToHash( strGroupName);
            INSTANCEGROUPING::iterator GroupIter = m_InstanceMap.find(HashNums);
            if (m_InstanceMap.end() == GroupIter)
            {
                m_InstanceMap.emplace(HashNums, INSTANCEVECTOR());
                m_strArrInstanceMeshName.push_back(strGroupName);
            }

            CStructure_Instance* pInstance = CStructure_Instance::Create(Data.strMeshPath, Data.iInstanceNums, Data.ArrInstanceVTX);
            pInstance->Initialize();
            CREATE_GAMEOBJECT(pInstance, GROUP_DECORATION);
            pInstance->Get_Transform()->Set_World(WORLD_POS, Data.InstancePosition);
            m_InstanceMap[HashNums].push_back(make_tuple(Data, pInstance));
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
    string SaveFullPath = BasePath;
    SaveFullPath += SaveName;
    SaveFullPath += ".LightData";
    ofstream	writeFile(SaveFullPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        assert(0);
    }


    _uint LightDescNums = _uint(m_LightDescs.size());
    writeFile.write((char*)&LightDescNums, sizeof(_uint));
    for (_uint i = 0; i < LightDescNums; ++i)
    {
        _int LightTagLength= _int(get<0>(m_LightDescs[i]).length()) + 1;
        writeFile.write((char*)&LightTagLength, sizeof(_int));
        char strPath[MAXCHAR];
        strcpy_s(strPath, sizeof(char) * MAXCHAR, get<0>(m_LightDescs[0]).c_str());
        writeFile.write(strPath, sizeof(char) * LightTagLength);

        LIGHTDESC tDesc = get<1>(m_LightDescs[i]);

        writeFile.write((char*)&tDesc.eType, sizeof(_uint));
        writeFile.write((char*)&tDesc.vPosition, sizeof(_float4));
        writeFile.write((char*)&tDesc.vDirection, sizeof(_float4));
        writeFile.write((char*)&tDesc.fRange, sizeof(_float));
        writeFile.write((char*)&tDesc.fRandomRange, sizeof(_float));
        writeFile.write((char*)&tDesc.vDiffuse, sizeof(_float4));
        writeFile.write((char*)&tDesc.vAmbient, sizeof(_float4));
        writeFile.write((char*)&tDesc.vSpecular, sizeof(_float4));
        writeFile.write((char*)&tDesc.LightOpt, sizeof(char));

    }

    writeFile.close();
}
void CWindow_Map::Load_LightGroup(string FilePath)
{
    for(_int i = 0; i < m_LightDescs.size(); ++i)
        GAMEINSTANCE->Pop_Light();
    m_LightDescs.clear();
    m_iLightPadding = _uint(GAMEINSTANCE->Get_LightSize());
    string LoadFullPath = FilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }
    if (readFile.eof()) {
        Call_MsgBox(L"Light - 저장된 데이터 없음");
        return;
    }
    char strPath[MAXCHAR];
    _uint LightDescLength = 0;
    readFile.read((char*)&LightDescLength, sizeof(_uint));
    for (_uint i = 0; i < LightDescLength; ++i)
    {
        _int LightTagLength = 0;
        readFile.read((char*)&LightTagLength, sizeof(_int));
        char strPath[MAXCHAR];
        readFile.read(strPath, sizeof(char) * LightTagLength);
        string LightTag = strPath;



        LIGHTDESC tData;
        readFile.read((char*)&tData.eType, sizeof(_uint));
        readFile.read((char*)&tData.vPosition, sizeof(_float4));
        readFile.read((char*)&tData.vDirection, sizeof(_float4));
        readFile.read((char*)&tData.fRange, sizeof(_float));
        readFile.read((char*)&tData.fRandomRange, sizeof(_float));
        readFile.read((char*)&tData.vDiffuse, sizeof(_float4));
        readFile.read((char*)&tData.vAmbient, sizeof(_float4));
        readFile.read((char*)&tData.vSpecular, sizeof(_float4));
        readFile.read((char*)&tData.LightOpt, sizeof(char));

        m_LightDescs.push_back(make_tuple(LightTag, tData));
        if (FAILED(GAMEINSTANCE->Add_Light(tData)))
        {
            assert(0);
        }
    }
    readFile.close();
}
#pragma endregion


#pragma region 터레인 컨트롤 함수
void CWindow_Map::Ready_TerrainBrushType()
{
    Add_Brush("Default");
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
    if (nullptr != m_pDefaultTerrain)
    {
        DELETE_GAMEOBJECT(m_pDefaultTerrain);
        //m_pDefaultTerrain->Set_Enable(false);
        m_pDefaultTerrain = nullptr;
    }
    return S_OK;
}

void CWindow_Map::Func_TerrainControl()
{
    if (ImGui::Checkbox("Activate WireMode", &m_TerrainWireFrame))
    {
        if (m_TerrainWireFrame)
            m_pCurTerrain->Change_ShaderPass(VTXNOR_PASS_NAVIGATION);
        else
            m_pCurTerrain->Change_ShaderPass(VTXNOR_PASS_TEXTUREARRAY);

    }

    string strBrushOnOffText;
    if (PICK_TERRAINVERT != m_ePickingType)
    {
        strBrushOnOffText = "On TerrainControl";
    }
    else
        strBrushOnOffText = "Off TerrainControl";

    if (ImGui::Button(strBrushOnOffText.c_str()))
    {
        if (PICK_TERRAINVERT == m_ePickingType)
        {
            m_ePickingType = PICK_NONE;
        }
        else
            m_ePickingType = PICK_TERRAINVERT;
    }
    ImGui::Spacing();

    if (PICK_TERRAINTEX != m_ePickingType)
    {
        strBrushOnOffText = "On ChangeTileTex";
    }
    else
        strBrushOnOffText = "Off TerrainControl";

    if (ImGui::Button(strBrushOnOffText.c_str()))
    {
        if (PICK_TERRAINTEX == m_ePickingType)
        {
            m_ePickingType = PICK_NONE;
        }
        else
            m_ePickingType = PICK_TERRAINTEX;
    }
    ImGui::Spacing();

    ImGui::Text("CurPickedIndex(Ready - Only)");
    ImGui::InputInt3("##CurPickedIndex", (int*)&m_i3PickedIndex, ImGuiInputTextFlags_ReadOnly);
    ImGui::Spacing();
    ImGui::Text("CurPickedWorldPos(Ready - Only)");
    ImGui::InputFloat4("##CurPickedWorld", (float*)&get<PICK_OUTPOS>(m_OutDatas), "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::Spacing();
    ImGui::Text("CurPickedLocalPos(Ready - Only)");
    ImGui::InputFloat4("##CurPickedLocal",  (float*)&get<PICK_OUTLOCALPOS>(m_OutDatas), "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::Spacing();
    ImGui::Text("CurPickedNormal(Ready - Only)");
    ImGui::InputFloat4("##CurPickedNormal",  (float*)&get<PICK_OUTNORM>(m_OutDatas), "%.2f", ImGuiInputTextFlags_ReadOnly);
    ImGui::Spacing();

    switch(m_ePickingType)
    {
    case PICK_TERRAINVERT:
        Edit_TerrainVert();
        break;

    case PICK_TERRAINTEX:
        Edit_TerrainTex();
        break;
    }
    if (ImGui::CollapsingHeader("Terrain PositionControl"))
    {
        
        Edit_TerrainData();
    }
        

}
void CWindow_Map::Generate_Terrain()
{
    Disable_DefaultTerrain();
    if (nullptr != m_pCurTerrain)
    {
        DELETE_GAMEOBJECT(m_pCurTerrain);
        m_pCurTerrain = nullptr;
    }
    CDrawable_Terrain* pTerrain = CDrawable_Terrain::Create(m_CurTerrainData.iNumVerticesX, m_CurTerrainData.iNumVerticesZ, true);
    CREATE_GAMEOBJECT(pTerrain, GROUP_DEFAULT);
    m_pCurTerrain = pTerrain;
    MTT_DATA::Terrain_TUPLE TupleData = m_pCurTerrain->Get_TerrainData();
    m_CurTerrainData.Make_Data(TupleData);
}
void CWindow_Map::Change_TileTexture()
{
    if (PICK_TERRAINTEX!= m_ePickingType)
        return;
    if (nullptr != m_pCurTerrain)
    {
        _float4* TileFlags = m_pCurTerrain->Get_TerrainTileFlag();
        _float3* pVertPos = m_pCurTerrain->Get_TerrainVerticesPos();
        _float4 OutPos = get<PICK_OUTLOCALPOS>(m_OutDatas);
        list<_uint> VertsList = Select_Vertices();
        for (list<_uint>::value_type& Value : VertsList)
        {

            _float4 vVertPos
                = _float4(
                    (pVertPos+ Value)->x,
                    OutPos.y,
                    (pVertPos+ Value)->z,
                    1.f);
            _float VertLength = XMVectorGetX(XMVector3Length(vVertPos.XMLoad() - OutPos.XMLoad()));
            _float fVertRatio = VertLength / (m_fBrushSize * 0.5f);
            _float4 vStartValue = _float4(0.f, 0.f, 0.f, 0.f);
            m_vTileTypeFlag.y += m_fBrushWeight * (1 - fVertRatio) * fDT(0);
            _float4 vTargetValue = m_vTileTypeFlag;
            _float4 vOut;
            CGame_Manager_MJ::Get_Instance()->Easing_Vert(m_iCurSelectTerrainBrush, &vOut, vStartValue, vTargetValue, 1 - fVertRatio);

            TileFlags[Value] = vOut;
            TileFlags[Value].w = 1.f;
        }
        VertsList.clear();
        m_pCurTerrain->Update_Vertices();
        m_pCurTerrain->Update_Texture(m_iSourIndex, m_iDestIndex, m_iBGIndex);
        //m_pCurTerrain->Update_Texture(m_iDestIndex);

    }

}
void CWindow_Map::Increase_Height()
{
    if (PICK_TERRAINVERT != m_ePickingType)
        return;
    if (nullptr != m_pCurTerrain)
    {
        _float3* Verts = m_pCurTerrain->Get_TerrainVerticesPos();
        list<_uint> VertsList = Select_Vertices();
        for (list<_uint>::value_type& Value : VertsList)
        {
            Verts[Value] = Easing_Vertices(&Verts[Value]);
        }
        VertsList.clear();
        m_pCurTerrain->Update_Vertices();
    }
}
void CWindow_Map::Edit_TerrainVert()
{
    if (ImGui::CollapsingHeader("Modify Vertex"))
    {

        if (ImGui::Button("Activate VertsNormal"))
        {
            m_pCurTerrain->Update_Normal();
        }
    }
}
void CWindow_Map::Edit_TerrainTex()
{
    if (ImGui::CollapsingHeader("Index Setting"))
    {
        string strSourDebug = m_curSelectSourTextureName;//to_string(m_iSourIndex);
        string strDestDebug = m_curSelectDestTextureName;//to_string(m_iDestIndex);
        string strBGDebug = m_curSelectBGTextureName;//to_string(m_iDestIndex);
        ImGui::Text("Select BGTex");
        if (ImGui::BeginCombo("##BGCombo", m_curSelectBGTextureName.c_str()))
        {
            list<tuple<string, _int>>& TupleList = m_pCurTerrain->Get_TextureList();
            for (list<tuple<string, _int>>::value_type& Value : TupleList)
            {
                _bool bSelect = false;
                if (m_curSelectBGTextureName == get<0>(Value))
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_curSelectBGTextureName = get<0>(Value);
                    m_iBGIndex = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        //if (ImGui::SliderFloat("##BGRate", &m_vTileTypeFlag.x, 0.f, 1.f, "%.4f"))
        //{
        //    m_vTileTypeFlag.Normalize();
        //}
        ImGui::Spacing();
        ImGui::Text("Select SourTex");
        if (ImGui::BeginCombo("##SourCombo", m_curSelectSourTextureName.c_str()))
        {
            list<tuple<string, _int>>& TupleList = m_pCurTerrain->Get_TextureList();
            for (list<tuple<string, _int>>::value_type& Value : TupleList)
            {
                _bool bSelect = false;
                if (m_curSelectSourTextureName == get<0>(Value))
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_curSelectSourTextureName = get<0>(Value);
                    m_iSourIndex = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
        ImGui::Text("Select DestTex");
        if (ImGui::BeginCombo("##DestCombo", m_curSelectDestTextureName.c_str()))
        {
            list<tuple<string, _int>>& TupleList = m_pCurTerrain->Get_TextureList();
            for (list<tuple<string, _int>>::value_type& Value : TupleList)
            {
                _bool bSelect = false;
                if (m_curSelectDestTextureName == get<0>(Value))
                {
                    bSelect = true;
                }
                if (ImGui::Selectable(get<0>(Value).c_str(), bSelect))
                {
                    m_curSelectDestTextureName = get<0>(Value);
                    m_iDestIndex = get<1>(Value);
                }
            }
            ImGui::EndCombo();
        }
        //if (ImGui::SliderFloat("##DestRate", &m_vTileTypeFlag.z, 0.f, 1.f, "%.4f"))
        //{
        //    m_vTileTypeFlag.Normalize();
        //}
        ImGui::Spacing();
        if (ImGui::RadioButton("##Active SourTex", &m_iTileIndexFlag, 0))
        {
            m_iTileIndexFlag = 0;
            m_vTileTypeFlag.x = 0.f;
        }
        if (ImGui::RadioButton("##Active DestTex", &m_iTileIndexFlag, 1))
        {
            m_iTileIndexFlag = 1;
            m_vTileTypeFlag.x = 1.f;
        }
        DebugData("Debug_Sour", strBGDebug, ImVec4(1.f, 0.f, 0.f, 1.f));
        DebugData("Debug_Sour", strSourDebug, ImVec4(0.f, 1.f, 0.f, 1.f));
        DebugData("Debug_Dest", strDestDebug, ImVec4(0.f, 0.f, 1.f, 1.f));
    }

}
void CWindow_Map::Edit_TerrainData()
{
    float terrainPos[3] =
    {
        m_pCurTerrain->Get_Transform()->Get_World(WORLD_POS).x,
        m_pCurTerrain->Get_Transform()->Get_World(WORLD_POS).y,
        m_pCurTerrain->Get_Transform()->Get_World(WORLD_POS).z
    };
    float terrainScale[3] =
    {
        m_pCurTerrain->Get_Transform()->Get_Scale().x,
        m_pCurTerrain->Get_Transform()->Get_Scale().y,
        m_pCurTerrain->Get_Transform()->Get_Scale().z
    };
    ImGui::Text("Terrain Scale");
    if (ImGui::InputFloat3("##ScaleInput", terrainScale))
    {
        if (nullptr != m_pCurTerrain)
        {
            _float4 vScale = _float4(terrainScale[0], terrainScale[1], terrainScale[2], 0.f);
            m_pCurTerrain->Get_Transform()->Set_Scale(vScale);
        }
    }
    ImGui::Text("Terrain Position");
    if (ImGui::InputFloat3("##PosInput", terrainPos))
    {
        if (nullptr != m_pCurTerrain)
        {
            _float4 vPosition = _float4(terrainPos[0], terrainPos[1], terrainPos[2], 1);
            m_pCurTerrain->Get_Transform()->Set_World(WORLD_POS, vPosition);
        }
    }

}
void CWindow_Map::Add_Brush(const char* BrushName)
{
    char* szTypeName = new char[MAXCHAR];
    memcpy_s(szTypeName, sizeof(char) * MAXCHAR, BrushName, sizeof(char) * (_int(strlen(BrushName)) + 1));
    m_arrBrushType.push_back(make_tuple(szTypeName, false));

}
list<_uint> CWindow_Map::Select_Vertices()
{
    _matrix matInverse = m_pCurTerrain->Get_Transform()->Get_WorldMatrix().XMLoad();
    matInverse = XMMatrixInverse(nullptr, matInverse);
    _float3* Verts = m_pCurTerrain->Get_TerrainVerticesPos();
    _float fRount = m_fBrushSize * 0.5f;
    _float4 Center = get<PICK_OUTLOCALPOS>(m_OutDatas);//XMVector3TransformCoord().XMLoad(), matInverse);
    _int VertXNums = m_pCurTerrain->Get_TerrainVerticesX();
    _int VertZNums = m_pCurTerrain->Get_TerrainVerticesZ();

    list<_uint> VertsList;
    _int IndexWidStart = _int((Center.x /*+ TerrainPos*/) - fRount);
    IndexWidStart = (0 > IndexWidStart) ? 0 : IndexWidStart;
    IndexWidStart = (VertXNums - 1 < IndexWidStart) ? VertXNums - 1 : IndexWidStart;

    _int IndexWidEnd = _int((Center.x /*+ TerrainPos*/) + fRount);
    IndexWidEnd = (0 > IndexWidEnd) ? 0 : IndexWidEnd;
    IndexWidEnd = (VertXNums - 1 < IndexWidEnd) ? VertXNums - 1 : IndexWidEnd;

    _int IndexHeightStart = _int((Center.z /*+ TerrainPos*/) - fRount);
    IndexHeightStart = (0 > IndexHeightStart) ? 0 : IndexHeightStart;
    IndexHeightStart = (VertZNums - 1 < IndexHeightStart) ? VertZNums - 1 : IndexHeightStart;
    
    _int IndexHeightEnd = _int((Center.z /*+ TerrainPos*/) + fRount);
    IndexHeightEnd = (0 > IndexHeightEnd) ? 0 : IndexHeightEnd;
    IndexHeightEnd = (VertZNums - 1 < IndexHeightEnd) ? VertZNums - 1 : IndexHeightEnd;


    for (_int i = IndexHeightStart; i <= IndexHeightEnd; ++i)
    {
        for (_int j = IndexWidStart; j <= IndexWidEnd; ++j)
        {
            _uint Index = i * VertXNums + j;
            if(Check_InBrush(&Verts[Index], Center))
                VertsList.push_back(Index);
        }
    }


    return VertsList;
}
_bool CWindow_Map::Check_InBrush(_float3* CompVert, _float4 vCenterPos)
{
    _float fRount = m_fBrushSize * 0.5f;
    _float4 Center = vCenterPos;//get<PICK_OUTPOS>(m_OutDatas);
    _float XPos = (*CompVert).x;
    _float ZPos = (*CompVert).z;

    //x - a
    _float XDiff = XPos - Center.x;
    //z - b
    _float ZDiff = ZPos - Center.z;

    //(x - a)^2 + (z - b)^2  <= r^2 --> 원 내부에 점이 존재
    if (XDiff * XDiff + ZDiff * ZDiff <= fRount * fRount)
        return true;
    else
        return false;
}
_float3 CWindow_Map::Easing_Vertices(_float3* pVertPos)
{
    _matrix matInverse = m_pCurTerrain->Get_Transform()->Get_WorldMatrix().XMLoad();
    matInverse = XMMatrixInverse(nullptr, matInverse);

    _float4 OutPos = get<PICK_OUTLOCALPOS>(m_OutDatas);//XMVector3TransformCoord(get<PICK_OUTPOS>(m_OutDatas).XMLoad(), matInverse);
    _float4 vVertPos 
        = _float4(
            pVertPos->x, 
            OutPos.y,
            pVertPos->z, 
            1.f);

    _float VertLength = XMVectorGetX(XMVector3Length(vVertPos.XMLoad() - OutPos.XMLoad()));
    _float fVertRatio = VertLength / (m_fBrushSize*0.5f);
    _float4 vStartPos = vVertPos;
    _float4 vTargetPos = _float4(vVertPos.x, vVertPos.y + m_fBrushWeight, vVertPos.z, vVertPos.w);
    _float4 vOut;
    CGame_Manager_MJ::Get_Instance()->Easing_Vert(m_iCurSelectTerrainBrush, &vOut, vStartPos, vTargetPos, 1- fVertRatio);
    _float3 vReturn = _float3(vOut.x, vOut.y, vOut.z);
    return vReturn;
}
void CWindow_Map::Func_InstanceObjectControl()
{

    //PICK_INSTANCEOBJECT
    string strBrushOnOffText;
    if (PICK_INSTANCEOBJECT != m_ePickingType)
    {
        strBrushOnOffText = "On InstancingObjectControl";
    }
    else
        strBrushOnOffText = "Off InstancingObjectControl";

    if (ImGui::Button(strBrushOnOffText.c_str()))
    {
        if (PICK_INSTANCEOBJECT == m_ePickingType)
        {
            m_ePickingType = PICK_NONE;
        }
        else
            m_ePickingType = PICK_INSTANCEOBJECT;
    }
    ImGui::Spacing();

    if (ImGui::Button("Confirm"))
    {
        m_pCurSelectInstanceObject = nullptr;
    }

    ImGui::Text("Draw InstanceNums");
    ImGui::InputInt("##InputInstanceNums", &m_iDrawInstanceObjectNums);

    ImGui::Text("Rotation RadomAngle");
    ImGui::InputFloat("##RandomAngle", &m_fRandomRatio);

    Show_TreeData((void*)&m_tInstanceMeshDataRoot, bind(&CWindow_Map::Routine_InstanceMeshSelect, this, placeholders::_1));

    if (!m_strArrInstanceMeshName.empty()) 
    {
        if (ImGui::BeginCombo("##InstanceGroupName", m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex].c_str()))
        {
            for (_int i = 0; i < _int(m_strArrInstanceMeshName.size()); ++i)
            {
                _bool bSelected = false;
                if (i == m_iCurSelectInstanceNameIndex)
                {
                    bSelected = true;
                }
                if (ImGui::Selectable(m_strArrInstanceMeshName[i].c_str(), bSelected))
                {
                    m_iCurSelectInstanceNameIndex = i;
                    m_strCurSelectInstanceMeshName = m_strArrInstanceMeshName[i];
                }
            }

            ImGui::EndCombo();
        }
    }
    INSTANCEGROUPING::iterator pTupleList;
    if (!m_strArrInstanceMeshName.empty())
    {
        pTupleList = m_InstanceMap.find(Convert_ToHash( m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex]));
    }
    else
    {
        pTupleList = m_InstanceMap.end();
    }

    ImGui::Text("Instancing List");
    if (ImGui::BeginListBox("##Instance List", ImVec2(360.f, 200.f)))
    {
        if (m_InstanceMap.end() != pTupleList)
        {
            _int Index = 0;
            for (INSTANCEVECTOR::value_type& Value : pTupleList->second)
            {
                if (get<1>(Value)->Is_Valid()) 
                {
                    _bool bSelected = false;
                    if (get<1>(Value) == m_pCurSelectInstanceObject)
                    {
                        bSelected = true;
                    }
                    if (ImGui::Selectable(to_string(Index).c_str(), bSelected))
                    {
                        m_pCurSelectInstanceObject = get<1>(Value);
                        m_iCurSelectInstanceObjectIndex = Index;
                    }
                    Index++;
                }
            }
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Merge InstanceObject"))
    {
        Merge_Instance();
    }
    ImGui::SameLine();
    if (ImGui::Button("Split InstanceObject"))
    {
        Split_Instance();
    }
    ImGui::Spacing();

    if (ImGui::Button("Delete InstanceObject"))
    {
        Delete_InstanceObject();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear InstanceGroup"))
    {
        Clear_InstanceGroup();
    }
    ImGui::Spacing();
}
void CWindow_Map::Make_InstanceObject()
{
    if (nullptr == m_pCurTerrain)
        return;
    list<_uint> VertsList = Select_Vertices();
    if (VertsList.empty())
        return;


    size_t HashNum = Convert_ToHash( m_strCurSelectInstanceMeshName);
    INSTANCEGROUPING::iterator InstanceIter = m_InstanceMap.find(HashNum);

    if (InstanceIter == m_InstanceMap.end())
    {
        m_InstanceMap.emplace(HashNum, INSTANCEVECTOR());
        m_strArrInstanceMeshName.push_back(m_strCurSelectInstanceMeshName);
    }

    _float4 OutPos = get<PICK_OUTLOCALPOS>(m_OutDatas);
    MTINSTANCE_DATA tInstanceData;
    tInstanceData.strInstanceGorupName = CFunctor::To_Wstring(m_strCurSelectInstanceMeshName);
    tInstanceData.strMeshPath = CFunctor::To_Wstring(m_strCurSelectInstanceMeshPath);
    tInstanceData.iInstanceNums = m_iDrawInstanceObjectNums;
    //여기서 로컬위치 잡아 --> 인스턴스들을 가진 오브젝트의 위치
    tInstanceData.InstancePosition = OutPos;
    tInstanceData.ArrInstanceVTX = new VTXINSTANCE[m_iDrawInstanceObjectNums];
    //생성 전 피킹된 위치 근처의 지형의 정점 가져오기
    _float4x4 CurTerrainMat = m_pCurTerrain->Get_Transform()->Get_WorldMatrix().XMLoad();
    _float4x4 CurTerrainMatInv = CurTerrainMat;
    CurTerrainMatInv = CurTerrainMatInv.Inverse();
    for (_int i = 0; i < m_iDrawInstanceObjectNums; ++i)
    {
        _float4 vInstancePosition = _float4(0.f , 0.f , 0.f , 1.f);
        _float fRatio = frandom(0.f, m_fBrushSize * 0.5f);
        //랜덤 증가치 할당 해
        vInstancePosition
            = _float4(
                cosf(frandom(0.f, 360.f)) * fRatio
                , 0.f
                , sinf(frandom(0.f, 360.f)) * fRatio
                , 1.f);
        //인스턴스 깔 위치 잡아 --> 로컬좌표임

        //터레인 픽킹 위치 잡아 --> 로컬좌표임
        _float4 TerrainPickPos = vInstancePosition.XMLoad() + tInstanceData.InstancePosition.XMLoad();
        TerrainPickPos.y += 10000.f;

        _float4 vPickOutPos;
        _float4 vPickOutNormal;
        //해당 정점 리스트의 피킹 --> 인스턴스 어레이로..
        if (Picked_VertList(VertsList, TerrainPickPos, vPickOutPos, vPickOutNormal))//터레인 정점리스트 피킹
        {
            // 위치와 노멀 받아서 적용
            //여기서 나온 vPickOutPos는 로컬좌표임
            //tInstanceData.InstancePosition.y += (vPickOutPos.y - OutPos.y);

            //로컬 좌표상의 데이터를 넘겨줌 --> 원점을 중심으로 오브젝트 인덱스들을 배치한 다음, 터레인 위치상에 픽킹 좌표로 옮겨줘야 함
            _float4 vObjectPosition = _float4(0.f, 0.f, 0.f, 1.f).XMLoad() + vInstancePosition.XMLoad();
            vObjectPosition = vObjectPosition.MultiplyCoord(CurTerrainMat);
            
            _float4 vWorldPickPos = vPickOutPos;
            vWorldPickPos = vWorldPickPos.MultiplyCoord(CurTerrainMat);

            _float4 vInstanceWorldPos = tInstanceData.InstancePosition;
            vInstanceWorldPos = vInstanceWorldPos.MultiplyCoord(CurTerrainMat);

            vObjectPosition.x += (vWorldPickPos.x - vInstanceWorldPos.x);
            vObjectPosition.y += (vWorldPickPos.y - vInstanceWorldPos.y);
            vObjectPosition.z += (vWorldPickPos.z - vInstanceWorldPos.z);
            vObjectPosition.w = 1.f;

            vObjectPosition = vObjectPosition.MultiplyCoord(CurTerrainMatInv);

            _matrix Mat = XMMatrixIdentity();
            _float4 IntiMatUp = -vPickOutNormal.XMLoad();
            _float4 IntiMatRight = XMVector3Cross(IntiMatUp.XMLoad(), XMVectorSet(0.f, 0.f, 1.f, 0.f));
            _float4 IntiMatLook = XMVector3Cross(IntiMatRight.XMLoad(), IntiMatUp.XMLoad());

            Mat.r[0] = IntiMatRight.XMLoad();
            Mat.r[1] = IntiMatUp.XMLoad();
            Mat.r[2] = IntiMatLook.XMLoad();

            _float4x4 mat = Mat;
            CUtility_Transform::Turn_ByAngle_Up(mat, frandom(0.f, 360.f));

            _float4 vRight = mat.XMLoad().r[0];
            _float4 vUp = mat.XMLoad().r[1];
            _float4 vLook = mat.XMLoad().r[2];

            tInstanceData.ArrInstanceVTX[i].vRight = vRight;
            tInstanceData.ArrInstanceVTX[i].vUp = vUp;
            tInstanceData.ArrInstanceVTX[i].vLook = vLook;
            tInstanceData.ArrInstanceVTX[i].vTranslation = vObjectPosition;
        }//성공
        else
        {
            i--;
            continue;
        }//실패

    }

    if (nullptr != tInstanceData.ArrInstanceVTX) 
    {
        // 인스턴스 오브젝트 생성
        CStructure_Instance* pInstanceStructure = CStructure_Instance::Create(
            tInstanceData.strMeshPath.c_str(),
            m_iDrawInstanceObjectNums,
            tInstanceData.ArrInstanceVTX);
        if (nullptr == pInstanceStructure)
            assert(0);
        pInstanceStructure->Initialize();
        CREATE_GAMEOBJECT(pInstanceStructure, GROUP_DECORATION);
        _float4 SetUpPos = OutPos.MultiplyCoord(CurTerrainMat);
        tInstanceData.InstancePosition = SetUpPos;
        //여기서 월드상의 좌표로 옮겨줌
        pInstanceStructure->Get_Transform()->Set_World(WORLD_POS, tInstanceData.InstancePosition);
        m_pCurSelectInstanceObject = pInstanceStructure;
        m_InstanceMap[HashNum].push_back(make_tuple(tInstanceData, pInstanceStructure));
    }
}
void CWindow_Map::Delete_InstanceObject()
{
    if (m_iCurSelectInstanceNameIndex >= _int(m_strArrInstanceMeshName.size()))
        return;
    size_t HashNum = Convert_ToHash( m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex]);
    INSTANCEGROUPING::iterator InstanceIter = m_InstanceMap.find(HashNum);
    if (m_InstanceMap.end() != InstanceIter)
    {
        INSTANCEVECTOR& InstanceGroup = InstanceIter->second;
        VTXINSTANCE* pInstanceVtx = get<0>(InstanceGroup[m_iCurSelectInstanceObjectIndex]).ArrInstanceVTX;
        CGameObject* pSelectObject = get<1>(InstanceGroup[m_iCurSelectInstanceObjectIndex]);

        Safe_Delete_Array(pInstanceVtx);
        DELETE_GAMEOBJECT(pSelectObject);

        INSTANCEVECTOR::iterator VectorIter = InstanceGroup.begin();

        for (_int i = 0; i < m_iCurSelectInstanceObjectIndex; ++i)
            VectorIter++;
        InstanceGroup.erase(VectorIter);
        m_pCurSelectInstanceObject = nullptr;
        m_iCurSelectInstanceObjectIndex--;
        if (InstanceGroup.empty())
        {
            m_iCurSelectInstanceObjectIndex = 0;
            m_InstanceMap.erase(InstanceIter);
            vector<string>::iterator ObjNameIter = m_strArrInstanceMeshName.begin();
            for (_int i = 0; i < m_iCurSelectInstanceNameIndex; ++i)
                ObjNameIter++;
            m_strArrInstanceMeshName.erase(ObjNameIter);
            m_iCurSelectInstanceNameIndex--;
            if (0 > m_iCurSelectInstanceNameIndex)
                m_iCurSelectInstanceNameIndex = 0;
        }
    }

}
void CWindow_Map::Clear_InstanceGroup()
{
    if (m_iCurSelectInstanceNameIndex >= _int(m_strArrInstanceMeshName.size()))
        return;
    size_t HashNum = Convert_ToHash( m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex]);
    INSTANCEGROUPING::iterator InstanceIter = m_InstanceMap.find(HashNum);
    if (m_InstanceMap.end() != InstanceIter)
    {
        for (INSTANCEVECTOR::value_type& Value : InstanceIter->second)
        {
            Safe_Delete_Array(get<0>(Value).ArrInstanceVTX);
            DELETE_GAMEOBJECT(get<1>(Value));
        }
        InstanceIter->second.clear();
        if (InstanceIter->second.empty())
        {
            m_pCurSelectInstanceObject = nullptr;
            m_iCurSelectInstanceObjectIndex = 0;
            m_InstanceMap.erase(InstanceIter);
            vector<string>::iterator ObjNameIter = m_strArrInstanceMeshName.begin();
            for (_int i = 0; i < m_iCurSelectInstanceNameIndex; ++i)
                ObjNameIter++;
            m_strArrInstanceMeshName.erase(ObjNameIter);
            m_iCurSelectInstanceNameIndex--;
            if (0 > m_iCurSelectInstanceNameIndex)
                m_iCurSelectInstanceNameIndex = 0;
        }
    }
}
void CWindow_Map::Merge_Instance()
{
    if (m_iCurSelectInstanceNameIndex >= _int(m_strArrInstanceMeshName.size()))
        return;
    size_t HashNum = Convert_ToHash( m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex]);
    INSTANCEGROUPING::iterator InstanceIter = m_InstanceMap.find(HashNum);

    if (m_InstanceMap.end() != InstanceIter)
    {
        wstring strGroupname = get<0>(InstanceIter->second.front()).strInstanceGorupName;
        wstring strPath = get<0>(InstanceIter->second.front()).strMeshPath;
        _float4 IsntancePos = get<0>(InstanceIter->second.front()).InstancePosition;
        _int TotalInstanceNums = 0;
        for (INSTANCEVECTOR::value_type& Value : InstanceIter->second)
        {
            TotalInstanceNums += get<0>(Value).iInstanceNums;
            DISABLE_GAMEOBJECT(get<1>(Value));
        }

        _int PrevNumInstance = 0;
        VTXINSTANCE* pInstanceVtx = new VTXINSTANCE[TotalInstanceNums];
        for (INSTANCEVECTOR::value_type& Value : InstanceIter->second)
        {
            memcpy(pInstanceVtx + PrevNumInstance
                , get<0>(Value).ArrInstanceVTX
                , sizeof(VTXINSTANCE) * get<0>(Value).iInstanceNums);
            PrevNumInstance += get<0>(Value).iInstanceNums;
        }

        PrevNumInstance = 0;
        for (INSTANCEVECTOR::value_type& Value : InstanceIter->second)
        {
            for (_int i = 0; i < _int(get<0>(Value).iInstanceNums); ++i)
            {
                pInstanceVtx[i + PrevNumInstance].vTranslation.x += get<0>(Value).InstancePosition.x;
                pInstanceVtx[i + PrevNumInstance].vTranslation.y += get<0>(Value).InstancePosition.y;
                pInstanceVtx[i + PrevNumInstance].vTranslation.z += get<0>(Value).InstancePosition.z;
            }
            PrevNumInstance += get<0>(Value).iInstanceNums;
        }

        size_t HashNum = Convert_ToHash( CFunctor::To_String(strGroupname));
        map<size_t, CGameObject*>::iterator MergeInstanceIter = m_MergeObjects.find(HashNum);
        if (MergeInstanceIter != m_MergeObjects.end()) 
        {
            DELETE_GAMEOBJECT(MergeInstanceIter->second);
            m_MergeObjects.erase(MergeInstanceIter);
        }
        CStructure_Instance* pInstanceObject = CStructure_Instance::Create(strPath, TotalInstanceNums, pInstanceVtx);
        pInstanceObject->Initialize();
        CREATE_GAMEOBJECT(pInstanceObject, GROUP_DECORATION);
        //pInstanceObject->Get_Transform()->Set_World(WORLD_POS, IsntancePos);
        m_MergeObjects.emplace(HashNum, pInstanceObject);
        Safe_Delete_Array(pInstanceVtx);
    }

}
void CWindow_Map::Split_Instance()
{
    if (m_iCurSelectInstanceNameIndex >= _int(m_strArrInstanceMeshName.size()))
        return;
    size_t HashNum = Convert_ToHash( m_strArrInstanceMeshName[m_iCurSelectInstanceNameIndex]);
    map<size_t, CGameObject*>::iterator MergeInstanceIter = m_MergeObjects.find(HashNum);

    if (m_MergeObjects.end() != MergeInstanceIter)
    {
        DELETE_GAMEOBJECT(MergeInstanceIter->second);
        m_MergeObjects.erase(MergeInstanceIter);
    }

    INSTANCEGROUPING::iterator InstanceIter = m_InstanceMap.find(HashNum);
    if (m_InstanceMap.end() != InstanceIter)
    {
        for (INSTANCEVECTOR::value_type& Value : InstanceIter->second)
        {
            ENABLE_GAMEOBJECT(get<1>(Value));
        }
    }
}

_bool CWindow_Map::Calculate_Pick()
{
    _bool bPicked = false;
    if (m_bHoverWindow)
        return bPicked;
    if (nullptr == m_PickTargetMesh)
        return bPicked;
    _float4x4 OwnerMat = m_PickTargetMesh->Get_Owner()->Get_Transform()->Get_WorldMatrix();

    if (KEY(LBUTTON, TAP))
    {
        _float4 OutPos;
        _float4 OutNorm;
        if (PICK_NONE == m_ePickingType)
            return bPicked;
        if (GAMEINSTANCE->Is_Picked_Mesh(m_PickTargetMesh, &m_i3PickedIndex, &OutPos, &OutNorm))
        {

            _float4 OutLocalPos = OutPos.MultiplyCoord(OwnerMat.Inverse());
            m_OutDatas = make_tuple(OutPos, OutLocalPos, OutNorm);
            bPicked = true;
            switch (m_ePickingType)
            {
            case PICK_CLONE:
                m_pObjectController->Func_PickStart();
                break;
            }

        }
    }


    if (KEY(LBUTTON, HOLD))
    {
        m_fDelayTimeAcc += fDT(0);
        if (m_fDelayTimeAcc < m_fTimeDelay)
            return false;
        else
        {
            m_fDelayTimeAcc = 0.f;

        }




        _float4 OutPos;
        _float4 OutNorm;
        if (PICK_NONE == m_ePickingType)
            return bPicked;
        if (GAMEINSTANCE->Is_Picked_Mesh(m_PickTargetMesh, &m_i3PickedIndex, &OutPos, &OutNorm))
        {
            _float4 OutLocalPos = OutPos.MultiplyCoord(OwnerMat.Inverse());
            m_OutDatas = make_tuple(OutPos, OutLocalPos, OutNorm);
            bPicked = true;
            switch (m_ePickingType)
            {
            case PICK_OBJECT:
            case PICK_ANCHOR:
                //Place_Object();
                //Change_Object_UpDir();
                m_pObjectController->Func_Picking();
                break;
                if (m_bTerrainPick) {
                case PICK_TERRAINVERT:
                    Increase_Height();
                    break;
                case PICK_TERRAINTEX:
                    Change_TileTexture();
                    break;
                case PICK_INSTANCEOBJECT:
                    Make_InstanceObject();
                    break;
                }
            }

        }
    }

    if (KEY(LBUTTON, AWAY))
    {
        _float4 OutPos;
        _float4 OutNorm;
        if (PICK_NONE == m_ePickingType)
            return bPicked;
        if (GAMEINSTANCE->Is_Picked_Mesh(m_PickTargetMesh, &m_i3PickedIndex, &OutPos, &OutNorm))
        {
            _float4 OutLocalPos = OutPos.MultiplyCoord(OwnerMat.Inverse());
            m_OutDatas = make_tuple(OutPos, OutLocalPos, OutNorm);
            bPicked = true;
            switch (m_ePickingType)
            {
            case PICK_CLONE:
                m_pObjectController->Func_PickEnd();
                break;
            }

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
    m_bHoverWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
        || ImGui::IsAnyItemHovered();
        //&& ImGui::IsMouseClicked(ImGuiMouseButton_Left);

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
void CWindow_Map::DebugData(const char* szTitleName, string& strData, ImVec4 Color)
{
    char szTitle[MAXCHAR] = "";
    strcat_s(szTitle, szTitleName);
    strcat_s(szTitle, " : ");
    ImGui::TextColored(Color, szTitle);
    ImGui::SameLine();
    ImGui::TextColored(Color, strData.c_str());
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
                _int iSlash = _int(strFileName.rfind("/")) + 1;
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

           /* if (strExtName == "dat")
                continue;*/
        }

        tRootTree.vecChildren.push_back(tTreeData);

    }
}
void CWindow_Map::Show_TreeData(void* tTree, function<void(void*)> SelectFunction)
{
    TREE_DATA* pTreeData = reinterpret_cast<TREE_DATA*>(tTree);
    if (!(*pTreeData).vecChildren.empty())
    {
        if (ImGui::TreeNode((*pTreeData).strFileName.c_str()))
        {
            for (auto& tTreeData : (*pTreeData).vecChildren)
            {
                Show_TreeData((void*)&tTreeData, SelectFunction);
            }

            ImGui::TreePop();
        }

    }
    else
    {
        SelectFunction(tTree);
    }
}
string CWindow_Map::CutOut_Ext(string& Origin, string& Ext)
{
    string strReturn = Origin;
    size_t ExtLength = Ext.size() + 1;
    strReturn = strReturn.substr(0, strReturn.size() - ExtLength);
    return strReturn;
}


void CWindow_Map::Routine_InstanceMeshSelect(void* tTreeNode)
{
    _bool bSelected = false;
    CWindow_Map::TREE_DATA* pTreeNode = reinterpret_cast<CWindow_Map::TREE_DATA*>(tTreeNode);
    if (nullptr == pTreeNode)
        return;
    if (m_strCurSelectInstanceMeshPath == (*pTreeNode).strFullPath)
    {
        bSelected = true;
    }
    if (ImGui::Selectable((*pTreeNode).strFileName.c_str(), bSelected))
    {
        m_strCurSelectInstanceMeshPath = (*pTreeNode).strFullPath;
        m_strCurSelectInstanceMeshName = (*pTreeNode).strFileName;
        _int CutLength = _int(m_strCurSelectInstanceMeshName.rfind(".")) - 1;
        m_strCurSelectInstanceMeshName = m_strCurSelectInstanceMeshName.substr(0, CutLength);
    }
}


HRESULT CWindow_Map::SetUp_Cameras()
{
    CCamera_FixedAngle* pRightCamera = CCamera_FixedAngle::Create(_float4(-1.f, 0.f, 0.f, 0.f));
    CREATE_STATIC(pRightCamera, Convert_ToHash(wstring(TEXT("RightCamera"))));
    DISABLE_GAMEOBJECT(pRightCamera);
    CGameInstance::Get_Instance()->Add_Camera(L"RightCam", pRightCamera);
    m_ArrCams.push_back(make_tuple(L"RightCam", pRightCamera, _float4(2.f, 0.f, 1.f, 1.f)));

    CCamera_FixedAngle* pUpCamera = CCamera_FixedAngle::Create(_float4(0.f, -1.f, 0.f, 0.f), _float4(0.f, 0.f, 1.f, 0.f));
    CREATE_STATIC(pUpCamera, Convert_ToHash(wstring(TEXT("UpCamera"))));
    DISABLE_GAMEOBJECT(pUpCamera);
    CGameInstance::Get_Instance()->Add_Camera(L"UpCam", pUpCamera);
    m_ArrCams.push_back(make_tuple(L"UpCam", pUpCamera, _float4(1.f, 100.f, 1.f, 1.f)));

    CCamera_FixedAngle* pLookCamera = CCamera_FixedAngle::Create(_float4(0.f, 0.f, -1.f, 0.f));
    CREATE_STATIC(pLookCamera, Convert_ToHash(wstring(TEXT("LookCamera"))));
    DISABLE_GAMEOBJECT(pLookCamera);
    CGameInstance::Get_Instance()->Add_Camera(L"LookCam", pLookCamera);
    m_ArrCams.push_back(make_tuple(L"LookCam", pLookCamera, _float4(1.f, 0.f, 2.f, 1.f)));

    return S_OK;
}

void CWindow_Map::Select_Camera()
{
    //if (KEY(NUM1, TAP))
    //{
    //    Change_Camera(CAM_RIGHT);
    //}
    //if (KEY(NUM2, TAP))
    //{
    //    Change_Camera(CAM_UP);
    //}
    //if (KEY(NUM3, TAP))
    //{
    //    Change_Camera(CAM_LOOK);
    //}
    //if (KEY(NUM4, TAP))
    //{
    //    Change_Camera(CAM_FREE);
    //}
}

void CWindow_Map::Change_Camera(_int Index)
{
    if (nullptr == m_pCurTerrain || CAM_FREE == Index || m_ArrCams.size() <= Index)
    {
        GAMEINSTANCE->Change_Camera(L"FreeCam");
        return;
    }
    CCamera* pCamera = nullptr;
    GAMEINSTANCE->Change_Camera(get<0>(m_ArrCams[Index]));
    pCamera = (get<1>(m_ArrCams[Index]));
    _float TerrainVertX = _float(m_pCurTerrain->Get_TerrainVerticesX());
    _float TerrainVertZ = _float(m_pCurTerrain->Get_TerrainVerticesZ());

    _float4 SetUpPosition = get<2>(m_ArrCams[Index]);
    SetUpPosition.x *= (TerrainVertX * 0.5f);
    SetUpPosition.z *= (TerrainVertZ * 0.5f);

    pCamera->Get_Transform()->Set_World(WORLD_POS, SetUpPosition);
}

void CWindow_Map::Set_BrushInform()
{
    if (ImGui::CollapsingHeader("Brush Setting"))
    {
        ImGui::Text("Brush Type");
        Make_Combo("##Brush Combo", m_arrBrushType, &m_iCurSelectTerrainBrush, bind(&CWindow_Map::EmptyFunction, this));
        ImGui::Spacing();

        ImGui::Text("Brush Size");
        ImGui::DragFloat("##BrushSize", &m_fBrushSize, 0.01f, 0.f, 100.f, "%.3f");
        ImGui::Spacing();


        ImGui::Text("Increase Value");
        ImGui::DragFloat("##IncreaseValue", &m_fBrushWeight, 0.1f, 0.f, 100.f, "%.3f");
        ImGui::Spacing();
    }
}

_bool CWindow_Map::Picked_VertList(list<_uint>& VertsList, _float4 vPosition, _float4& OutLocalPos, _float4& OutNormal)
{
    //_float4x4	matWorld = m_pCurTerrain->Get_Transform()->Get_WorldMatrix();
    //_matrix		WorldMatrixInv = matWorld.Inverse().XMLoad();
    //matWorld.Inverse();
    _float3     f3Position = _float3(vPosition.x, vPosition.y, vPosition.z);
    _vector			xRayPos, xRayDir;
    xRayPos = vPosition.XMLoad(); //XMVector3TransformCoord(XMLoadFloat3(&f3Position), WorldMatrixInv);
    xRayDir = XMVectorSet(0.f, -1.f, 0.f, 0.f); //XMVector3Normalize(XMVector3TransformNormal(XMVectorSet(0.f, -1.f, 0.f, 0.f), WorldMatrixInv));

    _uint VertXNum = m_pCurTerrain->Get_TerrainVerticesX();
    _uint VertZNum = m_pCurTerrain->Get_TerrainVerticesZ();
    _float3* vTerrainVertPos = m_pCurTerrain->Get_TerrainVerticesPos();
    _float		fDist, fMin = 999999.f;

    for (list<_uint>::value_type& Value : VertsList)
    {
        if (Value / VertXNum >= VertZNum - 1)
            continue;
        if (Value % VertZNum >= VertXNum - 1)
            continue;

        _uint Index[] =
        {
            Value + VertXNum,
            Value + VertXNum + 1,
            Value + 1,
            Value
        };

        _vector		vVecA = XMLoadFloat3(&vTerrainVertPos[Index[0]]);
        _vector 	vVecB = XMLoadFloat3(&vTerrainVertPos[Index[1]]);
        _vector 	vVecC = XMLoadFloat3(&vTerrainVertPos[Index[2]]);
        _vector 	vVecD = XMLoadFloat3(&vTerrainVertPos[Index[3]]);


        //Cross AB, AT && Cross AT

        if (true == TriangleTests::Intersects(xRayPos, xRayDir, vVecA, vVecB, vVecC, fDist))
        {
            _float4 V1, V2;
            _float4 vOutNormal, vPickedPos;
            //_float4x4 worldMat = matWorld;

            V1 = (vVecA - vVecB);
            V2 = (vVecC - vVecB);

            vOutNormal = XMVector3Cross(V1.Normalize().XMLoad(), V2.Normalize().XMLoad());
            //vOutNormal = vOutNormal.MultiplyNormal(worldMat);
            vOutNormal = vOutNormal.Normalize();
            vPickedPos = xRayPos + XMVector3Normalize(xRayDir) * fDist;
            //vPickedPos = vPickedPos.MultiplyCoord(worldMat);
            _float4 vRayPos = _float4(vPosition.x, vPosition.y, vPosition.z, 1.f);

            _float fDistance = (vRayPos - vPickedPos).Length();

            if (fMin > fDistance)
            {
                OutNormal = vOutNormal;
                OutLocalPos = vPickedPos;

                fMin = fDistance;
            }
        }
        else
        {
            if (true == TriangleTests::Intersects(xRayPos, xRayDir, vVecA, vVecC, vVecD, fDist))
            {
                _float4 V1, V2;
                _float4 vOutNormal, vPickedPos;
                //_float4x4 worldMat = matWorld;

                V1 = (vVecA - vVecC);
                V2 = (vVecD - vVecC);

                vOutNormal = XMVector3Cross(V1.Normalize().XMLoad(), V2.Normalize().XMLoad());
                //vOutNormal = vOutNormal.MultiplyNormal(worldMat);
                vOutNormal = vOutNormal.Normalize();
                vPickedPos = xRayPos + XMVector3Normalize(xRayDir) * fDist;

                _float4 vRayPos = _float4(vPosition.x, vPosition.y, vPosition.z, 1.f);

                _float fDistance = (vRayPos - vPickedPos).Length();

                if (fMin > fDistance)
                {
                    OutNormal = vOutNormal;
                    OutLocalPos = vPickedPos;

                    fMin = fDistance;
                }
            }
        }
    }

    if (fMin != 999999.f)
    {
        return true;
    }

    return false;
}


#pragma endregion

#pragma region MAPDATA 멤버함수

void CWindow_Map::MAPDATA::Initialize()
{
    TerrainDataPath = wstring();
    ObjectDataPath = wstring();
    InstanceDataPath = wstring();
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
    ObjectDataPath += TEXT(".ObjectData");

    InstanceDataPath = CFunctor::To_Wstring(BasePath);
    InstanceDataPath += TEXT("InstanceData/");
    InstanceDataPath += CFunctor::To_Wstring(DataName);
    InstanceDataPath += TEXT(".InstanceData");

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
        if (FAILED(SavePath(rhsWriteFile, InstanceDataPath)))
            throw InstanceDataPath;
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
        if (FAILED(LoadPath(rhsReadFile, InstanceDataPath)))
            throw InstanceDataPath;
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



void CWindow_Map::tagMapToolTerrainData::Initialize()
{
    strTileTexturePath = wstring();
    iNumVerticesX = 0;
    iNumVerticesZ = 0;
    pCurTerrainVertPos = nullptr;
    pCurTerrainColor = nullptr;
}

void CWindow_Map::tagMapToolTerrainData::Make_Data(tagMapToolTerrainData::Terrain_TUPLE& tTerrainData)
{
    Initialize();
    strTileTexturePath = get<Tuple_TileTexture>(tTerrainData);
    iNumVerticesX = get<Tuple_VerticesX>(tTerrainData);
    iNumVerticesZ = get<Tuple_VerticesZ>(tTerrainData);
    pCurTerrainVertPos = get<Tuple_TerrainPosPtr>(tTerrainData);
    pCurTerrainColor = get<Tuple_TerrainColorPtr>(tTerrainData);
}

void CWindow_Map::tagMapToolInstanceObjData::Initialize()
{
    strInstanceGorupName = wstring(TEXT(""));
    strMeshPath = wstring(TEXT(""));
    iInstanceNums = 0;
    InstancePosition = _float4(0.f, 0.f, 0.f, 1.f);
    ArrInstanceVTX = nullptr;
}

void CWindow_Map::tagMapToolInstanceObjData::Make_Data(InstancingTuple& tInstancingData)
{
    Initialize();
    strInstanceGorupName = get<Tuple_GroupName>(tInstancingData);
    strMeshPath = get<Tuple_MeshPath>(tInstancingData);
    iInstanceNums = get<Tuple_InstanceNums>(tInstancingData);
    InstancePosition = get<Tuple_Positon>(tInstancingData);
    ArrInstanceVTX = get<Tuple_VTXArrayPtr>(tInstancingData);
}

void CWindow_Map::tagMapToolInstanceObjData::Save(ofstream& wirteFile)
{
    _int GroupNameLength = _int(strInstanceGorupName.length()) + 1;
    wirteFile.write((char*)&GroupNameLength, sizeof(_int));
    char* pGroupName = new char[GroupNameLength];
    strcpy_s(pGroupName, sizeof(char) * GroupNameLength,CFunctor::To_String(strInstanceGorupName).c_str());
    wirteFile.write(pGroupName, sizeof(char)* GroupNameLength);
    Safe_Delete_Array(pGroupName);

    _int PathLength = _int(strMeshPath.length()) + 1;
    wirteFile.write((char*)&PathLength, sizeof(_int));
    char* pPathName = new char[PathLength];
    strcpy_s(pPathName, sizeof(char) * PathLength, CFunctor::To_String(strMeshPath).c_str());
    wirteFile.write(pPathName, sizeof(char) * PathLength);
    Safe_Delete_Array(pPathName);

    wirteFile.write((char*)&iInstanceNums, sizeof(_int));
    wirteFile.write((char*)&InstancePosition, sizeof(_float4));
    wirteFile.write((char*)ArrInstanceVTX, sizeof(VTXINSTANCE)* iInstanceNums);

}

void CWindow_Map::tagMapToolInstanceObjData::Load(ifstream& readFile)
{
    _int GroupNameLength = 0;
    readFile.read((char*)&GroupNameLength, sizeof(_int));
    char* pGroupName = new char[GroupNameLength];
    readFile.read(pGroupName, sizeof(char)* GroupNameLength);
    string strGroupName = pGroupName;
    Safe_Delete_Array(pGroupName);
    strInstanceGorupName = CFunctor::To_Wstring(strGroupName);

    _int PathLength = 0;
    readFile.read((char*)&PathLength, sizeof(_int));
    char* pPath = new char[PathLength];
    readFile.read(pPath, sizeof(char) * PathLength);
    string strPath = pPath;
    Safe_Delete_Array(pPath);
    strMeshPath = CFunctor::To_Wstring(strPath);

    readFile.read((char*)&iInstanceNums, sizeof(_int));
    readFile.read((char*)&InstancePosition, sizeof(_float4));

    ArrInstanceVTX = new VTXINSTANCE[iInstanceNums];
    readFile.read((char*)ArrInstanceVTX, sizeof(VTXINSTANCE) * iInstanceNums);

}

