#include "stdafx.h"
#include "CWindow_UI.h"

#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"

#include "ImGui_Manager.h"
#include "Functor.h"
#include "GameInstance.h"

CWindow_UI::CWindow_UI()
{
}

CWindow_UI::~CWindow_UI()
{
}

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

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(350.f, 350.f), window_flags);

	m_TextureRootNode.strFolderPath = "../bin/resources/textures";
	m_TextureRootNode.strFileName = "UI";
	m_TextureRootNode.strFullPath = "../bin/resources/textures/UI";
	Read_Folder("../bin/resources/textures/UI", m_TextureRootNode);

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
	// ImGui::ShowDemoWindow();

	ImGui::Text("= UI =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// 기본 유아이 객체 생성
	if (ImGui::Button("Add New UI"))
	{
		CUI_Object* pUI = CUI_Object::Create();
		CREATE_GAMEOBJECT(pUI, GROUP_UI);

		UI_Object tItem;
		tItem.pUI = pUI;
		tItem.bSelected = false;
		ZeroMemory(tItem.szBuf, sizeof(tItem.szBuf));

		m_vecUI.push_back(tItem);
	}

	if (!m_vecUI.empty())
	{
		CUI_Object* pUI = m_vecUI[0].pUI;

		// 트랜스폼 (위치, 크기, 회전??)
		Show_Transform(0);

		// UI 이미지 창
		// 선택한 이미지 확대해서 오른쪽 하단에 보이게 하기
		Show_TextureWindow(0);
	}

	// 셰이더 변경

	// 텍스처 변경

	ImGui::Spacing();

	__super::End();

	return S_OK;
}

void CWindow_UI::Show_Transform(_uint iIndex)
{
	CUI_Object* pUI = m_vecUI[iIndex].pUI;
	if (!pUI)
		return;

	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::TreeNode("Position"))
		{
			_float4 fUI_Pos = pUI->Get_Transform()->Get_World(WORLD_POS);
			float fPos[2] = { fUI_Pos.x, fUI_Pos.y};
			ImGui::DragFloat2("Position", fPos, 1.f, -999.f, 999.f);
			pUI->Set_Pos(fPos[0], fPos[1]);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scale"))
		{
			_float4 fUI_Scale = pUI->Get_Transform()->Get_Scale();
			float fScale[2] = { fUI_Scale.x, fUI_Scale.y };
			ImGui::DragFloat2("Scale", fScale, 1.f, -999.f, 999.f);

			for (int i = 0; i < 2; ++i)
			{
				if (fScale[i] <= 1.f)
					fScale[i] = 1.f;
			}

			pUI->Set_Scale(fScale[0], fScale[1]);

			ImGui::TreePop();
		}
	}
}

void CWindow_UI::Show_TextureWindow(_uint iIndex)
{
	ImGuiWindowFlags WindowFlags = 0;
	WindowFlags |= ImGuiWindowFlags_NoResize;
	WindowFlags |= ImGuiWindowFlags_NoTitleBar;
	WindowFlags |= ImGuiWindowFlags_NoMove;

	ImGui::Begin("Image", 0, WindowFlags);
	ImGui::SetWindowPos(ImVec2(0.f, 520.f));
	ImGui::SetWindowSize(ImVec2(1280.f, 200.f));

	Show_TreeTexture(m_TextureRootNode, iIndex);

	ImGui::End();
}

void CWindow_UI::Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree)
{
	for (filesystem::directory_iterator FileIter(pFolderPath);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		TREE_DATA	tTreeData;
		tTreeData.strFullPath = strFullPath;
		tTreeData.strFileName = strFileName;
		tTreeData.strFolderPath = pFolderPath;
		if (entry.is_directory())
		{
			Read_Folder(strFullPath.c_str(), tTreeData);
		}
		else
		{
			_int iFindExt = (int)strFileName.rfind(".") + 1;
			string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);

			if (strExtName == "dat")
				continue;
		}

		tRootTree.vecChildren.push_back(tTreeData);
	}
}

void CWindow_UI::Show_TreeTexture(TREE_DATA& tTree, _uint iIndex)
{
	if (!tTree.vecChildren.empty())
	{
		ImGui::NewLine();

		if (ImGui::TreeNode(tTree.strFileName.c_str()))
		{
			for (auto& tTreeData : tTree.vecChildren)
			{
				Show_TreeTexture(tTreeData, iIndex);
			}

			ImGui::TreePop();
		}
	}
	else
	{
		m_iSelectPath = CFunctor::To_Wstring(tTree.strFullPath);
		ID3D11ShaderResourceView* pSRV = GAMEINSTANCE->Get_Texture(m_iSelectPath).Get();
		if (ImGui::ImageButton(pSRV, ImVec2(50, 50)))
		{
			if (iIndex == 9999)
				return;

			m_vecUI[iIndex].pUI->Set_Texture(m_iSelectPath.c_str());
		}

		ImGui::SameLine();
	}
}
