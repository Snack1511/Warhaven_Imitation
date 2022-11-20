#define MJDEVELOP

#include "stdafx.h"
#include "CLevel_Test.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"
#include "CTerrain.h"

#include "CUnit_Warrior.h"
#include "CUnit_Spearman.h"
#include "CUnit_WarHammer.h"
#include "CUnit_Valkyrie.h"

#include "CBarricade.h"

#include "CCamera_Free.h"
#include "CCamera_Follow.h"

#include "CSword_Effect.h"
#include "CEffects_Factory.h"
#include "CBloodOverlay.h"

#include "Model.h"

// MJ_INCLUDE
#include "CDrawable_Terrain.h"
#include "CStructure_Instance.h"
#include "CUtility_Transform.h"
#include "CMap_Loader.h"
// YJ
#include "CDebugObject.h"

#include "CPhysXCharacter.h"
#include "CStructure.h"

CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
}

CLevel_Test* CLevel_Test::Create()
{
#ifdef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
		return nullptr;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
		return nullptr;
#endif // !_DEBUG


#endif


	CLevel_Test* pLevel = new CLevel_Test();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Test::Initialize()
{
	CUser::Get_Instance()->Initialize();


	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes()
{
	//여기다가 치는 코드는 로딩중에 수행됨 (멀티쓰레드로)
	//여기서 객체 생성한 후 Ready_GameObject 함수로 넣어놓으면 로딩 넘어가고 (멀티쓰레드 끝나고) 오브젝트 매니저에 추가됨.
	// 
	//로딩 Finish 수동으로 해야댐 ㅠ

	GAMEINSTANCE->Create_Scene(CPhysX_Manager::Scene::SCENE_CURRENT);


	CSkyBox* pSkyBox = CSkyBox::Create();

	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;


	m_fLoadingFinish = 0.1f;
	//CTerrain* pTerrain = CTerrain::Create(100, 100);
	//Ready_GameObject(pTerrain, GROUP_DEFAULT);

	m_fLoadingFinish = 0.2f;


	/* 각자 이름 함수에서 놀으셈*/
	if (FAILED(SetUp_Prototypes_MJ()))
		return E_FAIL;
	m_fLoadingFinish = 0.3f;

	/*사연이 있어서 이쪽으로 내려놓음*/
	Ready_GameObject(pSkyBox, GROUP_DEFAULT);


	if (FAILED(SetUp_Prototypes_HR()))
		return E_FAIL;
	m_fLoadingFinish = 0.4f;
	if (FAILED(SetUp_Prototypes_TH()))
		return E_FAIL;
	m_fLoadingFinish = 0.5f;
	if (FAILED(SetUp_Prototypes_JJ()))
		return E_FAIL;
	m_fLoadingFinish = 0.6f;
	if (FAILED(SetUp_Prototypes_YJ()))
		return E_FAIL;
	m_fLoadingFinish = 0.7f;


	if (FAILED(CEffects_Factory::Get_Instance()->Initialize()))
		return E_FAIL;


	CCamera* pFreeCam = GAMEINSTANCE->Find_Camera(L"FreeCam");
	DISABLE_GAMEOBJECT(pFreeCam);


	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(100.f, 200.f, 100.f, 1.f);
	LightDesc.fRange = 1000.f;
	LightDesc.vDiffuse = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Test::Enter()
{


	__super::Enter();
	/*Static Shadow*/



	/* Check for Collision */
	Col_Check();
	CCamera* pFreeCam = CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	CUser::Get_Instance()->SetUp_BloodOverlay();

	GAMEINSTANCE->Begin_PhysScene();

	CEffects_Factory::Get_Instance()->On_EnterLevel();

	return S_OK;
}

void CLevel_Test::Tick()
{
	if (!m_bStaticShadowBake)
	{
		m_fDealyAcc += fDT(0);
		if (m_fDealyAcc >= m_fDelayTime)
		{
			GAMEINSTANCE->Bake_StaticShadow(m_StaticShadowObjects, 200.f);
			m_bStaticShadowBake = true;
		}
	}


#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();


#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();

#endif // !_DEBUG
#endif

	CUser::Get_Instance()->Fix_CursorPosToCenter();
	CUser::Get_Instance()->KeyInput_FPSSetter();
	CUser::Get_Instance()->Update_KeyCommands();

}

void CLevel_Test::Late_Tick()
{
}

HRESULT CLevel_Test::Render()
{

#ifdef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Test Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Test Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif // !_DEBUG
#endif

	return S_OK;
}

HRESULT CLevel_Test::Exit()
{
	__super::Exit();
	GAMEINSTANCE->Delete_Scene(CPhysX_Manager::Scene::SCENE_CURRENT);

	CUser::Get_Instance()->On_ExitLevel();
	GAMEINSTANCE->End_PhysScene();

	return S_OK;
}

#include "CUI_HUD.h"
#include "CUI_Cursor.h"

HRESULT CLevel_Test::SetUp_Prototypes_JJ()
{
	m_pHUD = CUI_HUD::Create();
	Ready_GameObject(m_pHUD, GROUP_UI);

	CUI_Cursor* pCursor = CUI_Cursor::Create();
	Ready_GameObject(pCursor, GROUP_UI);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_TH()
{
	// 테스트할 객체 띄우기

	//if (FAILED(SetUp_Warrior_TH()))
	//	return E_FAIL;

	if (FAILED(SetUp_WarHammer_TH()))
		return E_FAIL;

	/*if (FAILED(SetUp_SpearMan_TH()))
	return E_FAIL;*/

	/*if (FAILED(SetUp_WarHammer_TH()))
		return E_FAIL;*/

	//if (FAILED(SetUp_Valkyrie_TH()))
	//	return E_FAIL;


	/*1. Jump_Fall이나 Land는 Tick에서 따로 넣어
		2. 뒤로가는 키는 Walk로 가게해라 뒤로가는 RUn은 없다
		3. Run 끝날때 모션
		4. Walk
		5. 공격 마우스 시야에 따라

			-
			절댓값이
			0.9~1 중단

			- 양수인지 음수인지 확인

			양수일때 0.9보다 작으면 올려베기
			음수일때
			0보다 작으면 내려베기


			6. 가드브레이크 E키로 변경*/









			/*fColRadius = 1.f;
			pTestEnemyWarrior->SetUp_UnitCollider(CUnit::BODY, fColRadius, COL_ENEMYHITBOX_BODY, _float4(0.f, fColRadius * 0.5f, 0.f), DEFAULT_TRANS_MATRIX);
			fColRadius = 0.3f;
			pTestEnemyWarrior->SetUp_UnitCollider(CUnit::HEAD, fColRadius, COL_ENEMYHITBOX_HEAD, _float4(0.f, 1.75f, 0.f), DEFAULT_TRANS_MATRIX, GET_COMPONENT_FROM(pTestEnemyWarrior, CModel)->Find_HierarchyNode("0B_Head"));
			fColRadius = 0.5f;*/
			/*pTestEnemyWarrior->SetUp_UnitCollider(CUnit::WEAPON_R, fColRadius, COL_ENEMYATTACK, _float4(0.f, 0.f, 50.f), DEFAULT_TRANS_MATRIX,
				GET_COMPONENT_FROM(pTestEnemyWarrior, CModel)->Find_HierarchyNode("0B_R_WP1"));*/



				//1. SpearMan



	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_MJ()
{
	//맵 데이타 불러오기
	 function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Test::Ready_GameObject, this, placeholders::_1, placeholders::_2);
	 CMap_Loader::Load_Data(wstring(TEXT("Effect_Test")), Ready_Object);
	 m_StaticShadowObjects.push_back(m_vecGameObjects.front().first);



	//터레인 불러오기
	//오브젝트 불러오기
	//인스턴스오브젝트 불러오기
	//네비
	//라이트


	//_float4x4 mat;
	//mat.Identity();
	//CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(10, 10);
	//pDrawableTerrain->Initialize();
	//Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	//CStructure* pTestStruct = CStructure::Create(wstring(TEXT("../Bin/Resources/Meshes/Map/Structure/Gate/SM_Module_Gate_CastleGate01a.FBX")), mat);
	/*CStructure* pTestStruct = CStructure::Create(wstring(TEXT("../Bin/Resources/Meshes/Effects/naruto/GroundBreak/SM_EFF_GroundBreak_C.FBX")), mat);
	if (nullptr == pTestStruct)
		return E_FAIL;
	pTestStruct->Get_Transform()->Set_Scale(_float4(40.f, 2.f, 40.f));
	pTestStruct->Get_Transform()->Set_World(WORLD_POS, _float4(10.f, -2.f, 10.f));
	pTestStruct->Get_Transform()->Make_WorldMatrix();
	pTestStruct->Initialize();
	Ready_GameObject(pTestStruct, GROUP_DECORATION);*/


	//CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	//Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);
	//SM_Grass_Weed_Weed02a.FBX

	//_int iNumInstance = 10000;
	//VTXINSTANCE* pVtxInstance = new VTXINSTANCE[iNumInstance];
	//for (_int i = 0; i < iNumInstance; ++i)
	//{
	//	_float4x4 mat;
	//	mat.Identity();
	//	CUtility_Transform::Turn_ByAngle_Up(mat, frandom(0.f, 360.f));
	//	_float4 vRight = mat.XMLoad().r[0];
	//	_float4 vUp = mat.XMLoad().r[1];
	//	_float4 vLook = mat.XMLoad().r[2];
	//	pVtxInstance[i].vRight = vRight;
	//	pVtxInstance[i].vUp = vUp;
	//	pVtxInstance[i].vLook = vLook;
	//	pVtxInstance[i].vTranslation = _float4(frandom(0.f, 100.f), 0.f, frandom(0.f, 100.f), 1.f);
	//}
	//CStructure_Instance* pInstanceStructure = CStructure_Instance::Create(wstring(TEXT("../Bin/Resources/Meshes/Map/Structure/Grass/Weed/SM_Grass_Weed_Weed02a.FBX")), iNumInstance, pVtxInstance);
	//if (nullptr == pInstanceStructure)
	//	return E_FAIL;
	//pInstanceStructure->Initialize();
	//Ready_GameObject(pInstanceStructure, GROUP_DECORATION);
	//Safe_Delete_Array(pVtxInstance);
	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_HR()
{
	/*wstring wstrModel = L"../bin/resources/Meshes/Effects/FBX/SM_Charge_Mesh_01.fbx";
	wstring wstrMask = L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_12_M.dds";
	wstring wstrColor = L"../bin/resources/Textures/Effects/GradationColor/T_EFF_GMS_AmeVillage_Sea_01_M.png";
	wstring wstrNoise = L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_12_M.dds";

	CTestEffect* pTest = CTestEffect::Create(wstrModel, wstrMask, wstrColor, wstrNoise);
	if (!pTest)
		return E_FAIL;

	pTest->Initialize();
	Ready_GameObject(pTest, GROUP_EFFECT);*/



	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_YJ()
{
	//CDebugObject* pDebugObject = CDebugObject::Create(_float4(0.f, -2.f, 0.f), _float4(20.f, 1.f, 20.f), ZERO_VECTOR);
	//pDebugObject->Initialize();
	//Ready_GameObject(pDebugObject, GROUP_PROP);



	return S_OK;
}

void CLevel_Test::Col_Check()
{
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_HEAD);
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_HEAD);
}

HRESULT CLevel_Test::SetUp_Warrior_TH()
{
	CUnit::UNIT_MODEL_DATA  tModelData;

	//0. Warrior
	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/characters/Warrior/Warrior.fbx";

	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx";
	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	CUnit_Warrior* pTestWarriorUnit = CUnit_Warrior::Create(tModelData);
	if (!pTestWarriorUnit)
		return E_FAIL;



	pTestWarriorUnit->Initialize();

	//상태 예약해놓고 Start에서 Enter 호출로 시작됨
	pTestWarriorUnit->Reserve_State(STATE_IDLE_PLAYER_R);



	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	};

	pTestWarriorUnit->SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	pTestWarriorUnit->SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestWarriorUnit, CModel)->Find_HierarchyNode("ob_Head"));

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.5f, _float4(0.f, 0.f, -80.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -135.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -190.f),COL_PLAYERATTACK }
	};

	pTestWarriorUnit->SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, 3, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT_FROM(pTestWarriorUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));

	Ready_GameObject(pTestWarriorUnit, GROUP_PLAYER);
	_float4 vPlayerPos = _float4(20.f, 2.f, 20.f);
	pTestWarriorUnit->Teleport_Unit(vPlayerPos);
	/* Game Collider */

	/* Test Enemy */
	CUnit_Warrior* pTestEnemyWarrior = CUnit_Warrior::Create(tModelData);
	if (!pTestEnemyWarrior)
		return E_FAIL;

	pTestEnemyWarrior->Initialize();
	pTestEnemyWarrior->Set_TargetUnit(pTestWarriorUnit);
	pTestEnemyWarrior->Reserve_State(STATE_IDLE_WARRIOR_R_AI_ENEMY);

	CUnit::UNIT_COLLIDREINFODESC tEnemyUnitInfoDesc;
	CUnit::UNIT_COLLIDERDESC tEnemyUnitColDesc[1];

	tEnemyUnitColDesc[0].fRadius = 1.f;
	tEnemyUnitColDesc[0].vOffsetPos = _float4(0.f, tEnemyUnitColDesc->fRadius * 0.5f, 0.f);
	tEnemyUnitColDesc[0].eColType = COL_ENEMYHITBOX_BODY;

	pTestEnemyWarrior->SetUp_UnitCollider(CUnit::BODY, tEnemyUnitColDesc);

	tEnemyUnitColDesc[0].fRadius = 0.3f;
	tEnemyUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tEnemyUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	pTestEnemyWarrior->SetUp_UnitCollider(CUnit::HEAD, tEnemyUnitColDesc);

	Ready_GameObject(pTestEnemyWarrior, GROUP_ENEMY);

	_float4 vEnemyPos = vPlayerPos;
	vEnemyPos.z += 4.f;
	pTestEnemyWarrior->Teleport_Unit(vEnemyPos);

	

	CUser::Get_Instance()->Set_Player(pTestWarriorUnit);

	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestWarriorUnit, nullptr);
	pFollowCam->Initialize();
	pFollowCam->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	pFollowCam->Get_Transform()->Make_WorldMatrix();
	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	DISABLE_GAMEOBJECT(pFollowCam);
	pTestWarriorUnit->Set_FollowCam(pFollowCam);

	m_pWarrior = pTestWarriorUnit;

	return S_OK;
}


HRESULT CLevel_Test::SetUp_SpearMan_TH()
{

	CUnit::UNIT_MODEL_DATA  tModelData;

	//0. Warrior
	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/characters/Spearman/Spearman.fbx";

	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/characters/Spearman/body/SK_Spearman0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/Spearman/Head/SK_Spearman0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/characters/Spearman/Head/SK_Spearman0001_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/LongSpear/SM_WP_LongSpear0002_A00.fbx";
	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	CUnit_Spearman* pTestSpearmanUnit = CUnit_Spearman::Create(tModelData);
	if (!pTestSpearmanUnit)
		return E_FAIL;



	pTestSpearmanUnit->Initialize();

	//상태 예약해놓고 Start에서 Enter 호출로 시작됨
	pTestSpearmanUnit->Reserve_State(STATE_IDLE_SPEARMAN_R);



	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	};



	pTestSpearmanUnit->SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	pTestSpearmanUnit->SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestSpearmanUnit, CModel)->Find_HierarchyNode("ob_Head"));

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.5f, _float4(0.f, 0.f, -115.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -160.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -55.0f),COL_PLAYERATTACK }
	};

	pTestSpearmanUnit->SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, 3, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT_FROM(pTestSpearmanUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));

	Ready_GameObject(pTestSpearmanUnit, GROUP_PLAYER);




	CUser::Get_Instance()->Set_Player(pTestSpearmanUnit);


	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestSpearmanUnit, nullptr);
	pFollowCam->Initialize();
	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	GAMEINSTANCE->Add_Camera(L"PlayerCam2", pFollowCam);
	DISABLE_GAMEOBJECT(pFollowCam);



	return S_OK;
}

HRESULT CLevel_Test::SetUp_WarHammer_TH()
{

	CUnit::UNIT_MODEL_DATA  tModelData;

	//2. WarHammer
	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx";


	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/Characters/WarHammer/body/SK_Engineer0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/Hammer/SM_WP_WarHammer0001_A00.fbx";

	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	CUnit_WarHammer* pTestWarHammerUnit = CUnit_WarHammer::Create(tModelData);
	if (!pTestWarHammerUnit)
		return E_FAIL;



	pTestWarHammerUnit->Initialize();

	//상태 예약해놓고 Start에서 Enter 호출로 시작됨
	pTestWarHammerUnit->Reserve_State(STATE_IDLE_WARHAMMER_R);



	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	};



	pTestWarHammerUnit->SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	pTestWarHammerUnit->SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestWarHammerUnit, CModel)->Find_HierarchyNode("ob_Head"));

	CUnit::UNIT_COLLIDERDESC tWeapon_RUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.5f, _float4(0.f, 0.f, -115.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -160.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -55.0f),COL_PLAYERATTACK }
	};

	pTestWarHammerUnit->SetUp_UnitCollider(CUnit::WEAPON_R, tWeapon_RUnitColDesc, 3, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT_FROM(pTestWarHammerUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));



	Ready_GameObject(pTestWarHammerUnit, GROUP_PLAYER);

	int iCnt = (int)CUnit_WarHammer::BARRICADE_CNT::BARRICADE_CNT_END;

	/* 바리게이트 생성 */
	for (int i = 0; i < iCnt; ++i)
	{
		CBarricade* pBarricade = CBarricade::Create(pTestWarHammerUnit);

	
		pTestWarHammerUnit->Disable_Barricade(pBarricade);

		Ready_GameObject(pBarricade, GROUP_PLAYER);
	}
	



	_float4 vPlayerPos = _float4(40.f, 5.f, 40.f);
	pTestWarHammerUnit->Teleport_Unit(vPlayerPos);


	CUser::Get_Instance()->Set_Player(pTestWarHammerUnit);


	 CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestWarHammerUnit, nullptr);
	 pFollowCam->Initialize();
	 pFollowCam->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	 pFollowCam->Get_Transform()->Make_WorldMatrix();
	 CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));

	 GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	 DISABLE_GAMEOBJECT(pFollowCam);
	 pTestWarHammerUnit->Set_FollowCam(pFollowCam);

	//DISABLE_GAMEOBJECT(pTestWarHammerUnit);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Valkyrie_TH()
{
	CUnit::UNIT_MODEL_DATA  tModelData;

	//3. Valkyrie
	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/Valkyrie/Valkyrie.fbx";

	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/Characters/Valkyrie/body/SK_Fiona0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00.fbx";
	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	tModelData.strModelPaths[MODEL_PART_WEAPON_L] = L"../bin/resources/meshes/weapons/Valkyrie_Shield/SK_WP_HeaterShield0001_A00.fbx";
	tModelData.strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";

	CUnit_Valkyrie* pTestValkyrieUnit = CUnit_Valkyrie::Create(tModelData);
	if (!pTestValkyrieUnit)
		return E_FAIL;

	pTestValkyrieUnit->Initialize();

	//상태 예약해놓고 Start에서 Enter 호출로 시작됨
	pTestValkyrieUnit->Reserve_State(STATE_IDLE_VALKYRIE_R);


	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;

	// 워해머를 꺼잖아
	// 카메라 매니저가 터지는 이유는 ? 팔로우캠을 못 받아서??

	// 팔로우 캠이 못 받는 이유는 ??

	// 유닛에서 생성하지

	// 유닛에서 복사해서 하는거야

	// 원본객체를 두번 생선한거야?

	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	};



	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("ob_Head"));

	CUnit::UNIT_COLLIDERDESC tWeapon_RUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.45f, _float4(0.f, 0.f, -115.f),COL_PLAYERATTACK },
		{0.45f, _float4(0.f, 0.f, -85.f),COL_PLAYERATTACK },
		{0.45f, _float4(0.f, 0.f, -55.0f),COL_PLAYERATTACK }
	};

	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::WEAPON_R, tWeapon_RUnitColDesc, 3, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));

	CUnit::UNIT_COLLIDERDESC tWeapon_LUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.5f, _float4(0.f, 0.f, -20.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f,  5.f),COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f,  30.f),COL_PLAYERATTACK }
	};

	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::WEAPON_L, tWeapon_LUnitColDesc, 3, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("0B_L_WP1"));


	Ready_GameObject(pTestValkyrieUnit, GROUP_PLAYER);


	_float4 vPlayerPos = _float4(20.f, 2.f, 20.f);
	pTestValkyrieUnit->Teleport_Unit(vPlayerPos);


	CUser::Get_Instance()->Set_Player(pTestValkyrieUnit);

	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestValkyrieUnit, nullptr);
	pFollowCam->Initialize();
	pFollowCam->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	pFollowCam->Get_Transform()->Make_WorldMatrix();
	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	DISABLE_GAMEOBJECT(pFollowCam);
	pTestValkyrieUnit->Set_FollowCam(pFollowCam);



	return S_OK;
}

void CLevel_Test::Change_Player()
{

	if (KEY(P, TAP))
	{
		ENABLE_GAMEOBJECT(m_pWarHammer);
		CUser::Get_Instance()->Set_Player(static_cast<CUnit*>(m_pWarHammer));
		DISABLE_GAMEOBJECT(m_pWarrior);
		
		
//		static_cast<CUnit*>(m_pWarHammer)->Set_FollowCam(static_cast<CCamera_Follow*>(GAMEINSTANCE->Find_Camera(L"PlayerCam")));

		
	}

	if (KEY(J, TAP))
	{
		ENABLE_GAMEOBJECT(m_pWarrior);
		CUser::Get_Instance()->Set_Player(static_cast<CUnit*>(m_pWarHammer));
		DISABLE_GAMEOBJECT(m_pWarHammer);

		//CUser::Get_Instance()->Set_Player(static_cast<CUnit*>(m_pWarrior));
		//DISABLE_GAMEOBJECT(m_pWarHammer);
		//ENABLE_GAMEOBJECT(m_pWarrior);

		//static_cast<CUnit*>(m_pWarrior)->Set_FollowCam(static_cast<CCamera_Follow*>(GAMEINSTANCE->Find_Camera(L"PlayerCam3")));
		
	}

}

