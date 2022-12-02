#include "stdafx.h"
#include "CPlayerInfo.h"

#include "UsefulHeaders.h"

CPlayerInfo::CPlayerInfo()
{
}

CPlayerInfo::~CPlayerInfo()
{
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
		break;
	case Client::ENGINEER:
		Set_CustomHead_Engineer(eHeadEnum);
		break;
	case Client::FIONA:
		Set_CustomHead_Fiona(eHeadEnum);
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
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
		break;
	case Client::ENGINEER:
		Set_CustomBody_Engineer(eBodyEnum);
		break;
	case Client::FIONA:
		Set_CustomBody_Fiona(eBodyEnum);
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
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
		break;
	case Client::ENGINEER:
		Set_CustomWeapon_Engineer(eWeaponEnum);
		break;
	case Client::FIONA:
		Set_CustomWeapon_Fiona(eWeaponEnum);
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}
}

CLASS_TYPE CPlayerInfo::Choose_Character()
{
	_uint iSize = m_vecPrefClassType.size();

	_uint iRand = random(0, iSize - 1);

	

	return m_eCurChosenClass = m_vecPrefClassType[iRand];
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

void CPlayerInfo::Set_CustomHead_Warrior(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0009_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior0002_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[WARRIOR] = L"../bin/resources/meshes/characters/warrior/head/SK_Warrior_Helmet_Rabbit_50.fbx";
		break;
	default:
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

	default:
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
		break;
	}
}

void	CPlayerInfo::Set_CustomHead_Paladin(eCUSTOM_HEAD eHeadEnum)
{
	switch (eHeadEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_HEAD::eDEFAULT:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladian0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/head/SK_Paladian0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Warrior_Helmet_Rabbit_50.fbx";
		break;
	default:
		break;
	}
}
void	CPlayerInfo::Set_CustomBody_Paladin(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladian0001_Body_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		m_tPlayerSetUpData.wstrBodyMeshPath[PALADIN] = L"../bin/resources/meshes/characters/Paladin/body/SK_Paladian0001_Body_A00_50.fbx";
		break;

	default:
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
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eHEAD1:
		m_tPlayerSetUpData.wstrHelmetMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/head/SK_Archer0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Warrior_Helmet_Rabbit_50.fbx";
		break;
	default:
		break;
	}
}
void	CPlayerInfo::Set_CustomBody_Arhcer(eCUSTOM_BODY eBodyEnum)
{
	switch (eBodyEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_BODY::eDEFAULT:
		m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0001_Body_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_BODY::eBODY1:
		m_tPlayerSetUpData.wstrBodyMeshPath[ARCHER] = L"../bin/resources/meshes/characters/Archer/body/SK_Archer0001_Body_A00.fbx";
		break;

	default:
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
		m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Engineer0001_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/head/SK_Warrior_Helmet_Rabbit_50.fbx";
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
		m_tPlayerSetUpData.wstrBodyMeshPath[ENGINEER] = L"../bin/resources/meshes/characters/WarHammer/body/SK_Engineer0001_Body_A00_50.fbx";
		break;

	default:
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
		m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_HEAD::eRABBIT:
		//m_tPlayerSetUpData.wstrHelmetMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/head/SK_Fiona0004_Helmet_A00_50.fbx";
		break;
	default:
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
		m_tPlayerSetUpData.wstrBodyMeshPath[FIONA] = L"../bin/resources/meshes/characters/valkyrie/body/SK_Fiona0004_Body_A00_50.fbx";
		break;

	default:
		break;
	}
}

void CPlayerInfo::Set_CustomWeapon_Fiona(eCUSTOM_WEAPON eWeaponEnum)
{
	switch (eWeaponEnum)
	{
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eDEFAULT:
		m_tPlayerSetUpData.wstrWeaponMeshPath[FIONA] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00.fbx";
		break;
	case Client::CPlayerInfo::eCUSTOM_WEAPON::eWEAPON1:
		m_tPlayerSetUpData.wstrWeaponMeshPath[FIONA] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00.fbx";
		break;

	default:
		break;
	}
}
