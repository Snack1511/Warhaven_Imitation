#include "stdafx.h"
#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CPositionTable.h"

#include "CTrigger_BootCamp.h"

#include "CPlayer.h"

#include "CUI_Cursor.h"
#include "CUI_Training.h"

#include "CDestructible.h"

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

	return S_OK;
}

HRESULT CGameSystem::Tick()
{
	return S_OK;
}

void CGameSystem::Release()
{
	SAFE_DELETE(m_pPositionTable);
}

HRESULT CGameSystem::On_ReadyTest(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    _float4 vPlayerPos = _float4(10.f, 3.f, 10.f);

    CPlayer* pUserPlayer = nullptr;

    if (!(pUserPlayer = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
        STATE_JUMPFALL_PLAYER_R, true, L"PlayerCam")))
        return E_FAIL;

    CUser::Get_Instance()->Set_Player(pUserPlayer);
    pUserPlayer->Set_MainPlayer();
    READY_GAMEOBJECT(pUserPlayer, GROUP_PLAYER);


    for (_uint i = 0; i < 1; ++i)
    {
        vPlayerPos.z += 3.f;
        vPlayerPos.x += 1.f;
        CPlayer* pEnemy = nullptr;

        wstring wstrCamName = L"FollowCam_";
        wstrCamName += to_wstring(i);

        if (!(pEnemy = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
            AI_STATE_IDLE_WARRIOR_R, false, wstrCamName)))
            return E_FAIL;

        pEnemy->Set_TargetUnit(pUserPlayer->Get_CurrentUnit());

        vecReadyObjects.push_back(make_pair(pEnemy, GROUP_ENEMY));


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

    if (!(pUserPlayer = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
        STATE_JUMPFALL_PLAYER_R, true, L"PlayerCam")))
        return E_FAIL;

    CUser::Get_Instance()->Set_Player(pUserPlayer);
    pUserPlayer->Set_MainPlayer();
    pUserPlayer->Set_TeamType(CPlayer::ePLAYERTEAM);

    vecReadyObjects.push_back(make_pair(pUserPlayer, GROUP_PLAYER));

    _float4 vEnemyPos;
    CPlayer* pEnemyUser = nullptr;
    
    for (_uint i = 0; i < 8; ++i)
    {
        STATE_TYPE eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
        string strKey;

        switch (i)
        {
        case 0:
            strKey = "EnemyTrio_1";
            break;
        case 1:
            strKey = "EnemyTrio_2";
            break;
        case 2:
            strKey = "q";
            break;
        case 3:
            strKey = "EnemyHall";
            break;
        case 4:
            strKey = "EnemyBlock";
            eEnemyState = STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;
            break;
        case 5:
            strKey = "EnemyHeadShot";
            eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
            break;
        case 6:
            strKey = "EnemyGuardBreak";
            eEnemyState = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;

            break;

        case 7:
            strKey = "EnemyFinal";
            eEnemyState = AI_STATE_IDLE_WARRIOR_L;

            break;

        default:
            break;
        }

      

        vEnemyPos = CGameSystem::Get_Instance()->Find_Position(strKey);


        if (!(pEnemyUser = SetUp_Player(vEnemyPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
            eEnemyState, false, CFunctor::To_Wstring(strKey))))
            return E_FAIL;

        pEnemyUser->Set_TargetUnit(pUserPlayer->Get_CurrentUnit());
        pEnemyUser->Set_TeamType(CPlayer::eENEMYTEAM);
        if (strKey == "EnemyFinal")
            pEnemyUser->Set_TeamType((CPlayer::ePLAYERTEAM | CPlayer::eSQUADMEMBER));

        vecReadyObjects.push_back(make_pair(pEnemyUser, GROUP_ENEMY));
        m_mapEnemyPlayers.emplace(strKey, pEnemyUser);
    }
   






    return S_OK;

}

HRESULT CGameSystem::On_ReadyUIs_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
 

    return S_OK;
}

HRESULT CGameSystem::On_ReadyTriggers_BootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    _float fRadius = 4.f;

    CTrigger_BootCamp* pBootCampTrigger0 = CTrigger_BootCamp::Create("StartPosition", 0, fRadius);
    CTrigger_BootCamp* pBootCampTrigger1 = CTrigger_BootCamp::Create("Popup01", 1, fRadius);
    CTrigger_BootCamp* pBootCampTrigger2 = CTrigger_BootCamp::Create("PopUp02", 2, fRadius);

    CTrigger_BootCamp* pBootCampTrigger3 = CTrigger_BootCamp::Create("Popup03", 3, fRadius);
    CTrigger_BootCamp* pBootCampTrigger4 = CTrigger_BootCamp::Create("Popup04", 4, fRadius);

    CTrigger_BootCamp* pBootCampTrigger5 = CTrigger_BootCamp::Create("Popup05", 5, fRadius);
    pBootCampTrigger5->Add_AdjPlayer(m_mapEnemyPlayers["EnemyTrio_1"]);
    pBootCampTrigger5->Add_AdjPlayer(m_mapEnemyPlayers["EnemyTrio_2"]);
    pBootCampTrigger5->Add_AdjPlayer(m_mapEnemyPlayers["q"]);

    CTrigger_BootCamp* pBootCampTrigger6 = CTrigger_BootCamp::Create("Popup06", 6, fRadius);
    pBootCampTrigger6->Add_AdjPlayer(m_mapEnemyPlayers["EnemyHall"]);

    CTrigger_BootCamp* pBootCampTrigger7 = CTrigger_BootCamp::Create("Popup07", 7, fRadius);
    pBootCampTrigger7->Add_AdjPlayer(m_mapEnemyPlayers["EnemyBlock"]);

    CTrigger_BootCamp* pBootCampTrigger8 = CTrigger_BootCamp::Create("Popup08", 8, fRadius);
    pBootCampTrigger8->Add_AdjPlayer(m_mapEnemyPlayers["EnemyHeadShot"]);

    CTrigger_BootCamp* pBootCampTrigger9 = CTrigger_BootCamp::Create("EnemyGuardBreak", 9, fRadius);
    pBootCampTrigger9->Add_AdjPlayer(m_mapEnemyPlayers["EnemyGuardBreak"]);

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
    pLastTrigger->Add_AdjPlayer(m_mapEnemyPlayers["EnemyFinal"]);


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
    m_mapEnemyPlayers.clear();

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

CPlayer* CGameSystem::SetUp_Player(_float4 vStartPos, _uint iClassType, STATE_TYPE eStartState, _bool bUserPlayer, wstring wstrCamName)
{
    CPlayer* pPlayerInstance = CPlayer::Create(wstrCamName, (CPlayer::CLASS_DEFAULT)(iClassType));

    if (nullptr == pPlayerInstance)
        return nullptr;

    pPlayerInstance->Reserve_State(eStartState);
    pPlayerInstance->SetUp_UnitColliders(bUserPlayer);
    pPlayerInstance->SetUp_UnitHitStates(bUserPlayer);
    pPlayerInstance->Set_Postion(vStartPos);
    pPlayerInstance->Get_CurrentUnit()->Synchronize_CamPos();

    return pPlayerInstance;
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
