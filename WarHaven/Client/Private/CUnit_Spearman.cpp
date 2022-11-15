#include "stdafx.h"
#include "CUnit_Spearman.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

CUnit_Spearman::CUnit_Spearman()
{
}

CUnit_Spearman::~CUnit_Spearman()
{
}

CUnit_Spearman* CUnit_Spearman::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Spearman* pInstance = new CUnit_Spearman;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Spearman");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Spearman");
		return nullptr;
	}

	return pInstance;
}

HRESULT CUnit_Spearman::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Spearman/SKEL_Spearman_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Spearman/SKEL_Spearman_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Spearman/SKEL_Spearman_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Spearman/SKEL_Spearman_HIT.fbx");

	//4. ETCK
	pAnimator->Add_Animations(L"../bin/resources/animations/Spearman/SKEL_Spearman_ETC.fbx");


	Add_Component(pAnimator);

	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);


	CBoneCollider::BONECOLLIDERDESC tDesc;
	// Į ����

	tDesc.fHeight = 0.9f;
	// Į �β�
	tDesc.fRadius = 0.1f;
	// Į ���� ��
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//Į ������(����)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	return S_OK;
}

HRESULT CUnit_Spearman::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CUnit_Spearman::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	Enter_State(m_eCurState);
	return S_OK;
}

void CUnit_Spearman::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Spearman::OnDisable()
{
	__super::OnDisable();
}
