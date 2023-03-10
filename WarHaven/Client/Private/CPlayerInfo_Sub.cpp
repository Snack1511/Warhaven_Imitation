#include "stdafx.h"
#include "CPlayerInfo_Sub.h"

#include "CPlayer.h"
#include "CUnit.h"

#include "UsefulHeaders.h"
#include "CPersonality_Sub.h"
#include "CPersonality_Default.h"

CPlayerInfo_Sub::CPlayerInfo_Sub()
{
}

CPlayerInfo_Sub::~CPlayerInfo_Sub()
{
}

CPlayerInfo_Sub* CPlayerInfo_Sub::Create()
{
	CPlayerInfo_Sub* pInstance = new CPlayerInfo_Sub;

	if (FAILED(pInstance->Initialize()))
		SAFE_DELETE(pInstance);

	return pInstance;
}

HRESULT CPlayerInfo_Sub::Initialize()
{
	for (_uint i = 0; i < CLASS_END; ++i)
	{
		Set_CustomHead((CLASS_TYPE)i, eCUSTOM_HEAD::eDEFAULT);
		Set_CustomBody((CLASS_TYPE)i, eCUSTOM_BODY::eDEFAULT);
		Set_CustomWeapon((CLASS_TYPE)i, eCUSTOM_WEAPON::eDEFAULT);
	}



	static _int g_iSubCnt = 0;
	m_tPlayerInfo.wstrCamName = L"SubCam_";
	m_tPlayerInfo.wstrCamName += to_wstring(g_iSubCnt++);

	if (g_iSubCnt == 1)
	{
		m_tPlayerInfo.wstrName = L"??????????";
#ifdef ARCHER_TH
		m_vecPrefClassType.push_back(ARCHER);
#else
		m_vecPrefClassType.push_back(WARRIOR);

#endif // ARCHER_TH

	}
	else if (g_iSubCnt == 2)
	{
		m_tPlayerInfo.wstrName = L"???ž?ī???̰???";
	}
	else if (g_iSubCnt == 3)
	{
		m_tPlayerInfo.wstrName = L"??ǰ?????????ġ";
#ifdef ENGINEER_TH
		m_vecPrefClassType.push_back(ENGINEER);
#else
		m_vecPrefClassType.push_back(WARRIOR);
#endif
	}
	else if (g_iSubCnt == 4)
	{
		m_tPlayerInfo.wstrName = L"???⼺?ǵ巡?ﺼ";
	}
	else if (g_iSubCnt == 5)
	{
		m_tPlayerInfo.wstrName = L"??â????Ÿ??";
	}
	else
	{
		m_tPlayerInfo.wstrName = L"SubPlayer_";
		m_tPlayerInfo.wstrName += to_wstring(g_iSubCnt);

	}


	if (g_iSubCnt != 1)
	{
#ifdef WARRIOR_TH
		m_vecPrefClassType.push_back(WARRIOR);
#endif // WARRIOR_TH

#ifdef ARCHER_TH
		m_vecPrefClassType.push_back(ARCHER);
#endif // ARCHER_TH

#ifdef PRIEST_TH
		m_vecPrefClassType.push_back(PRIEST);
#endif // PRIEST_TH
	}


	if (FAILED(SetUp_AIPersonality()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayerInfo_Sub::SetUp_AIPersonality()
{
	//CAIPersonality* pPersonality = CAIPersonality::Create(CGameSystem::Get_Instance()->Get_BXTable());
	//pPersonality->Load(L"Glide");
	CAIPersonality* pPersonality = CPersonality_Sub::Create(CGameSystem::Get_Instance()->Get_BXTable());
	m_pPersonality = pPersonality;

	m_iUnitType = ((_uint)CUnit::UNIT_TYPE::eAI_Default);

	if (!m_pPersonality)
		return E_FAIL;

	return S_OK;
}
