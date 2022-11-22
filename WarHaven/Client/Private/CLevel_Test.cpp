#define MJDEVELOP

#include "stdafx.h"
#include "CLevel_Test.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"
#include "CTerrain.h"

#include "CPlayer.h"

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

#include "CMapColliders.h"

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

	CUser::Get_Instance()->On_EnterLevel();

	GAMEINSTANCE->Begin_PhysScene();

	CEffects_Factory::Get_Instance()->On_EnterLevel();

	return S_OK;
}

void CLevel_Test::Tick()
{
	Change_Player();

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
	CUI_Cursor* pCursor = CUI_Cursor::Create();
	Ready_GameObject(pCursor, GROUP_UI);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_TH()
{
	_float4 vPlayerPos = _float4(20.f, 3.f, -10.f);

	CPlayer* pUserPlayer = nullptr;

	if (!(pUserPlayer = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_JUMPFALL_PLAYER_R, true, L"PlayerCam")))
		return E_FAIL;

	pUserPlayer->Set_MainPlayer();

	CUser::Get_Instance()->Set_Player(pUserPlayer);

	vPlayerPos.x = 70.f;
	vPlayerPos.z = -10.f;

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_IDLE_WARRIOR_R_AI_ENEMY, false, L"SandBackCam1")))
		return E_FAIL;

	vPlayerPos.x += 5.f;

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY, false, L"SandBackCam2")))
		return E_FAIL;

	vPlayerPos.x -= 10.f;

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY, false, L"SandBackCam3")))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_MJ()
{
	//맵 데이타 불러오기
	function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Test::Ready_GameObject, this, placeholders::_1, placeholders::_2);
	CMap_Loader::Load_Data(wstring(TEXT("TrainingRoom01")), Ready_Object);

	list<CGameObject*> ObjList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);

	for (auto& elem : ObjList)
	{
		m_StaticShadowObjects.push_back(elem);

	}

	//터레인 불러오기
	//오브젝트 불러오기
	//인스턴스오브젝트 불러오기
	//네비
	//라이트


	/* ============= PhysX 충돌체 불러오기 ===================== */

	//CMapColliders* pMapColliders = CMapColliders::Create("SaveTest");
	//Ready_GameObject(pMapColliders, GROUP_DEFAULT);


	//_float4x4 mat;
	//mat.Identity();
	//CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	//pDrawableTerrain->Initialize();
	//Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	/*CStructure* pTestStruct = CStructure::Create(wstring(TEXT("../Bin/Resources/Meshes/Map/Environments/prop/etc/SM_Prop_Etc_Scarecrow02a.FBX")), mat);
	pTestStruct->Initialize();
	pTestStruct->Make_PhysXCollider(CStructure::ePhysXEnum::eCONVEX);
	Ready_GameObject(pTestStruct, GROUP_DECORATION);*/


	/*CStructure* pTestStruct = CStructure::Create(wstring(TEXT("../Bin/Resources/Meshes/Effects/naruto/GroundBreak/SM_EFF_GroundBreak_C.FBX")), mat);
	if (nullptr == pTestStruct)
		return E_FAIL;
	pTestStruct->Get_Transform()->Set_Scale(_float4(40.f, 2.f, 40.f));
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

	/* 일반 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_HEAD);

	/* 가드 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYGUARD);

	/*가드 브레이크 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERGUARDBREAK, COL_ENEMYGUARD);
	GAMEINSTANCE->Check_Group(COL_PLAYERGUARDBREAK, COL_ENEMYHITBOX_BODY);

	/* 그로기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERGROGGYATTACK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERGROGGYATTACK, COL_ENEMYGUARD);
	GAMEINSTANCE->Check_Group(COL_PLAYERGROGGYATTACK, COL_ENEMYATTACK);


	/* 띄우기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERFLYATTACK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERFLYATTACK, COL_ENEMYGUARD);

	//==========================================================================================


	/* 일반 공격 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_HEAD);

	/* 가드 공격 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERGUARD);

	/*가드 브레이크 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYGUARDBREAK, COL_PLAYERGUARD);
	GAMEINSTANCE->Check_Group(COL_ENEMYGUARDBREAK, COL_PLAYERHITBOX_BODY);

	/* 그로기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYGROGGYATTACK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYGROGGYATTACK, COL_PLAYERGUARD);
	GAMEINSTANCE->Check_Group(COL_ENEMYGROGGYATTACK, COL_PLAYERATTACK);

	/* 띄우기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYFLYATTACK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYFLYATTACK, COL_ENEMYGUARD);






}

CPlayer* CLevel_Test::SetUp_Player(_float4 vStartPos, _uint iClassType, STATE_TYPE eStartState, _bool bUserPlayer, wstring wstrCamName)
{
	CPlayer* pPlayerInstance = CPlayer::Create(wstrCamName, (CPlayer::CLASS_DEFAULT)(iClassType));

	if (nullptr == pPlayerInstance)
		return nullptr;

	pPlayerInstance->Reserve_State(eStartState);
	pPlayerInstance->SetUp_UnitColliders(bUserPlayer);
	pPlayerInstance->SetUp_UnitHitStates(bUserPlayer);
	pPlayerInstance->Set_Postion(vStartPos);
	Ready_GameObject(pPlayerInstance, GROUP_USER);



	return pPlayerInstance;
}

HRESULT CLevel_Test::SetUp_Warrior_TH()
{
	return S_OK;
}


HRESULT CLevel_Test::SetUp_SpearMan_TH()
{



	return S_OK;
}

HRESULT CLevel_Test::SetUp_WarHammer_TH()
{

	//CUnit::UNIT_MODEL_DATA  tModelData;

	////2. WarHammer
	//tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx";


	//tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/Characters/WarHammer/body/SK_Engineer0001_Body_A00.fbx";
	//tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00.fbx";
	//tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Helmet_A00.fbx";

	//tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/Hammer/SM_WP_WarHammer0001_A00.fbx";

	//tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	//CUnit_WarHammer* pTestWarHammerUnit = CUnit_WarHammer::Create(tModelData);
	//if (!pTestWarHammerUnit)
	//	return E_FAIL;



	//pTestWarHammerUnit->Initialize();

	////상태 예약해놓고 Start에서 Enter 호출로 시작됨
	//pTestWarHammerUnit->Reserve_State(STATE_IDLE_WARHAMMER_R);
	//pTestWarHammerUnit->SetUp_Colliders(true);
	//pTestWarHammerUnit->SetUp_HitStates(true);


	//

	//Ready_GameObject(pTestWarHammerUnit, GROUP_PLAYER);

	//int iCnt = (int)CUnit_WarHammer::BARRICADE_CNT::BARRICADE_CNT_END;

	///* 바리게이트 생성 */
	//for (int i = 0; i < iCnt; ++i)
	//{
	//	CBarricade* pBarricade = CBarricade::Create(pTestWarHammerUnit, L"../Bin/Resources/Meshes/Map/Environments/Install/SM_Prop_Installation_Trap01a.fbx");

	//
	//	pTestWarHammerUnit->Disable_Barricade(pBarricade);

	//	Ready_GameObject(pBarricade, GROUP_DEFAULT);
	//}
	//


	//_float4 vPlayerPos = _float4(40.f, 5.f, 40.f);
	//pTestWarHammerUnit->Teleport_Unit(vPlayerPos);


	//CUser::Get_Instance()->Set_Player(pTestWarHammerUnit);


	// CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestWarHammerUnit, nullptr);
	// pFollowCam->Initialize();
	// pFollowCam->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	// pFollowCam->Get_Transform()->Make_WorldMatrix();
	// CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));

	// GAMEINSTANCE->Add_Camera(L"PlayerCam4", pFollowCam);
	// DISABLE_GAMEOBJECT(pFollowCam);
	// pTestWarHammerUnit->Set_FollowCam(pFollowCam);

	// m_pWarHammer = pTestWarHammerUnit;


	return S_OK;
}

HRESULT CLevel_Test::SetUp_Valkyrie_TH()
{
	//	CUnit::UNIT_MODEL_DATA  tModelData;
	//
	//	//3. Valkyrie
	//	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/Valkyrie/Valkyrie.fbx";
	//
	//	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/Characters/Valkyrie/body/SK_Fiona0004_Body_A00.fbx";
	//	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Face_A00.fbx";
	//	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0004_Helmet_A00.fbx";
	//
	//	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/Valkyrie_Sword/SM_WP_Sword0001_A00.fbx";
	//	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
	//
	//	tModelData.strModelPaths[MODEL_PART_WEAPON_L] = L"../bin/resources/meshes/weapons/Valkyrie_Shield/SK_WP_HeaterShield0001_A00.fbx";
	//	tModelData.strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
	//
	//	CUnit_Valkyrie* pTestValkyrieUnit = CUnit_Valkyrie::Create(tModelData);
	//	if (!pTestValkyrieUnit)
	//		return E_FAIL;
	//
	//	pTestValkyrieUnit->Initialize();
	//
	//	//상태 예약해놓고 Start에서 Enter 호출로 시작됨
	//	pTestValkyrieUnit->Reserve_State(STATE_IDLE_VALKYRIE_R);
	//
	//
	//	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;
	//
	//	// 워해머를 꺼잖아
	//	// 카메라 매니저가 터지는 이유는 ? 팔로우캠을 못 받아서??
	//
	//	// 팔로우 캠이 못 받는 이유는 ??
	//
	//	// 유닛에서 생성하지
	//
	//	// 유닛에서 복사해서 하는거야
	//
	//	// 원본객체를 두번 생선한거야?
	//
	//	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	//	{
	//		//Radius,	vOffsetPos.		eColType
	//		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
	//		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	//	};
	//
	//
	//
	//	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);
	//
	//	tUnitColDesc[0].fRadius = 0.4f;
	//	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	//	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;
	//
	//
	//	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("ob_Head"));
	//
	//	CUnit::UNIT_COLLIDERDESC tWeapon_RUnitColDesc[3] =
	//	{
	//		//Radius,	vOffsetPos.		eColType
	//		{0.45f, _float4(0.f, 0.f, -115.f),COL_PLAYERATTACK },
	//		{0.45f, _float4(0.f, 0.f, -85.f),COL_PLAYERATTACK },
	//		{0.45f, _float4(0.f, 0.f, -55.0f),COL_PLAYERATTACK }
	//	};
	//
	//	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::WEAPON_R, tWeapon_RUnitColDesc, 3, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));
	//
	//	CUnit::UNIT_COLLIDERDESC tWeapon_LUnitColDesc[3] =
	//	{
	//		//Radius,	vOffsetPos.		eColType
	//		{0.5f, _float4(0.f, 0.f, -20.f),COL_PLAYERATTACK },
	//		{0.5f, _float4(0.f, 0.f,  5.f),COL_PLAYERATTACK },
	//		{0.5f, _float4(0.f, 0.f,  30.f),COL_PLAYERATTACK }
	//	};
	//
	//	pTestValkyrieUnit->SetUp_UnitCollider(CUnit::WEAPON_L, tWeapon_LUnitColDesc, 3, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT_FROM(pTestValkyrieUnit, CModel)->Find_HierarchyNode("0B_L_WP1"));
	//
	//
	//	Ready_GameObject(pTestValkyrieUnit, GROUP_PLAYER);
	//
	//
	//	_float4 vPlayerPos = _float4(20.f, 2.f, 20.f);
	//	pTestValkyrieUnit->Teleport_Unit(vPlayerPos);
	//
	//
	//	CUser::Get_Instance()->Set_Player(pTestValkyrieUnit);
	//
	//	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestValkyrieUnit, nullptr);
	//	pFollowCam->Initialize();
	//	pFollowCam->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	//	pFollowCam->Get_Transform()->Make_WorldMatrix();
	//	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	//	GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	//	DISABLE_GAMEOBJECT(pFollowCam);
	//	pTestValkyrieUnit->Set_FollowCam(pFollowCam);




	return S_OK;
}

HRESULT CLevel_Test::SetUp_Warrior_Sandback()
{


	return S_OK;
}

void CLevel_Test::Change_Player()
{

	//if (KEY(P, TAP))
	//{

	//	_float4 vPlayerPos = m_pWarrior->Get_Transform()->Get_World(WORLD_POS);
	//	m_pWarrior->Teleport_Unit(vPlayerPos);

	//	if (m_pWarHammer)
	//		DISABLE_GAMEOBJECT(m_pWarHammer);

	//	ENABLE_GAMEOBJECT(m_pWarrior);


	////	CUser::Get_Instance()->Set_Player(m_pWarrior);



	//	//		static_cast<CUnit*>(m_pWarHammer)->Set_FollowCam(static_cast<CCamera_Follow*>(GAMEINSTANCE->Find_Camera(L"PlayerCam")));
	//	m_pWarrior->Set_FollowCam(static_cast<CCamera_Follow*>(GAMEINSTANCE->Find_Camera(L"PlayerCam")));


	//}

	//if (KEY(O, TAP))
	//{

	//	_float4 vPlayerPos = m_pWarHammer->Get_Transform()->Get_World(WORLD_POS);
	//	m_pWarHammer->Teleport_Unit(vPlayerPos);


	//	if (m_pWarrior)
	//		DISABLE_GAMEOBJECT(m_pWarrior);

	//	ENABLE_GAMEOBJECT(m_pWarHammer);

	//	//	ENABLE_GAMEOBJECT(m_pWarHammer);
	//	//CUser::Get_Instance()->Set_Player(m_pWarHammer);
	//	//	DISABLE_GAMEOBJECT(m_pWarrior);


	//	//CUser::Get_Instance()->Set_Player(static_cast<CUnit*>(m_pWarrior));
	//	//DISABLE_GAMEOBJECT(m_pWarHammer);
	//	//ENABLE_GAMEOBJECT(m_pWarrior);

	//	m_pWarHammer->Set_FollowCam(static_cast<CCamera_Follow*>(GAMEINSTANCE->Find_Camera(L"PlayerCam4")));

	//}

}

