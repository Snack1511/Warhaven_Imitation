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

#include "CGameSystem.h"

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
	Ready_GameObject(pSkyBox, GROUP_DEFAULT);


	m_fLoadingFinish = 0.1f;



	m_fLoadingFinish = 0.2f;

	/* 각자 이름 함수에서 놀으셈*/
	if (FAILED(SetUp_Prototypes_MJ()))
		return E_FAIL;

	m_fLoadingFinish = 0.5f;

	if (FAILED(CGameSystem::Get_Instance()->On_ReadyBootCamp(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 0.7f;

	if (FAILED(CEffects_Factory::Get_Instance()->Initialize()))
		return E_FAIL;

	CCamera* pFreeCam = GAMEINSTANCE->Find_Camera(L"FreeCam");
	DISABLE_GAMEOBJECT(pFreeCam);

	

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

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_EnterBootCamp()))
		return E_FAIL;

	

	return S_OK;
}

void CLevel_Test::Tick()
{
	if (!m_bStaticShadowBake)
	{
		m_fDealyAcc += fDT(0);
		if (m_fDealyAcc >= m_fDelayTime)
		{
			list<CGameObject*> ObjList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);

			for (auto& elem : ObjList)
			{
				m_StaticShadowObjects.push_back(elem);

			}

			GAMEINSTANCE->Bake_StaticShadow(m_StaticShadowObjects, 400.f);
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
#include "CUI_Training.h"

HRESULT CLevel_Test::SetUp_Prototypes_JJ()
{
	CUI_Cursor* pCursor = CUI_Cursor::Create();
	Ready_GameObject(pCursor, GROUP_UI);

	CUI_Training* pUI_Training = CUI_Training::Create();
	Ready_GameObject(pUI_Training, GROUP_UI);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_TH()
{
	/*if(FAILED(SetUp_Terrian_InPlayer()))
		return E_FAIL;*/
	
	if(FAILED(SetUp_Map_InPlayer()))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_MJ()
{
	//맵 데이타 불러오기
	//function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Test::Ready_GameObject, this, placeholders::_1, placeholders::_2);
	//CMap_Loader::Load_Data(wstring(TEXT("TrainingRoom01")), Ready_Object);

	

	_float4x4 mat;
	mat.Identity();
	CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(200, 200);
	pDrawableTerrain->Initialize();
	Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_HR()
{


	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes_YJ()
{


	return S_OK;
}

void CLevel_Test::Col_Check()
{
	/* TRIGGER */
	GAMEINSTANCE->Check_Group(COL_PLAYERTEAM, COL_TRIGGER);
	GAMEINSTANCE->Check_Group(COL_ENEMYTEAM, COL_TRIGGER);

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

	/* 가드 불가 띄우기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_PLAYERFLYATTACKGUARDBREAK, COL_ENEMYHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_PLAYERFLYATTACKGUARDBREAK, COL_ENEMYGUARD);

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
	GAMEINSTANCE->Check_Group(COL_ENEMYFLYATTACK, COL_PLAYERGUARD);


	/* 가드 불가 띄우기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_ENEMYFLYATTACKGUARDBREAK, COL_PLAYERHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_ENEMYFLYATTACKGUARDBREAK, COL_PLAYERGUARD);
	




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
	pPlayerInstance->Get_CurrentUnit()->Synchronize_CamPos();
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



	return S_OK;
}

HRESULT CLevel_Test::SetUp_Valkyrie_TH()
{


	return S_OK;
}

HRESULT CLevel_Test::SetUp_Warrior_Sandback()
{


	return S_OK;
}

HRESULT	CLevel_Test::SetUp_Terrian_InPlayer()
{
	_float4 vPlayerPos = _float4(20.f, 3.f, 20.f);

	CPlayer* pUserPlayer = nullptr;

	if (!(pUserPlayer = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_JUMPFALL_PLAYER_R, true, L"PlayerCam")))
		return E_FAIL;

	pUserPlayer->Set_MainPlayer();

	CUser::Get_Instance()->Set_Player(pUserPlayer);

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_IDLE_WARRIOR_R_AI_ENEMY, false, L"SandBackCam1")))
		return E_FAIL;

	vPlayerPos.x += 5.f;

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY, false, L"SandBackCam2")))
		return E_FAIL;

	vPlayerPos.x += 5.f;

	if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
		STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY, false, L"SandBackCam3")))
		return E_FAIL;

	return S_OK;
}


HRESULT	CLevel_Test::SetUp_Map_InPlayer()
{

	return S_OK;
}
