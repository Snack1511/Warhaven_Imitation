#include "stdafx.h"
#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CPositionTable.h"

#include "CTrigger_BootCamp.h"
#include "CTrigger_Paden.h"

#include "CPlayer.h"

#include "CUI_Cursor.h"
#include "CUI_Training.h"

#include "CDestructible.h"

#include "CPlayerInfo_Main.h"
#include "CPlayerInfo_SandBack.h"
#include "CPlayerInfo_Default.h"
#include "CPlayerInfo_Leader.h"

#include "CSquad.h"
#include "CTeamConnector.h"

#include "CDominion_Effect.h"

#include "CPath.h"

#include "CCannon.h"

#pragma region AI 성향
#include "CTable_Conditions.h"
#include "CPersonality_Default.h"
#include "CBehavior.h"
#include "CAIController.h"
#pragma endregion AI 성향

IMPLEMENT_SINGLETON(CGameSystem);


#define READY_GAMEOBJECT(instance, grouptype) vecReadyObjects.push_back(make_pair(instance, grouptype))

#define TRIGGER_PADEN(mapkey) static_cast<CTrigger_Paden*>(m_mapAllTriggers[Convert_ToHash(mapkey)])

CGameSystem::CGameSystem()
{
}

CGameSystem::~CGameSystem()
{
	Release();
}

HRESULT CGameSystem::Initialize()
{
#define SAFE_CREATE(name, className) name = className::Create(); if (!name) return E_FAIL;

	SAFE_CREATE(m_pPositionTable, CPositionTable);
	SAFE_CREATE(m_pConditionTable, CTable_Conditions);

	if (FAILED(m_pPositionTable->Load_Position("BootCamp")))
	{
		Call_MsgBox(L"Failed to Load_Position : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(m_pPositionTable->Load_Position("Paden_Position")))
	{
		Call_MsgBox(L"Failed to Load_Position : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(SetUp_AllPlayerInfos()))
	{
		Call_MsgBox(L"Failed to SetUp_AllPlayerInfos : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(SetUp_AllPathes()))
	{
		Call_MsgBox(L"Failed to SetUp_AllPathes : CGameSystem");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameSystem::Tick()
{
	return S_OK;
}

void CGameSystem::Release()
{
	SAFE_DELETE(m_pPositionTable);
	SAFE_DELETE(m_pConditionTable);

	for (_uint i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; ++i)
		SAFE_DELETE(m_pTeamConnector[i]);

	for (auto& elem : m_mapAllPlayers)
	{
		SAFE_DELETE(elem.second);
	}
	m_mapAllPlayers.clear();

	for (_uint i = 0; i < eSTAGE_CNT; ++i)
	{
		for (auto& elem : m_mapAllPathes[i])
		{
			SAFE_DELETE(elem.second);
		}
		m_mapAllPathes[i].clear();
	}

}

HRESULT CGameSystem::On_ExitLevel()
{
	for (auto& elem : m_mapAllPlayers)
	{
		elem.second->m_pMyPlayer = nullptr;
	}

	for (_uint i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; ++i)
		SAFE_DELETE(m_pTeamConnector[i]);

	m_mapAllTriggers.clear();

	return S_OK;
}

HRESULT CGameSystem::On_ReadyTest(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	_float4 vPlayerPos = _float4(10.f, 3.f, 10.f);

	CPlayer* pUserPlayer = nullptr;


	//pUserPlayer->Get_DefaultReserveStateIndex
	pUserPlayer = SetUp_Player(HASHCODE(CPlayerInfo_Main));
	pUserPlayer->Set_Postion(vPlayerPos);
	pUserPlayer->Reserve_State(STATE_IDLE_PLAYER_R);
	pUserPlayer->SetUp_UnitColliders(true);
	pUserPlayer->Enable_OnStart();
	CUser::Get_Instance()->Set_Player(pUserPlayer);
	READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);

	for (_uint i = 0; i < 0; ++i)
	{
		vPlayerPos.z += 10.f;
		vPlayerPos.x += 1.f;

		CPlayer* pEnemy = nullptr;

		pEnemy = SetUp_Player(Convert_ToHash(L"TestEnemy"));
		pEnemy->Set_OutlineType(CPlayer::eENEMY);
		pEnemy->Set_Postion(vPlayerPos);
		pEnemy->Set_TargetPlayer(pUserPlayer);
		pEnemy->Enable_OnStart();
		pEnemy->SetUp_UnitColliders(false);

		// 기본 코드
		// AI_STATE_DEFAULT_IDLE_WARRIOR_R
		//pEnemy->Reserve_State(AI_STATE_PATROL_DEFAULT_WARRIOR_R);
		pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eAI_Default);

		// 테스트용 샌드백
		//pEnemy->Reserve_State(STATE_IDLE_WARRIOR_L_AI_ENEMY);
		// AI 바보만드는 코드 (체력 10만)
		//pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eAI_idiot);

		// 궁수 테스트
		//pEnemy->Reserve_State(AI_STATE_PATROL_DEFAULT_ARCHER_R);
		//pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eAI_Default);

		// 힐러 테스트
		pEnemy->Reserve_State(AI_STATE_PATROL_DEFAULT_PRIEST);
		pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eAI_Default);



		READY_GAMEOBJECT(pEnemy, GROUP_PLAYER);
	}

	SetUp_DefaultLight_BootCamp();

	return S_OK;

}

HRESULT CGameSystem::On_EnterTest()
{

	return S_OK;
}

HRESULT CGameSystem::On_ReadyBootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	/* Player */
	if (FAILED(On_ReadyPlayers_BootCamp(vecReadyObjects)))
	{
		Call_MsgBox(L"Failed to On_ReadyPlayers_BootCamp : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(On_ReadyUIs_BootCamp(vecReadyObjects)))
	{
		Call_MsgBox(L"Failed to On_ReadyUIs_BootCamp : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(On_ReadyTriggers_BootCamp(vecReadyObjects)))
	{
		Call_MsgBox(L"Failed to On_ReadyTriggers_BootCamp : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(On_ReadyDestructible_BootCamp(vecReadyObjects)))
	{
		Call_MsgBox(L"Failed to On_ReadyDestructible_BootCamp : CGameSystem");
		return E_FAIL;
	}

	if (FAILED(SetUp_DefaultLight_BootCamp()))
	{
		Call_MsgBox(L"Failed to SetUp_DefaultLight_BootCamp : CGameSystem");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameSystem::On_ReadyPlayers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	_float4 vPlayerPos = CGameSystem::Get_Instance()->Find_Position("StartPosition");

	CPlayer* pUserPlayer = nullptr;

	pUserPlayer = SetUp_Player(HASHCODE(CPlayerInfo_Main));
	pUserPlayer->Set_Postion(vPlayerPos);
	pUserPlayer->SetUp_UnitColliders(true);
	pUserPlayer->Reserve_State(STATE_IDLE_PLAYER_R);
	pUserPlayer->Enable_OnStart();
	CUser::Get_Instance()->Set_Player(pUserPlayer);
	READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);

	_float4 vEnemyPos;
	CPlayer* pEnemyUser = nullptr;

	for (_uint i = 0; i < 8; ++i)
	{
		STATE_TYPE eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
		string strKey;
		wstring wstrInfoKey;

		switch (i)
		{
		case 0:
			strKey = "EnemyTrio_1";
			wstrInfoKey = L"EnemyTrio_0";
			eEnemyState = SANDBAG_STATE_IDLE_WARHAMMER_R;

			break;
		case 1:
			strKey = "EnemyTrio_2";
			wstrInfoKey = L"EnemyTrio_1";
			eEnemyState = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY; // STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY  가드 테스트용

			break;
		case 2:
			strKey = "q";
			wstrInfoKey = L"EnemyTrio_2";
			eEnemyState = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY; // STATE_IDLE_WARRIOR_R_AI_ENEMY
			break;
		case 3:
			strKey = "EnemyHall";
			wstrInfoKey = L"EnemyHall";
			break;
		case 4:
			strKey = "EnemyBlock";
			wstrInfoKey = L"EnemyBlock";
			eEnemyState = STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;
			break;
		case 5:
			strKey = "EnemyHeadShot";
			wstrInfoKey = L"EnemyHeadShot";
			eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
			break;
		case 6:
			strKey = "EnemyGuardBreak";
			wstrInfoKey = L"EnemyGuardBreak";
			eEnemyState = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;

			break;

		case 7:
			strKey = "EnemyFinal";
			wstrInfoKey = L"EnemyFinal";
			eEnemyState = AI_STATE_IDLE_WARRIOR_L;

			break;

		default:
			break;
		}



		vEnemyPos = CGameSystem::Get_Instance()->Find_Position(strKey);


		CPlayer* pEnemy = nullptr;

		pEnemy = SetUp_Player(Convert_ToHash(wstrInfoKey));

		if (i != 7)
			pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eSandbag);
		else
			pEnemy->Set_UnitType((_uint)CUnit::UNIT_TYPE::eAI_TG);

		if (!pEnemy)
			assert(0);

		pEnemy->Set_OutlineType(CPlayer::eENEMY);
		pEnemy->SetUp_UnitColliders(false);
		pEnemy->Set_Postion(vEnemyPos);
		pEnemy->Set_TargetPlayer(pUserPlayer);
		pEnemy->Reserve_State(eEnemyState);
		READY_GAMEOBJECT(pEnemy, GROUP_ENEMY);


	}







	return S_OK;

}

HRESULT CGameSystem::On_ReadyUIs_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{


	return S_OK;
}

HRESULT CGameSystem::On_ReadyTriggers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
#define ADD_ADJPLAYER(triggerName, Key)  triggerName->Add_AdjPlayer(m_mapAllPlayers[Convert_ToHash(Key)]->Get_Player());

	_float fRadius = 4.f;

	CTrigger_BootCamp* pBootCampTrigger0 = CTrigger_BootCamp::Create("StartPosition", 0, fRadius);
	CTrigger_BootCamp* pBootCampTrigger1 = CTrigger_BootCamp::Create("Popup01", 1, fRadius);
	CTrigger_BootCamp* pBootCampTrigger2 = CTrigger_BootCamp::Create("PopUp02", 2, fRadius);

	CTrigger_BootCamp* pBootCampTrigger3 = CTrigger_BootCamp::Create("Popup03", 3, fRadius);
	CTrigger_BootCamp* pBootCampTrigger4 = CTrigger_BootCamp::Create("Popup04", 4, fRadius);

	CTrigger_BootCamp* pBootCampTrigger5 = CTrigger_BootCamp::Create("Popup05", 5, fRadius);
	ADD_ADJPLAYER(pBootCampTrigger5, L"EnemyTrio_0");
	ADD_ADJPLAYER(pBootCampTrigger5, L"EnemyTrio_1");
	ADD_ADJPLAYER(pBootCampTrigger5, L"EnemyTrio_2");

	CTrigger_BootCamp* pBootCampTrigger6 = CTrigger_BootCamp::Create("Popup06", 6, fRadius);
	ADD_ADJPLAYER(pBootCampTrigger6, L"EnemyHall");

	CTrigger_BootCamp* pBootCampTrigger7 = CTrigger_BootCamp::Create("Popup07", 7, fRadius);
	ADD_ADJPLAYER(pBootCampTrigger7, L"EnemyBlock");

	CTrigger_BootCamp* pBootCampTrigger8 = CTrigger_BootCamp::Create("Popup08", 8, fRadius);
	ADD_ADJPLAYER(pBootCampTrigger8, L"EnemyHeadShot");

	CTrigger_BootCamp* pBootCampTrigger9 = CTrigger_BootCamp::Create("EnemyGuardBreak", 9, fRadius);
	ADD_ADJPLAYER(pBootCampTrigger9, L"EnemyGuardBreak");

	CTrigger_BootCamp* pBootCampTrigger10 = CTrigger_BootCamp::Create("Popup10", 10, 2.f);
	pBootCampTrigger10->Reserve_DisableOnStart();

	CTrigger_BootCamp* pBootCampTriggerBasicEnd = CTrigger_BootCamp::Create("BasicEnd", 99, 2.f);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger0);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger1);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger2);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger3);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger4);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger5);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger6);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger7);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger8);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger9);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger10);

	CTrigger_BootCamp* pLastTrigger = CTrigger_BootCamp::Create("LastTrigger", 100, 3.f);
	ADD_ADJPLAYER(pLastTrigger, L"EnemyFinal");


	vecReadyObjects.push_back(make_pair(pBootCampTrigger0, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger1, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger2, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger3, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger4, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger5, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger6, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger7, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger8, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger9, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTrigger10, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pBootCampTriggerBasicEnd, GROUP_TRIGGER));
	vecReadyObjects.push_back(make_pair(pLastTrigger, GROUP_TRIGGER));

	return S_OK;
}

HRESULT CGameSystem::On_ReadyDestructible_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	CDestructible* pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/prop/Barrier/SM_Prop_Barrier_Fence02a.fbx",
		L"WoodenDestroyParticle",
		L"WoodenHitParticle",
		1
	);

	_float4 vPos = Find_Position("Fence_0");
	vPos.x -= 0.25f;
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));

	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/prop/Barrier/SM_Prop_Barrier_Fence02a.fbx",
		L"WoodenDestroyParticle",
		L"WoodenHitParticle",
		1
	);

	vPos = Find_Position("Fence_1");
	vPos.x += 0.5f;
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));

	return S_OK;
}

HRESULT CGameSystem::On_EnterBootCamp()
{

	BootCamp_EnvironmentEffect();

	return S_OK;
}

HRESULT CGameSystem::BootCamp_EnvironmentEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_0"));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_1"));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_2"));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_3"));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Leaf_Particle", _float4(70.f, 0.f, -15.f));

	/*fire0~9*/
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_0"), _float4(29.2f, 2.95f, 20.65f));  //0
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(29.2f, 2.95f, 20.65f));  //0

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BonFire_0"), _float4(47.8f, 1.6f, 25.7f));  //1 화로
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BonFire_2"), _float4(47.8f, 1.6f, 25.7f));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", _float4(47.8f, 2.6f, 25.7f));

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_2"), _float4(50.9f, 4.0f, -20.5f)); //2 o
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(50.9f, 4.0f, -20.5f)); //2 o

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_0"), _float4(33.43f, 2.1f, 22.2f));  //3
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(33.43f, 2.1f, 22.2f));  //3

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_1"), _float4(36.87f, 3.2f, 21.77f));  //4
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(36.87f, 3.2f, 21.77f));  //4

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_2"), _float4(34.0f, 3.7f, 10.6f));  //5
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(34.0f, 3.7f, 10.6f));  //5

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_0"), _float4(27.35f, 4.35f, -29.3f)); //6
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(27.35f, 4.35f, -29.3f)); //6

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_1"), _float4(37.05f, 4.5f, -25.0f)); //7
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(37.05f, 4.5f, -25.0f)); //7

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_2"), _float4(26.6f, 3.55f, -21.1f)); //8
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_3"), _float4(26.6f, 3.55f, -21.1f)); //8

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BonFire_0"), _float4(41.4f, 1.8f, 27.8f));  //9 화로
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BonFire_2"), _float4(41.4f, 1.8f, 27.8f));  //9 화로

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", _float4(41.4f, 2.8f, 27.8f));


	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(29.2f, 2.95f, 20.65f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(47.8f, 1.6f, 25.7f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(50.9f, 4.0f, -20.5f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(33.43f, 2.1f, 22.2f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(36.87f, 3.2f, 21.77f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(34.0f, 3.7f, 10.6f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(27.35f, 4.35f, -29.3f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(37.05f, 4.5f, -25.0f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(26.6f, 3.55f, -21.1f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = Find_Position("Fire_0");
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(41.4f, 1.8f, 27.8f);
	LightDesc.fRange = 2.5f;
	LightDesc.fRandomRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameSystem::On_ReadyPaden(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	m_eCurStageType = eSTAGE_PADEN;

	/* 플레이어 모두 생성해서 분류까지 완료 */
	if (FAILED(On_ReadyPlayers_Stage(vecReadyObjects)))
		return E_FAIL;


#ifdef _DEBUG
	cout << "플레이어 생성 후 팀 분류 완료." << endl;
#endif // _DEBUG

	/* 플레이어 모두 생성해서 분류까지 완료 */
	if (FAILED(On_ReadyTirggers_Paden(vecReadyObjects)))
		return E_FAIL;

#ifdef _DEBUG
	cout << "트리거 생성 완료." << endl;
#endif // _DEBUG

	SetUp_DefaultLight_Paden();
	if (FAILED(On_ReadyDestructible_Paden(vecReadyObjects)))
		return E_FAIL;





	return S_OK;
}

HRESULT CGameSystem::On_ReadyPlayers_Stage(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	vector<CPlayer*> pArrPlayers;
	CPlayer* pPlayersSquad[MAX_SQUAD][MAX_SQUAD] = {};

	CPlayer* pUserPlayer = nullptr;

	pUserPlayer = SetUp_Player(HASHCODE(CPlayerInfo_Main));
	CUser::Get_Instance()->Set_Player(pUserPlayer);
	READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);

	/* 16개 플레이어 생성하고 섞어서 팀에 뿌림 */
	_uint iIndex = 0;
	pArrPlayers.push_back(pUserPlayer);


	for (auto& pInfo : m_mapAllPlayers)
	{
		if (pInfo.first == HASHCODE(CPlayerInfo_Main))
			continue;

		/* sandback들은 건너 뛰기 */
		if (dynamic_cast<CPlayerInfo_SandBack*>(pInfo.second))
			continue;

		CPlayer* pPlayer = pInfo.second->Make_Player();
		if (!pPlayer)
			return E_FAIL;

		pArrPlayers.push_back(pPlayer);
		pPlayer->Set_Unit_ReserveState(WARRIOR, STATE_IDLE_WARRIOR_R_AI_ENEMY);
		READY_GAMEOBJECT(pPlayer, GROUP_PLAYER);

	}

	//Leader Player부터 하나씩 뽑아서 스쿼드에 배치

	for (auto iter = pArrPlayers.begin(); iter != pArrPlayers.end();)
	{
		CPlayer* pCurPlayer = *iter;

		if (pCurPlayer->Get_Level() > 10 ||
			pCurPlayer->IsMainPlayer()
			)
		{
			//랜덤 스쿼드 인덱스 뽑기
			_uint iSquadIndex = random(0, 3);

			//이미 리더플레이어가 있는지 확인 
			while (pPlayersSquad[iSquadIndex][0])
			{
				iSquadIndex = random(0, 3);
			}

			//리더 플레이어 채워주기
			pPlayersSquad[iSquadIndex][0] = pCurPlayer;
			iter = pArrPlayers.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	/* 스쿼드 나머지 채우기 */
	for (_uint i = 0; i < MAX_SQUAD; ++i)
	{
		// 일반 플레이어들 3명씩 뽑기
		for (_uint j = 0; j < 3; ++j)
		{
			_uint iRandIndex = random(0, pArrPlayers.size() - 1);
			pPlayersSquad[i][j + 1] = pArrPlayers[iRandIndex];

			//뽑힌 애 다시 배열에서 빼기
			auto iter = pArrPlayers.begin();
			for (_uint k = 0; k < iRandIndex; ++k)
				++iter;

			iter = pArrPlayers.erase(iter);
		}
	}


	CSquad* pArrSquad[4] = {};


	for (_uint i = 0; i < MAX_SQUAD; ++i)
	{
		CSquad* pSquad = CSquad::Create(pPlayersSquad[i][0], pPlayersSquad[i][1], pPlayersSquad[i][2], pPlayersSquad[i][3]);
		pArrSquad[i] = pSquad;
	}
	list<CSquad*>   pSquadLists;
	pSquadLists.push_back(pArrSquad[0]);
	pSquadLists.push_back(pArrSquad[1]);
	if (!(m_pTeamConnector[(_uint)eTEAM_TYPE::eRED] = CTeamConnector::Create(pSquadLists)))
		return E_FAIL;

	pSquadLists.clear();
	pSquadLists.push_back(pArrSquad[2]);
	pSquadLists.push_back(pArrSquad[3]);
	if (!(m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE] = CTeamConnector::Create(pSquadLists)))
		return E_FAIL;

	m_pTeamConnector[(_uint)eTEAM_TYPE::eRED]->m_eTeamType = eTEAM_TYPE::eRED;
	m_pTeamConnector[(_uint)eTEAM_TYPE::eRED]->SetUp_TeamType();
	m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE]->m_eTeamType = eTEAM_TYPE::eBLUE;
	m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE]->SetUp_TeamType();

	pUserPlayer->Set_MainPlayerStartPath(0);

#ifdef _DEBUG
	for (_uint i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; ++i)
	{
		cout << "====== 팀 : " << i << endl;

		if (m_pTeamConnector[i]->IsMainPlayerTeam())
			cout << "MainPlayer Team " << i << endl;

		_uint iSquadCnt = 0;
		for (auto& elem : m_pTeamConnector[i]->m_SquadList)
		{
			cout << "----- 스쿼드 : " << iSquadCnt++ << endl;

			for (auto& pPlayer : elem->m_mapPlayers)
			{
				cout << " * 이름 : " << CFunctor::To_String(pPlayer.second->Get_PlayerName()) << endl;
			}
		}


	}
#endif // _DEBUG


	return S_OK;
}

HRESULT CGameSystem::On_ReadyTirggers_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	//0. 양쪽 진영
	CTrigger* pTrigger = nullptr;
	string  strTriggerName;

#define ADD_TRIGGER(name, radius, enumtype)   strTriggerName = name;\
    pTrigger = CTrigger_Paden::Create(strTriggerName, radius, enumtype);\
    if (!pTrigger)\
        return E_FAIL;\
    m_mapAllTriggers.emplace(Convert_ToHash(strTriggerName), pTrigger);\
    READY_GAMEOBJECT(pTrigger, GROUP_TRIGGER);

	_float fTriggerSize = 6.f;

	ADD_TRIGGER("Paden_RedTeam_StartTrigger", 2.f, CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART);
	ADD_TRIGGER("Paden_BlueTeam_StartTrigger", 2.f, CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART);

	m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE]->Add_Trigger(m_mapAllTriggers[Convert_ToHash("Paden_BlueTeam_StartTrigger")]);
	m_pTeamConnector[(_uint)eTEAM_TYPE::eRED]->Add_Trigger(m_mapAllTriggers[Convert_ToHash("Paden_RedTeam_StartTrigger")]);

	/*map에서 찾아다가 static cast 하는 매크로*/
	/*TRIGGER_PADEN("Paden_RedTeam_StartTrigger")->Set_StartTrigger(m_pTeamConnector[(_uint)eTEAM_TYPE::eRED]->IsMainPlayerTeam());
	TRIGGER_PADEN("Paden_BlueTeam_StartTrigger")->Set_StartTrigger(m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE]->IsMainPlayerTeam());*/

	//1. 메인 거점
	ADD_TRIGGER("Paden_Trigger_A", fTriggerSize, CTrigger_Paden::ePADEN_TRIGGER_TYPE::eMAIN);
	CDominion_Effect* pDominionEffect_A = CDominion_Effect::Create(_float4(1.35f, 1.35f, 1.35f), _float4(-0.8f, 2.f, -0.3f),
		(_uint)CTrigger_Paden::ePADEN_TRIGGER_TYPE::eMAIN);
	READY_GAMEOBJECT(pDominionEffect_A, GROUP_EFFECT);
	TRIGGER_PADEN("Paden_Trigger_A")->Set_DominionEffect(pDominionEffect_A);

	ADD_TRIGGER("Paden_Trigger_R", fTriggerSize, CTrigger_Paden::ePADEN_TRIGGER_TYPE::eRESPAWN);
	CDominion_Effect* pDominionEffect_R = CDominion_Effect::Create(_float4(0.8f, 0.8f, 0.8f), _float4(48.6f, 5.5f, -0.f),
		(_uint)CTrigger_Paden::ePADEN_TRIGGER_TYPE::eRESPAWN);
	READY_GAMEOBJECT(pDominionEffect_R, GROUP_EFFECT);
	TRIGGER_PADEN("Paden_Trigger_R")->Set_DominionEffect(pDominionEffect_R);

	ADD_TRIGGER("Paden_Trigger_C", fTriggerSize, CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCANNON);
	CDominion_Effect* pDominionEffect_C = CDominion_Effect::Create(_float4(1.f, 1.f, 1.f), _float4(-61.8f, 20.4f, 0.2f),
		(_uint)CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCANNON);
	READY_GAMEOBJECT(pDominionEffect_C, GROUP_EFFECT);
	TRIGGER_PADEN("Paden_Trigger_C")->Set_DominionEffect(pDominionEffect_C);





	return S_OK;
}

HRESULT CGameSystem::On_ReadyDestructible_Paden(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{

	CDestructible* pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel09a_Lod1.fbx",
		L"FishBarrelParticle",
		L"WoodenHitParticle",
		1
	);
	_float4 vPos = Find_Position("Barrel_0");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel09a_Lod1.fbx",
		L"FishBarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_1");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel09a_Lod1.fbx",
		L"FishBarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_2");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));

	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel09a_Lod1.fbx",
		L"FishBarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_3");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));

	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel09a_Lod1.fbx",
		L"FishBarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_4");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel04a_Lod1.fbx",
		L"BarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_5");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel04a_Lod1.fbx",
		L"BarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_6");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel04a_Lod1.fbx",
		L"BarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_7");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel04a_Lod1.fbx",
		L"BarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_8");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));


	pDestructible = CDestructible::Create(
		L"../bin/resources/meshes/map/environments/Prop/Storage/SM_Prop_Storage_Barrel04a_Lod1.fbx",
		L"BarrelParticle",
		L"WoodenHitParticle",
		1
	);
	vPos = Find_Position("Barrel_9");
	pDestructible->Set_Position(vPos);
	pDestructible->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	vecReadyObjects.push_back(make_pair(pDestructible, GROUP_PROP));



	CCannon* pCannon = CCannon::Create();
	vPos = Find_Position("Paden_Trigger_C");
	vPos.y -= 0.4f;
	vPos.x += 5.5f;
	pCannon->Get_Transform()->Set_World(WORLD_POS, vPos);
	pCannon->Get_Transform()->Set_Look(_float4(1.f, 0.f, 0.f, 0.f));
	pCannon->Get_Transform()->Make_WorldMatrix();
	vecReadyObjects.push_back(make_pair(pCannon, GROUP_PROP));



	return S_OK;
}

HRESULT CGameSystem::On_EnterStage()
{
	CTeamConnector* pAllyTeam = nullptr;
	CTeamConnector* pEnemyTeam = nullptr;
	
	if (m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE]->IsMainPlayerTeam())
	{
		pAllyTeam = m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE];
		pEnemyTeam = m_pTeamConnector[(_uint)eTEAM_TYPE::eRED];
	}
	else
	{
		pAllyTeam = m_pTeamConnector[(_uint)eTEAM_TYPE::eRED];
		pEnemyTeam = m_pTeamConnector[(_uint)eTEAM_TYPE::eBLUE];
	}
	
	CUser::Get_Instance()->Set_Team(pAllyTeam, pEnemyTeam);

	/* 모든 플레이어들 유닛 일단 꺼놓기 */
	//for (auto& elem : m_mapAllPlayers)
	//{
	//    /* sandback들은 건너 뛰기 */
	//    if (dynamic_cast<CPlayerInfo_SandBack*>(elem.second))
	//        continue;

	//    if (!elem.second->Get_Player()->Get_CurrentUnit())
	//        return E_FAIL;

	//    DISABLE_GAMEOBJECT(elem.second->Get_Player()->Get_CurrentUnit());
	//}

	return S_OK;
}

HRESULT CGameSystem::Paden_EnvironmentEffect()
{

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Paden_Envir", _float4(0.f, 0.f, 0.f));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Paden_Roof", _float4(0.f, 0.f, 0.f));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Paden_In", _float4(0.f, 0.f, 0.f));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ash", _float4(0.f, 10.f, 0.f));

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_0"), _float4(-27.7f, -0.5f, 34.8f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_1"), _float4(-38.f, -0.5f, 29.1f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_3"), _float4(-60.1f, 4.7f, 34.7f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_4"), _float4(-24.1f, 3.1f, -76.1f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_5"), _float4(19.6f, -0.3f, -72.f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_6"), _float4(-3.8f, -0.3f, -49.8f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_7"), _float4(-10.4f, 0.f, -57.2f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_8"), _float4(9.2f, -0.5f, -71.8f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_9"), _float4(-28.f, -0.5f, -37.1f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_10"), _float4(-2.2f, -0.4f, 51.1f));

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_2"), _float4(12.7f, 21.6f, 22.2f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_4"), _float4(16.6f, 20.f, -22.4f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_6"), _float4(-17.f, 20.f, -24.3f));

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_11"), _float4(11.5f, -0.4f, -41.8f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_12"), _float4(35.7f, 1.6f, -41.3f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_13"), _float4(42.7f, -0.5f, -15.f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_14"), _float4(46.f, -0.5f, 29.6f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_15"), _float4(28.6f, -0.3f, 44.7f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_16"), _float4(15.f, -0.5f, 38.4f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_17"), _float4(-13.5f, -0.5f, 37.5f));

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_18"), _float4(-8.5f, 24.7f, 23.9f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_20"), _float4(23.2f, 23.9f, 13.3f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_21"), _float4(22.f, 21.5f, -2.7f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_22"), _float4(16.9f, 21.7f, -27.1f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_23"), _float4(-21.1f, 21.5f, 22.4f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_24"), _float4(-22.f, 19.9f, -19.2f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_25"), _float4(-12.7f, 23.1f, -25.4f));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Paden_Smoke_26"), _float4(-50.8f, 4.6f, 19.8f));

	return S_OK;
}

HRESULT CGameSystem::On_Update_Paden()
{
	//점령한 거점 확인해서 점수 깎기

	CTeamConnector* pMinusScoreTeam = nullptr;

	// 각 팀의 커넥터 가져오기
	for (_uint i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; ++i)
	{
		if (m_pTeamConnector[i]->Has_MainTrigger())
		{
			_int j = 0;
			if (i == 0)
				j = 1;

			//반대쪽 팀이 깎일 팀
			pMinusScoreTeam = m_pTeamConnector[j];

			cout << m_pTeamConnector[j]->m_iScore << endl;

			break;
		}
	}

	if (pMinusScoreTeam)
	{
		m_fScoreAcc += fDT(0);

		if (m_fScoreAcc >= m_fScoreMinusTime)
		{
			m_fScoreAcc = 0.f;

			//if (!pMinusScoreTeam->Minus_Score())
			//	On_FinishGame(pMinusScoreTeam);

		}
	}


	/* for (auto& elem : m_mapAllTriggers)
	 {
		 CTrigger_Paden* pPadenTrigger = static_cast<CTrigger_Paden*>(elem.second);

		 switch (pPadenTrigger->Get_TriggerType())
		 {
		 case CTrigger_Paden::ePADEN_TRIGGER_TYPE::eMAIN:
			 pPadenTrigger


			 break;

		 default:
			 break;
		 }

	 }*/






	return S_OK;
}

void CGameSystem::On_StartGame()
{
	/* 작전회의 끝나고 호출되는 함수 */
	CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	for (_uint i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; ++i)
	{
		m_pTeamConnector[i]->On_EnterPaden();
	}

	//1. 모든 플레이어들 시작 위치에서 생성
	_bool   bTemp = false;

	/* 리더 먼저 */
	for (auto& elem : m_mapAllPlayers)
	{
		/* Leader가 아니면 건너 뛰기 */
		if (!dynamic_cast<CPlayerInfo_Leader*>(elem.second) && !dynamic_cast<CPlayerInfo_Main*>(elem.second))
			continue;

		/* ai들은 랜덤 선택 함수 호출 */
		if (!elem.second->m_bIsMainPlayer)
			elem.second->Choose_Character();


		/* 자기 진영에서 포지션 가져오기 */
		_float4 vStartPos = m_pTeamConnector[(_uint)(elem.second->m_pMyTeam->m_eTeamType)]->Find_RespawnPosition_Start();
		elem.second->m_pMyPlayer->Respawn_Unit(vStartPos, elem.second->m_eCurChosenClass);

	}

	/* Default 애덜 */
#ifdef _DEBUG
#else
	for (auto& elem : m_mapAllPlayers)
	{
		/* Default가 아니면 건너 뛰기 */
		if (!dynamic_cast<CPlayerInfo_Default*>(elem.second))
			continue;

		static _uint g_iIndex = 0;

		if (g_iIndex == 0)
		{
			g_iIndex++;
			continue;
		}

		g_iIndex = 0;

		/* ai들은 랜덤 선택 함수 호출 */
		elem.second->Choose_Character();

		/* 자기 진영에서 포지션 가져오기 */
		_float4 vStartPos = m_pTeamConnector[(_uint)(elem.second->m_pMyTeam->m_eTeamType)]->Find_RespawnPosition_Start();
		elem.second->m_pMyPlayer->Respawn_Unit(vStartPos, elem.second->m_eCurChosenClass);

	}

#endif // _DEBUG




}

void CGameSystem::On_FinishGame(CTeamConnector* pTeamConnector)
{
	//한쪽 점수가 0이 되면 끝
	for (auto& elem : m_mapAllPlayers)
	{
		CPlayer* pPlayer = elem.second->Get_Player();
		if (pPlayer)
		{
			pPlayer->On_FinishGame(pTeamConnector);
		}
	}

	if (pTeamConnector->IsMainPlayerTeam())
	{
		// 패배
		CUser::Get_Instance()->SetActive_Result(1, true);
	}
	else
	{
		// 승리
		CUser::Get_Instance()->SetActive_Result(0, true);
	}
}

HRESULT CGameSystem::On_ReadyHwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	m_eCurStageType = eSTAGE_HWARA;

	_float4 vPlayerPos = _float4(10.f, -5.f, 10.f);

	CPlayer* pUserPlayer = nullptr;


	//pUserPlayer->Get_DefaultReserveStateIndex
	pUserPlayer = SetUp_Player(HASHCODE(CPlayerInfo_Main));
	pUserPlayer->Set_Postion(vPlayerPos);
	pUserPlayer->Reserve_State(STATE_IDLE_PLAYER_R);
	pUserPlayer->SetUp_UnitColliders(true);
	pUserPlayer->Enable_OnStart();
	CUser::Get_Instance()->Set_Player(pUserPlayer);
	READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);


	/* 플레이어 모두 생성해서 분류까지 완료 */
//	if (FAILED(On_ReadyPlayers_Stage(vecReadyObjects)))
//		return E_FAIL;
//
//
//#ifdef _DEBUG
//	cout << "플레이어 생성 후 팀 분류 완료." << endl;
//#endif // _DEBUG
//
//	/* 플레이어 모두 생성해서 분류까지 완료 */
//	if (FAILED(On_ReadyTirggers_Hwara(vecReadyObjects)))
//		return E_FAIL;
//
//#ifdef _DEBUG
//	cout << "트리거 생성 완료." << endl;
//#endif // _DEBUG
//
//	if (FAILED(On_ReadyDestructible_Hwara(vecReadyObjects)))
//		return E_FAIL;



	SetUp_DefaultLight_Hwara();

	return S_OK;
}

HRESULT CGameSystem::On_ReadyTirggers_Hwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	return E_NOTIMPL;
}

HRESULT CGameSystem::On_ReadyDestructible_Hwara(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
	return E_NOTIMPL;
}

HRESULT CGameSystem::On_Update_Hwara()
{
	return E_NOTIMPL;
}

HRESULT CGameSystem::Hwara_EnvironmentEffect()
{
	return E_NOTIMPL;
}

HRESULT CGameSystem::Load_Position(string strFileKey)
{
	return m_pPositionTable->Load_Position(strFileKey);
}

_float4 CGameSystem::Find_Position(string wstrPositionKey)
{
	return m_pPositionTable->Find_Position(wstrPositionKey);
}

void CGameSystem::Add_Position(string wstrPositionKey, _float4 vPosition)
{
	m_pPositionTable->Add_Position(wstrPositionKey, vPosition);
}

CBehavior* CGameSystem::Clone_Behavior(wstring wstrBXKey)
{
	CBehavior* pBX = m_pConditionTable->Find_Behavior(wstrBXKey);
	if (pBX)
		return pBX->Clone();

	return nullptr;
}

CPath* CGameSystem::Find_Path(string strPathKey)
{
	for (_uint i = 0; i < eSTAGE_CNT; ++i)
	{
		auto iter = m_mapAllPathes[i].find(Convert_ToHash(strPathKey));

		if (iter != m_mapAllPathes[i].end())
			return iter->second;
	}

	return nullptr;
}

CPath* CGameSystem::Clone_Path(string strPathKey, CAIController* pOwnerController)
{
	CPath* pPath = Find_Path(strPathKey);
	if (pPath)
	{
		pPath = pPath->Clone();
		pPath->m_pOwnerController = pOwnerController;
		return pPath;
	}

	return nullptr;
}

CPath* CGameSystem::Clone_RandomStartPath(CAIController* pOwnerController, eTEAM_TYPE eTeamType)
{
	if (!pOwnerController)
		return nullptr;

	string strFindKey;

	switch (eTeamType)
	{
	case Client::eTEAM_TYPE::eRED:
		strFindKey = "RedTeam";
		break;

	case Client::eTEAM_TYPE::eBLUE:
		strFindKey = "BlueTeam";
		break;

	case Client::eTEAM_TYPE::eCOUNT:
		break;
	default:
		break;
	}

	_uint iSize = 0;


	vector<CPath*> PathVector;

	for (auto& elem : m_mapAllPathes[m_eCurStageType])
	{
		if ((_int)elem.second->m_strName.find(strFindKey) >= 0)
		{
			//iSize++;
			PathVector.push_back(elem.second);
		}
	}

	_int iRandIndex = random(0, _uint(PathVector.size()) - 1);

	CPath* pClonePath = nullptr;
	if (nullptr == PathVector[iRandIndex])
		assert(0);

	pClonePath = PathVector[iRandIndex]->Clone();

	if (nullptr == pClonePath)
		assert(0);

	pOwnerController->Set_NewPath(pClonePath);
	return pClonePath;
}

CPath* CGameSystem::Get_NearPath(_float4 vPosition)
{
	CPath* pReturnPath = nullptr;
	_float fMinLength = 9999999.f;
	for (auto& elem : m_mapAllPathes[m_eCurStageType])
	{
		CPath* pPath = elem.second;
		_float4 vPathStartPosition = pPath->Get_FrontPosition();

		if (fabsf(vPathStartPosition.y - vPosition.y) > 1.5f)
			continue;

		_float LengthDiff = (vPathStartPosition - vPosition).Length();


		if (LengthDiff < fMinLength)
		{



			pReturnPath = pPath;
			fMinLength = LengthDiff;
		}

	}
	return pReturnPath;
}

CTrigger* CGameSystem::Find_Trigger(string strTriggerKey)
{
	auto iter = m_mapAllTriggers.find(Convert_ToHash(strTriggerKey));

	if (iter == m_mapAllTriggers.end())
		return nullptr;

	return iter->second;
}

CPlayerInfo* CGameSystem::Find_PlayerInfo(_hashcode hcCode)
{
	auto iter = m_mapAllPlayers.find(hcCode);
	if (iter == m_mapAllPlayers.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameSystem::SetUp_AllPlayerInfos()
{
	/* 모든 플레이어들 정보 다 만들기 */
	CPlayerInfo* pPlayerInfo = nullptr;

#define ADD_PLAYERINFO(classname)   pPlayerInfo = classname::Create();\
    if (!pPlayerInfo)\
        return E_FAIL;\
    m_mapAllPlayers.emplace(HASHCODE(classname), pPlayerInfo)


	ADD_PLAYERINFO(CPlayerInfo_Main);

	CUser::Get_Instance()->Set_MainPlayerInfo(m_mapAllPlayers[HASHCODE(CPlayerInfo_Main)]);


	/* 일반 AI들 */
#define ADD_DEFAULTINFO(name) pPlayerInfo = CPlayerInfo_Default::Create();\
    pPlayerInfo->m_tPlayerInfo.wstrName = name;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName = pPlayerInfo->m_tPlayerInfo.wstrName;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName += L"_Cam";\
    m_mapAllPlayers.emplace(Convert_ToHash(pPlayerInfo->m_tPlayerInfo.wstrName), pPlayerInfo);\
	m_vecPlayerInfoNames.push_back(pPlayerInfo->m_tPlayerInfo.wstrName)

	_uint iDefaultCnt = 0;
	for (_uint i = 0; i < 12; ++i)
	{
		wstring wstrName = L"DefaultPlayer_";
		wstrName += to_wstring(iDefaultCnt++);
		ADD_DEFAULTINFO(wstrName);
	}

	/* 분대장급 AI들 */
#define ADD_LEADERINFO pPlayerInfo = CPlayerInfo_Leader::Create();\
    m_mapAllPlayers.emplace(Convert_ToHash(pPlayerInfo->m_tPlayerInfo.wstrName), pPlayerInfo);\
	m_vecPlayerInfoNames.push_back(pPlayerInfo->m_tPlayerInfo.wstrName)

	_uint iLeaderCnt = 0;
	for (_uint i = 0; i < 3; ++i)
	{
		ADD_LEADERINFO;
	}





	/* 훈련장 용 샌드백 Info들 */
#define ADD_SANDBACKINFO(name) pPlayerInfo = CPlayerInfo_SandBack::Create();\
    pPlayerInfo->m_tPlayerInfo.wstrName = name;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName = pPlayerInfo->m_tPlayerInfo.wstrName;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName += L"_Cam";\
    m_mapAllPlayers.emplace(Convert_ToHash(pPlayerInfo->m_tPlayerInfo.wstrName), pPlayerInfo);\
	m_vecPlayerInfoNames.push_back(pPlayerInfo->m_tPlayerInfo.wstrName)

	ADD_SANDBACKINFO(L"TestEnemy");
	ADD_SANDBACKINFO(L"EnemyTrio_0");
	ADD_SANDBACKINFO(L"EnemyTrio_1");
	ADD_SANDBACKINFO(L"EnemyTrio_2");
	ADD_SANDBACKINFO(L"EnemyHall");
	ADD_SANDBACKINFO(L"EnemyBlock");
	ADD_SANDBACKINFO(L"EnemyHeadShot");
	ADD_SANDBACKINFO(L"EnemyGuardBreak");
	ADD_SANDBACKINFO(L"EnemyFinal");


	return S_OK;
}

HRESULT CGameSystem::SetUp_AllPathes()
{

	for (filesystem::directory_iterator FileIter("../Bin/Data/GameSystem/Pathes/");
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("/") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);


		if (!entry.is_directory())
		{
			_int iFindExt = (int)strFileName.rfind(".");// +1;
			//string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);
			string HashKey = strFileName.substr(0, iFindExt);

			CPath* pPath = CPath::Create(HashKey);

			eSTAGE_TYPE	eType = eSTAGE_CNT;


			_int iFindKey = HashKey.find("Paden");

			if (iFindKey >= 0)
				eType = eSTAGE_PADEN;

			if (eType == eSTAGE_CNT)
			{
				SAFE_DELETE(pPath);
				continue;
			}

			if (nullptr != pPath)
				m_mapAllPathes[eType].emplace(Convert_ToHash(HashKey), pPath);
			else
				return E_FAIL;
		}

	}
	return S_OK;
}


CPlayer* CGameSystem::SetUp_Player(_hashcode hcName)
{
	auto PlayerInfoIter = m_mapAllPlayers.find(hcName);

	if (PlayerInfoIter == m_mapAllPlayers.end())
		return nullptr;

	return PlayerInfoIter->second->Make_Player();
}

HRESULT CGameSystem::SetUp_DefaultLight_BootCamp()
{
	/* Default Light */
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(100.f, 200.f, 50.f, 1.f);
	LightDesc.fRange = 1500.f;
	LightDesc.vDiffuse = _float4(0.45f, 0.45f, 0.45f, 1.f);
	LightDesc.vAmbient = _float4(0.15f, 0.15f, 0.15f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameSystem::SetUp_DefaultLight_Paden()
{
	/* Default Light */
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(100.f, 200.f, 100.f, 1.f);
	LightDesc.fRange = 1500.f;
	LightDesc.vDiffuse = _float4(0.5f, 0.3f, 0.2f, 1.f);
	LightDesc.vAmbient = _float4(0.15f, 0.15f, 0.15f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 0.7f, 0.7f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameSystem::SetUp_DefaultLight_Hwara()
{
	/* Default Light */
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(100.f, 200.f, 100.f, 1.f);
	LightDesc.fRange = 1500.f;
	LightDesc.vDiffuse = _float4(0.38f, 0.35f, 0.35f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}
