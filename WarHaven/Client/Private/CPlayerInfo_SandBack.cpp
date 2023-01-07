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


#define ON_WARRIOR
//#define ON_PALADIN
//#define ON_ARCHER
//#define ON_PRIEST


	if(g_SandBackCnt < 3 && g_SandBackCnt % 2 == 0)
		m_vecPrefClassType.push_back(ENGINEER);
	else
	{

#ifdef ON_PALADIN
		m_vecPrefClassType.push_back(PALADIN);
#endif // ON_PALADIN

#ifdef ON_ARCHER
		m_vecPrefClassType.push_back(ARCHER);
#endif // ON_ARCHER

#ifdef ON_PRIEST
		m_vecPrefClassType.push_back(PRIEST);
#endif // ON_PRIEST

#ifdef ON_WARRIOR
		m_vecPrefClassType.push_back(WARRIOR);
#endif // ON_WARRIOR

	}

	//m_iUnitType = ((_uint)CUnit::UNIT_TYPE::eSandbag);


	return S_OK;
}
