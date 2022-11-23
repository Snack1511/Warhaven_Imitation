#include "stdafx.h"
#include "CPlayer.h"

#include "CEffects_Factory.h"

#include "GameInstance.h"

#include "CCamera_Default.h"
#include "CCollider_Sphere.h"
#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CNavigation.h"
#include "CCell.h"
#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CScript_FollowCam.h"

#include "MeshContainer.h"

#include "CState.h"
#include "CState_Manager.h"

#include "CPhysXCharacter.h"

#include "CBoneCollider.h"

#include "CCamera_Follow.h"
#include "CState_Hit.h"

#include "CUnit_Warrior.h"
#include "CUnit_Spearman.h"
#include "CUnit_WarHammer.h"
#include "CUnit_Valkyrie.h"



#define ERR_MSG_TH


CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	
}

CPlayer* CPlayer::Create(wstring wstrCamKey, CLASS_DEFAULT eClass)
{
	CPlayer* pInstance = new CPlayer;


	if (FAILED(pInstance->Initialize_Prototype(wstrCamKey, eClass)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}

	return pInstance;
}


void CPlayer::Create_DefaultClass()
{/*
	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx";


	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/Characters/WarHammer/body/SK_Engineer0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/Hammer/SM_WP_WarHammer0001_A00.fbx";*/

	//wstring wstrModeSkel[CLASS_DEFAULT_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Warrior.fbx" // WARRIOR
	//};

	//wstring wstrModeBody[CLASS_DEFAULT_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx" // WARRIOR
	//};

	//wstring wstrModeFace[CLASS_DEFAULT_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx" // WARRIOR
	//};

	//wstring wstrModeHead[CLASS_DEFAULT_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx" // WARRIOR
	//};

	//wstring wstrModeWeapon_R[CLASS_DEFAULT_END] = {
	//	L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx" // WARRIOR
	//};

	//wstring wstrModeWeapon_L[CLASS_DEFAULT_END] = {
	//	L"" // WARRIOR
	//};

	wstring wstrModeSkel[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // WARRIOR
		L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx"
	};

	wstring wstrModeBody[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx", // WARRIOR
		L"../bin/resources/meshes/Characters/WarHammer/body/SK_Engineer0001_Body_A00.fbx"
	};

	wstring wstrModeFace[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx", // WARRIOR
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00.fbx"
	};

	wstring wstrModeHead[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx", // WARRIOR
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Helmet_A00.fbx"
	};

	wstring wstrModeWeapon_R[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx", // WARRIOR
		L"../bin/resources/meshes/weapons/Hammer/SM_WP_WarHammer0001_A00.fbx" };

	wstring wstrModeWeapon_L[CLASS_DEFAULT_END] = {
		L"", // WARRIOR
		L"" // 
	};




	CUnit::UNIT_MODEL_DATA  tModelData[CLASS_DEFAULT_END];

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{

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
	}

	m_pDefaultClass[CLASS_DEFAULT_WARRIOR] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_WARRIOR]);
	m_pDefaultClass[CLASS_DEFAULT_ENGINEER] = CUnit_WarHammer::Create(tModelData[CLASS_DEFAULT_ENGINEER]);
	/*_pDefaultClass[CLASS_DEFAULT_SPEAR] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_SPEAR]);
	m_pDefaultClass[CLASS_DEFAULT_ARCHER] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_ARCHER]);
	m_pDefaultClass[CLASS_DEFAULT_PALADIN] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_PALADIN]);
	m_pDefaultClass[CLASS_DEFAULT_PRIEST] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_PRIEST]);
	*/

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (nullptr == m_pDefaultClass[i])
		{
			Call_MsgBox(L"유닛 생성 안됐음.");
			return;
		}
		else
		{
			m_pDefaultClass[i]->Initialize();

			if (!m_pFollowCam)
			{
				Call_MsgBox(L"카메라 생성 안됐음.");
				return;
			}
				
			m_pDefaultClass[i]->Set_FollowCam(m_pFollowCam);
			m_pDefaultClass[i]->Teleport_Unit(_float4(20.f, 3.f, 20.f));
		}

	}

	m_iReserveState[CLASS_DEFAULT_WARRIOR] = STATE_IDLE_PLAYER_R;
	m_iReserveState[CLASS_DEFAULT_ENGINEER] = STATE_IDLE_WARHAMMER_R;

}

void CPlayer::Create_HeroClass()
{

	//wstring wstrModeSkel[CLASS_HERO_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // FIONA
	//	L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // HOEDT
	//	L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // ARCHER
	//	L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // LANCER
	//};

	//wstring wstrModeBody[CLASS_HERO_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx", // FIONA
	//	L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx", // HOEDT
	//	L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx", // QANDA
	//	L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx" // LANCER
	//};

	//wstring wstrModeFace[CLASS_HERO_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx", // FIONA
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx", // HOEDT
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx", // QANDA
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx" // LANCER
	//};

	//wstring wstrModeHead[CLASS_HERO_END] = {
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx", // FIONA
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx", // HOEDT
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx", // QANDA
	//	L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx" // LANCER
	//};


	//wstring wstrModeWeapon_R[CLASS_HERO_END] = {
	//	L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx", // FIONA
	//	L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx",// HOEDT
	//	L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx",// QANDA
	//	L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx" // LANCER
	//};

	//wstring wstrModeWeapon_L[CLASS_HERO_END] = {
	//	L"", // FIONA (사용함)
	//	L"", // HOEDT
	//	L"", // QANDA
	//	L"", // 
	//};

	//CUnit::UNIT_MODEL_DATA  tModelData[CLASS_HERO_END];

	//for (int i = 0; i < CLASS_HERO_END; ++i)
	//{
	//	tModelData[i].strModelPaths[MODEL_PART_SKEL] = wstrModeSkel[i];

	//	tModelData[i].strModelPaths[MODEL_PART_BODY] = wstrModeBody[i];
	//	tModelData[i].strModelPaths[MODEL_PART_FACE] = wstrModeFace[i];
	//	tModelData[i].strModelPaths[MODEL_PART_HEAD] = wstrModeHead[i];


	//	if (wstrModeWeapon_R[i] != L"")
	//	{
	//		tModelData[i].strModelPaths[MODEL_PART_WEAPON] = wstrModeWeapon_R[i];
	//		tModelData[i].strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
	//	}

	//	if (wstrModeWeapon_L[i] != L"")
	//	{
	//		tModelData[i].strModelPaths[MODEL_PART_WEAPON_L] = wstrModeWeapon_L[i];
	//		tModelData[i].strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
	//	}
	//}


	//m_pHeroClass[CLASS_HREO_FIONA] = CUnit_Warrior::Create(tModelData[CLASS_HREO_FIONA]);
	////m_pHeroClass[CLASS_HREO_QANDA] = CUnit_Warrior::Create(tModelData[CLASS_HREO_QANDA]);
	////m_pHeroClass[CLASS_HREO_HOEDT] = CUnit_Warrior::Create(tModelData[CLASS_HREO_HOEDT]);
	////m_pHeroClass[CLASS_HREO_LANCER] = CUnit_Warrior::Create(tModelData[CLASS_HREO_LANCER]);

	//m_pHeroClass[CLASS_HREO_QANDA] = m_pHeroClass[CLASS_HREO_FIONA];
	//m_pHeroClass[CLASS_HREO_HOEDT] = m_pHeroClass[CLASS_HREO_FIONA];
	//m_pHeroClass[CLASS_HREO_LANCER] = m_pHeroClass[CLASS_HREO_FIONA];

	//for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	//{
	//	if (nullptr == m_pDefaultClass[i])
	//	{
	//		Call_MsgBox(L"유닛 생성 안됐음.");
	//		return;
	//	}
	//	else
	//	{
	//		m_pHeroClass[i]->Initialize();
	//		m_pHeroClass[i]->SetUp_Colliders(true);
	//		m_pHeroClass[i]->SetUp_HitStates(true);

	//		if (!m_pFollowCam)
	//		{
	//			Call_MsgBox(L"카메라 생성 안됐음.");
	//			return;
	//		}

	//		m_pHeroClass[i]->Set_FollowCam(m_pFollowCam);
	//	}
	//		

	//}
}

HRESULT CPlayer::Set_FollowCam(wstring wstrCamKey)
{
	//if (nullptr != GAMEINSTANCE->Find_Camera(wstrCamKey))
	//{
	//	Call_MsgBox(L"CPlayer : 똑같은 HashKey 카메라 생성.. \ 카메라 이름을 다른 이름으로 사용하세요.");
	//	return E_FAIL;
	//}


	m_pFollowCam = CCamera_Follow::Create(this, nullptr);
	m_pFollowCam->Initialize();
	m_pFollowCam->Get_Transform()->Set_World(WORLD_POS, ZERO_VECTOR);
	m_pFollowCam->Get_Transform()->Make_WorldMatrix();
	CREATE_STATIC(m_pFollowCam, Convert_ToHash(wstrCamKey));
	GAMEINSTANCE->Add_Camera(wstrCamKey, m_pFollowCam);
	DISABLE_GAMEOBJECT(m_pFollowCam);

	return S_OK;
}


HRESULT CPlayer::Change_DefaultUnit(CLASS_DEFAULT eClass)
{
	if (eClass >= CLASS_DEFAULT_END)
		return E_FAIL;
		
	if (m_pCurrentUnit)
	{
		DISABLE_GAMEOBJECT(m_pCurrentUnit);
	}
		
	m_pCurrentUnit = m_pDefaultClass[eClass];
	ENABLE_GAMEOBJECT(m_pCurrentUnit);


	m_pCurrentUnit->Reserve_State((STATE_TYPE)m_iReserveState[eClass]);
	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);

	return S_OK;
}

HRESULT CPlayer::Change_HeroUnit(CLASS_HREO eClass)
{
	//if (eClass >= CLASS_HERO_END)
	//	return E_FAIL;

	//if (m_pCurrentUnit)
	//{
	//	DISABLE_GAMEOBJECT(m_pCurrentUnit);
	//}

	//m_pCurrentUnit = m_pHeroClass[eClass];
	//ENABLE_GAMEOBJECT(m_pCurrentUnit);

	//m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);


	return S_OK;
}

void CPlayer::Reserve_State(_uint eState)
{
	m_pCurrentUnit->Reserve_State(STATE_TYPE(eState)); 
}

void CPlayer::SetUp_UnitColliders(_bool bPlayer)
{
	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		m_pDefaultClass[i]->SetUp_Colliders(bPlayer);
	}

	/*for (int i = 0; i < CLASS_HERO_END; ++i)
	{
		m_pHeroClass[i]->SetUp_Colliders(bPlayer);
	}*/

}

void CPlayer::SetUp_UnitHitStates(_bool bPlayer)
{

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		m_pDefaultClass[i]->SetUp_HitStates(bPlayer);
	}

	/*for (int i = 0; i < CLASS_HERO_END; ++i)
	{
		m_pHeroClass[i]->SetUp_HitStates(bPlayer);
	}*/
}

void CPlayer::Set_Postion(_float4 vPos)
{
	if (m_pCurrentUnit)
	{
		m_pCurrentUnit->Teleport_Unit(vPos);
	}
}

void CPlayer::Set_MainPlayer()
{
	m_bIsMainPlayer = true;

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		m_pDefaultClass[i]->Set_MainPlayer();
	}
}


HRESULT CPlayer::Initialize_Prototype(wstring wstrCamKey, CLASS_DEFAULT eClass)
{
	Set_FollowCam(wstrCamKey);

	Create_DefaultClass();
	Create_HeroClass();


	m_pCurrentUnit = m_pDefaultClass[eClass];
	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);

	// 유닛 10개 생성
	// 카메라 1개 생성
	// 카메라 이름 안겹치게 생성
	// 적인지 아군인지 설정



	return S_OK;
}

HRESULT CPlayer::Initialize()
{

	return S_OK;
}

HRESULT CPlayer::Start()
{

	__super::Start();

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (nullptr == m_pDefaultClass[i])
		{
			Call_MsgBox(L"CPlayer : 기본 영웅 추가 안됐음");
			return E_FAIL;
		}

		CREATE_GAMEOBJECT(m_pDefaultClass[i], GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pDefaultClass[i]);
	
	}

	//for (int i = 0; i < CLASS_HERO_END; ++i)
	//{
	//	if (nullptr == m_pHeroClass[i])
	//	{
	//		Call_MsgBox(L"CPlayer : 영웅 추가 안됐음");
	//		return E_FAIL;
	//	}

	//		CREATE_GAMEOBJECT(m_pHeroClass[i], GROUP_PLAYER);
	//		DISABLE_GAMEOBJECT(m_pHeroClass[i]);
	//	
	//}



	if (m_pCurrentUnit)
	{
		ENABLE_GAMEOBJECT(m_pCurrentUnit);
	}
	else
	{
		Call_MsgBox(L"CPlayer : 현재 지정된 유닛이 없음.");
		return E_FAIL;
	}



	return S_OK;
}

void CPlayer::OnEnable()
{
	__super::OnEnable();
}

void CPlayer::OnDisable()
{
	__super::OnDisable();


}


void CPlayer::My_Tick()
{

}

void CPlayer::My_LateTick()
{

}
