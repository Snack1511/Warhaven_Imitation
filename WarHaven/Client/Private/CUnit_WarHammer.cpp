#include "stdafx.h"
#include "CUnit_WarHammer.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CBarricade.h"
#include "Camera.h"

CUnit_WarHammer::CUnit_WarHammer()
{
}

CUnit_WarHammer::~CUnit_WarHammer()
{
}

CUnit_WarHammer* CUnit_WarHammer::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_WarHammer* pInstance = new CUnit_WarHammer;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_WarHammer");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_WarHammer");
		return nullptr;
	}

	return pInstance;
}

HRESULT CUnit_WarHammer::Initialize_Prototype()
{
	__super::Initialize_Prototype();



	CUnit::UNIT_MODEL_DATA  tModelData;

	//1. �ٸ�����Ʈ
//	tModelData.strModelPaths[MODEL_PART_SKEL] = L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx";



	/*for (int i = 0; i < BarricadeCnt_END; ++i)
	{
		CBarricade* pBarricade = CBarricade::Create(tModelData, this);
		if (!pBarricade)
			return E_FAIL;

		CREATE_GAMEOBJECT(pBarricade, GROUP_PLAYER);
		DISABLE_GAMEOBJECT(pBarricade);
	}*/


	//Creaete_Barricade();

	// Create_GameObject �� ��ü�� ������, ��ü�� �߰��� �׷� Enum
	// �ٸ�����Ʈ ����
	// ������ Disable -> ������ü�� ���̱� �ʱ� ���ؼ�
	
	// �ʿ��� �� ���� ������ ������ �ͼ� Ŭ���ϰ� Ŭ���� ��ü�� Create::() ����
	// Create -> ������ü ���� Delete -> ������ü ����
	// Create_Object

	// ���� ���� -> ���纻 2�� ���� 
	// ���� �� ��Ȱ��ȭ �ٸ�����Ʈ ���鶧���� Enable Disal

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_HIT.fbx");

	//4. ETCK
	pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_ETC.fbx");


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

	
	m_tUnitStatus.fRunSpeed *= 0.7f;
	m_tUnitStatus.fWalkSpeed *= 0.95f;
	m_tUnitStatus.fSprintJumpSpeed *= 0.7f;
	m_tUnitStatus.fSprintAttackSpeed *= 0.7f;
	m_tUnitStatus.fSprintSpeed *= 0.65f;

	return S_OK;
}

HRESULT CUnit_WarHammer::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CUnit_WarHammer::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	return S_OK;
}

void CUnit_WarHammer::OnEnable()
{
	__super::OnEnable();
}

void CUnit_WarHammer::OnDisable()
{
	__super::OnDisable();
}
