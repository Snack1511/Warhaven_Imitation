#include "stdafx.h"
#include "CWindow_PhysX.h"

#include "ImGui_Manager.h"

#include "GameInstance.h"
#include "PhysXCollider.h"

#include "CTestObject.h"

#include "CDebugObject.h"

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

	m_bEnable = true;
	SetUp_ImGuiDESC(typeid(CWindow_PhysX).name(), ImVec2(400.f, 600.f), window_flags);

	return S_OK;
}

void CWindow_PhysX::Tick()
{
	if (KEY(SPACE, TAP))
	{
		if (m_pCurGameObject)
		{
			if (m_pCurGameObject->Is_Disable())
				ENABLE_GAMEOBJECT(m_pCurGameObject);
			else
				DISABLE_GAMEOBJECT(m_pCurGameObject);
		}
	}
}

HRESULT CWindow_PhysX::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	//벽 설치하는 툴임


	if (ImGui::Button("Create TestObject"))
	{
		Create_TestObject();
	}

	if (ImGui::CollapsingHeader("STATIC CUBE"))
	{
		static _float4	vScale = _float4(1.f, 1.f, 1.f, 1.f);

		if (ImGui::Button("Create Ground"))
		{
			Create_DebugObject(ZERO_VECTOR, vScale, ZERO_VECTOR);
		}


		static _float vScaleInput[3] = {};


		if (ImGui::InputFloat3("vScale", vScaleInput))
		{
			vScale.x = vScaleInput[0];
			vScale.y = vScaleInput[1];
			vScale.z = vScaleInput[2];
		}



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

void CWindow_PhysX::Create_DebugObject(_float4 vPos, _float4 vScale, _float4 vAngle)
{
	CDebugObject* pDebugObject = CDebugObject::Create(vPos, vScale, vAngle);
	pDebugObject->Initialize();
	CREATE_GAMEOBJECT(pDebugObject, GROUP_PROP);
}
