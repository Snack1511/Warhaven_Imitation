#include "stdafx.h"
#include "CWindow_PhysX.h"

#include "ImGui_Manager.h"

#include "PhysXCollider.h"

#include "CTestObject.h"

#include "CDebugObject.h"

#include "UsefulHeaders.h"
#include "CMesh_Terrain.h"
#include "CUtility_PhysX.h"
CWindow_PhysX::CWindow_PhysX()
{
}

CWindow_PhysX::~CWindow_PhysX()
{
}

CWindow_PhysX* CWindow_PhysX::Create()
{
	CWindow_PhysX* pInstance = new CWindow_PhysX;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_PhysX");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_PhysX::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_PhysX).name(), ImVec2(400.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_PhysX::Tick()
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
	else
	{
		Update_ObjectControl(m_iCurSelectedIndex);
	}
}

HRESULT CWindow_PhysX::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::Button("DISABLE_ALL"))
	{
		for (auto& elem : m_pDebugBoxes)
		{
			if (elem->Is_Valid())
				DISABLE_GAMEOBJECT(elem);
			else
				ENABLE_GAMEOBJECT(elem);
		}
	}

	if (ImGui::InputText("FileName", m_szName, sizeof(m_szName), ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
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
		if (!m_StaticBoxes.empty())
		{
			m_StaticBoxes.back()->release();
			m_StaticBoxes.pop_back();
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
			auto PxIter = m_StaticBoxes.begin();
			auto BoxIter = m_pDebugBoxes.begin();

			for (_uint i = 0; i < m_iCurSelectedIndex; ++i)
			{
				PxIter++;
				BoxIter++;
			}

			m_StaticBoxes[m_iCurSelectedIndex]->release();
			DELETE_GAMEOBJECT(m_pDebugBoxes[m_iCurSelectedIndex]);
			m_StaticBoxes.erase(PxIter);
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

	if (ImGui::Button("CLONE(G)") || KEY(G, TAP))
	{
		if (m_iCurSelectedIndex != 99999)
		{
			



			PxTransform tTransform = m_StaticBoxes[m_iCurSelectedIndex]->getGlobalPose();

			CDebugObject* pNewBox = CDebugObject::Create(tTransform, m_pDebugBoxes[m_iCurSelectedIndex]->Get_Transform()->Get_Scale());
			pNewBox->Initialize();
			CREATE_GAMEOBJECT(pNewBox, GROUP_PHYSX);
			m_pDebugBoxes.push_back(pNewBox);


			m_StaticBoxes.push_back(
				Create_StaticBox(tTransform, m_pDebugBoxes[m_iCurSelectedIndex]->Get_Transform()->Get_Scale()));


			m_pDebugBoxes[m_iCurSelectedIndex]->m_vFontColor = _float4(0.f, 1.f, 0.f, 1.f);


			m_iCurSelectedIndex = m_pDebugBoxes.size() - 1;


			m_pDebugBoxes[m_iCurSelectedIndex]->m_vFontColor = _float4(1.f, 0.f, 0.f, 1.f);

		}
	}


	ImGui::Text(" == Box Info == ");
	
	//1. ???? ???? : ????
	if (m_iCurSelectedIndex != 99999)
	{
		PxRigidStatic* pCurRigidStatic = m_StaticBoxes[m_iCurSelectedIndex];
		CDebugObject* pCurDebugBox = m_pDebugBoxes[m_iCurSelectedIndex];

		//1. ????
		PxTransform tTransform = pCurRigidStatic->getGlobalPose();
		tTransform.p.x;

		_float	vPosition[3] = { tTransform.p.x , tTransform.p.y, tTransform.p.z };

		
		if (ImGui::DragFloat3("Position", vPosition, 0.1f, -999.f, 999.f, "%.1f"))
		{
			tTransform.p.x = vPosition[0];
			tTransform.p.y = vPosition[1];
			tTransform.p.z = vPosition[2];

			pCurRigidStatic->setGlobalPose(tTransform);
			pCurDebugBox->Synchronize_Box(tTransform);

		}

		//2. ????
		_float4 vScale = pCurDebugBox->Get_Transform()->Get_Scale();

		_float	arrScale[3] = 
		{ vScale.x , vScale.y, vScale.z };

		if (ImGui::DragFloat3("Scale", arrScale, 0.05f, 0.1f, 200.f, "%.1f"))
		{
			if (arrScale[0] >= 0.0001f &&
				arrScale[1] >= 0.0001f &&
				arrScale[2] >= 0.0001f
				)
			{
				vScale.x = arrScale[0];
				vScale.y = arrScale[1];
				vScale.z = arrScale[2];

				pCurDebugBox->ReScale_Box(vScale);
				//?????? ?????? ???? ?q??
				pCurRigidStatic->release();
				pCurRigidStatic = Create_StaticBox(tTransform, vScale);
				m_StaticBoxes[m_iCurSelectedIndex] = pCurRigidStatic;
			}

			
			
		}

		//3. ????

		_float	arrAngle[3] =
		{ pCurDebugBox->m_vAngles.x , pCurDebugBox->m_vAngles.y, pCurDebugBox->m_vAngles.z };

		if (ImGui::DragFloat3("Angle", arrAngle, 1.f, 360.f, 360.f, "%.1f"))
		{
			pCurDebugBox->m_vAngles.x = arrAngle[0];
			pCurDebugBox->m_vAngles.y = arrAngle[1];
			pCurDebugBox->m_vAngles.z = arrAngle[2];

			_float4 vAngles = _float4(
				ToRadian(pCurDebugBox->m_vAngles.x),
				ToRadian(pCurDebugBox->m_vAngles.y),
				ToRadian(pCurDebugBox->m_vAngles.z)
			);

			_float4 vQuat =
				XMQuaternionRotationRollPitchYawFromVector(vAngles.XMLoad());

			memcpy(&tTransform.q, &vQuat, sizeof(_float4));

			pCurRigidStatic->setGlobalPose(tTransform);
			pCurDebugBox->Synchronize_Box(tTransform);

		}

	}


	

	__super::End();

	return S_OK;
}

void CWindow_PhysX::Find_Terrain()
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

void CWindow_PhysX::Update_CreateMode()
{
	if (KEY(LBUTTON, TAP))
	{
		if (!m_pMeshTerrain)
		{
			Find_Terrain();
		}


		//???????? ??????
		_float4 vPos, vNormal;
		GAMEINSTANCE->Compute_WorldRay();
		if (GAMEINSTANCE->Is_Picked(m_pMeshTerrain, &vPos, &vNormal))
		{
			PxTransform tTransform;
			ZeroMemory(&tTransform, sizeof(PxTransform));
			tTransform.p.x = vPos.x;
			tTransform.p.y = vPos.y;
			tTransform.p.z = vPos.z;

			_float4 vQuat = XMQuaternionRotationRollPitchYawFromVector(
				ZERO_VECTOR.XMLoad());
			
			memcpy(&tTransform.q, &vQuat, sizeof(_float4));

			m_StaticBoxes.push_back(
				Create_StaticBox(tTransform, _float4(1.f, 1.f, 1.f)));
			
			m_pDebugBoxes.push_back(
				CDebugObject::Create(tTransform)
			);

			CREATE_GAMEOBJECT(m_pDebugBoxes.back(), GROUP_PHYSX);
		}

	}

	

}

void CWindow_PhysX::Update_SelectMode()
{

	if (KEY(LBUTTON, TAP))
	{
		if (!m_pMeshTerrain)
		{
			Find_Terrain();
		}


		//???????? ??????

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

void CWindow_PhysX::Update_ObjectControl(_uint iCurIndex)
{
	if (iCurIndex == 99999)
		return;

	PxRigidStatic* pCurRigidStatic = m_StaticBoxes[iCurIndex];
	CDebugObject* pCurObj = m_pDebugBoxes[iCurIndex];

	PxTransform tTransform = pCurRigidStatic->getGlobalPose();
	_float4 PosValue = pCurObj->Get_Transform()->Get_World(WORLD_POS);
	_float4 vCurScale = pCurObj->Get_Transform()->Get_Scale();

	if (KEY(LBUTTON, HOLD) || (MOUSE_MOVE(MOUSEMOVE::MMS_WHEEL) != 0.f))
	{
		if (KEY(R, HOLD))
		{
			_float fMouseX = MOUSE_MOVE(MOUSEMOVE::MMS_X);
				_float fMouseY = MOUSE_MOVE(MOUSEMOVE::MMS_Y) * -1.f;

				_float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
				vDir.y = 0.f;
				vDir.Normalize();

				PosValue += vDir * fMouseX * fDT(0);

			vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_UP);

			if (vDir.y > 0.99f)
				vDir = _float4(0.f, 1.f, 0.f, 0.f);
			else
				vDir.y = 0.f;

			vDir.Normalize();

			PosValue += vDir * fMouseY * fDT(0);
		}

		if (KEY(LBUTTON, HOLD) && KEY(T, HOLD))
		{
			_float fMouseY = MOUSE_MOVE(MOUSEMOVE::MMS_Y) * -1.f;

			_float4 vDir = _float4(0.f, 1.f, 0.f, 0.f);

			vDir.Normalize();

			PosValue += vDir * fMouseY * fDT(0);
		}


		/* Scale */
		else if (KEY(C, HOLD))
		{
			_float4 vCamLook = GAMEINSTANCE->Get_CurCamLook();

			_float fMouseWheel = MOUSE_MOVE(MOUSEMOVE::MMS_WHEEL) * fDT(0);
			vCurScale.y += fMouseWheel;

			_float fMouseX = MOUSE_MOVE(MOUSEMOVE::MMS_X);
			_float fMouseY = MOUSE_MOVE(MOUSEMOVE::MMS_Y) * -1.f;

			//look?? 0 0 1 ?? ?? x ???? ????????

			vCurScale.x += fMouseX * fDT(0);


			vCurScale.z += fMouseY * fDT(0);
		}

		/* Rotate */
		else if (KEY(X, HOLD))
		{
			_float fMouseWheel = MOUSE_MOVE(MOUSEMOVE::MMS_WHEEL) * fDT(0) * 10.f;
			pCurObj->m_vAngles.x += fMouseWheel;

			_float fMouseX = MOUSE_MOVE(MOUSEMOVE::MMS_X);
			_float fMouseY = MOUSE_MOVE(MOUSEMOVE::MMS_Y) * -1.f;

			//look?? 0 0 1 ?? ?? x ???? ????????

			pCurObj->m_vAngles.z += fMouseY * fDT(0) * 10.f;

			if (pCurObj->m_vAngles.x >= 360.f)
				pCurObj->m_vAngles.x -= 360.f;
			if (pCurObj->m_vAngles.y >= 360.f)
				pCurObj->m_vAngles.y -= 360.f;
			if (pCurObj->m_vAngles.z >= 360.f)
				pCurObj->m_vAngles.z -= 360.f;


			_float4 vAngles = _float4(
				ToRadian(pCurObj->m_vAngles.x),
				ToRadian(pCurObj->m_vAngles.y),
				ToRadian(pCurObj->m_vAngles.z)
			);

			_float4 vQuat =
				XMQuaternionRotationRollPitchYawFromVector(vAngles.XMLoad());

			memcpy(&tTransform.q, &vQuat, sizeof(_float4));
		}

		else if (KEY(Z, HOLD))
		{
			_float fMouseWheel = MOUSE_MOVE(MOUSEMOVE::MMS_WHEEL) * fDT(0) * 10.f;
			pCurObj->m_vAngles.x += fMouseWheel;

			_float fMouseX = MOUSE_MOVE(MOUSEMOVE::MMS_X);
			_float fMouseY = MOUSE_MOVE(MOUSEMOVE::MMS_Y) * -1.f;

			//look?? 0 0 1 ?? ?? x ???? ????????

			pCurObj->m_vAngles.y += fMouseX * fDT(0) * 10.f;

			if (pCurObj->m_vAngles.x >= 360.f)
				pCurObj->m_vAngles.x -= 360.f;
			if (pCurObj->m_vAngles.y >= 360.f)
				pCurObj->m_vAngles.y -= 360.f;
			if (pCurObj->m_vAngles.z >= 360.f)
				pCurObj->m_vAngles.z -= 360.f;


			_float4 vAngles = _float4(
				ToRadian(pCurObj->m_vAngles.x),
				ToRadian(pCurObj->m_vAngles.y),
				ToRadian(pCurObj->m_vAngles.z)
			);

			_float4 vQuat =
				XMQuaternionRotationRollPitchYawFromVector(vAngles.XMLoad());

			memcpy(&tTransform.q, &vQuat, sizeof(_float4));
		}

		tTransform.p = CUtility_PhysX::To_PxVec3(PosValue);


		pCurRigidStatic->release();
		pCurRigidStatic = Create_StaticBox(tTransform, vCurScale);
		m_StaticBoxes[m_iCurSelectedIndex] = pCurRigidStatic;
		pCurObj->Synchronize_Box(tTransform);
		pCurObj->Get_Transform()->Set_Scale(vCurScale);
		pCurObj->Get_Transform()->Make_WorldMatrix();
	}

}

void CWindow_PhysX::Save_Data()
{
	string savePath;
	savePath = "../bin/Data/MapData/PhysXBoxData/";
	savePath += m_szName;
	savePath += ".bin";


	_uint iNumBoxes = m_StaticBoxes.size();

	ofstream	writeFile(savePath, ios::binary);


	if (!writeFile.is_open())
	{
		Call_MsgBox(L"SSave ???? ??!?!");
		return;
	}

	writeFile.write((char*)&iNumBoxes, sizeof(_uint));

	for (_uint i = 0; i < iNumBoxes; ++i)
	{
		PxTransform tTransform;
		tTransform = m_StaticBoxes[i]->getGlobalPose();

		_float4 vScale = m_pDebugBoxes[i]->Get_Transform()->Get_Scale();

		writeFile.write((char*)&tTransform, sizeof(PxTransform));
		writeFile.write((char*)&vScale, sizeof(_float4));

	}

	writeFile.close();

	Call_MsgBox(L"Save ????");

}

void CWindow_PhysX::Load_Data()
{
	string savePath;
	savePath = "../bin/Data/MapData/PhysXBoxData/";
	savePath += m_szName;
	savePath += ".bin";


	ifstream	readFile(savePath, ios::binary);


	if (!readFile.is_open())
	{
		Call_MsgBox(L"???? ???? ????");
		return;
	}
	Delete_All();
	_uint iNumBoxes = 0;

	readFile.read((char*)&iNumBoxes, sizeof(_uint));

	for (_uint i = 0; i < iNumBoxes; ++i)
	{
		PxTransform tTransform;
		_float4 vScale;

		readFile.read((char*)&tTransform, sizeof(PxTransform));
		readFile.read((char*)&vScale, sizeof(_float4));

		m_StaticBoxes.push_back(Create_StaticBox(tTransform, vScale));
		m_pDebugBoxes.push_back(
			CDebugObject::Create(tTransform, vScale)
		);

		CREATE_GAMEOBJECT(m_pDebugBoxes.back(), GROUP_PHYSX);
	}

	readFile.close();

}

PxRigidStatic* CWindow_PhysX::Create_StaticBox(PxTransform tTransform, _float4 vScale)
{
	PxRigidStatic* pRigidStatic = nullptr;

	pRigidStatic = GAMEINSTANCE->Create_StaticActor(
		tTransform,
		PxBoxGeometry(vScale.x * 0.5f, vScale.y * 0.5f, vScale.z * 0.5f),
		CPhysX_Manager::SCENE_CURRENT);

	return pRigidStatic;
}

void CWindow_PhysX::Delete_All()
{
	for (_uint i = 0; i < m_StaticBoxes.size(); ++i)
	{
		m_StaticBoxes[i]->release();
		DELETE_GAMEOBJECT(m_pDebugBoxes[i]);
	}
	m_StaticBoxes.clear();
	m_pDebugBoxes.clear();
	m_iCurSelectedIndex = 99999;
}

void CWindow_PhysX::Create_TestObject()
{
	CTestObject*	pTestObject = CTestObject::Create();
	pTestObject->Initialize();
	CREATE_GAMEOBJECT(pTestObject, GROUP_PROP);
}

void CWindow_PhysX::Create_DebugObject(_float4 vPos, _float4 vScale, _float4 vAngle)
{
	CDebugObject* pDebugObject = CDebugObject::Create(vPos, vScale, vAngle);
	pDebugObject->Initialize();
	CREATE_GAMEOBJECT(pDebugObject, GROUP_PROP);
}
