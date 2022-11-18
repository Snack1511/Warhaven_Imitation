#include "CMap_Loader.h"
#include "Functor.h"
#include "CDrawable_Terrain.h"
#include "CStructure.h"
#include "CStructure_Instance.h"

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
    rhsReadFile.read((char*)&GroupSize, sizeof(_int));
    for (_int i = 0; i < GroupSize; ++i)
    {
        //Loop~ 그룹 이름 저장
        _int ObjectGroupNameLength = 0;
        rhsReadFile.read((char*)&ObjectGroupNameLength, sizeof(_int));

        char pSkip[MAXCHAR] = "";
        rhsReadFile.read(pSkip, sizeof(char) * ObjectGroupNameLength);
        _int ObjectCount = 0;
        rhsReadFile.read((char*)&ObjectCount, sizeof(_int));
        for (_int j = 0; j < ObjectCount; ++j)
        {
            _int ObjectNameLength = 0;
            rhsReadFile.read((char*)&ObjectNameLength, sizeof(_int));
            rhsReadFile.read(pSkip, sizeof(char) * ObjectNameLength);

            //경로 저장
            _int ObjectPathLength = 0;
            rhsReadFile.read((char*)&ObjectPathLength, sizeof(_int));
            char pObjectPath[MAXCHAR]="";
            rhsReadFile.read(pObjectPath, sizeof(char) * ObjectPathLength);
            string strObjPath = pObjectPath;
            wstring wstrObjPath = CFunctor::To_Wstring(strObjPath);

            //행렬 저장
            _float4x4 StateMatrix;
            rhsReadFile.read((char*)&StateMatrix, sizeof(_float4x4));

            //스케일 저장
            _float4 vScale;
            rhsReadFile.read((char*)&vScale, sizeof(_float4));

            //라이트플래그 저장
            _byte LightFlag = 0;
            rhsReadFile.read((char*)&LightFlag, sizeof(_byte));

            CStructure* pGameObject = CStructure::Create(wstrObjPath, vScale, StateMatrix);
            if (nullptr == pGameObject)
                assert(0);
            pGameObject->Initialize();
            Ready_Object(pGameObject, GROUP_DECORATION);

            //콜라이더 타입
            _uint ColType = 0;
            rhsReadFile.read((char*)&ColType, sizeof(_uint));

            //LOD
            _uint LODType = 0;
            rhsReadFile.read((char*)&LODType, sizeof(_uint));
            pGameObject->Make_PhysXCollider(CStructure::ePhysXEnum(ColType), LODType);

            //박스콜라이더 수
            _uint BoxCount = 0;
            rhsReadFile.read((char*)&BoxCount, sizeof(_uint));

            for (_uint i = 0; i < BoxCount; ++i)
            {
                pGameObject->Make_PhysXCollier_Box();
            }
            for (_uint i = 0; i < BoxCount; ++i)
            {
                _float4 vPosition = _float4(0.f, 0.f, 0.f, 1.f);
                _float4 vAngle = _float4(0.f, 0.f, 0.f, 0.f);
                _float4 vScale = _float4(0.f, 0.f, 0.f, 0.f);

                rhsReadFile.read((char*)&vPosition, sizeof(_float4));
                rhsReadFile.read((char*)&vAngle, sizeof(_float4));
                rhsReadFile.read((char*)&vScale, sizeof(_float4));

                pGameObject->RePosition_Box(i, vPosition);
                pGameObject->ReScale_Box(i, vScale);
                pGameObject->Rotate_Box(i, vAngle);
            }

            //박스콜라이더 정보 저장

            //Safe_Delete_Array(pObjectName);
        }


    }
    rhsReadFile.close();

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
    m_strDataPath += TEXT("../Bin/Data/MapData/");
    m_strDataPath += strDataPath;
    m_strDataPath += TEXT(".MapData");

    Ready_Object = Func_ReadyObject;

    return S_OK;
}
