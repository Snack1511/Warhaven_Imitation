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
#include "CCellLayer.h"

pair<const char*, _int> CWindow_Tile::m_iAttributeArray[] =
{
	make_pair("Blocked", (_int)CELL_BLOCKED),
	make_pair("Ground", (_int)CELL_GROUND),
	make_pair("Wall", (_int)CELL_WALL),
	make_pair("Grass", (_int)CELL_GRASS),
	make_pair("Stair", (_int)CELL_STAIR),
};

CWindow_Tile::CWindow_Tile()
{
}

CWindow_Tile::~CWindow_Tile()
{
	SAFE_DELETE_ARRAY(m_bRenderTile);
	for (auto& value : m_pLayers)
	{
		SAFE_DELETE(value.second);
	}
	m_pLayers.clear();

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

	//SetUp_DataFiles(m_DataDirectory.c_str());
	SetUp_DataFiles(m_CellDataDirectory.c_str());
	return S_OK;
}
static _float4 debugPick;
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
				if (!m_bRenderTile[iPlusIndex])
				{
					iPlusIndex++;
					continue;
				}

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
				if (!m_bSelectNeighbor)
					On_Picking(iCurIndex, vFinalPos);
				else
					On_Pick_Neighbor(iCurIndex, vFinalPos);


			}
		}
	}

	//셀 선택
	if (KEY(I, HOLD))
	{
		if (KEY(LBUTTON, TAP))
		{
			if (m_pCurLayer)
			{
				_float4 vOutPos, vFinalPos = _float4(0.f, -999.f, 0.f);
				_uint3 iIndex(1, 2, 3);
				_uint iCurIndex = 0;

				_uint iPlusIndex = 0;

				if (GAMEINSTANCE->Is_Picked_FixedHeight(m_pCurLayer->Get_MinHeight(), &vOutPos))
				{
					if(m_bSelectNeighbor)
					{
						On_CellPick_Neighbor(iCurIndex, vOutPos);
					}
					else
					{
						debugPick = vOutPos;
						On_CellPicking(iCurIndex, vOutPos);
					}
				}
			}
		}
	}
	//셀 속성 변경
	if (KEY(O, HOLD))
	{
		if (KEY(LBUTTON, HOLD))
		{
			if (m_pCurLayer)
			{
				_float4 vOutPos, vFinalPos = _float4(0.f, -999.f, 0.f);
				_uint3 iIndex(1, 2, 3);
				_uint iCurIndex = 0;

				_uint iPlusIndex = 0;

				if (GAMEINSTANCE->Is_Picked_FixedHeight(m_pCurLayer->Get_MinHeight(), &vOutPos))
				{
					debugPick = vOutPos;
					On_CellSetAttribute(iCurIndex, vOutPos, m_fBrushSize);
				}
			}
		}
	}
	if (KEY(P, HOLD))
	{
		if (KEY(LBUTTON, HOLD))
		{
			if (m_pCurLayer)
			{
				_float4 vOutPos, vFinalPos = _float4(0.f, -999.f, 0.f);
				_uint3 iIndex(1, 2, 3);
				_uint iCurIndex = 0;

				_uint iPlusIndex = 0;

				if (GAMEINSTANCE->Is_Picked_FixedHeight(m_pCurLayer->Get_MinHeight(), &vOutPos))
				{
					debugPick = vOutPos;
					On_Replace_PickedAttribute(vOutPos);
				}
			}
		}
	}
#ifdef _DEBUG
#ifdef DEBUGRENDER


	if (m_pCurLayer)
	{
		m_pCurLayer->DebugTick();
	}
#endif // _DEBUG
#endif // _DEBUG
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

		static _float fMinHeight = 0.f;
		static _float fMaxHeight = 1.f;
		static _float4 vCenterPos = _float4(0.f, 0.f, 0.f, 1.f);
		ImGui::InputInt("iNumTilesX", &iNumTilesX);
		ImGui::InputInt("iNumTilesZ", &iNumTilesZ);
		ImGui::InputInt("iNumLayers", &iNumLayers);
		ImGui::InputFloat("fTileSize", &fTileSize);

		ImGui::DragFloat("fHeightMin", &fMinHeight);
		ImGui::DragFloat("fHeightMax", &fMaxHeight);
		_float vCenter[3];
		memcpy_s(vCenter, sizeof(_float3), &vCenterPos, sizeof(_float3));
		if (ImGui::DragFloat3("vCenter", vCenter))
		{
			memcpy_s(&vCenterPos, sizeof(_float3), vCenter, sizeof(_float3));
		}
		vCenterPos.y = fMinHeight;

		if (iNumTilesX <= 1) iNumTilesX = 1;
		if (iNumTilesZ <= 1) iNumTilesZ = 1;
		if (iNumLayers <= 1) iNumLayers = 1;
		if (fTileSize <= 0.01f) fTileSize = 0.01f;
		if (fMinHeight >= fMaxHeight) fMaxHeight = fMinHeight + 0.001f;

		if (ImGui::Button("GENERATE_CELLLAYER")) 
		{
			vCenterPos.w = 1.f;
			m_pCurLayer = CCellLayer::Create(iNumTilesX, iNumTilesZ, fTileSize, vCenterPos, fMinHeight, fMaxHeight);
			m_iNumTilesX = iNumTilesX;
			m_iNumTilesZ = iNumTilesZ;
			m_fTileSize = fTileSize;
			m_fTileHeightMinRange = fMinHeight;
			m_fTileHeightMaxRange = fMaxHeight;
			auto Iter = m_pLayers.find(m_fTileHeightMinRange);
			if (Iter != m_pLayers.end())
			{
				SAFE_DELETE(Iter->second);
				m_pLayers.erase(Iter);
			}
			m_pCurLayer->Reset_Neighbor();
			m_pLayers.emplace(m_fTileHeightMinRange, m_pCurLayer);
		}
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

			SAFE_DELETE_ARRAY(m_bRenderTile);
			m_bRenderTile = new _bool[iNumLayers];
			for (_uint i = 0; i < iNumLayers; ++i)
				m_bRenderTile[i] = true;

			m_iNumTilesX = iNumTilesX;
			m_iNumTilesZ = iNumTilesZ;
			m_fTileSize = fTileSize;
			m_iCurSelectLayer = 0;
			m_iCurSelectNeighbor = 0;
			m_bSelectNeighbor = false;
			m_iCurSelectTileIndex = 0;
			m_pSelectTile = nullptr;

			Call_MsgBox(L"Generate Success");

		}
	}


	if (ImGui::CollapsingHeader("SetUp_Layers")) 
	{
		if (m_pCurLayer)
		{
			m_fTileHeightMinRange = m_pCurLayer->Get_MinHeight();
			_float Key = m_fTileHeightMinRange;
			if (ImGui::DragFloat("ChangeHeight", &m_fTileHeightMinRange))
			{
				m_pLayers.erase(Key);
				m_pCurLayer->Set_MinHeight(m_fTileHeightMinRange);
				m_pLayers.emplace(m_fTileHeightMinRange, m_pCurLayer);

			}
		}

		if (!m_pLayers.empty())
		{
			if (ImGui::CollapsingHeader("Select Layer"))
			{
				for (auto& Layers : m_pLayers)
				{
					_float Key = Layers.first;
					if (ImGui::Button(to_string(Key).c_str()))
					{
						m_pCurLayer = Layers.second;
						m_iNumTilesX = m_pCurLayer->Get_XTileNums();
						m_iNumTilesZ = m_pCurLayer->Get_ZTileNums();
						m_fTileSize = m_pCurLayer->Get_TileSize();
						m_fTileHeightMinRange = m_pCurLayer->Get_MinHeight();
						m_fTileHeightMaxRange = m_pCurLayer->Get_MaxHeight();
					}
				}
			}
			if (ImGui::CollapsingHeader("Attributes")) 
			{
				ImGui::DragFloat("fBrushSize", &m_fBrushSize, 0.01f, 0.f, 100.f, "%.2f");
				if (ImGui::BeginCombo("SelectAttribute", m_iAttributeArray[m_iAttrubuteIndex].first))
				{
					for (_uint i = 0; i < 5; ++i)
					{
						_bool bSelect = false;
						if (m_iAttrubuteIndex == i)
						{
							bSelect = true;
						}
						if (ImGui::Selectable(m_iAttributeArray[i].first, bSelect))
						{
							m_iAttrubuteIndex = i;
						}
					}
					ImGui::EndCombo();
				}
			}
			if (ImGui::CollapsingHeader("Layer Copy"))
			{
				if (ImGui::Button("Clone_CurLayer"))
				{
					CCellLayer* pLayer = m_pCurLayer->Clone_Layer(m_pCurLayer->Get_MinHeight() + 1.f);
					
					m_pLayers.emplace(pLayer->Get_MinHeight(), pLayer);
					m_pCurLayer = pLayer;
					
				}
			}
			if (ImGui::CollapsingHeader("Visibility"))
			{
				if (m_listDebug.empty()) 
				{
					if (ImGui::Button("Show_BlockedPos"))
					{
						Create_DebugObject();
					}
				}
				else
				{
					if (ImGui::Button("Hide_BlockedPos"))
					{
						Clear_DebugObject();
					}
				}
				if (ImGui::Button("Create_Visibility"))
				{
					if (m_pCurLayer)
					{
						m_pCurLayer->SetUp_Nodes();
						m_pCurLayer->SetUp_Visibility();
						//Bin\Data\GameSystem\CellData\Map_Paden\Map_Paden_0
						//
						string strSavePath = m_CellDataDirectory;
						string strDebugName = CFunctor::To_String(m_pCurLayer->Get_DebugName());
						_int iFind = strDebugName.rfind("_");
						string strFolderName = strDebugName.substr(0, iFind);
						strSavePath += "/";
						strSavePath += strFolderName;
						strSavePath += "/";
						strSavePath += strDebugName;
						strSavePath += "/";
						strSavePath += strDebugName;
						strSavePath += "_Visibility.bin";
						m_pCurLayer->Save_Visiblity(CFunctor::To_Wstring(strSavePath));
					}
					
				}
				if (ImGui::Button("Load_Visibility"))
				{
					if (m_pCurLayer)
					{
						m_pCurLayer->SetUp_Nodes();

						string strLoadPath = m_CellDataDirectory;
						string strDebugName = CFunctor::To_String(m_pCurLayer->Get_DebugName());
						_int iFind = strDebugName.rfind("_");
						string strFolderName = strDebugName.substr(0, iFind);
						strLoadPath += "/";
						strLoadPath += strFolderName;
						strLoadPath += "/";
						strLoadPath += strDebugName;
						strLoadPath += "/";
						strLoadPath += strDebugName;
						strLoadPath += "_Visibility.bin";
						m_pCurLayer->Load_Visibility(CFunctor::To_Wstring(strLoadPath));
					}
				}
			}
			if (ImGui::CollapsingHeader("LayerRoute"))
			{
				if (ImGui::Button("SetUp_LayerRoute"))
				{
					for (auto Layer : m_pLayers)
					{
						Layer.second->SetUp_StairList(m_pLayers);
					}
					for (auto Layer : m_pLayers)
					{
						Layer.second->SetUp_LayerRoute(m_pLayers);
					}
				}
			}

		}

		if (!m_vecTileDebugger.empty())
		{
			if (ImGui::Button("SHOOT_RAY"))
			{
				On_ShootRay();
			}

			_float4 vWorldPos = GAMEINSTANCE->Get_TileWorldPos();

			for (_int i = (m_vecTileDebugger.size() - 1); i >= 0; --i)
			{
				string strTemp = "RENDER_LAYER(";
				strTemp += to_string(i);
				strTemp += ")";

				if (ImGui::RadioButton(strTemp.c_str(), m_bRenderTile[i]))
				{
					m_bRenderTile[i] = !m_bRenderTile[i];

					if (m_bRenderTile[i])
					{
						ENABLE_GAMEOBJECT(m_vecTileDebugger[i]);
					}
					else
					{
						DISABLE_GAMEOBJECT(m_vecTileDebugger[i]);
					}
				}

				ImGui::SameLine();

				/* 높이값 처리 */
				strTemp = "Standard Y(";
				strTemp += to_string(i);
				strTemp += ")";
				_float fCurY = GAMEINSTANCE->Get_StandardY(i);
				if (ImGui::SliderFloat(strTemp.c_str(), &fCurY, -10.f, 50.f, "%.2f"))
				{
					GAMEINSTANCE->Set_StandardY(i, fCurY);
				}
			}

			static _float fStepY = 0.f;
			if (ImGui::SliderFloat("StepY", &fStepY, 0.f, 20.f, "%.2f"))
			{
				for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
				{
					GAMEINSTANCE->Set_StandardY(i, i * fStepY);
				}
			}



			// 2. 위치

			static _float	vPosition[3] = {};

			memcpy_s(vPosition, sizeof(_float3), &vWorldPos, sizeof(_float4));

			if (ImGui::DragFloat3("Position", vPosition, 0.1f, -999.f, 999.f, "%.1f"))
			{
				memcpy_s(&vWorldPos, sizeof(_float3), vPosition, sizeof(_float3));
				GAMEINSTANCE->Set_Tile_WorldPos(vWorldPos);

			}

			for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
			{
				_float4 vPos = vWorldPos;
				_float fCurY = GAMEINSTANCE->Get_StandardY(i);
				vPos.y += fCurY;
				m_vecTileDebugger[i]->Get_Transform()->Set_World(WORLD_POS, vPos);
			}
		}
	}
	if (ImGui::CollapsingHeader("Save/Load")) 
	{
		static string SaveName = "";
		char szSaveInput[MAXCHAR] = "";
		strcpy_s(szSaveInput, SaveName.c_str());
		if (ImGui::InputText("##SavePathInput", szSaveInput, sizeof(char)*MAXCHAR))
		{
			SaveName = szSaveInput;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			Save_AllLayer(SaveName);
			SetUp_DataFiles(m_CellDataDirectory.c_str());
		}
		string PreviewData = (m_FileDatas.empty()) ? "" : get<SL_FileName>(m_FileDatas[m_iFileDataIndex]);
		if (ImGui::BeginCombo("##SelectLoadData", PreviewData.c_str()))
		//if (ImGui::BeginListBox("##SelectLoadData"))
		{
			for(_uint i = 0; i < _uint(m_FileDatas.size()); ++i)
			{
				_bool bSelect = false;
				if (m_iFileDataIndex == i)
				{
					bSelect = true;
				}
				if (ImGui::Selectable(get<SL_FileName>(m_FileDatas[i]).c_str(), bSelect))
				{
					m_iFileDataIndex = i;
				}
			}
			//ImGui::EndListBox();
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if(ImGui::Button("Load"))
		{
			Load_AllLayer(get<SL_FilePath>(m_FileDatas[m_iFileDataIndex]));
		}
	}

	if (nullptr != m_pSelectTile)
	{
		ImVec2 vPannelSize = ImVec2(300.f, 300.f);
		ImVec2 vTileControlPos = ImVec2(vPannelSize.x, 0.f);
		Create_SubWindow("Tile_Control", vTileControlPos, vPannelSize, bind(&CWindow_Tile::Control_SelectTile, this));
	}
	if (nullptr != m_pSelectCell)
	{
		ImVec2 vPannelSize = ImVec2(300.f, 300.f);
		ImVec2 vTileControlPos = ImVec2(vPannelSize.x, 0.f);
		Create_SubWindow("Cell_Control", vTileControlPos, vPannelSize, bind(&CWindow_Tile::Control_SelectCell, this));
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
		if (entry.is_directory())
		{
			wstring wstrPath = entry.path().relative_path();
			string strFullPath;
			strFullPath.assign(wstrPath.begin(), wstrPath.end());

			_int iFind = (_int)strFullPath.rfind("\\") + 1;
			string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

			m_FileDatas.push_back(make_tuple(strFileName, strFullPath));
		}

		//_int iFind = (_int)strFullPath.rfind("\\") + 1;
		//string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		//if (!entry.is_directory())
		//{
		//	_int iFindExt = (int)strFileName.rfind(".") + 1;
		//	string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);
		//	strFileName = strFileName.substr(0, iFindExt);
		//}


	

	}
}

void CWindow_Tile::Control_SelectTile()
{
	if (ImGui::CollapsingHeader("Tile Info"))
	{
		string SelectLayer = to_string(m_iCurSelectLayer);
		string SelectIndex = to_string(m_iCurSelectTileIndex);
		_float4 vPos = m_pSelectTile->Get_LocalCenterPos();
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

		ImGui::Text("LocalCenterPos : ");
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

		ImVec2 ButtonSize = ImVec2(50.f, 50.f);
		string ButtonLabel = "";

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_LeftTop))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_LeftTop));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_LeftTop)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Top))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Top));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Top)->Get_CurLayerIndex());
		}
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
		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_RightTop))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_RightTop));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_RightTop)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Left))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Left));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Left)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Right))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Right));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Right)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_LeftBottom))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_LeftBottom));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_LeftBottom)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Bottom))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_Bottom));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_Bottom)->Get_CurLayerIndex());
		}
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

		if (m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_RightBottom))
		{
			ButtonLabel = to_string(m_pSelectTile->Get_NeighborIndex(CTile::eNeighborFlags_RightBottom));
			ButtonLabel += "/";
			ButtonLabel += to_string(m_pSelectTile->Get_AdjTile(CTile::eNeighborFlags_RightBottom)->Get_CurLayerIndex());
		}
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
		

		if (m_bSelectNeighbor)
		{
			if (ImGui::Button("Set_Null"))
			{
				m_pSelectTile->Set_Neighbor(m_iCurSelectNeighbor, nullptr);
				m_bSelectNeighbor = false;
			}


		}
	}
}

void CWindow_Tile::Control_SelectCell()
{
	if (ImGui::CollapsingHeader("CellInfo"))
	{
		if (!m_pSelectCell)
			return;
		_int Index = m_pSelectCell->Get_Index();
		_float4 PointA = m_pSelectCell->Get_Point(CCell::POINT_A);
		_float4 PointB = m_pSelectCell->Get_Point(CCell::POINT_B);
		_float4 PointC = m_pSelectCell->Get_Point(CCell::POINT_C);

		_float center[3] = {};
		_float A[3] = {};
		_float B[3] = {};
		_float C[3] = {};
		_float Pick[3] = {};
		memcpy_s(center, sizeof(_float3), &m_pSelectCell->Get_Position(), sizeof(_float3));
		memcpy_s(A, sizeof(_float3), &PointA, sizeof(_float3));
		memcpy_s(B, sizeof(_float3), &PointB, sizeof(_float3));
		memcpy_s(C, sizeof(_float3), &PointC, sizeof(_float3));
		memcpy_s(Pick, sizeof(_float3), &debugPick, sizeof(_float3));
		_int PickIndex = ((_int(debugPick.z / m_fTileSize)) * m_iNumTilesX + _int(debugPick.x / m_fTileSize)) * 2;

		ImGui::InputFloat3("Pick", Pick);
		ImGui::InputInt("PickedIndex", &PickIndex);
		ImGui::InputInt("Index", &Index);
		ImGui::InputFloat3("Center", center);
		ImGui::InputFloat3("A", A);
		ImGui::InputFloat3("B", B);
		ImGui::InputFloat3("C", C);



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
	/* 기존꺼 알파 지우기 */
	m_vecTileDebugger[m_iCurSelectLayer]->Set_TileAlpha(m_iCurSelectTileIndex, 0.5f);

	m_iCurSelectTileIndex = GAMEINSTANCE->Find_TileIndex(vPickedPos);
	m_iCurSelectLayer = iLayerIndex;
	CTile* pTile = GAMEINSTANCE->Find_Tile(iLayerIndex, m_iCurSelectTileIndex);

	//pTile->Set_TileFlag(CTile::eTileFlags_Default);
	m_vecTileDebugger[m_iCurSelectLayer]->Set_TileAlpha(m_iCurSelectTileIndex, 1.f);
	
	m_pSelectTile = pTile;
}

void CWindow_Tile::On_ShootRay()
{
	/* 아래부터 위로 Ray 쏘기 */
	Find_Terrain();

	//1. 0층부터

	for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
	{
		if (FAILED(ShootRay_Layer(i)))
			return;

		m_vecTileDebugger[i]->Initialize();
	}


	/* 다 쐈으면 쌍방 연결 */
	for (_uint i = 0; i < m_vecTileDebugger.size(); ++i)
	{
		if (FAILED(ReConnect_Tiles(i)))
			return;
	}


	Call_MsgBox(L"SHOOT DONE");

}

HRESULT CWindow_Tile::ShootRay_FirstFloor()
{
	
	return S_OK;

}

HRESULT CWindow_Tile::ShootRay_Layer(_uint iLayerIndex)
{
	_float4x4 matWorld = m_vecTileDebugger[iLayerIndex]->m_pTransform->Get_WorldMatrix();
	_float4 vRayDir = _float4(0.f, -1.f, 0.f, 0.f);

	_float fDownStandardY = -999.f;

	if (iLayerIndex > 0)
	{
		fDownStandardY = m_vecTileDebugger[iLayerIndex - 1]->Get_Transform()->Get_World(WORLD_POS).y;
	}


	for (_uint i = 0; i < m_iNumTilesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumTilesX; ++j)
		{

			_uint iCurTileIndex = i * m_iNumTilesX + j;
			CTile* pCurTile = GAMEINSTANCE->Find_Tile(iLayerIndex, iCurTileIndex);

			if (!pCurTile)
			{
				Call_MsgBox_Index(L"타일을 못찾음 뭔가 이상함 Index : ", iCurTileIndex);
				return E_FAIL;
			}

			_float4 vCenterPos = pCurTile->Get_LocalCenterPos().MultiplyCoord(matWorld);

			_bool bTerrainHit = false;
			_bool bBoxHit = false;

			/* 일단 터레인한테 쏘고 */
			_float4 vHitPos, vBoxHitNormal, vBoxHitPos;

			bTerrainHit = ShootRay_ToTerrain(vCenterPos, vRayDir, vHitPos);
			if (bTerrainHit)
			{
				//0. HitPos들이 DownStandardY보다 작으면 걍 무효
				if (vHitPos.y < fDownStandardY)
				{
					bTerrainHit = false;
				}
			}

			bBoxHit = ShootRay_ToBoxes(vCenterPos, vRayDir, vBoxHitPos, vBoxHitNormal);
			if (bBoxHit)
			{
				//0. HitPos들이 DownStandardY보다 작으면 걍 무효
				if (vBoxHitPos.y < fDownStandardY)
				{
					bBoxHit = false;
				}
			}

			//1. Terrain도 없고 Cube도 없다
			if (!bTerrainHit)
			{
				if (!bBoxHit)
				{
					pCurTile->Set_WorldCenterPos(_float4(0.f, -999.f, 0.f));
					pCurTile->Set_TileFlag(CTile::eTileFlags_None);
				}
				else
				{
					//2. Terrain없지만 Cube는 있따

					//
					pCurTile->Set_WorldCenterPos(vBoxHitPos);
					pCurTile->Set_TileFlag(CTile::eTileFlags_Default);
				}
			}
			else
			{
				//3. Terrain 있고 Cube 없다
				if (!bBoxHit)
				{
					pCurTile->Set_WorldCenterPos(vHitPos);
					pCurTile->Set_TileFlag(CTile::eTileFlags_Default);
				}
				else
				{
					//4. Terrain있고 Cube도 있어
					
					//4-1. 박스가 더 높이 있다 : 박스가 더 높이 있따는건 막힌 길이란 뜻인디
					if (vBoxHitPos.y > vHitPos.y)
					{
						//막혀있는 경우임
						pCurTile->Set_TileFlag(CTile::eTileFlags_Blocked);
						pCurTile->Set_WorldCenterPos(vBoxHitPos);
					}
					else
					{
						//4-2. 박스가 더 낮게 있다 : Terrain 아래 깔린 박스이므로 무시하믄 댐
						pCurTile->Set_WorldCenterPos(vHitPos);
						pCurTile->Set_TileFlag(CTile::eTileFlags_Default);
					}

				}
			}



		}
	}


	



	return S_OK;
}

HRESULT CWindow_Tile::ReConnect_Tiles(_uint iLayerIndex)
{

	for (_uint i = 0; i < m_iNumTilesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumTilesX; ++j)
		{

			_uint iCurTileIndex = i * m_iNumTilesX + j;
			CTile* pCurTile = GAMEINSTANCE->Find_Tile(iLayerIndex, iCurTileIndex);

			if (!pCurTile)
			{
				Call_MsgBox_Index(L"타일을 못찾음 뭔가 이상함 Index : ", iCurTileIndex);
				return E_FAIL;
			}

			if (!pCurTile->Is_ValidTile())
			{
				/*바로 위 아래 레이어의 같은 인덱스를 조사.*/
				CTile* pUpTile = GAMEINSTANCE->Find_Tile(iLayerIndex + 1, iCurTileIndex);
				CTile* pDownTile = GAMEINSTANCE->Find_Tile(iLayerIndex - 1, iCurTileIndex);

				if (pUpTile && pUpTile->Is_ValidTile())
				{
					pCurTile->Switch_AllAdjTiles_To(pUpTile);
				}

				if (pDownTile && pDownTile->Is_ValidTile())
				{
					pCurTile->Switch_AllAdjTiles_To(pDownTile);
				}
			}

		}
	}

	return S_OK;
}

_bool CWindow_Tile::ShootRay_ToTerrain(_float4 vStartPos, _float4 vRayDir, _float4& vOutPos)
{
	_float4 vPickedPos, vPickedNormal;

	if (GAMEINSTANCE->Is_Picked(m_pMeshTerrain, vStartPos, vRayDir, &vPickedPos, &vPickedNormal))
	{
		vOutPos = vPickedPos;
		return true;
	}

	return false;
}

_bool CWindow_Tile::ShootRay_ToBoxes(_float4 vStartPos, _float4 vRayDir, _float4& vOutPos, _float4& vOutNormal)
{
	list<CGameObject*>	m_BoxLists = GAMEINSTANCE->Get_ObjGroup(GROUP_PHYSX);

	_float4 vPickedPos, vPickedNormal, vFinalPickedPos = ZERO_VECTOR;
	_float fDist = 999999.f;

	for (auto& elem : m_BoxLists)
	{
		CMesh* pMesh = GET_COMPONENT_FROM(elem, CMesh);

		if (GAMEINSTANCE->Is_Picked(pMesh, vStartPos, vRayDir, &vPickedPos, &vPickedNormal))
		{
			_float fNewDist = (vStartPos - vPickedPos).Length();

			if (fNewDist < fDist)
			{
				fDist = fNewDist;
				vOutPos = vPickedPos;
				vOutNormal = vPickedNormal;
			}
		}
	}

	if (fDist < 999999.f)
		return true;


	return false;
}

void CWindow_Tile::Find_Terrain()
{
	list<CGameObject*>& DefaultList = GAMEINSTANCE->Get_ObjGroup(GROUP_DEFAULT);

	for (auto& elem : DefaultList)
	{
		list<CComponent*> MeshList = elem->Get_Component<CMesh>();
		if (MeshList.empty())
			continue;

		m_pMeshTerrain = dynamic_cast<CMesh_Terrain*>(MeshList.front());

		if (!m_pMeshTerrain)
			continue;
		else
			break;
	}

	if (!m_pMeshTerrain)
	{
		list<CGameObject*>& DefaultList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);

		for (auto& elem : DefaultList)
		{
			list<CComponent*> MeshList = elem->Get_Component<CMesh>();
			if (MeshList.empty())
				continue;

			m_pMeshTerrain = dynamic_cast<CMesh_Terrain*>(MeshList.front());

			if (!m_pMeshTerrain)
				continue;
			else
				break;
		}
	}
}

void CWindow_Tile::Save_All(string strKey)
{
	if (m_vecTileDebugger.empty())
		return;

	string strPath = m_DataDirectory;
	strPath  += "/";
	strPath += strKey;
	strPath += ".bin";

	ofstream	writeFile(strPath, ios::binary);


	if (!writeFile.is_open())
	{
		Call_MsgBox(L"SSave 실패 ??!?!");
		return;
	}

	_int iNumLayers = m_vecTileDebugger.size();

	writeFile.write((char*)&m_iNumTilesX, sizeof(_uint));
	writeFile.write((char*)&m_iNumTilesZ, sizeof(_uint));
	writeFile.write((char*)&iNumLayers, sizeof(_uint));
	writeFile.write((char*)&m_fTileSize, sizeof(_float));

	

	
}

void CWindow_Tile::Save_AllLayer(string strKey)
{
	if (strKey.empty())
		return;

	string strPath = m_CellDataDirectory;
	strPath += "/";
	strPath += strKey;
	strPath += "/";
	if (!filesystem::exists(strPath))
	{
		filesystem::create_directory(strPath);
	}
	
	filesystem::directory_iterator FileIter(strPath);
	const filesystem::directory_entry& entry = *FileIter;
	_uint Index = 0;
	for (auto& Layers : m_pLayers)
	{
		wstring strSavePath = CFunctor::To_Wstring(strPath);
		//strSavePath += L"/";
		strSavePath += CFunctor::To_Wstring(strKey);
		strSavePath += L"_";
		strSavePath += to_wstring(Index);
		strSavePath += L"/";

		wstring Name = CFunctor::To_Wstring(strKey);
		Name += L"_";
		Name += to_wstring(Index);

		Layers.second->Set_DebugName(Name);

		Layers.second->Save(strSavePath, Name);
		Index++;
	}
}

void CWindow_Tile::Load_All(string strKey)
{
}

void CWindow_Tile::Load_AllLayer(string strKey)
{
	/*string strPath = m_CellDataDirectory;
	strPath += "/";
	strPath += strKey;
	strPath += "/";*/
	string strPath = strKey;//CellData/Map_Paden
	if (!filesystem::exists(strPath))
	{
		return;
	}

	for (auto& Layer : m_pLayers)
		SAFE_DELETE(Layer.second);
	m_pLayers.clear();


	for (filesystem::directory_iterator FileIter(strKey);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		//CellData/Map_Paden/Map_Paden_0
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\");
		string strFileName = strFullPath.substr(iFind + 1, strFullPath.length());
		if (entry.is_directory())
		{
			//_int iFindExt = (int)strFileName.rfind(".") + 1;
			//string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);
			//strFileName = strFileName.substr(0, iFindExt);
			
			CCellLayer* pLayer = CCellLayer::Create(wstrPath, CFunctor::To_Wstring(strFileName));
			//pLayer->Set_DebugName(CFunctor::To_Wstring(strFileName));

			m_pLayers.emplace(pLayer->Get_MinHeight(), pLayer);
		}
	}

	for (auto& Layers : m_pLayers)
	{
		Layers.second->SetUp_Neighbor(m_pLayers);
		////노드 설정
		//Layers.second->SetUp_Nodes();
		////가시성 설정
		//Layers.second->SetUp_Visibility();
	}
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

void CWindow_Tile::On_CellPicking(_uint iLayerIndex, _float4 vPickedPos)
{
	if (m_pCurLayer)
	{
		m_pSelectCell = m_pCurLayer->Find_Cell(vPickedPos);
	}
}

void CWindow_Tile::On_CellSetAttribute(_uint iLayerIndex, _float4 vPickedPos, _float fRange)
{
	if (m_pCurLayer)
	{
		list<CCell*> pCellList = m_pCurLayer->Find_Cell_InRange(vPickedPos, fRange);
		if (pCellList.empty())
			return;
		for (auto& Value : pCellList) 
		{
			if(Value)
				Value->Set_Flags(m_iAttributeArray[m_iAttrubuteIndex].second);
		}
	}
}

void CWindow_Tile::On_Replace_PickedAttribute(_float4 vPickedPos)
{
	if (m_pCurLayer)
	{
		CCell* pCell = m_pCurLayer->Find_Cell(vPickedPos);
		_byte PickedAttribute = pCell->Get_Attribute();

		m_pCurLayer->Replace_Attribute(PickedAttribute, m_iAttributeArray[m_iAttrubuteIndex].second);
	}
}

void CWindow_Tile::On_CellPick_Neighbor(_uint iLayerIndex, _float4 vPickedPos)
{
	if (m_pCurLayer)
	{
		//if (nullptr == m_pSelectCell)
		//	return;


		//CCell* pNeighbor = m_pCurLayer->Find_Cell(vPickedPos);
		//m_pSelectCell->Set_Neighbor(m_iNeighborLine, pNeighbor);
	}
}

void CWindow_Tile::Create_DebugObject()
{
	if (m_pCurLayer)
	{
		for (auto Pos : m_pCurLayer->Get_BlockedPositionList())
		{
			PxTransform tTransform;
			ZeroMemory(&tTransform, sizeof(PxTransform));
			tTransform.p.x = Pos.x;
			tTransform.p.y = Pos.y;
			tTransform.p.z = Pos.z;
			m_listDebug.push_back(
				CDebugObject::Create(tTransform)
			);
			CREATE_GAMEOBJECT(m_listDebug.back(), GROUP_PHYSX);
		}

		for (auto Link : m_pCurLayer->Get_LinkList())
		{
			_float4 vDir = (Link.second - Link.first);
			_float fLength = vDir.Length();
			_float4 vLinePos = (Link.second + Link.first) * 0.5f;

			_float4 vScale = _float4(0.2f, 0.2f, fLength);

			CDebugObject* pDebugLine = CDebugObject::Create(vLinePos, vScale);
			pDebugLine->Initialize();
			pDebugLine->Set_Blue();
			pDebugLine->Get_Transform()->Set_Look(vDir);
			CREATE_GAMEOBJECT(pDebugLine, GROUP_PROP);
			m_listDebugLine.push_back(pDebugLine);
		}


	}
}

void CWindow_Tile::Clear_DebugObject()
{
	for (auto& Debug : m_listDebug)
	{
		DELETE_GAMEOBJECT(Debug);
	}
	m_listDebug.clear();

	for (auto& Debug : m_listDebugLine)
	{
		DELETE_GAMEOBJECT(Debug);
	}
	m_listDebugLine.clear();
}
