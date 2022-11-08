#include "stdafx.h"
#include "CWindow_UI.h"

#include "CUI_Object.h"
#include "Transform.h"
#include "Texture.h"

#include "ImGui_Manager.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUtility_File.h"

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
	// window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(300.f, 250.f), window_flags);

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
	ImGui::ShowDemoWindow();

	ImGui::Text("= UI =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::Button("Load"))
		Load_UI_Info();

	if (ImGui::Button("Creat UI"))
		Add_UI();

	ImGui::SameLine();

	if (ImGui::Button("Clone UI"))
		Clone_UI();

	Show_UIList();

	if (m_iSelectIndex < 9999)
	{
		CUI_Object* pUI = m_vecUI[m_iSelectIndex].pUI;
		Show_Inspector(pUI);
	}

	ImGui::Spacing();

	__super::End();

	return S_OK;
}

CUI_Object* CWindow_UI::Add_UI()
{
	CUI_Object* pUI = CUI_Object::Create();
	CREATE_GAMEOBJECT(pUI, GROUP_UI);

	UI_Object tItem;
	tItem.pUI = pUI;
	tItem.bSelected = false;

	m_vecUI.push_back(tItem);

	return pUI;
}

CUI_Object* CWindow_UI::Clone_UI()
{
	CUI_Object* pUI_Clone = m_vecUI[m_iSelectIndex].pUI->Clone();

	CREATE_GAMEOBJECT(pUI_Clone, GROUP_UI);

	UI_Object tItem;
	tItem.pUI = pUI_Clone;
	tItem.bSelected = false;

	m_vecUI.push_back(tItem);

	return pUI_Clone;
}

void CWindow_UI::Show_UIList()
{
	if (ImGui::BeginListBox("UI_List"))
	{
		for (_uint i = 0; i < m_vecUI.size(); ++i)
		{
			if (ImGui::Selectable(CFunctor::To_String(m_vecUI[i].pUI->Get_UIName()).c_str(), m_vecUI[i].bSelected))
			{
				m_iSelectIndex = i;
				m_vecUI[i].bSelected = true;

				for (_uint j = 0; j < m_vecUI.size(); ++j)
				{
					if (j == i)
						continue;

					m_vecUI[j].bSelected = false;
				}
			}

			if (m_vecUI[i].bSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UI::Show_Inspector(CUI_Object* pUI)
{
	ImGui::Begin("Inspector");

	Set_Name(pUI);

	Show_Transform(pUI);

	Show_Etc(pUI);

	// 선택한 이미지 확대해서 오른쪽 하단에 보이게 하기
	Show_Texture();

	Show_File_IO();

	ImGui::End();
}

void CWindow_UI::Set_Name(CUI_Object* pUI)
{
	static char strName[128] = "UI_Object";
	if (ImGui::InputText("UI Name", strName, IM_ARRAYSIZE(strName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		wstring wstr(strName, &strName[128]);
		pUI->Set_UIName(wstr);
	}

	ImGui::NewLine();
}

void CWindow_UI::Show_Transform(CUI_Object* pUI)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		// Position
		_float4 fUI_Pos = pUI->Get_Transform()->Get_World(WORLD_POS);
		float fPos[2] = { fUI_Pos.x, fUI_Pos.y };
		ImGui::DragFloat2("Position", fPos, 1.f, -9999.f, 9999.f);
		pUI->Set_Pos(fPos[0], fPos[1]);

		// Scale
		_float4 fUI_Scale = pUI->Get_Transform()->Get_Scale();
		float fScale[2] = { fUI_Scale.x, fUI_Scale.y };
		ImGui::DragFloat2("Scale", fScale, 1.f, -9999.f, 9999.f);

		for (int i = 0; i < 2; ++i)
		{
			if (fScale[i] <= 1.f)
				fScale[i] = 1.f;
		}

		pUI->Set_Scale(fScale[0], fScale[1]);
	}

	ImGui::NewLine();
}

void CWindow_UI::Show_Etc(CUI_Object* pUI)
{
	if (ImGui::CollapsingHeader("Etc"))
	{
		static bool bIsTarget = pUI->Get_MouseTarget();
		ImGui::Checkbox("Mouse Interact", &bIsTarget);
		pUI->Set_MouseTarget(bIsTarget);

		static bool bIsMulti = pUI->Get_MultiTexture();
		ImGui::Checkbox("Multi Texture", &bIsMulti);
		pUI->Set_MultiTexture(bIsMulti);
	}

	ImGui::NewLine();
}

void CWindow_UI::Show_Texture()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::Begin("Image", 0, window_flags);

	Show_TreeTexture(m_TextureRootNode, m_iSelectIndex);

	ImGui::End();
}

void CWindow_UI::Show_File_IO()
{
	if (ImGui::Button("Save"))
		Save_UI_Info();
}

void CWindow_UI::Save_UI_Info()
{
	string savePath = "../Bin/Data/UIData/UIListInfo";
	savePath += ".bin";
	ofstream	writeFile(savePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"UI Save Failed");
		return;
	}

	_uint iSize = m_vecUI.size();
	writeFile.write((char*)&iSize, sizeof(_uint));

	for (_uint i = 0; i < m_vecUI.size(); ++i)
	{
		string strName = CFunctor::To_String(m_vecUI[i].pUI->Get_UIName());
		CUtility_File::Write_Text(&writeFile, strName.c_str());

		_float4 vPos = m_vecUI[i].pUI->Get_Transform()->Get_World(WORLD_POS);
		writeFile.write((char*)&vPos, sizeof(_float4));

		_float4 vScale = m_vecUI[i].pUI->Get_Transform()->Get_Scale();
		writeFile.write((char*)&vScale, sizeof(_float4));

		_bool bTarget = m_vecUI[i].pUI->Get_MouseTarget();
		writeFile.write((char*)&bTarget, sizeof(_bool));

		_bool bMulti = m_vecUI[i].pUI->Get_MultiTexture();
		writeFile.write((char*)&bMulti, sizeof(_bool));

		CTexture* pTexture = GET_COMPONENT_FROM(m_vecUI[i].pUI, CTexture);

		_uint iMaxSize = pTexture->Get_vecTexture().size();
		writeFile.write((char*)&iMaxSize, sizeof(_uint));

		for (size_t i = 0; i < iMaxSize; ++i)
		{
			string strFilePath = CFunctor::To_String(pTexture->Get_vecTexture()[i].strFilePath);
			CUtility_File::Write_Text(&writeFile, strFilePath.c_str());
		}
	}

	writeFile.close();

	Call_MsgBox(L"UI_Object Save Succes");
}

void CWindow_UI::Load_UI_Info()
{
	string strLoadPath = "../Bin/Data/UIData/UIListInfo.bin";
	ifstream readFile(strLoadPath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"UI Load Failed");
		return;
	}

	_uint iSize = 0;
	readFile.read((char*)&iSize, sizeof(_uint));

	for (_uint i = 0; i < iSize; ++i)
	{
		CUI_Object* pUI = Add_UI();

		string strName = CUtility_File::Read_Text(&readFile);
		pUI->Set_UIName(CFunctor::To_Wstring(strName));

		_float4 vPos;
		readFile.read((char*)&vPos, sizeof(_float4));
		pUI->Set_Pos(vPos.x, vPos.y);

		_float4 vScale;
		readFile.read((char*)&vScale, sizeof(_float4));
		pUI->Set_Scale(vScale.x, vScale.y);

		_bool bTarget = false;
		readFile.read((char*)&bTarget, sizeof(_bool));
		pUI->Set_MouseTarget(bTarget);

		_bool bMulti = false;
		readFile.read((char*)&bMulti, sizeof(_bool));
		pUI->Set_MultiTexture(bMulti);

		_uint iMaxSize = 0;
		readFile.read((char*)&iMaxSize, sizeof(_uint));

		for (_uint i = 0; i < iMaxSize; ++i)
		{
			string strPath = CUtility_File::Read_Text(&readFile);
			pUI->Set_Texture(CFunctor::To_Wstring(strPath).c_str());
		}
	}

	readFile.close();

	Call_MsgBox(L"UI_Object Save Succes");
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

		// CFunctor에 함수로 빼기
		_int iFind = (_int)tTree.strFullPath.rfind("\\") + 1;
		string strFileName = tTree.strFullPath.substr(iFind, tTree.strFullPath.length() - iFind);
		ImGui::Text(tTree.strFileName.c_str());
	}
}
