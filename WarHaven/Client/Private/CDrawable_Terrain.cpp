#include "stdafx.h"
#include "CDrawable_Terrain.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "CTerrain_Renderer.h"

#include "CCell.h"

#include "PhysXCollider.h"

CDrawable_Terrain::CDrawable_Terrain()
{
}

CDrawable_Terrain::~CDrawable_Terrain()
{
}

CDrawable_Terrain* CDrawable_Terrain::Create(_uint iNumVerticesX, _uint iNumVerticesZ)
{
    CDrawable_Terrain* pInstance = new CDrawable_Terrain;
    if (FAILED(pInstance->SetUp_TerrainMesh(iNumVerticesX, iNumVerticesZ)))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->SetUp_TerrainTextures()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainTextures : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CTerrain");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

CDrawable_Terrain* CDrawable_Terrain::Create(const _tchar* pFilePath)
{
    CDrawable_Terrain* pInstance = new CDrawable_Terrain;

    if (FAILED(pInstance->SetUp_TerrainMesh(pFilePath)))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->SetUp_TerrainTextures()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainTextures : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }
    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDrawable_Terrain");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDrawable_Terrain::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXNOR,
        VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    //CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXNOR_PASS_TERRAIN);
    //Add_Component(pRenderer);
    //m_pRenderer = pRenderer;

    CTerrain_Renderer* pRenderer = CTerrain_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXNOR_PASS_TERRAIN);
    Add_Component<CRenderer>(pRenderer);
    m_pRenderer = pRenderer;

    //CTexture* pTexture = CTexture::Create(0, m_strTileTexturePath.c_str(), 1);
    //Add_Component(pTexture);
    //D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Resources\Textures\Terrain\LandScape\Ash
    //wstring strTestBasePath = L"../bin/resources/Textures/Terrain/LandScape/";
    //wstring strTestFirstTexturePath = strTestBasePath + L"Ash/T_Landscape_Ash_Ash01a_01a_B.dds";
    //wstring strTestSecondTexturePath = strTestBasePath + L"Grass/T_Landscape_Grass_Grass03a_01b_B.dds";
    //CTexture* pTexture = CTexture::Create(0, strTestFirstTexturePath.c_str(), 1);
    //Add_Component(pTexture);

    //pTexture = CTexture::Create(0, strTestSecondTexturePath.c_str(), 1);
    //Add_Component(pTexture);

    Change_ShaderPass(2);
    return S_OK;
}

HRESULT CDrawable_Terrain::Initialize()
{
    m_pTerrainMesh = static_cast<CMesh_Terrain*>(GET_COMPONENT(CMesh));
   

    if (!m_pTerrainMesh)
        return E_FAIL;

    return S_OK;
}

HRESULT CDrawable_Terrain::Start()
{
    if (!m_vecCells.empty())
        assert(0);

    if (FAILED(__super::Start()))
        return E_FAIL;
    m_pTerrainMesh->ReMap_Vertices();
    m_pTerrainMesh->Update_VertsNormal();


    if (nullptr == m_pRenderer)
        assert(0);//호출 순서문제
    m_pRenderer->Update_BackGround(m_iBGIndex);
    m_pRenderer->Update_TextureIndex(m_iSourIndex, m_iDestIndex);

    m_pTerrainMesh->ReadyforPhysX();
    CPhysXCollider* pCol = CPhysXCollider::Create(0, m_pTerrainMesh, m_pTransform);
    pCol->Initialize();
    Add_Component(pCol);

    m_pTerrainMesh->ClearPhysX();
    m_pTerrainMesh->Memory_Save();

    return S_OK;
}

void CDrawable_Terrain::My_Tick()
{
    __super::My_Tick();
}

void CDrawable_Terrain::My_LateTick()
{
    __super::My_LateTick();
}

_uint CDrawable_Terrain::Get_TerrainVerticesCount()
{
    if (nullptr == m_pTerrainMesh)
        return -1;
    return m_pTerrainMesh->Get_NumVertices();
}

_float3* CDrawable_Terrain::Get_TerrainVerticesPos()
{
    if (nullptr == m_pTerrainMesh)
        return nullptr;
    return m_pTerrainMesh->Get_VerticesPos();
}

_float4* CDrawable_Terrain::Get_TerrainTileFlag()
{
    if (nullptr == m_pTerrainMesh)
        return nullptr;
    return m_pTerrainMesh->Get_VerticesColor();
}

CDrawable_Terrain::Terrain_TUPLE CDrawable_Terrain::Get_TerrainData()
{
    return make_tuple(m_strTileTexturePath, m_iTerrainVertX, m_iTerrainVertZ, m_pTerrainVertPos, m_pTerrainColor);
}

void CDrawable_Terrain::Update_Vertices()
{
    m_pTerrainMesh->ReMap_Vertices();

}

void CDrawable_Terrain::Update_Normal()
{
    m_pTerrainMesh->Update_VertsNormal();
    //int a = 0;
}

void CDrawable_Terrain::Update_Texture(_int Sour, _int Dest, _int BG)
{
    m_pRenderer->Update_TextureIndex(Sour, Dest);
    m_pRenderer->Update_BackGround(BG);
}

_uint3 CDrawable_Terrain::Get_TerrainTexIndex()
{
    return _uint3(m_iBGIndex, m_iSourIndex, m_iDestIndex);
}

HRESULT CDrawable_Terrain::SetUp_TerrainMesh(_uint iNumVerticesX, _uint iNumVerticesZ)
{
    CMesh_Terrain* pTerrain = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!pTerrain)
        return E_FAIL;

    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);

    m_iTerrainVertX = iNumVerticesX;
    m_iTerrainVertZ = iNumVerticesZ;
    m_pTerrainVertPos = pTerrain->Get_VerticesPos();
    m_pTerrainColor = pTerrain->Get_VerticesColor();
    return S_OK;
}

HRESULT CDrawable_Terrain::SetUp_TerrainMesh(const _tchar* pFilePath)
{
    wstring wstrFilePath = pFilePath;

    ifstream	readFile(wstrFilePath, ios::binary);

    if (!readFile.is_open())
    {
        return E_FAIL;
    }

    _uint iTilePathPathLength = 0;
    readFile.read((char*)&iTilePathPathLength, sizeof(_uint));

    char* szTexturePath = new char[MAX_PATH];
    readFile.read(szTexturePath, sizeof(char) * iTilePathPathLength);
    string strTexPath = szTexturePath;
    m_strTileTexturePath = CFunctor::To_Wstring(strTexPath);
    Safe_Delete_Array(szTexturePath);

    _uint       iNumVerticesX, iNumVerticesZ;

    readFile.read((char*)&iNumVerticesX, sizeof(_uint));
    readFile.read((char*)&iNumVerticesZ, sizeof(_uint));

    CMesh_Terrain* pTerrain = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!pTerrain)
        return E_FAIL;

    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);

    if (FAILED(SetUp_TerrainInfo(readFile, pTerrain))) 
    {
        readFile.close();
        return E_FAIL;
    }
    readFile.close();

    m_iTerrainVertX = iNumVerticesX;
    m_iTerrainVertZ = iNumVerticesZ;
    m_pTerrainVertPos = pTerrain->Get_VerticesPos();
    m_pTerrainColor = pTerrain->Get_VerticesColor();

    return S_OK;
}

HRESULT CDrawable_Terrain::SetUp_TerrainInfo(ifstream& readFile, CMesh_Terrain* pTerrain)
{
    if (nullptr == pTerrain)
        assert(0);
    _uint iNumVertices = pTerrain->Get_NumVertices();
    _float3* pVerticesPos = pTerrain->Get_VerticesPos();
    _float4* pVerticesColor = pTerrain->Get_VerticesColor();

    readFile.read((char*)pVerticesPos, sizeof(_float3) * iNumVertices);
    readFile.read((char*)pVerticesColor, sizeof(_float4) * iNumVertices);
    
    if (readFile.eof())
    {
        Call_MsgBox(TEXT("인덱스 저장 이전"));
        return S_OK;
    }
    readFile.read((char*)&m_iBGIndex, sizeof(_int));
    readFile.read((char*)&m_iSourIndex, sizeof(_int));
    readFile.read((char*)&m_iDestIndex, sizeof(_int));
    if (readFile.eof())
    {
        Call_MsgBox(TEXT("월드 매트릭스 저장 이전버전"));
        return S_OK;
    }
    _float4x4 mat_Terrain;
    readFile.read((char*)&mat_Terrain, sizeof(_float4x4));

    _float4 TerrainScale = _float4
    (
        XMVectorGetX(XMVector3Length(mat_Terrain.XMLoad().r[0])),
        XMVectorGetX(XMVector3Length(mat_Terrain.XMLoad().r[1])),
        XMVectorGetX(XMVector3Length(mat_Terrain.XMLoad().r[2])),
        0.f
    );
    m_pTransform->Set_Scale(TerrainScale);
    m_pTransform->Set_MyMatrix(mat_Terrain);
    m_pTransform->Make_WorldMatrix();
    return S_OK;
}

HRESULT CDrawable_Terrain::SetUp_TerrainTextures()
{
    wstring strTerrainTexturePath = L"../bin/resources/Textures/Terrain/";
    list<wstring> listTerrainTexturePath;
    Read_TerrainTexture(strTerrainTexturePath, listTerrainTexturePath);

    //이름만 자르는 구문..
    CTexture* pTexture = nullptr;
    _int Index = 0;
    for (list<wstring>::value_type& Value : listTerrainTexturePath)
    {
        pTexture = CTexture::Create(0, Value.c_str(), 1);
        Add_Component(pTexture);

        _int iFind = (_int)Value.rfind(L"\\") + 1;
        wstring wStrName = Value.substr(iFind, Value.length() - iFind);
        string strName = "";
        strName.assign(wStrName.begin(), wStrName.end());
        m_TextureIndex.push_back(make_tuple(strName.c_str(), Index++));
        strName.assign(wStrName.begin(), wStrName.end());
    }



    return S_OK;
}
