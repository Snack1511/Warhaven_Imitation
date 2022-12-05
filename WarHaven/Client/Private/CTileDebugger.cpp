#include "stdafx.h"
#include "CTileDebugger.h"

#include "CMesh_Terrain.h"

#include "UsefulHeaders.h"

CTileDebugger::CTileDebugger()
{
}

CTileDebugger::~CTileDebugger()
{
}

CTileDebugger* CTileDebugger::Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize)
{
    CTileDebugger* pInstance = new CTileDebugger;
    pInstance->m_iNumTilesX = iNumTilesX;
    pInstance->m_iNumTilesZ = iNumTilesZ;
    pInstance->m_fTileSize = fTileSize;

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CTileDebugger");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CTileDebugger::Initialize_Prototype()
{
    _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;

    m_pTerrainMesh = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!m_pTerrainMesh)
        return E_FAIL;

    m_pTerrainMesh->Initialize();
    Add_Component<CMesh>(m_pTerrainMesh);



    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXNOR,
        VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXNOR_PASS_NAVIGATION);
    Add_Component(pRenderer);

    return S_OK;
}

HRESULT CTileDebugger::Initialize()
{

    //타일매니저의 값 대로 vertices 만들어주기
    

    m_pTerrainMesh->ReMap_Vertices();

    return S_OK;
}

HRESULT CTileDebugger::Start()
{
    return S_OK;
}
