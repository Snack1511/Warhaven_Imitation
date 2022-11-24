#include "stdafx.h"
#include "CGameSystem.h"

#include "UsefulHeaders.h"
#include "CPositionTable.h"

#include "CTrigger_BootCamp.h"

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
