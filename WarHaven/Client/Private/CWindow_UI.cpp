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

	// ���� Ȯ�ο� ����â
	ImGui::ShowDemoWindow();

	ImGui::Text("= UI =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// UI ��ü�� ������ ������ UI �ڷ� ������ �߰�
	// �ش� �ڷᱸ������ Ÿ���� UI�� ��ü���� �����ͼ� �Ѹ���.

	CGameObject* pObject;
	if (!pObject)
		return E_FAIL;

	// Ʈ������ (��ġ, ũ��, ȸ��??)

	// �ؽ�ó ����

	// ���̴� ����


	ImGui::Spacing();

	__super::End();

	return S_OK;
}
