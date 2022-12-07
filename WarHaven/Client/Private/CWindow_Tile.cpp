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
	
	if (ImGui::RadioButton("RENDER_TILE", &m_bRenderTile))
	{
		m_bRenderTile = !m_bRenderTile;

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


	


	__super::End();

	return S_OK;
}
