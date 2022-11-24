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


    if (FAILED(m_pPositionTable->Load_Position("PositionTest")))
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
    CTrigger_BootCamp* pBootCampTrigger = CTrigger_BootCamp::Create("PositionTest_0", 0, 3.f);

    if (!pBootCampTrigger)
        return E_FAIL;

    CREATE_GAMEOBJECT(pBootCampTrigger, GROUP_TRIGGER);


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
