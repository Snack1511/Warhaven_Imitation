#include "stdafx.h"
#include "CPlayerInfo_Main.h"

CPlayerInfo_Main::CPlayerInfo_Main()
{
}

CPlayerInfo_Main::~CPlayerInfo_Main()
{
}

HRESULT CPlayerInfo_Main::Initialize()
{
	for (_uint i = 0; i < CLASS_END; ++i)
	{
		Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eHEAD1);
		Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eBODY1);
		Set_CustomWeapon((CLASS_TYPE)i, eCUSTOM_WEAPON::eDEFAULT);
	}

	m_bIsMainPlayer = true;

	m_tPlayerInfo.wstrCamName = L"PlayerCam";
	m_tPlayerInfo.wstrName = L"Jusin128";

	m_vecPrefClassType.push_back(WARRIOR);

	return S_OK;
}
