#include "CMap_Loader.h"
#include "Functor.h"
#include "CDrawable_Terrain.h"
#include "CStructure.h"
#include "CStructure_Instance.h"
#include "CMapColliders.h"
CMap_Loader::CMap_Loader()
{
}

CMap_Loader::~CMap_Loader()
{
}

void CMap_Loader::Load_Data(wstring strPath, function<void(CGameObject*, _uint)> Func_ReadyObject, _int Index)
{
    CMap_Loader* pInstance = new CMap_Loader();
    if (FAILED(pInstance->SetUp_LoadOption(strPath, Func_ReadyObject, Index)))
    {
        Call_MsgBox(TEXT("Fail to SetUp_LoadOption : CMap_Loader"));
        delete pInstance;
    }

    if (FAILED(pInstance->Load_MapData()))
    {
        Call_MsgBox(TEXT("Fail to Load_MapData : CMap_Loader"));
        delete pInstance;
    }  
    if (FAILED(pInstance->Load_ColliderData()))
    {
        Call_MsgBox(TEXT("Fail to Load_ColliderData : CMap_Loader"));
        delete pInstance;
    }

    delete pInstance;

}

HRESULT CMap_Loader::Load_MapData()
{
    string strData = CFunctor::To_String(m_strDataPath);
    ifstream readFile;
    readFile.open(strData.c_str(), ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(TEXT("안열리는데요?"));
        readFile.close();
        return E_FAIL;
    }
    string TerrainData, StructureData, InstancingData, NavData, LightData;
    try {
        if (FAILED(LoadPath(readFile, TerrainData)))
            throw 0;
        if (FAILED(LoadPath(readFile, StructureData)))
            throw 0;
        if (FAILED(LoadPath(readFile, InstancingData)))
            throw 0;
        if (FAILED(LoadPath(readFile, NavData)))
            throw 0;
        if (FAILED(LoadPath(readFile, LightData)))
            throw 0;

    }
    catch (_int ErrIndex)
    {
        Call_MsgBox(TEXT("1. 저장된 경로 없음\n2. 문자열 해제 안됨"));
    }
    readFile.close();


    try {
        if (FAILED(SetUp_TerrainData(readFile, TerrainData)))
            throw 0;
        if (FAILED(SetUp_StructureData(readFile, StructureData)))
            throw 0;
        if (FAILED(SetUp_InstancingData(readFile, InstancingData)))
            throw 0;
        if (FAILED(SetUp_NavData(readFile, NavData)))
            throw 0;
        if (FAILED(SetUp_LightData(readFile, LightData)))
            throw 0;
    }
    catch (_int ErrIndex)
    {
        Call_MsgBox(TEXT("객체 못만드는 대요..?"));
    }


    return S_OK;
}

HRESULT CMap_Loader::Load_ColliderData()
{

    CMapColliders* pMapColliders = CMapColliders::Create(CFunctor::To_String(m_strDataName));
    Ready_Object(pMapColliders, GROUP_DEFAULT);

    return S_OK;
}

HRESULT CMap_Loader::SetUp_TerrainData(ifstream& rhsReadFile, string& strPath)
{
    CDrawable_Terrain* pTerrain = nullptr;
    wstring strTerrainPath = CFunctor::To_Wstring(strPath);
    pTerrain = CDrawable_Terrain::Create(strTerrainPath.c_str());
    
    if (nullptr == pTerrain)
        assert(0);
    
    Ready_Object(pTerrain, GROUP_DECORATION);

    return S_OK;
}

HRESULT CMap_Loader::SetUp_StructureData(ifstream& rhsReadFile, string& strPath)
{
    string LoadFullPath = strPath;
    _int GroupSize = 0;
    //그룹 개수 저장
    rhsReadFile.open(LoadFullPath, ios::binary);
    if (!rhsReadFile.is_open())
    {
        Call_MsgBox(TEXT("Fail to Read : StructureData"));
        return E_FAIL;
    }
    
    string GroupFilePath;
    string SplitFilePath;
    string MergeFilePath;

    _uint GroupFilePathLength = 0;
    rhsReadFile.read((char*)&GroupFilePathLength, sizeof(_uint));
    char GroupPath[MAX_PATH] = "";
    rhsReadFile.read(GroupPath, sizeof(char) * GroupFilePathLength);
    GroupFilePath = GroupPath;

    _uint SplitFilePathLength = 0;
    rhsReadFile.read((char*)&SplitFilePathLength, sizeof(_uint));
    char SplitPath[MAX_PATH] = "";
    rhsReadFile.read(SplitPath, sizeof(char) * SplitFilePathLength);
    SplitFilePath = SplitPath;

    _uint MergeFilePathLength = 0;
    rhsReadFile.read((char*)&MergeFilePathLength, sizeof(_uint));
    char MergePath[MAX_PATH] = "";
    rhsReadFile.read(MergePath, sizeof(char) * MergeFilePathLength);
    MergeFilePath = MergePath;

    rhsReadFile.close();



    LoadFullPath = MergeFilePath;
    ifstream	readFile(LoadFullPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?!");
        assert(0);
    }

    _uint GroupMapSize = 0;
    readFile.read((char*)&GroupMapSize, sizeof(_uint));

    for (_uint i = 0; i < GroupMapSize; ++i)
    {
        _int NameLength = 0;
        char ObjectName[MAXCHAR] = "";
        readFile.read((char*)&NameLength, sizeof(_int));
        readFile.read(ObjectName, sizeof(char) * NameLength);

        _int PathLength = 0;
        char szMeshPath[MAX_PATH] = "";
        readFile.read((char*)&PathLength, sizeof(_int));
        readFile.read(szMeshPath, sizeof(char) * PathLength);

        wstring strMeshPath = CFunctor::To_Wstring(szMeshPath);
        _uint iInstanceNums = 0;
        readFile.read((char*)&iInstanceNums, sizeof(_uint));

        VTXINSTANCE* pInstance = new VTXINSTANCE[iInstanceNums];
        ZeroMemory(pInstance, sizeof(VTXINSTANCE) * iInstanceNums);
        readFile.read((char*)pInstance, sizeof(VTXINSTANCE) * iInstanceNums);

        CStructure_Instance* pInstanceStructure = CStructure_Instance::Create(strMeshPath, iInstanceNums, pInstance);
        pInstanceStructure->Initialize();
        Ready_Object(pInstanceStructure, GROUP_DECORATION);

        Safe_Delete_Array(pInstance);
    }

    readFile.close();


    return S_OK;
}

HRESULT CMap_Loader::SetUp_InstancingData(ifstream& rhsReadFile, string& strPath)
{
    string LoadFullPath = strPath;
    //그룹 개수 저장
    rhsReadFile.open(LoadFullPath, ios::binary);
    if (!rhsReadFile.is_open())
    {
        Call_MsgBox(TEXT("Fail to Read : InstancingData"));
        return E_FAIL;
    }

    _int PathLength = 0;
    char Path[MAX_PATH] = "";
    rhsReadFile.read((char*)&PathLength, sizeof(_int));
    rhsReadFile.read(Path, sizeof(char)* PathLength);//splitSkip;

    rhsReadFile.read((char*)&PathLength, sizeof(_int));
    rhsReadFile.read(Path, sizeof(char) * PathLength);
    rhsReadFile.close();
    string strMergePath = Path;

    rhsReadFile.open(strMergePath, ios::binary);
    if (!rhsReadFile.is_open())
    {
        Call_MsgBox(TEXT("Fail to Read : InstancingData"));
        return E_FAIL;
    }

    _int GroupSize = 0;
    rhsReadFile.read((char*)&GroupSize, sizeof(_int));
    for (_int i = 0; i < GroupSize; ++i)
    {
        _int PathLength = 0;
        char MeshPath[MAX_PATH] = "";
        _int TotalInstanceNum = 0;
        rhsReadFile.read((char*)&PathLength, sizeof(_int));
        rhsReadFile.read(MeshPath, sizeof(char) * PathLength);
        rhsReadFile.read((char*)&TotalInstanceNum, sizeof(_int));

        VTXINSTANCE* pInstance = new VTXINSTANCE[TotalInstanceNum];
        rhsReadFile.read((char*)pInstance, sizeof(VTXINSTANCE) * TotalInstanceNum);

        CStructure_Instance* pInstanceObject = CStructure_Instance::Create(CFunctor::To_Wstring(MeshPath), TotalInstanceNum, pInstance);
        pInstanceObject->Initialize();
        Ready_Object(pInstanceObject, GROUP_DECORATION);
        Safe_Delete_Array(pInstance);
    }
    return S_OK;
}

HRESULT CMap_Loader::SetUp_NavData(ifstream& rhsReadFile, string& strPath)
{
    return S_OK;
}

HRESULT CMap_Loader::SetUp_LightData(ifstream& rhsReadFile, string& strPath)
{
    return S_OK;
}

HRESULT CMap_Loader::LoadPath(ifstream& rhsReadFile, string& strPath)
{
    _int DataPathLength = 0;
    rhsReadFile.read((char*)&DataPathLength, sizeof(_int));

    if (0 > DataPathLength)
        return E_FAIL;

    char* szDataPath = new char[DataPathLength];
    rhsReadFile.read(szDataPath, sizeof(char) * DataPathLength);

    strPath = szDataPath;
    Safe_Delete_Array(szDataPath);

    if (nullptr != szDataPath)
        return E_FAIL;

    return S_OK;
}

HRESULT CMap_Loader::SetUp_LoadOption(wstring strDataPath, function<void(CGameObject*, _uint)> Func_ReadyObject, _int Index)
{
    m_strDataName = strDataPath;
    m_strDataPath += TEXT("../Bin/Data/MapData/");
    m_strDataPath += strDataPath;
    m_strDataPath += TEXT(".MapData");

    Ready_Object = Func_ReadyObject;

    return S_OK;
}
