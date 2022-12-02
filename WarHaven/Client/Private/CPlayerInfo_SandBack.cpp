#include "stdafx.h"
#include "CPlayerInfo_SandBack.h"

#include "CPlayer.h"
#include "CUnit.h"

CPlayerInfo_SandBack::CPlayerInfo_SandBack()
{
}

CPlayerInfo_SandBack::~CPlayerInfo_SandBack()
{
}

CPlayerInfo_SandBack* CPlayerInfo_SandBack::Create()
{
	CPlayerInfo_SandBack* pInstance = new CPlayerInfo_SandBack;

	if (FAILED(pInstance->Initialize()))
		SAFE_DELETE(pInstance);

	return pInstance;
}

HRESULT CPlayerInfo_SandBack::Initialize()
{
	for (_uint i = 0; i < CLASS_END; ++i)
	{
		Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eDEFAULT);
		Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eDEFAULT);
		Set_CustomWeapon((CLASS_TYPE)i, eCUSTOM_WEAPON::eDEFAULT);
	}

	static _int g_SandBackCnt = 0;
	m_tPlayerInfo.wstrCamName = L"SandBackCam_";
	m_tPlayerInfo.wstrCamName += to_wstring(g_SandBackCnt);
	m_tPlayerInfo.wstrName = L"SandBack_";
	m_tPlayerInfo.wstrName += to_wstring(g_SandBackCnt++);

	m_vecPrefClassType.push_back(WARRIOR);

	m_iUnitType = 1;

	return S_OK;
}
