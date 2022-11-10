#include "stdafx.h"
#include "CWindow_PhysX.h"

#include "ImGui_Manager.h"

#include "GameInstance.h"
#include "PhysXCollider.h"

#include "CTestObject.h"

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
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC(typeid(CWindow_PhysX).name(), ImVec2(400.f, 600.f), window_flags);

	return S_OK;
}

void CWindow_PhysX::Tick()
{

}

HRESULT CWindow_PhysX::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::Button("Create Scene"))
	{
		GAMEINSTANCE->Create_Scene(CPhysX_Manager::Scene::SCENE_CURRENT);
	}

	if (ImGui::Button("Create TestObject"))
	{
		Create_TestObject();
	}


	__super::End();

	return S_OK;
}

void CWindow_PhysX::Create_TestObject()
{
	CTestObject*	pTestObject = CTestObject::Create();
	pTestObject->Initialize();
	CREATE_GAMEOBJECT(pTestObject, GROUP_PROP);
	m_GameObjects.push_back(pTestObject);
	m_pCurGameObject = pTestObject;
}
