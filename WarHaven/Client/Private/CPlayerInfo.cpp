#include "stdafx.h"
#include "CPlayerInfo.h"
#include "CAIPersonality.h"

#include "UsefulHeaders.h"

//#include "CPersonality_Default.h"

CPlayerInfo::CPlayerInfo()
{
}

CPlayerInfo::~CPlayerInfo()
{
	Release();
}

CPlayer* CPlayerInfo::Make_Player()
{
	if (!Can_Make_Player(m_tPlayerInfo))
	{
		Call_MsgBox(L"Not Valid PlayerInfo :: CPlayerInfo");
		return nullptr;
	}

	CPlayer* pPlayer = CPlayer::Create(this);
	if (m_bIsMainPlayer)
		pPlayer->Set_MainPlayer();

	m_pMyPlayer = pPlayer;

	m_pMyPlayer->Set_UnitType(m_iUnitType);

	return pPlayer;
}

void CPlayerInfo::Set_CustomHead(CLASS_TYPE eClassType, eCUSTOM_HEAD eHeadEnum)
{
	switch (eClassType)
	{
	case Client::WARRIOR:
		Set_CustomHead_Warrior(eHeadEnum);
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		Set_CustomHead_Arhcer(eHeadEnum);
		break;
	case Client::PALADIN:
		Set_CustomHead_Paladin(eHeadEnum);
		break;
	case Client::PRIEST:
		Set_CustomHead_Priest(eHeadEnum);
		break;
	case Client::ENGINEER:
		Set_CustomHead_Engineer(eHeadEnum);
		break;
	case Client::FIONA:
		Set_CustomHead_Fiona(eHeadEnum);
		break;
	case Client::QANDA:
		Set_CustomHead_Qanda(eHeadEnum);
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		Set_CustomHead_Lancer(eHeadEnum);
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}


	
}


void CPlayerInfo::Set_CustomBody(CLASS_TYPE eClassType, eCUSTOM_BODY eBodyEnum)
{
	switch (eClassType)
	{
	case Client::WARRIOR:
		Set_CustomBody_Warrior(eBodyEnum);
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		Set_CustomBody_Arhcer(eBodyEnum);
		break;
	case Client::PALADIN:
		Set_CustomBody_Paladin(eBodyEnum);
		break;
	case Client::PRIEST:
		Set_CustomBody_Priest(eBodyEnum);
		break;
	case Client::ENGINEER:
		Set_CustomBody_Engineer(eBodyEnum);
		break;
	case Client::FIONA:
		Set_CustomBody_Fiona(eBodyEnum);
		break;
	case Client::QANDA:
		Set_CustomBody_Qanda(eBodyEnum);
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		Set_CustomBody_Lancer(eBodyEnum);
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}
}

void CPlayerInfo::Set_CustomWeapon(CLASS_TYPE eClassType, eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eClassType)
	{
	case Client::WARRIOR:
		Set_CustomWeapon_Warrior(eWeaponEnum);
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		Set_CustomWeapon_Arhcer(eWeaponEnum);
		break;
	case Client::PALADIN:
		Set_CustomWeapon_Paladin(eWeaponEnum);
		break;
	case Client::PRIEST:
		Set_CustomWeapon_Priest(eWeaponEnum);
		break;
	case Client::ENGINEER:
		Set_CustomWeapon_Engineer(eWeaponEnum);
		break;
	case Client::FIONA:
		Set_CustomWeapon_Fiona(eWeaponEnum);
		break;
	case Client::QANDA:
		Set_CustomWeapon_Qanda(eWeaponEnum);
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		Set_CustomWeapon_Lancer(eWeaponEnum);
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}
}

CLASS_TYPE CPlayerInfo::Choose_Character()
{
	_uint iSize = (_uint)m_vecPrefClassType.size();

	_uint iRand = random(0, iSize - 1);

	return m_eCurChosenClass = m_vecPrefClassType[iRand];
}

void CPlayerInfo::Release()
{
	SAFE_DELETE(m_pPersonality);
}

void CPlayerInfo::Set_Personality(CAIPersonality* pPersonality)
{
	if (nullptr != pPersonality)
	{
		SAFE_DELETE(m_pPersonality);
		m_pPersonality = pPersonality;
	}

}


_bool CPlayerInfo::Can_Make_Player(const PLAYER_INFO& tInfo)
{
	if (tInfo.wstrCamName.empty() ||
		tInfo.wstrName.empty() ||
		m_vecPrefClassType.empty()
		)
		return false;


	return true;
}

HRESULT CPlayerInfo::SetUp_AIPersonality()
{
	/*CAIPersonality* pPersonality = CPersonality_Default::Create(CGameSystem::Get_Instance()->Get_BXTable());
	m_pPersonality = pPersonality;

	if (!m_pPersonality)
		return E_FAIL;*/

	return S_OK;
}

void CPlayerInfo::Set_CustomHead_Warrior(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0005_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0002_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD2:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0009_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior_Helmet_Rabbit_50.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0005_Helmet_A00_50.fbx";
		break;
	}
}

void CPlayerInfo::Set_CustomBody_Warrior(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/body/SK_Warrior0004_Body_A00_25.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		m_tPlayerSetUpData.wstrBodyMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/body/SK_Warrior0001_Body_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY2:
		m_tPlayerSetUpData.wstrBodyMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/body/SK_Warrior0002_Body_A00_50.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/body/SK_Warrior0004_Body_A00_25.fbx";
		break;
	}
}

void CPlayerInfo::Set_CustomWeapon_Warrior(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[WARRIOR] = L"../bin/resources/meshes/weapons/longsword/SM_WP_LongSword0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[WARRIOR] = L"../bin/resources/meshes/weapons/longsword/SM_WP_LongSword0001_A00.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrWeaponMeshPath[WARRIOR] = L"../bin/resources/meshes/weapons/longsword/SM_WP_LongSword0001_A00.fbx";
		break;
	}
}

void	CPlayerInfo::Set_CustomHead_Paladin(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladin0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladin0001_Helmet_A00_50.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladin_Helm_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladin_Helm_50.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladin0001_Helmet_A00_50.fbx";
		break;
	}
}
void	CPlayerInfo::Set_CustomBody_Paladin(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		//m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladin0001_Body_A00_50.fbx";
		m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladin0011_Body_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladin0001_Body_A00_50.fbx";
		//m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladin0011_Body_A00_20.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladin0001_Body_A00_50.fbx";
		break;
	}
}
void	CPlayerInfo::Set_CustomWeapon_Paladin(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[PALADIN] = L"../bin/resources/meshes/Weapons/Mace/WP_Mace0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[PALADIN] = L"../bin/resources/meshes/Weapons/Mace/WP_Mace0001_A00.fbx";
		break;

	default:
		break;
	}
}
void	CPlayerInfo::Set_CustomHead_Arhcer(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0001_Helmet_A00_50.fbx"; // 기본
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0010_Helmet_A00_50.fbx"; // 누더기 
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0001_Helmet_A00_50.fbx"; // 기본 
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0010_Helmet_A00_50.fbx"; // 누더기 
		
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/warrior/head/SK_Archer1002_Helm_50.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0001_Helmet_A00_50.fbx";
		break;
	}
}
void	CPlayerInfo::Set_CustomBody_Arhcer(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		//m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0010_Body_A00_20.fbx"; // 기본
		m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0010_Body_A00_20.fbx"; // 누더기
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		//m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0001_Body_A00_50.fbx"; // 기본
		m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0010_Body_A00_20.fbx"; // 누더기
		
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0001_Body_A00_50.fbx";
		break;
	}
}
void	CPlayerInfo::Set_CustomWeapon_Arhcer(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[ARCHER] = L"../bin/resources/meshes/weapons/Claw/SK_WP_Claw0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[ARCHER] = L"../bin/resources/meshes/weapons/Claw/SK_WP_Claw0001_A00.fbx";
		break;

	default:
		break;
	}
}

void CPlayerInfo::Set_CustomHead_Engineer(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0004_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0008_Helmet_A00_20.fbx";
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0006_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD2:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0001_Helmet_A00_50.fbx";
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0006_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer1002_Helm_50.fbx";
		break;
	default:
		break;
	}
}

void CPlayerInfo::Set_CustomBody_Engineer(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/body/SK_Engineer0004_Body_A00_30.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		m_tPlayerSetUpData.wstrBodyMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/body/SK_Engineer0008_Body_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY2:
		m_tPlayerSetUpData.wstrBodyMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/body/SK_Engineer0001_Body_A00_50.fbx";
		break;

		m_tPlayerSetUpData.wstrBodyMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/body/SK_Engineer0004_Body_A00_30.fbx";
		break;
	}
}

void CPlayerInfo::Set_CustomWeapon_Engineer(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[ENGINEER] = L"../bin/resources/meshes/weapons/hammer/SM_WP_WarHammer0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[ENGINEER] = L"../bin/resources/meshes/weapons/hammer/SM_WP_WarHammer0001_A00.fbx";
		break;

	default:
		break;
	}

}

void CPlayerInfo::Set_CustomHead_Fiona(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0001_Helmet_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		
		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";


		// 토끼모자 머리카락 안됨
		m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Flona_Helm_50.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0001_Helmet_A00_20.fbx";
		break;
	}
}

void CPlayerInfo::Set_CustomBody_Fiona(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0004_Body_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		//m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0004_Body_A00_50.fbx";
		m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0001_Body_A00.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0001_Body_A00.fbx";
		break;
	}
}

void CPlayerInfo::Set_CustomWeapon_Fiona(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[FIONA] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00_30_4.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[FIONA] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00_30_4.fbx";
		break;

	default:
		break;
	}
}

void CPlayerInfo::Set_CustomHead_Qanda(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/head/SK_Qanda0001_Helmet_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/head/SK_Qanda0001_Helmet_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:

		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/head/SK_Qanda0001_Helmet_A00_20.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/head/SK_Qanda0001_Helmet_A00_20.fbx";
		break;
	}
}
void CPlayerInfo::Set_CustomBody_Qanda(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/body/SK_Qanda0001_Body_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		//m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0004_Body_A00_50.fbx";
		m_tPlayerSetUpData.wstrBodyMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/body/SK_Qanda0001_Body_A00_20.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[QANDA] = L"../bin/resources/meshes/characters/Qanda/body/SK_Qanda0001_Body_A00_20.fbx";
		break;
	}

}
void CPlayerInfo::Set_CustomWeapon_Qanda(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		//m_tPlayerSetUpData.wstrWeaponMeshPath[QANDA] = L"../bin/resources/meshes/weapons/Cane/SK_Priest_Staff_30.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		//m_tPlayerSetUpData.wstrWeaponMeshPath[QANDA] = L"../bin/resources/meshes/weapons/Cane/SK_Priest_Staff_30.fbx";
		break;

	default:
		break;
	}
}

void CPlayerInfo::Set_CustomHead_Priest(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0001_Helm_A00_40.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0010_Helm_A00_20.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0001_Helm_A00_40.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:

		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0010_Helm_A00_20.fbx";
		m_tPlayerSetUpData.wstrHelmetMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0001_Helm_A00_40.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/head/SK_Priest0001_Helm_A00_40.fbx";
		break;
	}
}
void CPlayerInfo::Set_CustomBody_Priest(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/body/SK_Priest0001_Body_A00_10.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		//m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/Priest/body/SK_Priest0010_Body_A00_20.fbx";
		m_tPlayerSetUpData.wstrBodyMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/body/SK_Priest0001_Body_A00_10.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[PRIEST] = L"../bin/resources/meshes/characters/Priest/body/SK_Priest0001_Body_A00_10.fbx";
		break;
	}
}
void CPlayerInfo::Set_CustomWeapon_Priest(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Lance/Lance.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0008_A00.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0004.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Lance/Lance.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0008_A00.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0004.fbx";
		//m_tPlayerSetUpData.wstrWeaponMeshPath[PRIEST] = L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0001_A00.fbx";
		break;

	default:
		break;
	}
}

void CPlayerInfo::Set_CustomHead_Lancer(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0001_Helm_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0001_Helm_A00_20.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0001_Helm_A00_20.fbx";
		break;
	default:
		m_tPlayerSetUpData.wstrHelmetMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0001_Helm_A00_20.fbx";
		break;
	}
}
void CPlayerInfo::Set_CustomBody_Lancer(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/body/SK_Lancer0001_Body_A00_10.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		//m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0004_Body_A00_50.fbx";
		m_tPlayerSetUpData.wstrBodyMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/body/SK_Lancer0001_Body_A00_10.fbx";
		break;

	default:
		m_tPlayerSetUpData.wstrBodyMeshPath[LANCER] = L"../bin/resources/meshes/characters/Lancer/body/SK_Lancer0001_Body_A00_10.fbx";
		break;
	}
}
void CPlayerInfo::Set_CustomWeapon_Lancer(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[LANCER] = L"../bin/resources/meshes/weapons/Lance/Lance.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[LANCER] = L"../bin/resources/meshes/weapons/Lance/Lance.fbx";
		break;

	default:
		break;
	}
}
