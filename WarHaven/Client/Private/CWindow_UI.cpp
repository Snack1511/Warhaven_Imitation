#include "stdafx.h"
#include "CWindow_UI.h"

#include "ImGui_Manager.h"

CWindow_UI* CWindow_UI::Create()
{
	CWindow_UI* pInstance = new CWindow_UI;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_UI");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_UI::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(200.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_UI::Tick()
{
}

HRESULT CWindow_UI::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	// 위젯 확인용 데모창
	ImGui::ShowDemoWindow();

	ImGui::Text("= UI =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// UI 객체가 생성될 때마다 UI 자료 구조에 추가
	// 해당 자료구조에서 타입이 UI인 객체들을 가져와서 뿌린다.

	CGameObject* pObject;
	if (!pObject)
		return E_FAIL;

	// 트랜스폼 (위치, 크기, 회전??)

	// 텍스처 변경

	// 셰이더 변경


	ImGui::Spacing();

	__super::End();

	return S_OK;
}
