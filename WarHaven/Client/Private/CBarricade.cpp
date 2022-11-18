#include "stdafx.h"
#include "CBarricade.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CUnit_WarHammer.h"

CBarricade::CBarricade()
{
}

CBarricade::~CBarricade()
{
}

CBarricade* CBarricade::Create(CUnit_WarHammer* pOwner)
{
	CBarricade* pInstance = new CBarricade;


	if (FAILED(pInstance->__super::SetUp_Model(L"../Bin/Resources/Meshes/Map/Environments/Module/Fence/SM_Module_Fence_Wood08a.dat")))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CBarricade");
		return nullptr;
	}

	//if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	//{
	//	SAFE_DELETE(pInstance);
	//	Call_MsgBox(L"Failed to SetUp_Model : CBarricade");
	//	return nullptr;
	//}

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}


	

	return pInstance;
}

HRESULT CBarricade::Initialize_Prototype(CUnit_WarHammer* pOwner)
{
	__super::Initialize_Prototype();

	m_pOwner = pOwner;

	// Create_GameObject �� ��ü�� ������, ��ü�� �߰��� �׷� Enum
	// �ٸ�����Ʈ ����
	// ������ Disable -> ������ü�� ���̱� �ʱ� ���ؼ�
	
	// �ʿ��� �� ���� ������ ������ �ͼ� Ŭ���ϰ� Ŭ���� ��ü�� Create::() ����
	// Create -> ������ü ���� Delete -> ������ü ����
	// Create_Object

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	

	//CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_R.fbx");
	//if (!pAnimator)
	//	return E_FAIL;

	//
	////1. L_Base
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_L.fbx");

	////2. Attack
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Attack.fbx");

	////3. hit
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_HIT.fbx");

	////4. ETCK
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_ETC.fbx");


	//Add_Component(pAnimator);

	///* PhysX Collider */

	//CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	//if (!pCController)
	//	return E_FAIL;

	//Add_Component(pCController);

	//CBoneCollider::BONECOLLIDERDESC tDesc;
	//// Į ����

	//tDesc.fHeight = 0.9f;
	//// Į �β�
	//tDesc.fRadius = 0.1f;
	//// Į ���� ��
	//tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	////Į ������(����)
	//tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	return S_OK;
}

HRESULT CBarricade::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CBarricade::Start()
{
	 __super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	return S_OK;
}

void CBarricade::OnEnable()
{
	__super::OnEnable();

	// m_pOwner->Creaete_Barricade();
}

void CBarricade::OnDisable()
{
	__super::OnDisable();

	// m_pOwner->Destroy_Barricade(this);
}
