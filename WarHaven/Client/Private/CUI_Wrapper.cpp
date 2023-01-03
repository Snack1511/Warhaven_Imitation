#include "stdafx.h"
#include "CUI_Wrapper.h"

#include "GameInstance.h"
#include "Transform.h"
#include "CUI_Object.h"

#include "Texture.h"

#include "Functor.h"
#include "CUtility_File.h"

#include "CButton.h"
#include "CFader.h"

CUI_Wrapper::CUI_Wrapper()
{
}

CUI_Wrapper::~CUI_Wrapper()
{
}

HRESULT CUI_Wrapper::Initialize_Prototype()
{
	Load_UI(m_wstrName);

	return S_OK;
}

HRESULT CUI_Wrapper::Initialize()
{
	if (!m_pUI)
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Wrapper::Start()
{
	/*if (!m_pUI)
		return E_FAIL;*/

	if (m_pUI)
		CREATE_GAMEOBJECT(m_pUI, GROUP_UI);

	__super::Start();

	return S_OK;
}

void CUI_Wrapper::On_PointEnterEvent(const _uint& iEventNum)
{
	//위에서 넣은 객체한테 발생하는 상호작용이
	//이 함수로 들어옴
	switch (iEventNum)
	{
	case 0:

		break;

	case 1:

	default:
		break;
	}

}

void CUI_Wrapper::On_PointExitEvent(const _uint& iEventNum)
{
}

void CUI_Wrapper::On_ButtonClickEvent(const _uint& iEventNum)
{
}

void CUI_Wrapper::SetActive(_bool value)
{
	if (value == true)
		ENABLE_GAMEOBJECT(this);
	else
		DISABLE_GAMEOBJECT(this);
}

void CUI_Wrapper::My_Tick()
{
	__super::My_Tick();

	//즐겨
	//GET_COMPONENT_FROM(m_m_pUI, CTexture);
}

void CUI_Wrapper::My_LateTick()
{
	__super::My_LateTick();
	//놀아
}

void CUI_Wrapper::OnEnable()
{
	__super::OnEnable();

	if (m_pUI)
		ENABLE_GAMEOBJECT(m_pUI);
}

void CUI_Wrapper::OnDisable()
{
	__super::OnDisable();

	if (m_pUI)
		DISABLE_GAMEOBJECT(m_pUI);
}

void CUI_Wrapper::Fade_In(CUI_Object* m_pUI)
{
	GET_COMPONENT_FROM(m_pUI, CFader)->Re_FadeIn();
}

void CUI_Wrapper::Fade_Out(CUI_Object* m_pUI)
{
	GET_COMPONENT_FROM(m_pUI, CFader)->Re_FadeOut();
}

void CUI_Wrapper::Enable_Fade(CUI_Object* m_pUI, _float fDuration)
{
	ENABLE_GAMEOBJECT(m_pUI);
	GET_COMPONENT_FROM(m_pUI, CFader)->Re_FadeIn();

	if (m_pUI->Get_FontRender())
	{
		m_pUI->Fade_Font(true, fDuration);
	}
}

void CUI_Wrapper::Disable_Fade(CUI_Object* m_pUI, _float fDuration)
{
	GET_COMPONENT_FROM(m_pUI, CFader)->Re_FadeOut();

	if (m_pUI->Get_FontRender())
	{
		m_pUI->Fade_Font(false, fDuration);
	}
}

void CUI_Wrapper::Read_UI(string key)
{
	const char* pFilePath = "../Bin/Data/UIData";

	for (filesystem::directory_iterator FileIter(pFilePath); FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind0 = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind0);

		_int iFind2 = (_int)strFileName.rfind(".");
		string strKey = strFileName.substr(0, iFind2);

		string strPathTemp = CFunctor::To_String(wstrPath);

		if (strPathTemp.find(key) != string::npos)
		{
			CUI_Object* pUI = Load_UI(strFullPath);;
			m_pUIMap.emplace(CFunctor::To_Wstring(strKey), pUI);
		}
	}
}

void CUI_Wrapper::Read_Texture(string strFolderPath, string key)
{
	string strBasePath = "../Bin/Resources/Textures/UI";;
	strBasePath += strFolderPath;

	const char* pFilePath = strBasePath.c_str();

	for (filesystem::directory_iterator FileIter(pFilePath); FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		string strPathTemp = CFunctor::To_String(wstrPath);

		if (strPathTemp.find(key) != string::npos)
		{
			const _tchar* pFullFilePath = wstrPath.c_str();
			GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(pFullFilePath);
		}
	}
}

void CUI_Wrapper::Read_Texture(CUI_Object* pUI, string strFolderPath, string key)
{
	string strBasePath = "../Bin/Resources/Textures/UI";
	strBasePath += strFolderPath;

	const char* pFilePath = strBasePath.c_str();

	for (filesystem::directory_iterator FileIter(pFilePath); FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		string strPathTemp = CFunctor::To_String(wstrPath);

		if (strPathTemp.find(key) != string::npos)
		{
			const _tchar* pFullFilePath = wstrPath.c_str();
			GET_COMPONENT_FROM(pUI, CTexture)->Add_Texture(pFullFilePath);
		}
	}
}

CUI_Object* CUI_Wrapper::Load_UI(string m_wstrName)
{
	ifstream	readFile(m_wstrName, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"UI Load Failed");
		return nullptr;
	}

	CUI_Object* pUI = CUI_Object::Create();

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

	return pUI;
}

void CUI_Wrapper::Load_UI(wstring m_wstrName)
{
	string savePath = "../Bin/Data/UIData/";
	savePath += CFunctor::To_String(m_wstrName);
	savePath += ".bin";

	ifstream	readFile(savePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"UI Save Failed");
		return;
	}

	m_pUI = CUI_Object::Create();

	string strName = CUtility_File::Read_Text(&readFile);
	m_pUI->Set_Name(CFunctor::To_Wstring(strName));

	_float4 vPos;
	readFile.read((char*)&vPos, sizeof(_float4));
	m_pUI->Set_Pos(vPos.x, vPos.y);

	_float4 vScale;
	readFile.read((char*)&vScale, sizeof(_float4));
	m_pUI->Set_Scale(vScale.x, vScale.y);

	_bool bTarget = false;
	readFile.read((char*)&bTarget, sizeof(_bool));
	m_pUI->Set_MouseTarget(bTarget);

	_bool bMulti = false;
	readFile.read((char*)&bMulti, sizeof(_bool));
	m_pUI->Set_MultiTexture(bMulti);

	_float4 vColor;
	readFile.read((char*)&vColor, sizeof(_float4));
	m_pUI->Set_Color(vColor);

	_uint iMaxSize = 0;
	readFile.read((char*)&iMaxSize, sizeof(_uint));

	for (_uint i = 0; i < iMaxSize; ++i)
	{
		string strPath = CUtility_File::Read_Text(&readFile);
		m_pUI->Set_Texture(CFunctor::To_Wstring(strPath).c_str());
	}

	_float fSort = m_pUI->Get_Sort();
	readFile.read((char*)&fSort, sizeof(_float));
	m_pUI->Set_Sort(fSort);

	readFile.close();
}

