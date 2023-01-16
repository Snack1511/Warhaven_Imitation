#include "stdafx.h"
#include "CPlayerInfo_Default.h"

#include "CPlayer.h"
#include "CUnit.h"

#include "UsefulHeaders.h"
#include "CPersonality_Default.h"

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
	m_tPlayerInfo.wstrCamName += to_wstring(g_iDefaultCnt++);

	//총 6마리

	//1. 시연회난동꾼
	if (g_iDefaultCnt == 1)
	{
		m_tPlayerInfo.wstrName = L"서른도넘은설운도";

#ifdef PALADIN_TH
		m_vecPrefClassType.push_back(PALADIN);
		Set_CustomHead(PALADIN, eCUSTOM_HEAD::eRABBIT);

#endif // WARRIOR_TH
	}
	else if (g_iDefaultCnt == 2)
	{
		m_tPlayerInfo.wstrName = L"점심장터";
	}
	else if (g_iDefaultCnt == 3)
	{
		m_tPlayerInfo.wstrName = L"NULL죽이겠다";
	}
	else if (g_iDefaultCnt == 4)
	{
		m_tPlayerInfo.wstrName = L"아기상어";
	}
	else if (g_iDefaultCnt == 5)
	{
		m_tPlayerInfo.wstrName = L"밤새면그만이야";
	}
	else if (g_iDefaultCnt == 6)
	{
		m_tPlayerInfo.wstrName = L"DefaultPlayer";
	}
#ifdef ENGINEER_TH
	m_vecPrefClassType.push_back(ENGINEER);
#endif // WARRIOR_TH

#ifdef WARRIOR_TH
	m_vecPrefClassType.push_back(WARRIOR);
#endif // WARRIOR_TH

	m_vecPrefClassType.push_back(PRIEST);

	if (FAILED(SetUp_AIPersonality()))
		return E_FAIL;
	return S_OK;
}

HRESULT CPlayerInfo_Default::SetUp_AIPersonality()
{
	//CAIPersonality* pPersonality = CAIPersonality::Create(CGameSystem::Get_Instance()->Get_BXTable());
	//pPersonality->Load(L"Glide");
	CAIPersonality* pPersonality = CPersonality_Default::Create(CGameSystem::Get_Instance()->Get_BXTable());
	m_pPersonality = pPersonality;

	m_iUnitType = ((_uint)CUnit::UNIT_TYPE::eAI_Default);

	if (!m_pPersonality)
		return E_FAIL;

	return S_OK;
}
