#include "stdafx.h"
#include "CPlayerInfo_Leader.h"

CPlayerInfo_Leader::CPlayerInfo_Leader()
{
}

CPlayerInfo_Leader::~CPlayerInfo_Leader()
{
}

CPlayerInfo_Leader* CPlayerInfo_Leader::Create()
{
	CPlayerInfo_Leader* pInstance = new CPlayerInfo_Leader;

	if (FAILED(pInstance->Initialize()))
		SAFE_DELETE(pInstance);

	return pInstance;
}

HRESULT CPlayerInfo_Leader::Initialize()
{
	for (_uint i = 0; i < CLASS_END; ++i)
	{
		Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eHEAD1);
		Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eBODY1);
		Set_CustomWeapon((CLASS_TYPE)i, eCUSTOM_WEAPON::eWEAPON1);
	}

	static _int g_LeaderCnt = 0;
	m_tPlayerInfo.wstrCamName = L"LeaderCam_";
	m_tPlayerInfo.wstrCamName += to_wstring(g_LeaderCnt);
	m_tPlayerInfo.wstrName = L"Leader_";
	m_tPlayerInfo.wstrName += to_wstring(g_LeaderCnt++);

	m_vecPrefClassType.push_back(WARRIOR);

	m_tPlayerInfo.iLevel = 20;


	return S_OK;
}
