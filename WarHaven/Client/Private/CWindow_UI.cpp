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
	// window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(300.f, 350.f), window_flags);

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
	Drag_Object();
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

	Show_Inspector();

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
	{
		string key = CFunctor::To_String(m_pSelectUI->Get_Name());
		Load_UI_Info(key);
	}
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

void CWindow_UI::Show_UIList()
{
	if (ImGui::BeginListBox("UI_List", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (_uint i = 0; i < m_vecUI.size(); ++i)
		{
			if (ImGui::Selectable(CFunctor::To_String(m_vecUI[i].pUI->Get_Name()).c_str(), m_vecUI[i].bSelected))
			{
				m_iSelectIndex = i;
				m_vecUI[i].bSelected = true;

				if (m_vecUI[i].bSelected)
					m_pSelectUI = m_vecUI[i].pUI;

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

	CUI_Object* pUI = m_pSelectUI;
	if (!pUI)
		return;

	_float4 vUIPos = pUI->Get_Transform()->Get_World(WORLD_POS);

	_bool bIsInMouse = pUI->Get_IsInMouse();
	if (bIsInMouse)
	{
		if (KEY(LBUTTON, TAP))
		{
			m_vDisPos.x = vUIPos.x - vMousePos.x;
			m_vDisPos.y = vUIPos.y + vMousePos.y;
		}

		if (KEY(LBUTTON, HOLD))
		{
			_float4 vResultPos;
			vResultPos.x = vMousePos.x + m_vDisPos.x;
			vResultPos.y = vMousePos.y - m_vDisPos.y;

			pUI->Set_Pos(vResultPos.x, -vResultPos.y);
		}
	}

	if (KEY(LBUTTON, AWAY))
		m_vDisPos = 0;
}

void CWindow_UI::Set_Object_Info()
{
	CUI_Object* pUI = m_pSelectUI;
	if (!pUI)
		return;

	static char szBuf[MIN_STR] = "Name";
	string strName = CFunctor::To_String(pUI->Get_Name());

	strcpy_s(szBuf, strName.c_str());

	if (ImGui::InputText("UI_Object", szBuf, IM_ARRAYSIZE(szBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		pUI->Set_Name(CFunctor::To_Wstring(szBuf));
	}

	if (ImGui::CollapsingHeader("Transform"))
	{
		if (ImGui::TreeNode("Position"))
		{
			ImGui::PushItemWidth(150);

			_float4 fUI_Pos = pUI->Get_Transform()->Get_World(WORLD_POS);
			float fPos[2] = { fUI_Pos.x, fUI_Pos.y };
			ImGui::DragFloat2("Position", fPos, 1.f, -9999.f, 9999.f, "%.f");
			pUI->Set_Pos(fPos[0], fPos[1]);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scale"))
		{
			static _bool bScaleRatio = false;
			ImGui::Checkbox("Ratio", &bScaleRatio);

			ImGui::SameLine();

			_float4 fUI_Scale = pUI->Get_Transform()->Get_Scale();
			_float fScale[2] = { fUI_Scale.x, fUI_Scale.y };
			_float fOriginScale[2] = { fScale[0], fScale[1] };
			_float fRatio = fScale[1] / fScale[0];

			ImGui::PushItemWidth(150);
			ImGui::DragFloat2("Scale", fScale, 1.f, 1.f, 1280.f, "%.f");

			for (int i = 0; i < 2; ++i)
			{
				if (fScale[i] <= 1.f)
					fScale[i] = 1.f;
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

		if (ImGui::TreeNode("Rotation"))
		{
			_float fRotZ = pUI->Get_RotationValue();
			ImGui::DragFloat("Rotation", &fRotZ, 1.f, -360.f, 360.f);
			pUI->Set_RotationZ(fRotZ);

			ImGui::TreePop();
		}
	}

	Show_Font();

	if (ImGui::CollapsingHeader("Etc"))
	{
		_float fSort = pUI->Get_Sort();
		ImGui::DragFloat("Sort", &fSort, 0.001f, 0.f, 1.f, "%.3f");
		pUI->Set_Sort(fSort);

		bool bIsTarget = pUI->Get_MouseTarget();
		ImGui::Checkbox("Mouse Target", &bIsTarget);
		pUI->Set_MouseTarget(bIsTarget);

		bool bIsMulti = pUI->Get_MultiTexture();
		ImGui::Checkbox("Multi Texture", &bIsMulti);
		pUI->Set_MultiTexture(bIsMulti);

		_float4 vColor = pUI->Get_Color();
		ImGui::ColorEdit4("Color", (_float*)&vColor);
		pUI->Set_Color(vColor);

		bool bIsSlice = pUI->Get_IsSlice();
		ImGui::Checkbox("Nine-Slice", &bIsSlice);
		pUI->Set_IsSlice(bIsSlice);

		_float4 vRatio = pUI->Get_SliceRatio();
		float fRatio[4] = { vRatio.x, vRatio.y, vRatio.z, vRatio.w };
		ImGui::DragFloat4("Ratio", fRatio, 0.1f, -9999.f, 9999.f, "%.1f");
		pUI->Set_SliceRatio(_float4(fRatio[0], fRatio[1], fRatio[2], fRatio[3]));

		_float2 vSize = pUI->Get_TextureSize();
		float fSize[2] = { vSize.x, vSize.y };
		ImGui::DragFloat2("Size", fSize, 0.1f, -9999.f, 9999.f, "%.1f");
		pUI->Set_TextureSzie(_float2(fSize[0], fSize[1]));
	}
}

void CWindow_UI::Show_Font()
{
	CUI_Object* pUI = m_pSelectUI;
	if (!pUI)
		return;

	if (ImGui::CollapsingHeader("Text"))
	{
		bool bIsRenderText = pUI->Get_FontRender();
		ImGui::Checkbox("Text Active", &bIsRenderText);
		pUI->Set_FontRender(bIsRenderText);

		if (bIsRenderText)
		{
			bool bIsBold = pUI->Get_FontStyle();
			ImGui::Checkbox("Bold", &bIsBold);
			pUI->Set_FontStyle(bIsBold);

			bool bIsCenter = pUI->Get_FontCenter();
			ImGui::Checkbox("Center", &bIsCenter);
			pUI->Set_FontCenter(bIsCenter);

			if (ImGui::TreeNode("Text"))
			{
				static char szBuf[MIN_STR] = "Input Text";
				string strText = CFunctor::To_String(pUI->Get_FontText());

				strcpy_s(szBuf, strText.c_str());

				if (ImGui::InputText("Input Text", szBuf, IM_ARRAYSIZE(szBuf), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					pUI->Set_FontText(CFunctor::To_Wstring(szBuf));
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Offset"))
			{
				ImGui::PushItemWidth(150);

				_float4 vOffset = pUI->Get_FontOffset();
				float fOffset[2] = { vOffset.x, vOffset.y };
				ImGui::DragFloat2("Offset", fOffset, 1.f, -9999.f, 9999.f, "%.f");
				pUI->Set_FontOffset(fOffset[0], fOffset[1]);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Scale"))
			{
				_float fScale = pUI->Get_FontScale();
				ImGui::DragFloat("Scale", &fScale, 0.1f, 0.1f, 9999.f, "%.1f");
				pUI->Set_FontScale(fScale);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Color"))
			{
				_float4 vColor = pUI->Get_FontColor();
				ImGui::ColorEdit4("Color", (_float*)&vColor);
				pUI->Set_FontColor(vColor);

				ImGui::TreePop();
			}
		}
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
	ImGui::Text("= I/O =");

	static char szKey[MIN_STR] = "Bin";
	ImGui::InputText("Input Bin File", szKey, IM_ARRAYSIZE(szKey));
	string strName = szKey;

	if (ImGui::Button("UI Load"))
		Load_UI_Info(strName);

	if (ImGui::Button("Select Save"))
		Save_UI_Info(m_iSelectIndex);

	if (ImGui::Button("All Save"))
		Save_UI_List();

	ImGui::SameLine();
	if (ImGui::Button("All Load"))
		Load_UI_List();

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

	_float fSort = pUI->Get_Sort();
	writeFile.write((char*)&fSort, sizeof(_float));

	/*_bool bRenderFont = pUI->Get_FontRender();
	writeFile.write((char*)&bRenderFont, sizeof(_bool));

	_bool bBold = pUI->Get_FontStyle();
	writeFile.write((char*)&bBold, sizeof(_bool));

	string strText = CFunctor::To_String(pUI->Get_FontText());
	CUtility_File::Write_Text(&writeFile, strText.c_str());

	_float4 vOffset = pUI->Get_FontOffset();
	writeFile.write((char*)&vOffset, sizeof(_float4));

	_float4 vFontColor = pUI->Get_FontColor();
	writeFile.write((char*)&vFontColor, sizeof(_float4));

	_float fFontScale = pUI->Get_FontScale();
	writeFile.write((char*)&fFontScale, sizeof(_float));*/

	writeFile.close();
}

void CWindow_UI::Save_UI_List()
{
	for (_uint i = 0; i < m_vecUI.size(); ++i)
		Save_UI_Info(i);

	Call_MsgBox(L"UI_List Save Succes");
}

void CWindow_UI::Load_UI_Info(string key)
{
	string readPath = "../Bin/Data/UIData/";
	readPath += key;
	readPath += ".bin";

	ifstream readFile(readPath, ios::binary);

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

	_float fSort = pUI->Get_Sort();
	readFile.read((char*)&fSort, sizeof(_float));
	pUI->Set_Sort(fSort);

	/*_bool bRenderFont = pUI->Get_FontRender();
	readFile.read((char*)&bRenderFont, sizeof(_bool));
	pUI->Set_FontRender(bRenderFont);

	_bool bBold = pUI->Get_FontStyle();
	readFile.read((char*)&bBold, sizeof(_bool));
	pUI->Set_FontStyle(bBold);

	string strText = CUtility_File::Read_Text(&readFile);
	pUI->Set_FontText(CFunctor::To_Wstring(strText));

	_float4 vOffset = pUI->Get_FontOffset();
	readFile.read((char*)&vOffset, sizeof(_float4));
	pUI->Set_FontOffset(vOffset.x, vOffset.y);

	_float4 vFontColor = pUI->Get_FontColor();
	readFile.read((char*)&vFontColor, sizeof(_float4));
	pUI->Set_FontColor(vFontColor);

	_float fFontScale = pUI->Get_FontScale();
	readFile.read((char*)&fFontScale, sizeof(_float));
	pUI->Set_FontScale(fFontScale);*/

	readFile.close();
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

			_float fSort = pUI->Get_Sort();
			readFile.read((char*)&fSort, sizeof(_float));
			pUI->Set_Sort(fSort);

			/*_bool bRenderFont = pUI->Get_FontRender();
			readFile.read((char*)&bRenderFont, sizeof(_bool));
			pUI->Set_FontRender(bRenderFont);

			_bool bBold = pUI->Get_FontStyle();
			readFile.read((char*)&bBold, sizeof(_bool));
			pUI->Set_FontStyle(bBold);

			string strText = CUtility_File::Read_Text(&readFile);
			pUI->Set_FontText(CFunctor::To_Wstring(strText));

			_float4 vOffset = pUI->Get_FontOffset();
			readFile.read((char*)&vOffset, sizeof(_float4));
			pUI->Set_FontOffset(vOffset.x, vOffset.y);

			_float4 vFontColor = pUI->Get_FontColor();
			readFile.read((char*)&vFontColor, sizeof(_float4));
			pUI->Set_FontColor(vFontColor);

			_float fFontScale = pUI->Get_FontScale();
			readFile.read((char*)&fFontScale, sizeof(_float));
			pUI->Set_FontStyle(fFontScale);*/

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
