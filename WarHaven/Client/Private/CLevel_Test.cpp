#include "stdafx.h"
#include "CLevel_Test.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"
#include "CTerrain.h"
#include "CUnit_Warrior.h"

#include "CStructure.h"

#include "CCamera_Free.h"

#include "CSword_Effect.h"
#include "CEffects_Factory.h"
#include "CCamera_Follow.h"


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
	


	CSkyBox* pSkyBox = CSkyBox::Create();

	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;

	Ready_GameObject(pSkyBox, GROUP_DEFAULT);

	m_fLoadingFinish = 0.1f;
	CTerrain* pTerrain = CTerrain::Create(100, 100);
	Ready_GameObject(pTerrain, GROUP_DECORATION);

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

	CEffects_Factory::Get_Instance()->Initialize();


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

	CCamera* pFreeCam = CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	return S_OK;
}

void CLevel_Test::Tick()
{

#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();
#endif
	CUser::Get_Instance()->Fix_CursorPosToCenter();
	CUser::Get_Instance()->KeyInput_FPSSetter();
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

	return S_OK;
}

#include "CUI_Object.h"
#include "CUI_Cursor.h"
HRESULT CLevel_Test::SetUp_Prototypes_JJ()
{
	// CUI_Object* pUI_Object = CUI_Object::Create();
	// Ready_GameObject(pUI_Object, GROUP_UI);

	CUI_Cursor* pCursor = CUI_Cursor::Create();
	Ready_GameObject(pCursor, GROUP_UI);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_TH()
{
    CUnit::UNIT_MODEL_DATA  tModelData;

    tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/characters/warrior/Warrior.fbx";

    tModelData.strModelPaths[MODEL_PART_BODY] = L"../bin/resources/meshes/characters/Warrior/body/SK_Warrior0001_Body_A00.fbx";
    tModelData.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx";
    tModelData.strModelPaths[MODEL_PART_HEAD] = L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00.fbx";

    tModelData.strModelPaths[MODEL_PART_WEAPON] = L"../bin/resources/meshes/weapons/LongSword/SM_WP_LongSword0001_A00.fbx";
    tModelData.strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";

    CUnit_Warrior* pTestUnit = CUnit_Warrior::Create(tModelData);
    if (!pTestUnit)
        return E_FAIL;

    pTestUnit->Initialize();
    Ready_GameObject(pTestUnit, GROUP_PLAYER);

	CUser::Get_Instance()->Set_Player(pTestUnit);

	CCamera_Follow* pFollowCam = CCamera_Follow::Create(pTestUnit, nullptr);
	pFollowCam->Initialize();
	CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
	GAMEINSTANCE->Add_Camera(L"PlayerCam", pFollowCam);
	DISABLE_GAMEOBJECT(pFollowCam);
	

    return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_MJ()
{
	CStructure* pStruct = CStructure::Create();
	if (!pStruct)
		return E_FAIL;
	pStruct->Initialize();
	Ready_GameObject(pStruct, GROUP_DECORATION);

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
	return S_OK;
}


