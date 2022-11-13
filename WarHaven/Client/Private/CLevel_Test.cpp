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

#include "CCamera_Free.h"
#include "CCamera_Follow.h"

#include "CSword_Effect.h"
#include "CEffects_Factory.h"
#include "CBloodOverlay.h"

#include "Model.h"

// MJ_INCLUDE
#include "CDrawable_Terrain.h"

// YJ
#include "CDebugObject.h"

#include "CPhysXCharacter.h"

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

	CLevel_Test* pLevel = new CLevel_Test();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Test::Initialize()
{



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

	Ready_GameObject(pSkyBox, GROUP_DEFAULT);

	m_fLoadingFinish = 0.1f;
	CTerrain* pTerrain = CTerrain::Create(100, 100);
	Ready_GameObject(pTerrain, GROUP_DEFAULT);

	m_fLoadingFinish = 0.2f;


	/* 각자 이름 함수에서 놀으셈*/

    if (FAILED(SetUp_Prototypes_TH()))
        return E_FAIL;
    m_fLoadingFinish = 0.3f;
	if (FAILED(SetUp_Prototypes_HR()))
		return E_FAIL;
	m_fLoadingFinish = 0.4f;
	if (FAILED(SetUp_Prototypes_MJ()))
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
	LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
	LightDesc.fRange = 1000.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Test::Enter()
{
	__super::Enter();
	/* Check for Collision */
	Col_Check();
	CCamera* pFreeCam = CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	CUser::Get_Instance()->SetUp_BloodOverlay();

	GAMEINSTANCE->Begin_PhysScene();

	return S_OK;
}

void CLevel_Test::Tick()
{

#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();
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

#include "CUI_HeroGauge.h"
#include "CUI_HpBar.h"
#include "CUI_Portrait.h"
#include "CUI_Crosshair.h"
#include "CUI_Skill.h"

#include "CUnit.h"

HRESULT CLevel_Test::SetUp_Prototypes_JJ()
{
	CUI_Portrait* pPort = CUI_Portrait::Create();
	Ready_GameObject(pPort, GROUP_UI);

	CUI_Skill* pSkill = CUI_Skill::Create(); 
	Ready_GameObject(pSkill, GROUP_UI);

	CUI_HeroGauge* pHeroGauge = CUI_HeroGauge::Create();
	Ready_GameObject(pHeroGauge, GROUP_UI);

	CUI_HpBar* pHpBar = CUI_HpBar::Create();
	Ready_GameObject(pHpBar, GROUP_UI);

	CUI_Crosshair* pCrosshair = CUI_Crosshair::Create();
	Ready_GameObject(pCrosshair, GROUP_UI);

	

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_TH()
{
    CUnit::UNIT_MODEL_DATA  tModelData;

	//0. 검맨
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

	/* Game Collider */
	_float fColRadius = 1.f;
	pTestWarriorUnit->SetUp_UnitCollider(CUnit::BODY, fColRadius, COL_PLAYERHITBOX_BODY, _float4(0.f, fColRadius * 0.5f, 0.f), DEFAULT_TRANS_MATRIX);
	fColRadius = 0.3f;
	pTestWarriorUnit->SetUp_UnitCollider(CUnit::HEAD, fColRadius, COL_PLAYERHITBOX_HEAD, _float4(0.f, 1.5f, 0.f), DEFAULT_TRANS_MATRIX);
	fColRadius = 0.6f;
	pTestWarriorUnit->SetUp_UnitCollider(CUnit::WEAPON_R, fColRadius, COL_PLAYERATTACK, _float4(0.f, 0.f, -100.f), DEFAULT_TRANS_MATRIX,
		GET_COMPONENT_FROM(pTestWarriorUnit, CModel)->Find_HierarchyNode("0B_R_WP1"));

	Ready_GameObject(pTestWarriorUnit, GROUP_PLAYER);

	/* Test Enemy */
	CUnit_Warrior* pTestEnemyWarrior = CUnit_Warrior::Create(tModelData);
	if (!pTestEnemyWarrior)
		return E_FAIL;

	pTestEnemyWarrior->Initialize();
	pTestEnemyWarrior->Set_TargetUnit(pTestWarriorUnit);
	pTestEnemyWarrior->Reserve_State(STATE_IDLE_WARRIOR_R_AI_ENEMY);

	_float4 vStartPos = _float4(-10.f, 10.f, 10.f);
	pTestEnemyWarrior->Get_Transform()->Set_World(WORLD_POS, vStartPos);
	pTestEnemyWarrior->Get_Transform()->Make_WorldMatrix();
	GET_COMPONENT_FROM(pTestEnemyWarrior, CPhysXCharacter)->Set_Position(vStartPos);

	fColRadius = 1.f;
	pTestEnemyWarrior->SetUp_UnitCollider(CUnit::BODY, fColRadius, COL_ENEMYHITBOX_BODY, _float4(0.f, fColRadius * 0.5f, 0.f), DEFAULT_TRANS_MATRIX);
	fColRadius = 0.3f;
	pTestEnemyWarrior->SetUp_UnitCollider(CUnit::HEAD, fColRadius, COL_ENEMYHITBOX_HEAD, _float4(0.f, 1.5f, 0.f), DEFAULT_TRANS_MATRIX);
	fColRadius = 0.5f;
	pTestEnemyWarrior->SetUp_UnitCollider(CUnit::WEAPON_R, fColRadius, COL_ENEMYATTACK, _float4(0.f, 0.f, 50.f), DEFAULT_TRANS_MATRIX,
		GET_COMPONENT_FROM(pTestEnemyWarrior, CModel)->Find_HierarchyNode("0B_R_WP1"));

	Ready_GameObject(pTestEnemyWarrior, GROUP_ENEMY);


	//1. 창맨
	/*tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/characters/Spearman/Spearman.fbx";

	tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/characters/Spearman/body/SK_Spearman0001_Body_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/Spearman/Head/SK_Spearman0001_Face_A00.fbx";
	tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/characters/Spearman/Head/SK_Spearman0001_Helmet_A00.fbx";

	tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/LongSpear/SM_WP_LongSpear0002_A00.fbx";
	tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

	CUnit_Spearman* pTestSpearmanUnit = CUnit_Spearman::Create(tModelData);
	if (!pTestSpearmanUnit)
		return E_FAIL;

	pTestSpearmanUnit->Initialize();
	Ready_GameObject(pTestSpearmanUnit, GROUP_PLAYER);*/

	CUser::Get_Instance()->Set_Player(pTestWarriorUnit);

	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestWarriorUnit, nullptr);
	pFollowCam->Initialize();
	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	DISABLE_GAMEOBJECT(pFollowCam);
	

    return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_MJ()
{
	//_float4x4 mat;
	//mat.Identity();
	//CStructure* pTestStruct = CStructure::Create(wstring(TEXT("../Bin/Resources/Meshes/Map/Structure/Gate/SM_Module_Gate_CastleGate01a.FBX")), mat);
	//if (nullptr == pTestStruct)
	//	return E_FAIL;
	//pTestStruct->Initialize();
	//Ready_GameObject(pTestStruct, GROUP_DECORATION);

	//CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	//Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

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
	CDebugObject* pDebugObject = CDebugObject::Create(_float4(0.f, -2.f, 0.f), _float4(40.f, 1.f, 40.f), ZERO_VECTOR);
	pDebugObject->Initialize();
	Ready_GameObject(pDebugObject, GROUP_PROP);



	return S_OK;
}

void CLevel_Test::Col_Check()
{
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX_HEAD);
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX_HEAD);
}


