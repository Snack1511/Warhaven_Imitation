#include "stdafx.h"
#include "CMainMenuPlayer.h"

#include "UsefulHeaders.h"

#include "CMainMenuUnit.h"

CMainMenuPlayer::CMainMenuPlayer()
{
}

CMainMenuPlayer::~CMainMenuPlayer()
{
}

CMainMenuPlayer* CMainMenuPlayer::Create(CPlayerInfo* pMainPlayerInfo)
{
	CMainMenuPlayer* pInstance = new CMainMenuPlayer;

	pInstance->m_pMainPlayerInfo = pMainPlayerInfo;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CMainMenuPlayer");
		return nullptr;
	}

	return pInstance;
}

void CMainMenuPlayer::Set_CurClassType(CLASS_TYPE eClassType)
{
	DISABLE_GAMEOBJECT(m_pMainMenuUnit[m_eCurClassType]);
	m_eCurClassType = eClassType;
	ENABLE_GAMEOBJECT(m_pMainMenuUnit[m_eCurClassType]);
	
}

void CMainMenuPlayer::Change_ModelParts(CLASS_TYPE eClassType, MODEL_PART_TYPE eModelPartType)
{
	if (eClassType >= CLASS_END)
		return;

	wstring wstrModelFilePath;

	switch (eModelPartType)
	{
	case Client::MODEL_PART_BODY:
		wstrModelFilePath = m_pMainPlayerInfo->Get_SetUpData().wstrBodyMeshPath[eModelPartType];
		break;
	case Client::MODEL_PART_HEAD:
		wstrModelFilePath = m_pMainPlayerInfo->Get_SetUpData().wstrHelmetMeshPath[eModelPartType];
		break;
	case Client::MODEL_PART_WEAPON:
		wstrModelFilePath = m_pMainPlayerInfo->Get_SetUpData().wstrWeaponMeshPath[eModelPartType];
		break;
	default:
		return;
		break;
	}

	//m_pMainPlayerInfo->Get_SetUpData().

	CModel* pModelCom = GET_COMPONENT_FROM(m_pMainMenuUnit[eClassType], CModel);
	pModelCom->Delete_ModelParts(eModelPartType);
	pModelCom->Add_Model(wstrModelFilePath, eModelPartType);
}

HRESULT CMainMenuPlayer::Initialize_Prototype()
{
	
	wstring wstrModeSkel[CLASS_END] =
	{
		L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // WARRIOR
		L"",
		L"../bin/resources/meshes/characters/Archer/Archer.fbx",
		L"../bin/resources/meshes/characters/Paladin/Paladin.fbx",
		L"../bin/resources/meshes/Characters/Priest/Priest.fbx",
		L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Valkyrie.fbx",
		L"../bin/resources/meshes/Characters/Qanda/Qanda.fbx",
		L"",
		L"../bin/resources/meshes/Characters/Lancer/Lancer.fbx",
	};

	wstring wstrModeFace[CLASS_END] =
	{
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR
		L"",
		L"../bin/resources/meshes/characters/Archer/Head/SK_Archer001_Face_A00_50.fbx",
		L"../bin/resources/meshes/characters/Paladin/Head/SK_Paladin0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Priest/Head/SK_Priest0000_Face_A00_30.fbx",
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Qanda/Head/SK_Qanda0001_Face_A00_50.fbx",
		L"",
		L"",
		//L"../bin/resources/meshes/Characters/Lancer/Head/SK_Lancer0000_Face_A00_20.fbx",
	};

	//L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR

	wstring wstrModeWeapon_L[CLASS_END] =
	{
	L"",
	L"",
	L"",
	L"../bin/resources/meshes/Weapons/KiteShield/SM_WP_KiteShield0001_A00.fbx",
	L"",
	L"",	// ENGINEER
	L"../bin/resources/meshes/Weapons/Valkyrie_Shield/SK_WP_HeaterShield0001_A00_30.fbx",
	L"",
	L"",
	L"",
	};

	CPlayerInfo::PLAYER_SETUP_DATA	tSetUpData = m_pMainPlayerInfo->Get_SetUpData();

	wstring wstrModeBody[CLASS_END];

	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeBody[i] = tSetUpData.wstrBodyMeshPath[i];

	wstring wstrModeHead[CLASS_END];

	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeHead[i] = tSetUpData.wstrHelmetMeshPath[i];

	wstring wstrModeWeapon_R[CLASS_END];
	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeWeapon_R[i] = tSetUpData.wstrWeaponMeshPath[i];


	CUnit::UNIT_MODEL_DATA  tModelData[CLASS_END];

	for (int i = 0; i < CLASS_END; ++i)
	{
		if (wstrModeSkel[i].empty())
			continue;

		tModelData[i].strModelPaths[MODEL_PART_SKEL] = wstrModeSkel[i];
		tModelData[i].strModelPaths[MODEL_PART_BODY] = wstrModeBody[i];
		tModelData[i].strModelPaths[MODEL_PART_FACE] = wstrModeFace[i];
		tModelData[i].strModelPaths[MODEL_PART_HEAD] = wstrModeHead[i];

		if (wstrModeWeapon_R[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON] = wstrModeWeapon_R[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
		}

		if (wstrModeWeapon_L[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON_L] = wstrModeWeapon_L[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
		}

		m_pMainMenuUnit[i] = CMainMenuUnit::Create(tModelData[i], CLASS_TYPE(i));
		m_pMainMenuUnit[i]->Initialize();
	}


    return S_OK;
}

HRESULT CMainMenuPlayer::Initialize()
{
	

    return S_OK;
}

HRESULT CMainMenuPlayer::Start()
{
	for (int i = 0; i < CLASS_END; ++i)
	{
		CREATE_GAMEOBJECT(m_pMainMenuUnit[i], GROUP_PLAYER);

		if (i == WARRIOR)
			continue;

		DISABLE_GAMEOBJECT(m_pMainMenuUnit[i]);
	}

    return S_OK;
}

void CMainMenuPlayer::My_Tick()
{
	if (KEY(UP, TAP))
	{
		_uint iTemp = m_eCurClassType;
		if (iTemp == 9)
			return;
		iTemp++;
		Set_CurClassType((CLASS_TYPE)iTemp);

	}
	else if (KEY(DOWN, TAP))
	{
		_uint iTemp = m_eCurClassType;
		if (iTemp == 0)
			return;
		iTemp--;
		Set_CurClassType((CLASS_TYPE)iTemp);

	}


}
