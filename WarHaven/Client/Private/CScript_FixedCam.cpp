#include "stdafx.h"
#include "CScript_FixedCam.h"


#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"
#include "CUser.h"

CScript_FixedCam::CScript_FixedCam(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_FixedCam::~CScript_FixedCam()
{
}

CScript_FixedCam* CScript_FixedCam::Create(_uint iGroupIdx)
{
	CScript_FixedCam* pInstance = new CScript_FixedCam(iGroupIdx);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_FixedCam");
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

HRESULT CScript_FixedCam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_FixedCam::Initialize()
{
	return S_OK;
}

void CScript_FixedCam::Tick()
{
	if (KEY(TAB, TAP))
	{
		//ShowCursor(true);
		CUser::Get_Instance()->Set_FixCursor(false);

	}


	//if (KEY(RBUTTON, HOLD))
	//	OnMouseMove();

	Key_Input();
}

void CScript_FixedCam::Late_Tick()
{
}

void CScript_FixedCam::Release()
{
}

void CScript_FixedCam::Regist_KeyInput(KEY eKeyInput, function<void(_float4&)> function)
{
	switch (eKeyInput)
	{
	case KEY::W:
		m_W_InputFunction = function;
		break;
	case KEY::A:
		m_A_InputFunction = function;
		break;
	case KEY::S:
		m_S_InputFunction = function;
		break;
	case KEY::D:
		m_D_InputFunction = function;
		break;
	}
}

void CScript_FixedCam::Key_Input()
{
	CTransform* pTransform = m_pOwner->Get_Transform();

	_float4 vPos = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS);
	_float4 vDir = _float4(0.f, 0.f, 0.f, 0.f);

	_float fSpeed = 5.f;

	if (KEY(W, HOLD))
	{
		m_W_InputFunction(vDir);
		//vDir += pTransform->Get_MyWorld(WORLD_LOOK);
	}
	if (KEY(A, HOLD))
	{
		m_A_InputFunction(vDir);
		//vDir -= pTransform->Get_MyWorld(WORLD_RIGHT);
	}
	if (KEY(S, HOLD))
	{
		m_S_InputFunction(vDir);
		//vDir -= pTransform->Get_MyWorld(WORLD_LOOK);
	}
	if (KEY(D, HOLD))
	{
		m_D_InputFunction(vDir);
		//vDir += pTransform->Get_MyWorld(WORLD_RIGHT);
	}
	//if (KEY(SPACE, HOLD))
	//{
	//	vDir += _float4(0.f, 1.f, 0.f, 0.f);
	//}
	//if (KEY(CTRL, HOLD))
	//{
	//	vDir -= _float4(0.f, 1.f, 0.f, 0.f);
	//}
	if (KEY(LSHIFT, HOLD))
	{
		fSpeed = 40.f;
	}
	if (KEY(Z, HOLD))
	{
		fSpeed = 1.f;
	}

	vPos += vDir * fSpeed * fDT(0);
	vPos.w = 1.f;

	pTransform->Set_World(WORLD_POS, vPos);
}
