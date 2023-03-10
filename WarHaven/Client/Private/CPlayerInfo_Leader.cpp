#include "stdafx.h"
#include "CPlayerInfo_Leader.h"

#include "CPlayer.h"
#include "CUnit.h"

#include "CPersonality_Leader.h"
#include "CPersonality_Default.h"

#include "CGameSystem.h"

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


	m_tPlayerInfo.wstrName = L"Leader_Player";
	m_tPlayerInfo.wstrName += to_wstring(g_LeaderCnt++);

	if (g_LeaderCnt == 3)
	{
		m_tPlayerInfo.wstrName = L"버거형";
		m_bBurger = true;

#ifdef PALADIN_TH
		m_vecPrefClassType.push_back(PALADIN);
		Set_CustomBody(PALADIN, eCUSTOM_BODY::eBODY2);
		Set_CustomHead(PALADIN, eCUSTOM_HEAD::eRABBIT);

#else
		m_vecPrefClassType.push_back(WARRIOR);

#endif // WARRIOR_TH

	}
	else if (g_LeaderCnt == 2)
	{
		m_tPlayerInfo.wstrName = L"너네선비형";

#ifdef WARRIOR_TH
		m_vecPrefClassType.push_back(WARRIOR);
#endif // WARRIOR_TH

#ifdef ARCHER_TH
		m_vecPrefClassType.push_back(ARCHER);

#endif // WARRIOR_TH


		Set_CustomHead(WARRIOR, eCUSTOM_HEAD::eRABBIT);
		Set_CustomBody(WARRIOR, eCUSTOM_BODY::eBODY1);

		Set_CustomHead(ARCHER, eCUSTOM_HEAD::eHEAD1);
		Set_CustomBody(ARCHER, eCUSTOM_BODY::eBODY1);

	}
	else
	{
		m_tPlayerInfo.wstrName = L"128기전설송로민";

		for (_uint i = 0; i < CLASS_END; ++i)
		{
			Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eHEAD2);
			Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eBODY2);
		}


#ifdef WARRIOR_TH
		m_vecPrefClassType.push_back(WARRIOR);
#endif // WARRIOR_TH


#ifdef ENGINEER_TH
		m_vecPrefClassType.push_back(ENGINEER);
#endif // ENGINEER_TH

		

	}




	m_tPlayerInfo.iLevel = 20;

	if (FAILED(SetUp_AIPersonality()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerInfo_Leader::SetUp_AIPersonality()
{
	//CAIPersonality* pPersonality = CAIPersonality::Create(CGameSystem::Get_Instance()->Get_BXTable());
	//pPersonality->Load(L"Glide");
	CAIPersonality* pPersonality = CPersonality_Leader::Create(CGameSystem::Get_Instance()->Get_BXTable());
	m_pPersonality = pPersonality;

	m_iUnitType = ((_uint)CUnit::UNIT_TYPE::eAI_Default);

	if (!m_pPersonality)
		return E_FAIL;

	return S_OK;
}
