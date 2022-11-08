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

	// ���� Ȯ�ο� ����â
	ImGui::ShowDemoWindow();

	ImGui::Text("= UI =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// ������ ����Ʈ

	const char pUIName[MAX_PATH] = {};
	size_t iMaxIndex = m_vecUI.size();
	_uint iIndex = 0;

	static int iSelectUIIndex = 0;


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
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndListBox();
	}

	// �⺻ ������ ��ü ����
	if (ImGui::Button("Add New UI"))
		Add_UI();

	// ������ ��ü�� ������ �ν�����
	if (!m_vecUI.empty())
	{
		if (m_iSelectIndex < 9999)
		{
			CUI_Object* pUI = m_vecUI[m_iSelectIndex].pUI;

			// �̸� ���ϱ�
			static char strName[128] = "UI_Object";
			if (ImGui::InputText("UI Name", strName, IM_ARRAYSIZE(strName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				wstring wstr(strName, &strName[128]);
				pUI->Set_UIName(wstr);
			}

			// Ʈ������ (��ġ, ũ��, ȸ��??)
			Show_Transform(pUI);

			Show_Etc(pUI);

			// UI �̹��� â
			// ������ �̹��� Ȯ���ؼ� ������ �ϴܿ� ���̰� �ϱ�
			Show_Texture(m_iSelectIndex);

			if (ImGui::Button("Save"))
				Save_UI_Info(pUI);
		}		
	}

	// ���̴� ����

	// �ؽ�ó ����

	ImGui::Spacing();

	__super::End();

	return S_OK;
}

void CWindow_UI::Add_UI()
{
	CUI_Object* pUI = CUI_Object::Create();
	CREATE_GAMEOBJECT(pUI, GROUP_UI);

	UI_Object tItem;
	tItem.pUI = pUI;
	tItem.bSelected = false;

	m_vecUI.push_back(tItem);
}

void CWindow_UI::Show_Transform(CUI_Object* pUI)
{
	if (!pUI)
		return;

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
}

void CWindow_UI::Show_Etc(CUI_Object* pUI)
{
	if (!pUI)
		return;

	if (ImGui::CollapsingHeader("Etc"))
	{
		static bool bIsTarget = false;
		ImGui::Checkbox("Mouse Interact", &bIsTarget);
		pUI->Set_MouseTarget(bIsTarget);
	}
}

void CWindow_UI::Show_Texture(_uint iIndex)
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

void CWindow_UI::Save_UI_Info(CUI_Object* pUI)
{
	CUI_Object::UI_Desc tUI_Desc;
	ZeroMemory(&tUI_Desc, sizeof(CUI_Object::UI_Desc));

	// ������Ʈ�� Ʈ�������� 
	tUI_Desc.vPos = pUI->Get_Transform()->Get_World(WORLD_POS);

	_tchar szFileFullPath[MAX_PATH] = L"";
	const _tchar* szFilePath = TEXT("../Bin//Data/UIData/");

	wcscat_s(szFileFullPath, szFilePath);
	wcscat_s(szFileFullPath, pUI->Get_UIName().c_str());
	wcscat_s(szFileFullPath, L".dat");

	HANDLE	hFile = CreateFile
	(
		szFileFullPath,			// ������ ��ο� �̸��� ���
		GENERIC_WRITE,			// ���� ���� ��� (��� : WRITE, �Է� : READ)
		NULL,					// ���� ���, ������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����ϴ°��� ���� ����, NULL ���� �� ��������
		NULL,					// ���� �Ӽ� ���, NULL�� ��� �⺻�� ����
		CREATE_ALWAYS,			// ������ ������ ����, ������ ���� ����, OPEN_EXISTING - ������ �ִ� ��쿡�� �ε�
		FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ���� ����� ����), �ƹ��� �Ӽ��� ���� ���� ��� ����
		NULL					// ������ ������ �Ӽ��� ������ ���ø� ����, �츰 �� �����̱� ������ NULL
	);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		Call_MsgBox(TEXT("UI_Object Save failed"));
		return;
	}

	DWORD dwByte = 0;

	WriteFile(hFile, &tUI_Desc, sizeof(CUI_Object::UI_Desc), &dwByte, nullptr);

	CloseHandle(hFile);
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
