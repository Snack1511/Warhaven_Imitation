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

	SetUp_DataFiles(m_DataDirectory.c_str());
	return S_OK;
}

void CWindow_Tile::Tick()
{
	if (m_bHoverWindow)
		return;

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
					vOutPos = vOutPos.MultiplyCoord(m_vecTileDebugger[iPlusIndex]->Get_Transform()->Get_WorldMatrix());

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
				if (!m_bSelectNeighbor)
					On_Picking(iCurIndex, vFinalPos);
				else
					On_Pick_Neighbor(iCurIndex, vFinalPos);


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

	if (ImGui::CollapsingHeader("SetUp_Layers")) 
	{
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
	}
	if (ImGui::CollapsingHeader("Save/Load")) 
	{
		char szSaveInput[MAXCHAR] = "";
		if (ImGui::InputText("##SavePathInput", szSaveInput, sizeof(char)*MAXCHAR))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{

			SetUp_DataFiles(m_DataDirectory.c_str());
		}
		string PreviewData = (m_FileDatas.empty()) ? "" : get<SL_FileName>(m_FileDatas[m_iFileDataIndex]);
		if (ImGui::BeginCombo("##SelectLoadData", PreviewData.c_str()))
		{
			for(_uint i = 0; i < _uint(m_FileDatas.size()); ++i)
			{
				_bool bSelect = false;
				if (m_iFileDataIndex == i)
				{
					bSelect = true;
				}
				if (ImGui::Selectable(get<SL_FileName>(m_FileDatas[i]).c_str()), bSelect)
				{
					m_iFileDataIndex = i;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if(ImGui::Button("Load"))
		{
		}
	}

	if (nullptr != m_pSelectTile)
	{
		ImVec2 vPannelSize = ImVec2(300.f, 300.f);
		ImVec2 vTileControlPos = ImVec2(vPannelSize.x, 0.f);
		Create_SubWindow("Tile_Control", vTileControlPos, vPannelSize, bind(&CWindow_Tile::Control_SelectTile, this));
	}
	


	__super::End();

	return S_OK;
}

void CWindow_Tile::SetUp_DataFiles(const char* pPath)
{
	m_FileDatas.clear();
	for (filesystem::directory_iterator FileIter(pPath);
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
			strFileName = strFileName.substr(0, iFindExt);
		}
		m_FileDatas.push_back(make_tuple(strFileName, strFullPath));

	}
}

void CWindow_Tile::Control_SelectTile()
{
	if (ImGui::CollapsingHeader("Tile Info"))
	{
		string SelectLayer = to_string(m_iCurSelectLayer);
		string SelectIndex = to_string(m_iCurSelectTileIndex);
		_float4 vPos = m_pSelectTile->Get_CenterPos();
		string SelectCenterPosX = to_string(vPos.x);
		string SelectCenterPosY = to_string(vPos.y);
		string SelectCenterPosZ = to_string(vPos.z);


		ImGui::Text("Layer : ");
		ImGui::SameLine();
		ImGui::Text(SelectLayer.c_str());

		ImGui::Text("Index : ");
		ImGui::SameLine();
		ImGui::Text(SelectIndex.c_str());

		ImGui::Spacing();

		ImGui::Text("CenterPos : ");
		ImGui::SameLine();
		ImGui::Text(SelectCenterPosX.c_str(), "%.2f");
		ImGui::SameLine();
		ImGui::Text(" ");
		ImGui::SameLine();
		ImGui::Text(SelectCenterPosY.c_str(), "%.2f");
		ImGui::SameLine();
		ImGui::Text(" ");
		ImGui::SameLine();
		ImGui::Text(SelectCenterPosZ.c_str(), "%.2f");
	}

	if (ImGui::CollapsingHeader("Flag Info"))
	{
		ImGui::Text("Selected");
		ImGui::SameLine();
		ImGui::Text("UnSelected");
		if(ImGui::ListBoxHeader("##SelectFlagListBox", ImVec2(80.f, 100.f)))
		{
			_uint Flag = 0;
			if (nullptr == m_pSelectTile)
				goto SELECTSKIPLOOP;

			Flag = m_pSelectTile->Get_TileFlag();

			if (Flag == CTile::eTileFlags_None)
			{
				ImGui::Selectable(m_pSelectTile->FlagOption_Display[0]);
			}
			else 
			{
				if (Flag & CTile::eTileFlags_Blocked)
				{
					if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[1]))
					{
						m_pSelectTile->Remove_TileFlag(CTile::eTileFlags_Blocked);
						Flag &= ~(CTile::eTileFlags_Blocked);
					}
				}
				else
				{
					if (Flag & CTile::eTileFlags_Default)
					{
						if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[2]))
						{
							m_pSelectTile->Remove_TileFlag(CTile::eTileFlags_Default);
							Flag &= ~(CTile::eTileFlags_Default);
						}
					}
					if (Flag & CTile::eTileFlags_Stair)
					{
						if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[3]))
						{
							m_pSelectTile->Remove_TileFlag(CTile::eTileFlags_Stair);
							Flag &= ~(CTile::eTileFlags_Stair);
						}
					}
				}
			}

			SELECTSKIPLOOP:
			ImGui::ListBoxFooter();
		}
		ImGui::SameLine();
		if (ImGui::ListBoxHeader("##UnSelectFlagListBox", ImVec2(80.f, 100.f)))
		{
			_uint Flag = 0;

			if (nullptr == m_pSelectTile)
				goto UNSELECTSKIPLOOP;

			Flag = ~(m_pSelectTile->Get_TileFlag());
			if (Flag ^ CTile::eTileFlags_None)
			{
				if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[0]))
				{
					m_pSelectTile->Set_TileFlag(CTile::eTileFlags_None);
					Flag = (CTile::eTileFlags_None);
				}
			}
			if (Flag & CTile::eTileFlags_Blocked)
			{
				if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[1]))
				{
					m_pSelectTile->Add_TileFlag(CTile::eTileFlags_Blocked);
					Flag = (CTile::eTileFlags_Blocked);
				}
			}
			if (Flag & CTile::eTileFlags_Default)
			{
				if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[2]))
				{
					m_pSelectTile->Add_TileFlag(CTile::eTileFlags_Default);
					Flag &= (CTile::eTileFlags_Default);
				}
			}
			if (Flag & CTile::eTileFlags_Stair)
			{
				if (ImGui::Selectable(m_pSelectTile->FlagOption_Display[3]))
				{
					m_pSelectTile->Add_TileFlag(CTile::eTileFlags_Stair);
					Flag &= (CTile::eTileFlags_Stair);
				}
			}

			UNSELECTSKIPLOOP:
			ImGui::ListBoxFooter();
		}
	}

	if(ImGui::CollapsingHeader("Neighbor Info"))
	{
		ImGui::Text("CurSelectNeighbor : ");
		ImGui::SameLine();
		ImGui::Text(m_pSelectTile->Neighbor_Display[m_iCurSelectNeighbor]);

		string strSelect = (m_bSelectNeighbor) ? "Select Neighbor" : "UnSelect Neighbor";
		ImGui::Text(strSelect.c_str());

		ImVec2 ButtonSize = ImVec2(25.f, 25.f);
		string ButtonLabel = "";
		
		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_LeftTop));
		ButtonLabel += "##LT";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_LeftTop && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_LeftTop;
				m_bSelectNeighbor = true;
			}
		}
		ImGui::SameLine();

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Top));
		ButtonLabel += "##T";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_Top && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_Top;
				m_bSelectNeighbor = true;
			}
		}
		ImGui::SameLine();
		
		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_RightTop));
		ButtonLabel += "##RT";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_RightTop && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_RightTop;
				m_bSelectNeighbor = true;
			}
		}

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Left));
		ButtonLabel += "##L";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_Left && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_Left;
				m_bSelectNeighbor = true;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("##NONEButton", ButtonSize))
		{
			m_iCurSelectNeighbor = 0;
			m_bSelectNeighbor = false;
		}
		ImGui::SameLine();

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Right));
		ButtonLabel += "##R";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_Right && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_Right;
				m_bSelectNeighbor = true;
			}
		}

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_LeftBottom));
		ButtonLabel += "##LB";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_LeftBottom && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_LeftBottom;
				m_bSelectNeighbor = true;
			}
		}
		ImGui::SameLine();

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Bottom));
		ButtonLabel += "##B";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_Bottom && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_Bottom;
				m_bSelectNeighbor = true;
			}
		}
		ImGui::SameLine();

		ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_RightBottom));
		ButtonLabel += "##RB";
		if (ImGui::Button(ButtonLabel.c_str(), ButtonSize))
		{
			if (m_iCurSelectNeighbor == CTile::eNeighborFlags_RightBottom && m_bSelectNeighbor == true)
			{
				m_iCurSelectNeighbor = 0;
				m_bSelectNeighbor = false;
			}
			else
			{
				m_iCurSelectNeighbor = CTile::eNeighborFlags_RightBottom;
				m_bSelectNeighbor = true;
			}
		}
		

		if (m_bSelectNeighbor) {
			if (ImGui::Button("Set_Null"))
			{
				m_pSelectTile->Set_Neighbor(m_iCurSelectNeighbor, nullptr);
				m_bSelectNeighbor = false;
			}
		}
	}
}

void CWindow_Tile::Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Tile&)> func)
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

void CWindow_Tile::On_Picking(_uint iLayerIndex, _float4 vPickedPos)
{
	m_iCurSelectTileIndex = GAMEINSTANCE->Find_TileIndex(vPickedPos);
	m_iCurSelectLayer = iLayerIndex;
	CTile* pTile = GAMEINSTANCE->Find_Tile(iLayerIndex, m_iCurSelectTileIndex);

	//pTile->Set_TileFlag(CTile::eTileFlags_Default);
	m_vecTileDebugger[iLayerIndex]->Set_TileColor(m_iCurSelectTileIndex, _float4(0.f, 1.f, 0.f));
	
	m_pSelectTile = pTile;


}

void CWindow_Tile::On_Pick_Neighbor(_uint iLayerIndex, _float4 vPickedPos)
{
	if (nullptr == m_pSelectTile)
		return;

	_int TileIndex = GAMEINSTANCE->Find_TileIndex(vPickedPos);
	CTile* pTile = GAMEINSTANCE->Find_Tile(iLayerIndex, TileIndex);
	m_pSelectTile->Set_Neighbor(m_iCurSelectNeighbor, pTile);

	m_bSelectNeighbor = false;
}
