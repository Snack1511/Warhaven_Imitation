#include "stdafx.h"
#include "CUnit_Warrior.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"

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

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/warrior/SKEL_Warrior_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	
	//CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Spearman/A_AttackStab_Spearman_01.fbx");
	//if (!pAnimator)
	//	return E_FAIL;

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Hit.fbx");

	//4. ETCK
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_ETC.fbx");



	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	Add_Component(pAnimator);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// Į ����
	tDesc.fHeight = 1.3f;
	// Į �β�
	tDesc.fRadius = 0.1f;
	// Į ���� ��
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//Į ������(����)
	tDesc.vOffset = _float4(0.f, 0.f, -80.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);



	

	return S_OK;
}

HRESULT CUnit_Warrior::Initialize()
{
	__super::Initialize();

	
	
	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CUnit_Warrior::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);


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

void CUnit_Warrior::My_LateTick()
{
	
}
