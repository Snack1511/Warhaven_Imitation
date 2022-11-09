#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"

CUI_Crosshair::CUI_Crosshair()
{
}

CUI_Crosshair::CUI_Crosshair(const CUI_Crosshair& Prototype)
{
}

CUI_Crosshair::~CUI_Crosshair()
{
}

HRESULT CUI_Crosshair::Initialize_Prototype()
{
	Read_File();

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	for (auto& pair : m_pUIMap)
	{
		CUI_Object* pUI = pair.second;
		CREATE_GAMEOBJECT(pUI, GROUP_UI);
	}

	__super::Start();

	return S_OK;
}

void CUI_Crosshair::Read_File()
{
	const char* pFilePath = "../Bin/Data/UIData";

	for (filesystem::directory_iterator FileIter(pFilePath); FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		// 바이너리 파일 이름 찾기
		_int iFind0 = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind0);

		// 저장된 객체 이름 
		_int iFind2 = (_int)strFileName.rfind(".");
		string strKey = strFileName.substr(0, iFind2);

		string strPathTemp = CFunctor::To_String(wstrPath);

		if (strPathTemp.find("Crosshair") != string::npos)
		{
			CUI_Object* pUI = Load_UI(strFullPath);;
			m_pUIMap.emplace(CFunctor::To_Wstring(strKey), pUI);
		}
	}
}
