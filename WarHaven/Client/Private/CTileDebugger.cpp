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

    CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_ALPHA, VTXNOR_PASS_NAVIGATION);
    Add_Component(pRenderer);

    return S_OK;
}

HRESULT CTileDebugger::Initialize()
{
    //타일매니저의 값 대로 vertices 만들어주기
    _float3* pVerticesPos = m_pTerrainMesh->Get_VerticesPos();
    _float4* pVerticesColor = m_pTerrainMesh->Get_VerticesColor();

    _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;

    for (_uint i = 0; i < iNumVerticesZ; ++i)
    {
        for (_uint j = 0; j < iNumVerticesX; ++j)
        {
            _uint iIndex = j + (i * iNumVerticesX);
            pVerticesColor[iIndex] = _float4(0.f, 1.f, 0.f, 0.5f);
        }
    }

    
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
                continue;
            else if (pTile->Check_TileFlag(CTile::eTileFlags_Blocked))
                vColor = _float4(1.f, 0.f, 0.f, 0.5f);
            else
                vColor = _float4(0.f, 0.f, 0.f, 0.5f);


            //타일 기준으로 모서리 점들 4개 검출
            _uint	iIndexLT = (i * iNumVerticesX) + j + iNumVerticesX;
            _uint	iIndexLB = (i * iNumVerticesX) + j;
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

void CTileDebugger::Set_TileColor(_uint iTileIndex, _float4 vColor)
{
    _float4* pVerticesColor = m_pTerrainMesh->Get_VerticesColor();

    //타일 주변 4개의 버텍스를 찾기 위해선
    _uint iCurIndexZ = iTileIndex / m_iNumTilesX;
    _uint iCurIndexX = iTileIndex % m_iNumTilesX;

    _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;

    _uint	iIndexLT = (iCurIndexZ * iNumVerticesX) + iCurIndexX + iNumVerticesX;
    _uint	iIndexLB = (iCurIndexZ * iNumVerticesX) + iCurIndexX;
    _uint	iIndexRT = iIndexLT + 1;
    _uint	iIndexRB = iIndexLB + 1;

    memcpy(&pVerticesColor[iIndexLT], &vColor, sizeof(_float4));
    memcpy(&pVerticesColor[iIndexLB], &vColor, sizeof(_float4));
    memcpy(&pVerticesColor[iIndexRT], &vColor, sizeof(_float4));
    memcpy(&pVerticesColor[iIndexRB], &vColor, sizeof(_float4));

    m_pTerrainMesh->ReMap_Vertices();
}

void CTileDebugger::Set_TileAlpha(_uint iTileIndex, _float fAlpha)
{
    _float4* pVerticesColor = m_pTerrainMesh->Get_VerticesColor();

    //타일 주변 4개의 버텍스를 찾기 위해선
    _uint iCurIndexZ = iTileIndex / m_iNumTilesX;
    _uint iCurIndexX = iTileIndex % m_iNumTilesX;

    _uint iNumVerticesX = m_iNumTilesX + 1;
    _uint iNumVerticesZ = m_iNumTilesZ + 1;

    _uint	iIndexLT = (iCurIndexZ * iNumVerticesX) + iCurIndexX + iNumVerticesX;
    _uint	iIndexLB = (iCurIndexZ * iNumVerticesX) + iCurIndexX;
    _uint	iIndexRT = iIndexLT + 1;
    _uint	iIndexRB = iIndexLB + 1;

    pVerticesColor[iIndexLT].w = fAlpha;
    pVerticesColor[iIndexLB].w = fAlpha;
    pVerticesColor[iIndexRT].w = fAlpha;
    pVerticesColor[iIndexRB].w = fAlpha;

    m_pTerrainMesh->ReMap_Vertices();
}
