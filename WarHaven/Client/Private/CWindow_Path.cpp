#include "stdafx.h"
#include "CWindow_Path.h"

#include "ImGui_Manager.h"

#include "PhysXCollider.h"

#include "CTestObject.h"

#include "CDebugObject.h"

#include "UsefulHeaders.h"
#include "CMesh_Terrain.h"

#include "CUtility_File.h"

#include "CPath.h"

CWindow_Path::CWindow_Path()
{
}

CWindow_Path::~CWindow_Path()
{
	for (auto& elem : m_vecPathDesc)
		SAFE_DELETE(elem.pPath);
}

CWindow_Path* CWindow_Path::Create()
{
	CWindow_Path* pInstance = new CWindow_Path;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Path");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Path::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Path).name(), ImVec2(400.f, 500.f), window_flags);

	return S_OK;
}

void CWindow_Path::Tick()
{
	if (KEY(Q, NONE))
	{
		m_bPicking = false;
	}
	else
		m_bPicking = true;

	if (m_bPicking)
	{
		if (m_iCurSelectedIndex != 99999)
		{
			if (m_bCreateMode)
				Update_CreateMode();
			else
				Update_SelectMode();
		}

	}
}

HRESULT CWindow_Path::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::Button("Load_All"))
		Load_AllPathes();

	/* list box */
	if (ImGui::CollapsingHeader("- PATH LIST -"))
	{
		if (ImGui::Button("Create New Path"))
			Create_NewPath();

		if (ImGui::Button("Delete Selected Path"))
			Deleted_SelectedPath();

		for (_uint i = 0; i < m_vecPathDesc.size(); ++i)
		{

			if (ImGui::Selectable(m_vecPathDesc[i].pPath->m_strName.c_str(), &m_vecPathDesc[i].bSelected))
			{
				//이전꺼 다 초록색으로
				if (m_iCurSelectedIndex != 99999)
				{
					for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes)
					{
						elem->Set_Green();
					}
					for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecLineBox)
					{
						elem->Set_Green();
					}
					m_vecPathDesc[m_iCurSelectedIndex].bSelected = false;
				}


				m_vecPathDesc[i].bSelected = true;
				m_iCurSelectedIndex = i;

				//현재꺼 다 파란색으로
				for (auto& elem : m_vecPathDesc[i].vecDebugBoxes)
					elem->Set_Blue();

				for (auto& elem : m_vecPathDesc[i].vecLineBox)
					elem->Set_Blue();

			}
		}
	}

	if (m_iCurSelectedIndex != 99999)
	{
		if (ImGui::CollapsingHeader(" - Selected Path Info - "))
		{
			static char	szName[MIN_STR];
			strcpy_s(szName, m_vecPathDesc[m_iCurSelectedIndex].pPath->m_strName.c_str());

			if (ImGui::InputText("Name", szName, sizeof(szName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_vecPathDesc[m_iCurSelectedIndex].pPath->m_strName = szName;
			}

			string strCurSize = "Current Size : ";
			strCurSize += to_string(m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions);

			ImGui::Text(strCurSize.c_str());

			if (ImGui::Button("SAVE PATH"))
			{
				m_vecPathDesc[m_iCurSelectedIndex].pPath->Save_CurPath();
			}

			if (ImGui::Button("POP_BACK") || KEY(DELETEKEY, TAP))
			{
				if (!m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.empty())
				{
					DELETE_GAMEOBJECT(m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.back());
					m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.pop_back();

					if (!m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.empty())
					{
						DELETE_GAMEOBJECT(m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.back());
						m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.pop_back();
					}
					

					m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.pop_back();
					m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions = m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.size();

					if (m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx >= m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions)
					{
						if (m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.empty())
						{
							m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx = 99999;
						}
						else
						{
							m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx = m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions - 1;
							m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes[m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx]->Set_Red();
						}

						
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("DELETE_SELECTED"))
			{
				if (!m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.empty())
				{
					if (m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx < m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions)
					{
						auto iter = m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.begin();

						for (_uint i = 0; i < m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx; ++i)
							++iter;

						m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.erase(iter);

						DELETE_GAMEOBJECT(m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes[m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx]);
						auto iter2 = m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.begin();

						for (_uint i = 0; i < m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx; ++i)
							++iter2;

						m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.erase(iter2);

						/* Line은 인덱스가 1이상이었을때만 지우기 */
						if (m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx >= 1)
						{
							_uint iLineIndex = m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx - 1;
							DELETE_GAMEOBJECT(m_vecPathDesc[m_iCurSelectedIndex].vecLineBox[iLineIndex]);
							auto iter3 = m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.begin();

							for (_uint i = 0; i < iLineIndex; ++i)
								++iter3;

							m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.erase(iter3);
						}
						



						m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions = m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.size();

						m_vecPathDesc[m_iCurSelectedIndex].iCurSelectedPositionIdx = 99999;

						ReMake_Lines(m_iCurSelectedIndex);
					}

				}
			}
		}
	}
	


	ImGui::Text(" - HOLD Q to picking - ");
	if (m_bPicking)
		ImGui::Text(" Picking ! ON ! ");
	else
		ImGui::Text(" Picking = OFF = ");

	if (ImGui::RadioButton("Create_Mode(V)", m_bCreateMode) || KEY(V, TAP))
	{
		m_bCreateMode = !m_bCreateMode;
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Select_Mode(V)", !m_bCreateMode))
	{
		m_bCreateMode = !m_bCreateMode;
	}

	__super::End();

	return S_OK;
}

void CWindow_Path::Find_Terrain()
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

void CWindow_Path::Update_CreateMode()
{
	if (KEY(LBUTTON, TAP))
	{
		Find_Terrain();


		//피킹해서 만들기
		_float4 vPos = ZERO_VECTOR, vNormal = ZERO_VECTOR, vPos2 = ZERO_VECTOR, vNormal2;
		GAMEINSTANCE->Compute_WorldRay();

		_bool bTerrain = GAMEINSTANCE->Is_Picked(m_pMeshTerrain, &vPos, &vNormal);

		list<CGameObject*>	PhysXList = GAMEINSTANCE->Get_ObjGroup(GROUP_PHYSX);

		vector<CGameObject*>	vecBoxes;

		for (auto& elem : PhysXList)
			vecBoxes.push_back(elem);

		_uint iIndexTemp;
		_bool bCubes = GAMEINSTANCE->Is_Picked_Cubes(vecBoxes, &vPos2, &iIndexTemp, &vNormal2);

		if (!bTerrain && !bCubes)
			return;

		_float4 vFinalPos;


		if (bTerrain && bCubes)
		{
			_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
			_float fDist1 = (vPos - vCamPos).Length();
			_float fDist2 = (vPos2 - vCamPos).Length();

			if (fDist1 < fDist2)
				vFinalPos = vPos;
			else
				vFinalPos = vPos2;
		}
		else if (bTerrain)
			vFinalPos = vPos;
		else
			vFinalPos = vPos2;

		

		CDebugObject* pDebugObject = CDebugObject::Create(vFinalPos);
		pDebugObject->Initialize();
		pDebugObject->Set_Blue();
		CREATE_GAMEOBJECT(pDebugObject, GROUP_DEBUG);
		m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.push_back(pDebugObject);
		m_vecPathDesc[m_iCurSelectedIndex].pPath->m_iNumPositions++;
		m_vecPathDesc[m_iCurSelectedIndex].pPath->m_vecPositions.push_back(vFinalPos);

		/* 이전 노드의 녀석과 연결하는 선을 하나 만들자 */
		if (m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.size() <= 1)
			return;

		_float4 vPrevPos = m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes[m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes.size() - 2]
			->Get_Transform()->Get_World(WORLD_POS);

		_float4 vDir = (vFinalPos - vPrevPos);
		_float fLength = vDir.Length();
		_float4 vLinePos = (vFinalPos + vPrevPos) * 0.5f;

		_float4 vScale = _float4(0.2f, 0.2f, fLength);

		CDebugObject* pDebugLine = CDebugObject::Create(vLinePos, vScale);
		pDebugLine->Initialize();
		pDebugLine->Set_Blue();
		pDebugLine->Get_Transform()->Set_Look(vDir);
		CREATE_GAMEOBJECT(pDebugLine, GROUP_PROP);
		m_vecPathDesc[m_iCurSelectedIndex].vecLineBox.push_back(pDebugLine);

	}



}

void CWindow_Path::Update_SelectMode()
{

	if (KEY(LBUTTON, TAP))
	{
		Find_Terrain();

		//피킹해서 만들기

		for (_uint i = 0; i < m_vecPathDesc.size(); ++i)
		{
			vector<CGameObject*> GameObjects;
			for (_uint j = 0; j < m_vecPathDesc[i].vecDebugBoxes.size(); ++j)
			{
				GameObjects.push_back(m_vecPathDesc[i].vecDebugBoxes[j]);
			}

			_float4 vPos, vNormal;
			_uint iIndex;

			if (GAMEINSTANCE->Is_Picked_Cubes(GameObjects, &vPos, &iIndex, &vNormal))
			{
				if (m_iCurSelectedIndex != 99999)
				for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes)
					elem->Set_Green();

				for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecLineBox)
					elem->Set_Green();

				for (auto& elem : m_vecPathDesc[i].vecDebugBoxes)
					elem->Set_Blue();
				
				for (auto& elem : m_vecPathDesc[i].vecLineBox)
					elem->Set_Blue();

				m_vecPathDesc[i].vecDebugBoxes[iIndex]->Set_Red();
				m_iCurSelectedIndex = i;
				m_vecPathDesc[i].iCurSelectedPositionIdx = iIndex;

				return;
			}

		}

		
	}
}



void CWindow_Path::Create_DebugObject(_float4 vPos)
{
	CDebugObject* pDebugObject = CDebugObject::Create(vPos);
	pDebugObject->Initialize();
	CREATE_GAMEOBJECT(pDebugObject, GROUP_DEBUG);
}

void CWindow_Path::Create_NewPath()
{
	string strDefaultName = "Hi_Im_Path";
	static _uint g_iPathIndex = 0;
	strDefaultName += to_string(g_iPathIndex++);
	PATH_DESC	tDesc;
	tDesc.pPath = new CPath;
	tDesc.pPath->m_strName = strDefaultName;

	m_vecPathDesc.push_back(tDesc);
}

void CWindow_Path::Deleted_SelectedPath()
{
	if (m_iCurSelectedIndex == 99999)
		return;

	auto iter = m_vecPathDesc.begin();

	for (_uint i = 0; i < m_iCurSelectedIndex; ++i)
		++iter;

	for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecDebugBoxes)
	{
		DELETE_GAMEOBJECT(elem);
	}

	for (auto& elem : m_vecPathDesc[m_iCurSelectedIndex].vecLineBox)
	{
		DELETE_GAMEOBJECT(elem);
	}

	SAFE_DELETE(m_vecPathDesc[m_iCurSelectedIndex].pPath);
	m_vecPathDesc.erase(iter);
	m_iCurSelectedIndex = 99999;
}

void CWindow_Path::ReMake_Lines(_uint iCurPathIndex)
{
	//다 지우고 다시 연결
	for (auto& elem : m_vecPathDesc[iCurPathIndex].vecLineBox)
		DELETE_GAMEOBJECT(elem);

	m_vecPathDesc[iCurPathIndex].vecLineBox.clear();

	for (_uint i = 1; i < m_vecPathDesc[iCurPathIndex].vecDebugBoxes.size(); ++i)
	{
		//이전꺼랑 현재꺼를 연결쓰

		_float4 vPrevPos = m_vecPathDesc[iCurPathIndex].vecDebugBoxes[i-1]->Get_Transform()->Get_World(WORLD_POS);
		_float4 vCurPos = m_vecPathDesc[iCurPathIndex].vecDebugBoxes[i]->Get_Transform()->Get_World(WORLD_POS);

		_float4 vDir = (vCurPos - vPrevPos);
		_float fLength = vDir.Length();
		_float4 vLinePos = (vCurPos + vPrevPos) * 0.5f;

		_float4 vScale = _float4(0.2f, 0.2f, fLength);

		CDebugObject* pDebugLine = CDebugObject::Create(vLinePos, vScale);
		pDebugLine->Initialize();
		pDebugLine->Set_Blue();
		pDebugLine->Get_Transform()->Set_Look(vDir);
		CREATE_GAMEOBJECT(pDebugLine, GROUP_PROP);
		m_vecPathDesc[iCurPathIndex].vecLineBox.push_back(pDebugLine);
	}
}

void CWindow_Path::Load_AllPathes()
{
	for (auto& elem : m_vecPathDesc)
		SAFE_DELETE(elem.pPath);

	m_vecPathDesc.clear();


	_uint iPlusIndex = 0;
	for (_uint j = 0; j < CGameSystem::eSTAGE_CNT; ++j)
	{
		for (auto& elem : CGameSystem::Get_Instance()->m_mapAllPathes[j])
		{
			PATH_DESC	tDesc;

			tDesc.pPath = elem.second->Clone();

			for (_uint i = 0; i < tDesc.pPath->m_iNumPositions; ++i)
			{
				//debugbox 생성
				CDebugObject* pDebugObject = CDebugObject::Create(tDesc.pPath->m_vecPositions[i]);
				pDebugObject->Initialize();
				pDebugObject->Set_Green();
				CREATE_GAMEOBJECT(pDebugObject, GROUP_DEBUG);
				tDesc.vecDebugBoxes.push_back(pDebugObject);
			}

			m_vecPathDesc.push_back(tDesc);
			ReMake_Lines(iPlusIndex++);
		}
	}
	
}
