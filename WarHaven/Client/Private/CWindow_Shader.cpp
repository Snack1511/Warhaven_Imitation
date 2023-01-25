#include "stdafx.h"
#include "CWindow_Shader.h"

#include "ImGui_Manager.h"

#include "GameInstance.h"

CWindow_Shader::CWindow_Shader()
{
}

CWindow_Shader::~CWindow_Shader()
{
}

CWindow_Shader* CWindow_Shader::Create()
{
	CWindow_Shader* pInstance = new CWindow_Shader;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Shader");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Shader::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Shader).name(), ImVec2(200.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_Shader::Tick()
{
}

HRESULT CWindow_Shader::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	static _bool	bPBR = true;
	static _bool	bDOF = true;
	static _float fSSAO = 1.f;
	static _float fBrightness = 1.f;
	static _float fContrast = 1.f;
	static _float fSaturation = 1.f;
	static _float fShadowQuality = 1.f;
	static _float fDOFPower = 1.f;

	ImGui::Text("= SHADER OPTION =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::Button("DEFAULT"))
	{
		bPBR = true;
		fSSAO = 1.f;
		fBrightness = 1.f;
		fContrast = 1.f;
		fSaturation = 1.f;
		fShadowQuality = 1.f;
		fDOFPower = 1.f;

		GAMEINSTANCE->Set_PBR(true);
		GAMEINSTANCE->Set_SSAO(fSSAO);
		GAMEINSTANCE->Set_Brightness(fBrightness);
		GAMEINSTANCE->Set_Contrast(fBrightness);
		GAMEINSTANCE->Set_Saturation(fBrightness);
		GAMEINSTANCE->Set_ShadowQuality(fBrightness);
		GAMEINSTANCE->Set_DOFPower(fBrightness);
	}
	ImGui::Spacing();


	if (ImGui::RadioButton("PBR(BRDF specular)", bPBR))
	{
		bPBR = !bPBR;
		GAMEINSTANCE->Set_PBR(bPBR);
	}

	ImGui::Spacing();


	ImGui::Text("SSAO Power");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("ssao", &fSSAO, 0.01f, 10.f, " % .2f"))
	{
		GAMEINSTANCE->Set_SSAO(fSSAO);
	}


	ImGui::Text("Brightness");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("Brightness", &fBrightness, 0.01f, 2.f, " % .2f"))
	{
		GAMEINSTANCE->Set_Brightness(fBrightness);
	}

	ImGui::Text("Contrast");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("Contrast", &fContrast, 0.01f, 2.f, " % .2f"))
		GAMEINSTANCE->Set_Contrast(fContrast);

	ImGui::Text("Saturation");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("Saturation", &fSaturation, 0.01f, 2.f, " % .2f"))
	{
		GAMEINSTANCE->Set_Saturation(fSaturation);
	}

	ImGui::Text("ShadowQuality");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("ShadowQuality", &fShadowQuality, 0.01f, 3.f, " % .2f"))
	{
		GAMEINSTANCE->Set_ShadowQuality(fShadowQuality);
	}

	ImGui::Text("DOF Power");
	ImGui::SameLine();

	ImGui::PushItemWidth(100);
	if (ImGui::SliderFloat("DOF", &fDOFPower, 0.01f, 3.f, " % .2f"))
	{
		GAMEINSTANCE->Set_DOFPower(fDOFPower);
	}
	__super::End();

	return S_OK;
}
