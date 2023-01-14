#include "stdafx.h"
#include "CMainMenuPlayer.h"

#include "UsefulHeaders.h"

#include "CMainMenuUnit.h"

#include "Easing_Utillity.h"

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
	m_fRotateY[m_eCurClassType] = 180.f;

	
}

void CMainMenuPlayer::Change_ModelParts(CLASS_TYPE eClassType, MODEL_PART_TYPE eModelPartType)
{
	if (eClassType >= CLASS_END)
		return;

	DELETE_GAMEOBJECT(m_pMainMenuUnit[eClassType]);

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

	if (wstrModeSkel[eClassType].empty())
		return;

	tModelData[eClassType].strModelPaths[MODEL_PART_SKEL] = wstrModeSkel[eClassType];
	tModelData[eClassType].strModelPaths[MODEL_PART_BODY] = wstrModeBody[eClassType];
	tModelData[eClassType].strModelPaths[MODEL_PART_FACE] = wstrModeFace[eClassType];
	tModelData[eClassType].strModelPaths[MODEL_PART_HEAD] = wstrModeHead[eClassType];

	if (wstrModeWeapon_R[eClassType] != L"")
	{
		tModelData[eClassType].strModelPaths[MODEL_PART_WEAPON] = wstrModeWeapon_R[eClassType];
		tModelData[eClassType].strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
	}

	if (wstrModeWeapon_L[eClassType] != L"")
	{
		tModelData[eClassType].strModelPaths[MODEL_PART_WEAPON_L] = wstrModeWeapon_L[eClassType];
		tModelData[eClassType].strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
	}

	m_pMainMenuUnit[eClassType] = CMainMenuUnit::Create(tModelData[eClassType], eClassType);
	m_pMainMenuUnit[eClassType]->Initialize();
	CREATE_GAMEOBJECT(m_pMainMenuUnit[eClassType], GROUP_PLAYER);

}

void CMainMenuPlayer::Set_Rotatable(_bool b)
{


	if (!b)
	{
		if (m_pMainMenuUnit[m_eCurClassType])
		{
			if (m_fRotateY[m_eCurClassType] != 180.f)
				m_pMainMenuUnit[m_eCurClassType]->Get_Transform()->Set_LerpLook(_float4(0.f, 0.f, -1.f, 0.f), 1.f);

		}
	}
	m_fRotateY[m_eCurClassType] = 180.f;

	m_bRotatable = b; m_bLerpFOV = true; m_fTimeAcc = 0.f;

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
		m_fRotateY[i] = 180.f;

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
		if (!m_pMainMenuUnit[i])
			continue;

		CREATE_GAMEOBJECT(m_pMainMenuUnit[i], GROUP_PLAYER);

		if (i == WARRIOR)
			continue;

		DISABLE_GAMEOBJECT(m_pMainMenuUnit[i]);
	}

	m_fDefaultFOV = GAMEINSTANCE->Get_CurCam()->Get_Proj().fFOV;
	m_fTargetFOV = m_fDefaultFOV * 1.4f;
	m_fMaxTime = 0.5f;

    return S_OK;
}

void CMainMenuPlayer::My_Tick()
{
	if (!m_bLerpFOV)
		return;

	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc >= m_fMaxTime)
	{
		m_bLerpFOV = false;
		m_fTimeAcc = 0.f;

		if (m_bRotatable)
			GAMEINSTANCE->Get_CurCam()->Get_Proj().fFOV = m_fTargetFOV;
		else
			GAMEINSTANCE->Get_CurCam()->Get_Proj().fFOV = m_fDefaultFOV;

		return;
	}

	_float fRatio = 0.f;

	if (m_bRotatable)
		fRatio = CEasing_Utillity::QuadOut(m_fDefaultFOV, m_fTargetFOV, m_fTimeAcc, m_fMaxTime);
	else
		fRatio = CEasing_Utillity::QuadOut(m_fTargetFOV, m_fDefaultFOV, m_fTimeAcc, m_fMaxTime);

	GAMEINSTANCE->Get_CurCam()->Get_Proj().fFOV = fRatio;



}

void CMainMenuPlayer::My_LateTick()
{
	if (!m_bRotatable)
		return;


	if (KEY(LBUTTON, HOLD))
	{
		if (!m_pMainMenuUnit[m_eCurClassType])
			return;

		_float fMouseMove = (MOUSE_MOVE(MOUSEMOVE::MMS_X) / fDT(0)) * -0.0005f;

		m_fRotateY[m_eCurClassType] += fMouseMove;

		_float4 vCurPos = m_pMainMenuUnit[m_eCurClassType]->Get_Transform()->Get_World(WORLD_POS);
		_float4x4 matRot;
		matRot.Identity();
		CUtility_Transform::Turn_ByAngle(matRot, _float4(0.f, 1.f, 0.f, 0.f), m_fRotateY[m_eCurClassType]);
		m_pMainMenuUnit[m_eCurClassType]->Get_Transform()->Get_Transform().matMyWorld = matRot;
		m_pMainMenuUnit[m_eCurClassType]->Get_Transform()->Set_World(WORLD_POS, vCurPos);
		m_pMainMenuUnit[m_eCurClassType]->Get_Transform()->Make_WorldMatrix();
	}
}
