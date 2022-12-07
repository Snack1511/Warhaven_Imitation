#include "stdafx.h"
#include "CWindow_Tile.h"

#include "ImGui_Manager.h"

#include "PhysXCollider.h"

#include "CTestObject.h"

#include "CDebugObject.h"

#include "UsefulHeaders.h"
#include "CMesh_Terrain.h"

#include "CUtility_File.h"
#include "CTileDebugger.h"

CWindow_Tile::CWindow_Tile()
{
}

CWindow_Tile::~CWindow_Tile()
{
}

CWindow_Tile* CWindow_Tile::Create()
{
	CWindow_Tile* pInstance = new CWindow_Tile;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Tile");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Tile::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Tile).name(), ImVec2(400.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_Tile::Tick()
{
	if (KEY(Q, HOLD))
	{
		if (KEY(LBUTTON, TAP))
		{
			
			_float4 vOutPos, vFinalPos = _float4(0.f, -999.f, 0.f);
			_uint3 iIndex(1,2,3);
			_uint iCurIndex = 0;


			_uint iPlusIndex = 0;


			for (auto& elem : m_vecTileDebugger)
			{
				if (GAMEINSTANCE->Is_Picked_Mesh(elem->m_pTerrainMesh, &iIndex, &vOutPos))
				{
					if (vOutPos.y > vFinalPos.y)
					{
						vFinalPos = vOutPos;
						iCurIndex = iPlusIndex;
					}
				}

				++iPlusIndex;
			}


			/* 피킹 성공 */
			if (vFinalPos.y > -999.f)
			{
				vFinalPos = vFinalPos.MultiplyCoord(m_vecTileDebugger[iCurIndex]->Get_Transform()->Get_WorldMatrix());
				On_Picking(iCurIndex, vFinalPos);


			}
		}
	}
}

HRESULT CWindow_Tile::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::CollapsingHeader("Create_Tiles"))
	{
		static _int iNumTilesX = 1;
		static _int iNumTilesZ = 1;
		static _int iNumLayers = 1;
		static _float fTileSize = 1.f;
		ImGui::InputInt("iNumTilesX", &iNumTilesX);
		ImGui::InputInt("iNumTilesZ", &iNumTilesZ);
		ImGui::InputInt("iNumLayers", &iNumLayers);
		ImGui::InputFloat("fTileSize", &fTileSize);

		if (iNumTilesX <= 1) iNumTilesX = 1;
		if (iNumTilesZ <= 1) iNumTilesZ = 1;
		if (iNumLayers <= 1) iNumLayers = 1;
		if (fTileSize <= 0.01f) fTileSize = 0.01f;

		if (ImGui::Button("GENERATE_TILE"))
		{
			for (auto& elem : m_vecTileDebugger)
				DELETE_GAMEOBJECT(elem);

			m_vecTileDebugger.clear();

			GAMEINSTANCE->Clear_AllTiles();
			if (FAILED(GAMEINSTANCE->Create_Layers(iNumTilesX, iNumTilesZ, fTileSize, iNumLayers)))
			{
				Call_MsgBox(L"Failed to Create_Layers");
				return E_FAIL;
			}

			for (_uint i = 0; i < iNumLayers; ++i)
			{
				m_vecTileDebugger.push_back(CTileDebugger::Create(iNumTilesX, iNumTilesZ, fTileSize, i));
				m_vecTileDebugger.back()->Initialize();
				CREATE_GAMEOBJECT(m_vecTileDebugger.back(), GROUP_DEBUG);
			}

			Call_MsgBox(L"Generate Success");

		}
	}
	
	static _bool bRenderTile = true;

	if (ImGui::RadioButton("RENDER_TILE", bRenderTile))
	{
		m_bRenderTile = !m_bRenderTile;
		bRenderTile = m_bRenderTile;

		if (m_bRenderTile)
		{
			for (auto& elem : m_vecTileDebugger)
				ENABLE_GAMEOBJECT(elem);
		}
		else
		{
			for (auto& elem : m_vecTileDebugger)
				DISABLE_GAMEOBJECT(elem);
		}
	}

	if (!m_vecTileDebugger.empty())
	{
		_float4 vWorldPos = GAMEINSTANCE->Get_TileWorldPos();


		// 1. Layer 사이 Y 값
		if (ImGui::DragFloat("Step Y", &m_fStepY, 0.1f, 0.1f, 100.f, "%.2f"))
		{
			for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
			{
				_float4 vPos = vWorldPos;
				vPos.y += i * m_fStepY;
				m_vecTileDebugger[i]->Get_Transform()->Set_World(WORLD_POS, vPos);
			}
		}

		


		// 2. 위치

		static _float	vPosition[3] = {};

		memcpy(vPosition, &vWorldPos, sizeof(_float3));


		if (ImGui::DragFloat3("Position", vPosition, 0.1f, -999.f, 999.f, "%.1f"))
		{
			memcpy(&vWorldPos, vPosition, sizeof(_float3));

			for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
			{
				_float4 vPos = vWorldPos;
				vPos.y += i * m_fStepY;
				m_vecTileDebugger[i]->Get_Transform()->Set_World(WORLD_POS, vPos);
			}

			GAMEINSTANCE->Set_Tile_WorldPos(vWorldPos);

		}
	}


	


	__super::End();

	return S_OK;
}

void CWindow_Tile::On_Picking(_uint iLayerIndex, _float4 vPickedPos)
{
	_uint iCurTileIndex = GAMEINSTANCE->Find_TileIndex(vPickedPos);
	CTile* pTile = GAMEINSTANCE->Find_Tile(iLayerIndex, iCurTileIndex);

	pTile->Set_TileFlag(CTile::eTileFlags_Default);
	m_vecTileDebugger[iLayerIndex]->Set_TileColor(iCurTileIndex, _float4(0.f, 1.f, 0.f));
}
