#include "stdafx.h"
#include "CWindow_Position.h"

#include "ImGui_Manager.h"

#include "PhysXCollider.h"

#include "CTestObject.h"

#include "CDebugObject.h"

#include "UsefulHeaders.h"
#include "CMesh_Terrain.h"

#include "CUtility_File.h"

CWindow_Position::CWindow_Position()
{
}

CWindow_Position::~CWindow_Position()
{
}

CWindow_Position* CWindow_Position::Create()
{
	CWindow_Position* pInstance = new CWindow_Position;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Position");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Position::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Position).name(), ImVec2(400.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_Position::Tick()
{
	if (KEY(Q, NONE))
	{
		m_bPicking = false;
	}
	else
		m_bPicking = true;

	if (m_bPicking)
	{

		if (m_bCreateMode)
			Update_CreateMode();
		else
			Update_SelectMode();



	}
}

HRESULT CWindow_Position::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	//위치 지정하고 저장해놓는 툴

	if (ImGui::InputText("FileName", m_szFileName, sizeof(m_szFileName), ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}

	if (ImGui::Button("SAVE"))
	{
		Save_Data();
	}

	ImGui::SameLine();

	if (ImGui::Button("LOAD"))
	{
		Load_Data();
	}

	if (ImGui::Button("POP_BACK(DELETE)") || KEY(DELETEKEY, TAP))
	{
		if (!m_vecPosition.empty())
		{
			m_vecPosition.pop_back();
			DELETE_GAMEOBJECT(m_pDebugBoxes.back());
			m_pDebugBoxes.pop_back();

			if (m_iCurSelectedIndex != 99999)
			{
				if (m_iCurSelectedIndex >= m_pDebugBoxes.size())
					m_iCurSelectedIndex = m_pDebugBoxes.size() - 1;
			}

		}
	}

	ImGui::SameLine();

	if (ImGui::Button("DELETE"))
	{
		if (m_iCurSelectedIndex != 99999)
		{
			auto PosIter = m_vecPosition.begin();
			auto BoxIter = m_pDebugBoxes.begin();

			for (_uint i = 0; i < m_iCurSelectedIndex; ++i)
			{
				PosIter++;
				BoxIter++;
			}

			DELETE_GAMEOBJECT(m_pDebugBoxes[m_iCurSelectedIndex]);
			m_vecPosition.erase(PosIter);
			m_pDebugBoxes.erase(BoxIter);

			m_iCurSelectedIndex = 99999;
		}

	}

	ImGui::SameLine();
	if (ImGui::Button("DELETE ALL"))
	{
		Delete_All();
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


	ImGui::Text(" == Box Info == ");

	//1. 박스 설치 : 피킹
	if (m_iCurSelectedIndex != 99999)
	{
		char	m_szCurName[MIN_STR] = {};
		strcpy_s(m_szCurName, m_vecPosition[m_iCurSelectedIndex].first.c_str());

		if (ImGui::InputText("PositionName", m_szCurName, sizeof(m_szCurName)))
		{
			string strTemp = m_szCurName;
			m_vecPosition[m_iCurSelectedIndex].first = strTemp;
		}



		CDebugObject* pCurDebugBox = m_pDebugBoxes[m_iCurSelectedIndex];

		//1. 위치

		_float	vPosition[3] = {};
		memcpy(vPosition, &m_vecPosition[m_iCurSelectedIndex].second, sizeof(_float3));


		if (ImGui::DragFloat3("Position", vPosition, 0.1f, -999.f, 999.f, "%.1f"))
		{
			memcpy(&m_vecPosition[m_iCurSelectedIndex].second, vPosition, sizeof(_float3));
			pCurDebugBox->Get_Transform()->Set_World(WORLD_POS, m_vecPosition[m_iCurSelectedIndex].second);

		}
	}


	__super::End();

	return S_OK;
}

void CWindow_Position::Find_Terrain()
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

void CWindow_Position::Update_CreateMode()
{
	if (KEY(LBUTTON, TAP))
	{
		if (!m_pMeshTerrain)
		{
			Find_Terrain();
		}


		//피킹해서 만들기
		_float4 vPos, vNormal;
		GAMEINSTANCE->Compute_WorldRay();
		if (GAMEINSTANCE->Is_Picked(m_pMeshTerrain, &vPos, &vNormal))
		{
			PxTransform tTransform;
			ZeroMemory(&tTransform, sizeof(PxTransform));
			vPos = vPos.MultiplyCoord(m_pMeshTerrain->Get_Owner()->Get_Transform()->Get_WorldMatrix());
			tTransform.p.x = vPos.x;
			tTransform.p.y = vPos.y;
			tTransform.p.z = vPos.z;

			_float4 vQuat = XMQuaternionRotationRollPitchYawFromVector(
				ZERO_VECTOR.XMLoad());

			memcpy(&tTransform.q, &vQuat, sizeof(_float4));

			static _uint iTemp = 0;
			string strName = "Position_";
			strName += to_string(iTemp++);

			m_vecPosition.push_back(make_pair(strName, vPos));
			m_pDebugBoxes.push_back(
				CDebugObject::Create(tTransform)
			);

			CREATE_GAMEOBJECT(m_pDebugBoxes.back(), GROUP_DEBUG);
		}

	}



}

void CWindow_Position::Update_SelectMode()
{

	if (KEY(LBUTTON, TAP))
	{
		if (!m_pMeshTerrain)
		{
			Find_Terrain();
		}


		//피킹해서 만들기

		vector<CGameObject*> GameObjects;
		for (_uint i = 0; i < m_pDebugBoxes.size(); ++i)
		{
			GameObjects.push_back(m_pDebugBoxes[i]);
		}

		_float4 vPos, vNormal;
		_uint iIndex;

		if (GAMEINSTANCE->Is_Picked_Cubes(GameObjects, &vPos, &iIndex, &vNormal))
		{
			if (m_iCurSelectedIndex != 99999)
				m_pDebugBoxes[m_iCurSelectedIndex]->m_vFontColor = _float4(0.f, 1.f, 0.f, 1.f);

			m_iCurSelectedIndex = iIndex;

			m_pDebugBoxes[m_iCurSelectedIndex]->m_vFontColor = _float4(1.f, 0.f, 0.f, 1.f);

		}

	}

}

void CWindow_Position::Save_Data()
{
	string savePath;
	savePath = "../bin/Data/GameSystem/Positions/";
	savePath += m_szFileName;
	savePath += ".bin";


	_uint iNumPositions = m_vecPosition.size();

	ofstream	writeFile(savePath, ios::binary);


	if (!writeFile.is_open())
	{
		Call_MsgBox(L"SSave 실패 ??!?!");
		return;
	}

	writeFile.write((char*)&iNumPositions, sizeof(_uint));

	for (_uint i = 0; i < iNumPositions; ++i)
	{
		CUtility_File::Write_Text(&writeFile, m_vecPosition[i].first.c_str());
		writeFile.write((char*)&m_vecPosition[i].second, sizeof(_float4));
	}

	writeFile.close();

	Call_MsgBox(L"Save 성공");

}

void CWindow_Position::Load_Data()
{
	string savePath;
	savePath = "../bin/Data/GameSystem/Positions/";
	savePath += m_szFileName;
	savePath += ".bin";


	ifstream	readFile(savePath, ios::binary);


	if (!readFile.is_open())
	{
		Call_MsgBox(L"로드 경로 없음");
		return;
	}

	Delete_All();
	_uint iNumBoxes = 0;

	readFile.read((char*)&iNumBoxes, sizeof(_uint));

	for (_uint i = 0; i < iNumBoxes; ++i)
	{
		string strTemp;
		_float4 vTemp;

		strTemp = CUtility_File::Read_Text(&readFile);
		readFile.read((char*)&vTemp, sizeof(_float4));

		m_vecPosition.push_back(make_pair(strTemp, vTemp));
		
		m_pDebugBoxes.push_back(CDebugObject::Create(vTemp));
		CREATE_GAMEOBJECT(m_pDebugBoxes.back(), GROUP_DEBUG);
	}

	readFile.close();

}

void CWindow_Position::Delete_All()
{
	for (_uint i = 0; i < m_pDebugBoxes.size(); ++i)
	{
		DELETE_GAMEOBJECT(m_pDebugBoxes[i]);
	}

	m_vecPosition.clear();
	m_pDebugBoxes.clear();
	m_iCurSelectedIndex = 99999;
}

void CWindow_Position::Create_DebugObject(_float4 vPos)
{
	CDebugObject* pDebugObject = CDebugObject::Create(vPos);
	pDebugObject->Initialize();
	CREATE_GAMEOBJECT(pDebugObject, GROUP_PROP);
}
