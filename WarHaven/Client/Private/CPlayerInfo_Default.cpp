#include "stdafx.h"
#include "CPlayerInfo_Default.h"

#include "CPlayer.h"
#include "CUnit.h"

CPlayerInfo_Default::CPlayerInfo_Default()
{
}

CPlayerInfo_Default::~CPlayerInfo_Default()
{
}

CPlayerInfo_Default* CPlayerInfo_Default::Create()
{
	CPlayerInfo_Default* pInstance = new CPlayerInfo_Default;

	if (FAILED(pInstance->Initialize()))
		SAFE_DELETE(pInstance);

	return pInstance;
}

HRESULT CPlayerInfo_Default::Initialize()
{
	for (_uint i = 0; i < CLASS_END; ++i)
	{
		Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eDEFAULT);
		Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eDEFAULT);
		Set_CustomWeapon((CLASS_TYPE)i, eCUSTOM_WEAPON::eDEFAULT);
	}

	static _int g_iDefaultCnt = 0;
	m_tPlayerInfo.wstrCamName = L"DefaultCam_";
	m_tPlayerInfo.wstrCamName += to_wstring(g_iDefaultCnt);
	m_tPlayerInfo.wstrName = L"Default_";
	m_tPlayerInfo.wstrName += to_wstring(g_iDefaultCnt++);

	m_vecPrefClassType.push_back(WARRIOR);


	return S_OK;
}