#include "stdafx.h"
#include "CUnit_Warrior.h"

#include "GameInstance.h"

#include "Model.h"
#include "CAnimator.h"

CUnit_Warrior::CUnit_Warrior()
{
}

CUnit_Warrior::~CUnit_Warrior()
{
}

CUnit_Warrior* CUnit_Warrior::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Warrior* pInstance = new CUnit_Warrior;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Warrior");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Warrior");
		return nullptr;
	}

	return pInstance;
}

HRESULT CUnit_Warrior::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//추가적으로 Animator 만들어야댐.

	//0. Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/warrior/Warrior_Test_Idle.fbx");
	if (!pAnimator)
		return E_FAIL;
	//1. Attack
	//pAnimator->Add_Animations(L"../bin/resources/animations/warrior/Warrior_Test_Attack.fbx");

	Add_Component(pAnimator);

	return S_OK;
}

HRESULT CUnit_Warrior::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CUnit_Warrior::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	Enter_State(STATE_IDLE_PLAYER);
	return S_OK;
}

void CUnit_Warrior::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Warrior::OnDisable()
{
	__super::OnDisable();
}
