#include "stdafx.h"
#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CPositionTable.h"

#include "CTrigger_BootCamp.h"

#include "CPlayer.h"

#include "CUI_Cursor.h"
#include "CUI_Training.h"

IMPLEMENT_SINGLETON(CGameSystem);

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

HRESULT CGameSystem::On_ReadyBootCamp(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    /* Player */
  
    if (FAILED(On_ReadyPlayers(vecReadyObjects)))
    {
        Call_MsgBox(L"Failed to On_ReadyPlayers : CGameSystem");
        return E_FAIL;
    }


    if (FAILED(On_ReadyUIs(vecReadyObjects)))
    {
        Call_MsgBox(L"Failed to On_ReadyPlayers : CGameSystem");
        return E_FAIL;
    }


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

HRESULT CGameSystem::On_ReadyPlayers(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    _float4 vPlayerPos = CGameSystem::Get_Instance()->Find_Position("StartPosition");

    CPlayer* pUserPlayer = nullptr;

    if (!(pUserPlayer = SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
        STATE_JUMPFALL_PLAYER_R, true, L"PlayerCam")))
        return E_FAIL;

    CUser::Get_Instance()->Set_Player(pUserPlayer);
    pUserPlayer->Set_MainPlayer();
    vecReadyObjects.push_back(make_pair(pUserPlayer, GROUP_PLAYER));


    if (!(SetUp_Player(vPlayerPos, (_uint)CPlayer::CLASS_DEFAULT::CLASS_DEFAULT_WARRIOR,
        STATE_IDLE_WARRIOR_R_AI_ENEMY, false, L"SandBackCam1")))
        return E_FAIL;



    return S_OK;

}

HRESULT CGameSystem::On_ReadyUIs(vector<pair<CGameObject*, _uint>>& vecReadyObjects)
{
    CUI_Cursor* pCursor = CUI_Cursor::Create();
    vecReadyObjects.push_back(make_pair(pCursor, GROUP_UI));

    CUI_Training* pUI_Training = CUI_Training::Create();
    vecReadyObjects.push_back(make_pair(pUI_Training, GROUP_UI));

    return E_NOTIMPL;
}

HRESULT CGameSystem::On_EnterBootCamp()
{
   /* CTrigger_BootCamp* pBootCampTrigger = CTrigger_BootCamp::Create("PositionTest_0", 0, 3.f);

    if (!pBootCampTrigger)
        return E_FAIL;

    CREATE_GAMEOBJECT(pBootCampTrigger, GROUP_TRIGGER);*/

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_0"));

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_1"));

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_2"));
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"TrainigRoomSmoke", Find_Position("Smoke_3"));

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Leaf_Particle", _float4(70.f, 0.f, -15.f));


    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = Find_Position("Fire_0");
    LightDesc.fRange = 2.5f;
    LightDesc.fRandomRange = 0.3f;
    LightDesc.vDiffuse = _float4(0.6f, 0.2f, 0.f, 1.f);
    LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 0.5f, 0.5f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;


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
