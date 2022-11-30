#include "stdafx.h"
#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CPositionTable.h"

#include "CTrigger_BootCamp.h"

#include "CPlayer.h"

#include "CUI_Cursor.h"
#include "CUI_Training.h"

#include "CDestructible.h"

#include "CPlayerInfo_Main.h"
#include "CPlayerInfo_SandBack.h"

IMPLEMENT_SINGLETON(CGameSystem);


#define READY_GAMEOBJECT(instance, grouptype) vecReadyObjects.push_back(make_pair(instance, grouptype))

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

	if (FAILED(m_pPositionTable->Load_Position("BootCamp")))
	{
		Call_MsgBox(L"Failed to Load_Position : CGameSystem");
		return E_FAIL;
	}

    if (FAILED(SetUp_AllPlayerInfos()))
    {
        Call_MsgBox(L"Failed to SetUp_AllPlayerInfos : CGameSystem");
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
    SAFE_DELETE(m_pRedTeam);
    SAFE_DELETE(m_pBlueTeam);

    for (auto& elem : m_mapAllPlayers)
    {
        SAFE_DELETE(elem.second);
    }

    m_mapAllPlayers.clear();
}

HRESULT CGameSystem::On_ExitLevel()
{
    for (auto& elem : m_mapAllPlayers)
    {
        elem.second->m_pMyPlayer = nullptr;
    }

    SAFE_DELETE(m_pRedTeam);
    SAFE_DELETE(m_pBlueTeam);

    return S_OK;
}

HRESULT CGameSystem::On_ReadyTest(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    _float4 vPlayerPos = _float4(10.f, 3.f, 10.f);

    CPlayer* pUserPlayer = nullptr;

    pUserPlayer = SetUp_Player(HASHCODE(CPlayerInfo_Main));
    pUserPlayer->Set_TeamType(CPlayer::ePLAYERTEAM);
    pUserPlayer->Set_Postion(vPlayerPos);
    pUserPlayer->Reserve_State(STATE_IDLE_PLAYER_R);
    CUser::Get_Instance()->Set_Player(pUserPlayer);
    READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);

    for (_uint i = 0; i < 0; ++i)
    {
        vPlayerPos.z += 3.f;
        vPlayerPos.x += 1.f;

        CPlayer* pEnemy = nullptr;

        pEnemy = SetUp_Player(Convert_ToHash(L"TestEnemy"));
        pEnemy->Set_TeamType(CPlayer::eENEMYTEAM);
        pEnemy->Set_Postion(vPlayerPos);
        pEnemy->Set_TargetPlayer(pUserPlayer);
        pEnemy->Reserve_State(AI_STATE_IDLE_WARRIOR_L);
        READY_GAMEOBJECT(pEnemy, GROUP_ENEMY);
    }

    SetUp_DefaultLight_BootCamp();

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
    pUserPlayer->Set_TeamType(CPlayer::ePLAYERTEAM);
    pUserPlayer->Set_Postion(vPlayerPos);
    pUserPlayer->Reserve_State(STATE_IDLE_PLAYER_R);
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
            break;
        case 1:
            strKey = "EnemyTrio_2";
            wstrInfoKey = L"EnemyTrio_1";
            break;
        case 2:
            strKey = "q";
            wstrInfoKey = L"EnemyTrio_2";
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

        if (!pEnemy)
            assert(0);

        pEnemy->Set_TeamType(CPlayer::eENEMYTEAM);
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
    return S_OK;
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

HRESULT CGameSystem::SetUp_AllPlayerInfos()
{
    /* 모든 플레이어들 정보 다 만들기 */
    CPlayerInfo* pPlayerInfo = nullptr;

#define ADD_PLAYERINFO(classname)   pPlayerInfo = classname::Create();\
    if (!pPlayerInfo)\
        return E_FAIL;\
    m_mapAllPlayers.emplace(HASHCODE(classname), pPlayerInfo)


    ADD_PLAYERINFO(CPlayerInfo_Main);





    /* 훈련장 용 샌드백 Info들 */
#define ADD_SANDBACKINFO(name) pPlayerInfo = CPlayerInfo_SandBack::Create();\
    pPlayerInfo->m_tPlayerInfo.wstrName = name;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName = pPlayerInfo->m_tPlayerInfo.wstrName;\
    pPlayerInfo->m_tPlayerInfo.wstrCamName += L"_Cam";\
    m_mapAllPlayers.emplace(Convert_ToHash(pPlayerInfo->m_tPlayerInfo.wstrName), pPlayerInfo);

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

CPlayer* CGameSystem::SetUp_Player(_hashcode hcName)
{
    auto iter = m_mapAllPlayers.find(hcName);

    if (iter == m_mapAllPlayers.end())
        return nullptr;

    return iter->second->Make_Player();
}

HRESULT CGameSystem::SetUp_DefaultLight_BootCamp()
{
    /* Default Light */
    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(100.f, 200.f, 50.f, 1.f);
    LightDesc.fRange = 1500.f;
    LightDesc.vDiffuse = _float4(0.9f, 0.9f, 0.9f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;

    return S_OK;
}
