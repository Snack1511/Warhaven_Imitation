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
	CTrigger_BootCamp* pBootCampTrigger0 = CTrigger_BootCamp::Create("StartPosition", 0, 3.f);
	CTrigger_BootCamp* pBootCampTrigger1 = CTrigger_BootCamp::Create("Popup01", 1, 3.f);
	CTrigger_BootCamp* pBootCampTrigger2 = CTrigger_BootCamp::Create("PopUp02", 2, 3.f);
	CTrigger_BootCamp* pBootCampTrigger3 = CTrigger_BootCamp::Create("Popup03", 3, 3.f);
	CTrigger_BootCamp* pBootCampTrigger5 = CTrigger_BootCamp::Create("Popup05", 5, 3.f);
	CTrigger_BootCamp* pBootCampTrigger6 = CTrigger_BootCamp::Create("Popup06", 6, 3.f);
	CTrigger_BootCamp* pBootCampTrigger7 = CTrigger_BootCamp::Create("Popup07", 7, 3.f);
	CTrigger_BootCamp* pBootCampTrigger8 = CTrigger_BootCamp::Create("Popup08", 8, 3.f);
	CTrigger_BootCamp* pBootCampTrigger9 = CTrigger_BootCamp::Create("Popup09", 9, 3.f);

	CTrigger_BootCamp* pBootCampTrigger10 = CTrigger_BootCamp::Create("Popup10", 10, 2.f);

	CTrigger_BootCamp* pBootCampTriggerBasicEnd = CTrigger_BootCamp::Create("BasicEnd", 99, 2.f);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger0);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger1);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger2);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger3);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger5);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger6);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger7);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger8);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger9);
	pBootCampTriggerBasicEnd->Add_AdjTriggers(pBootCampTrigger10);

	if (!pBootCampTrigger0)
		return E_FAIL;

	CREATE_GAMEOBJECT(pBootCampTrigger0, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger1, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger2, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger3, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger5, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger6, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger7, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger8, GROUP_TRIGGER);
	CREATE_GAMEOBJECT(pBootCampTrigger9, GROUP_TRIGGER);

	CREATE_GAMEOBJECT(pBootCampTrigger10, GROUP_TRIGGER);
	DISABLE_GAMEOBJECT(pBootCampTrigger10, GROUP_TRIGGER);

	CREATE_GAMEOBJECT(pBootCampTriggerBasicEnd, GROUP_TRIGGER);

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
