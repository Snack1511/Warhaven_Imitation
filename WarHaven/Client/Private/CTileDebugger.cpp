#include "stdafx.h"
#include "CTileDebugger.h"

#include "CMesh_Terrain.h"

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
  /* m_pTerrainMesh = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!pTerrain)
        return E_FAIL;

    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);*/


    return S_OK;
}

HRESULT CTileDebugger::Initialize()
{
    return S_OK;
}

HRESULT CTileDebugger::Start()
{
    return S_OK;
}
