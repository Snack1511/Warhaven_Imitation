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

	// 유아이 리소스
	m_TextureRootNode.strFolderPath = "../bin/resources/textures";
	m_TextureRootNode.strFileName = "UI";
	m_TextureRootNode.strFullPath = "../bin/resources/textures/UI";
	Read_Folder("../bin/resources/textures/UI", m_TextureRootNode);

	// 유아이 객체 정보
	m_BinRootNode.strFolderPath = "../Bin/Data";
	m_BinRootNode.strFileName = "UIData";
	m_BinRootNode.strFullPath = "../Bin/Data/UIData";
	Read_Folder("../Bin/Data/UIData", m_BinRootNode);

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

	UI_IO();

	Create_UI();

	Show_UIList();

	if (m_iSelectIndex < 9999)
	{
		Show_Inspector();
	}

	Drag_Object();

	ImGui::Spacing();

	__super::End();

	return S_OK;
}

void CWindow_UI::Create_UI()
{
	if (ImGui::Button("Creat UI"))
		Add_UI();

	ImGui::SameLine();

	if (ImGui::Button("Clone UI"))
		Clone_UI();

	ImGui::NewLine();
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
			if (ImGui::Selectable(CFunctor::To_String(m_vecUI[i].pUI->Get_Name()).c_str(), m_vecUI[i].bSelected))
			{
				m_iSelectIndex = i;
				m_vecUI[i].bSelected = true;

				// 리스트에서 선택된 객체를 드래그할 유아이에 집어넣어
				// 리스트 선택이 아니라 좀 더 효율적으로
				if (m_vecUI[i].bSelected)
					m_pVecUI.push_back(m_vecUI[i].pUI);

				for (_uint j = 0; j < m_vecUI.size(); ++j)
				{
					if (j == i)
						continue;

					m_vecUI[j].bSelected = false;
					// 여기서 셀렉트가 아니면 뺴기
				}
			}

			if (m_vecUI[i].bSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void CWindow_UI::Show_Inspector()
{
	ImGui::Begin("Inspector");

	Set_Object_Info();

	// 선택한 이미지 확대해서 오른쪽 하단에 보이게 하기
	Show_Texture();

	ImGui::End();
}

void CWindow_UI::Drag_Object()
{
	POINT tMouse;
	GetCursorPos(&tMouse);
	ScreenToClient(g_hWnd, &tMouse);
	_float4 vMousePos = CFunctor::To_Window(_float4(tMouse.x, tMouse.y, 0.f));

	for (_uint i = 0; i < m_pVecUI.size(); ++i)
	{
		CUI_Object* pUI = m_pVecUI[i];
		_float4 vUIPos = pUI->Get_Transform()->Get_World(WORLD_POS);

		_bool bIsInMouse = pUI->Get_IsInMouse();
		if (bIsInMouse)
		{
			if (KEY(LBUTTON, TAP))
			{
				m_vDisPos[i].x = vUIPos.x - vMousePos.x;
				m_vDisPos[i].y = vUIPos.y + vMousePos.y;
			}

			if (KEY(LBUTTON, HOLD))
			{
				_float4 vResultPos;
				vResultPos.x = vMousePos.x + m_vDisPos[i].x;
				vResultPos.y = vMousePos.y - m_vDisPos[i].y;

				pUI->Set_Pos(vResultPos.x, -vResultPos.y);
			}
		}

		if (KEY(LBUTTON, AWAY))
			m_vDisPos[i] = 0;
	}
}

void CWindow_UI::Set_Object_Info()
{
	CUI_Object* pUI = m_vecUI[m_iSelectIndex].pUI;

	if (ImGui::CollapsingHeader("Name"))
	{
		static char strName[128] = "UI_Object";
		if (ImGui::InputText("UI_Object", strName, IM_ARRAYSIZE(strName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			wstring wstr(strName, &strName[128]);
			pUI->Set_Name(wstr);
		}
	}

	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::TreeNode("Position"))
		{
			_float4 fUI_Pos = pUI->Get_Transform()->Get_World(WORLD_POS);
			float fPos[2] = { fUI_Pos.x, fUI_Pos.y };
			ImGui::DragFloat2("Position", fPos, 1.f, -9999.f, 9999.f);
			pUI->Set_Pos(fPos[0], fPos[1]);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scale"))
		{
			_float4 fUI_Scale = pUI->Get_Transform()->Get_Scale();

			_float fScale[2] = { fUI_Scale.x, fUI_Scale.y };
			_float fOriginScale[2] = { fScale[0], fScale[1] };

			_float fRatio = fScale[1] / fScale[0];

			static _bool bScaleRatio = false;
			ImGui::Checkbox("Ratio", &bScaleRatio);

			if (ImGui::DragFloat2("Scale", fScale, 1.f, 1.f, 9999.f))
			{
				for (int i = 0; i < 2; ++i)
				{
					if (fScale[i] <= 1.f)
						fScale[i] = 1.f;
				}
			}

			if (bScaleRatio)
			{
				_float2 vDelta;
				vDelta.x = (fOriginScale[1] - fScale[1]) / fRatio;
				vDelta.y = (fOriginScale[0] - fScale[0]) * fRatio;

				_float2 vResultScale;
				vResultScale.x = fScale[0] - vDelta.x;
				vResultScale.y = fScale[1] - vDelta.y;

				pUI->Set_Scale(vResultScale.x, vResultScale.y);
			}
			else
			{
				pUI->Set_Scale(fScale[0], fScale[1]);
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Etc"))
	{
		if (ImGui::TreeNode("Mouse Interact"))
		{
			static bool bIsTarget = pUI->Get_MouseTarget();
			ImGui::Checkbox("True", &bIsTarget);
			pUI->Set_MouseTarget(bIsTarget);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Multi Texture"))
		{
			static bool bIsMulti = pUI->Get_MultiTexture();
			ImGui::Checkbox("True", &bIsMulti);
			pUI->Set_MultiTexture(bIsMulti);

			ImGui::TreePop();
		}
	}

	ImGui::NewLine();

	Set_Color();
}

void CWindow_UI::Set_Color()
{
	if (ImGui::CollapsingHeader("Color"))
	{
		CUI_Object* pUI = m_vecUI[m_iSelectIndex].pUI;

		static _float4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
		ImGui::ColorEdit4("Color", (_float*)&vColor);

		pUI->Set_Color(vColor);
	}
}

void CWindow_UI::Show_Texture()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::Begin("Image", 0, window_flags);

	Show_TreeTexture(m_TextureRootNode, m_iSelectIndex);

	ImGui::End();
}

void CWindow_UI::UI_IO()
{
	if (ImGui::CollapsingHeader("I/O"))
	{
		if (ImGui::Button("All Save"))
			Save_UI_List();

		ImGui::SameLine();

		if (ImGui::Button("Select Save"))
			Save_UI_Info(m_iSelectIndex);

		if (ImGui::Button("All Load"))
			Load_UI_List();

		ImGui::SameLine();

		//if (ImGui::Button("Select Load"))
			//Load_UI_Info();
	}

	ImGui::NewLine();
}

void CWindow_UI::Save_UI_Info(_uint iSelectIndex)
{
	CUI_Object* pUI = m_vecUI[iSelectIndex].pUI;

	string savePath = "../Bin/Data/UIData/";
	savePath += CFunctor::To_String(pUI->Get_Name()).c_str();
	savePath += ".bin";

	ofstream	writeFile(savePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"UI Save Failed");
		return;
	}

	string strName = CFunctor::To_String(pUI->Get_Name());
	CUtility_File::Write_Text(&writeFile, strName.c_str());

	_float4 vPos = pUI->Get_Transform()->Get_World(WORLD_POS);
	writeFile.write((char*)&vPos, sizeof(_float4));

	_float4 vScale = pUI->Get_Transform()->Get_Scale();
	writeFile.write((char*)&vScale, sizeof(_float4));

	_bool bTarget = pUI->Get_MouseTarget();
	writeFile.write((char*)&bTarget, sizeof(_bool));

	_bool bMulti = pUI->Get_MultiTexture();
	writeFile.write((char*)&bMulti, sizeof(_bool));

	_float4 vColor = pUI->Get_Color();
	writeFile.write((char*)&vColor, sizeof(_float4));

	CTexture* pTexture = GET_COMPONENT_FROM(pUI, CTexture);

	_uint iMaxSize = pTexture->Get_vecTexture().size();
	writeFile.write((char*)&iMaxSize, sizeof(_uint));

	for (size_t i = 0; i < iMaxSize; ++i)
	{
		string strFilePath = CFunctor::To_String(pTexture->Get_vecTexture()[i].strFilePath);
		CUtility_File::Write_Text(&writeFile, strFilePath.c_str());
	}

	writeFile.close();
}

void CWindow_UI::Save_UI_List()
{
	for (_uint i = 0; i < m_vecUI.size(); ++i)
		Save_UI_Info(i);

	Call_MsgBox(L"UI_List Save Succes");
}

void CWindow_UI::Load_UI_List()
{
	if (!m_BinRootNode.vecChildren.empty())
	{
		_uint iSize = m_BinRootNode.vecChildren.size();
		for (_uint i = 0; i < iSize; ++i)
		{
			ifstream readFile(m_BinRootNode.vecChildren[i].strFullPath, ios::binary);

			if (!readFile.is_open())
			{
				Call_MsgBox(L"UI Load Failed");
				return;
			}

			CUI_Object* pUI = Add_UI();

			string strName = CUtility_File::Read_Text(&readFile);
			pUI->Set_Name(CFunctor::To_Wstring(strName));

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

			_float4 vColor;
			readFile.read((char*)&vColor, sizeof(_float4));
			pUI->Set_Color(vColor);

			_uint iMaxSize = 0;
			readFile.read((char*)&iMaxSize, sizeof(_uint));

			for (_uint i = 0; i < iMaxSize; ++i)
			{
				string strPath = CUtility_File::Read_Text(&readFile);
				pUI->Set_Texture(CFunctor::To_Wstring(strPath).c_str());
			}

			readFile.close();
		}

		Call_MsgBox(L"UI_Object Load Succes");
	}
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

		ImGui::Text(CFunctor::Remove_String(tTree.strFileName).c_str());
	}
}
