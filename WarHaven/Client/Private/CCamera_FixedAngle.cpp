#include "stdafx.h"
#include "CCamera_FixedAngle.h"
#include "GameInstance.h"
#include "CScript_FixedCam.h"
#include "Transform.h"
CCamera_FixedAngle::CCamera_FixedAngle()
{
}

CCamera_FixedAngle::~CCamera_FixedAngle()
{
}

CCamera_FixedAngle* CCamera_FixedAngle::Create(_float4 LookatDir, _float4 AbsUpDir)
{
	CCamera_FixedAngle* pInstance = new CCamera_FixedAngle;

	if (FAILED(pInstance->SetUp_Components()))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CCamera_FixedAngle");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_LookAt(LookatDir, AbsUpDir)))
	{
		Call_MsgBox(L"Failed to SetUp_LookAt : CCamera_FixedAngle");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCamera_FixedAngle");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCamera_FixedAngle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCamera_FixedAngle::Initialize()
{
	return GET_COMPONENT(CScript_FixedCam)->Initialize();
}

HRESULT CCamera_FixedAngle::Start()
{
	//m_pNaviCom = GET_COMPONENT(CNavigation);
	//if (!m_pNaviCom)
		//return E_FAIL;

	if (!m_pTransformCom)
		return E_FAIL;

	if (!m_pScriptableCom)
		return E_FAIL;

	

	m_pScriptableCom->Set_Owner(this);
	m_pScriptableCom->Regist_KeyInput(KEY::W, bind(&CCamera_FixedAngle::Input_KB_W, this, placeholders::_1));
	m_pScriptableCom->Regist_KeyInput(KEY::A, bind(&CCamera_FixedAngle::Input_KB_A, this, placeholders::_1));
	m_pScriptableCom->Regist_KeyInput(KEY::S, bind(&CCamera_FixedAngle::Input_KB_S, this, placeholders::_1));
	m_pScriptableCom->Regist_KeyInput(KEY::D, bind(&CCamera_FixedAngle::Input_KB_D, this, placeholders::_1));

	return __super::Start();
}

HRESULT CCamera_FixedAngle::SetUp_Components()
{
	Add_Component(CScript_FixedCam::Create(CP_AFTER_TRANSFORM));

	m_pTransformCom = GET_COMPONENT(CTransform);
	m_pScriptableCom = GET_COMPONENT(CScript_FixedCam);
	return S_OK;
}

HRESULT CCamera_FixedAngle::SetUp_LookAt(_float4 LookatDir, _float4 AbsUpDir)
{
	if (nullptr == m_pTransformCom)
		assert(0);
	_float4 Look_Dir = LookatDir;

	_float4 Right, Up, Look;

	XMStoreFloat4(&Right, XMVector3Cross(XMVector3Normalize(AbsUpDir.XMLoad()), XMVector3Normalize(LookatDir.XMLoad())));
	XMStoreFloat4(&Up, XMVector3Cross(XMVector3Normalize(LookatDir.XMLoad()), XMVector3Normalize(Right.XMLoad())));
	XMStoreFloat4(&Look, XMVector3Normalize(LookatDir.XMLoad()));
	

	m_pTransformCom->Set_World(WORLD_RIGHT, Right);
	m_pTransformCom->Set_World(WORLD_UP, Up);
	m_pTransformCom->Set_World(WORLD_LOOK, Look);
	m_pTransformCom->Make_WorldMatrix();
	return S_OK;
}

void CCamera_FixedAngle::My_Tick()
{
}

void CCamera_FixedAngle::Late_Tick()
{
}

void CCamera_FixedAngle::Input_KB_W(_float4& OutDir)
{
	OutDir += m_pTransformCom->Get_MyWorld(WORLD_UP);
}

void CCamera_FixedAngle::Input_KB_A(_float4& OutDir)
{
	OutDir -= m_pTransformCom->Get_MyWorld(WORLD_RIGHT);
}

void CCamera_FixedAngle::Input_KB_S(_float4& OutDir)
{
	OutDir -= m_pTransformCom->Get_MyWorld(WORLD_UP);
}

void CCamera_FixedAngle::Input_KB_D(_float4& OutDir)
{
	OutDir += m_pTransformCom->Get_MyWorld(WORLD_RIGHT);
}
