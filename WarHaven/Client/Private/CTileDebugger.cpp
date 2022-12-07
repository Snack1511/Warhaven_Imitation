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

CTileDebugger* CTileDebugger::Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iMyLayerIndex)
{
    CTileDebugger* pInstance = new CTileDebugger;
    pInstance->m_iNumTilesX = iNumTilesX;
    pInstance->m_iNumTilesZ = iNumTilesZ;
    pInstance->m_fTileSize = fTileSize;
    pInstance->m_iMyLayerIndex = iMyLayerIndex;

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CTileDebugger");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CTileDebugger::Initialize_Prototype()
{
   /* _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;*/

    m_pTerrainMesh = CMesh_Terrain::Create(CP_BEFORE_RENDERER, m_iNumTilesX, m_iNumTilesZ, m_fTileSize);

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
    /* For Test */
    CTile* pTile = GAMEINSTANCE->Find_Tile(m_iMyLayerIndex, 0);
    if (pTile)
     pTile->Set_TileFlag(CTile::eTileFlags_Default);
    //GAMEINSTANCE->Find_Tile(m_iMyLayerIndex, 5)->Set_TileFlag(CTile::eTileFlags_Default);

    //타일매니저의 값 대로 vertices 만들어주기
    _float3* pVerticesPos = m_pTerrainMesh->Get_VerticesPos();
    _float4* pVerticesColor = m_pTerrainMesh->Get_VerticesColor();

    _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;
    
    for (_uint i = 0; i < m_iNumTilesZ; ++i)
    {
        for (_uint j = 0; j < m_iNumTilesX; ++j)
        {
            _uint iIndex = j + (i * m_iNumTilesX);

            CTile* pTile = GAMEINSTANCE->Find_Tile(m_iMyLayerIndex, iIndex);

            if (!pTile)
                continue;

            _float4 vColor;
            if (pTile->Is_ValidTile())
                vColor = _float4(0.f, 1.f, 0.f, 1.f);
            else
                vColor = _float4(1.f, 0.f, 0.f, 1.f);


            //타일 기준으로 모서리 점들 4개 검출
            _uint	iIndexLT = (i * m_iNumTilesX) + j + m_iNumTilesX;
            _uint	iIndexLB = (i * m_iNumTilesX) + j;
            _uint	iIndexRT = iIndexLT + 1;
            _uint	iIndexRB = iIndexLB + 1;

            memcpy(&pVerticesColor[iIndexLT], &vColor, sizeof(_float4));
            memcpy(&pVerticesColor[iIndexLB], &vColor, sizeof(_float4));
            memcpy(&pVerticesColor[iIndexRT], &vColor, sizeof(_float4));
            memcpy(&pVerticesColor[iIndexRB], &vColor, sizeof(_float4));
        }
    }


    m_pTerrainMesh->ReMap_Vertices();

    return S_OK;
}

HRESULT CTileDebugger::Start()
{
    __super::Start();

    return S_OK;
}
