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

void CUnit_WarHammer::SetUp_Colliders(_bool bPlayer)
{
	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_PLAYERHITBOX_BODY : COL_ENEMYHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_PLAYERHITBOX_HEAD : COL_ENEMYHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_PLAYERGUARD : COL_ENEMYGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_PLAYERATTACK : COL_ENEMYATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_PLAYERGUARDBREAK : COL_ENEMYGUARDBREAK;
	COL_GROUP_CLIENT	eGroggy = (bPlayer) ? COL_PLAYERGROGGYATTACK : COL_ENEMYGROGGYATTACK;

	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),COL_PLAYERHITBOX_BODY },
		{0.6f, _float4(0.f, 1.f, 0.f),COL_PLAYERHITBOX_BODY },
	};



	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = COL_PLAYERHITBOX_HEAD;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("ob_Head"));

	const _uint iWeaponSphereNum = 6;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		if (i == iWeaponSphereNum - 1)
		{
			tWeaponUnitColDesc[i].fRadius = 0.5f;
			tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
			tWeaponUnitColDesc[i].eColType = eAttack;
		}
		else
		{
			tWeaponUnitColDesc[i].fRadius = 0.2f;
			tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
			tWeaponUnitColDesc[i].eColType = eAttack;
		}

	}

	CUnit::UNIT_COLLIDERDESC tWeapon_RUnitColDesc[3] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.f, -115.f),	COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -80.f),	COL_PLAYERATTACK },
		{0.5f, _float4(0.f, 0.f, -55.f),	COL_PLAYERATTACK }
	};

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeapon_RUnitColDesc, 3, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	tUnitColDesc[0].fRadius = 2.f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eGroggy;

	SetUp_UnitCollider(CUnit::GROGGY, tUnitColDesc, 1);



}

void CUnit_WarHammer::SetUp_HitStates(_bool bPlayer)
{

}

void CUnit_WarHammer::Set_BarricadeMatrix()
{
	
	m_BarricadeMatrix = m_pTransform->Get_WorldMatrix();
}

// push_back �ϸ鼭 Disable �����ִ� �Լ�. -> �ٸ�����Ʈ �ı�
void CUnit_WarHammer::Disable_Barricade(CGameObject* pBarricade)
{
	DISABLE_GAMEOBJECT(pBarricade);
	m_Barricade.push_back(pBarricade);
}

// pop_back �ϸ鼭 Enable �����ִ� �Լ�. -> �ٸ�����Ʈ ��ġ (���� �޾Ƽ� ����)
void CUnit_WarHammer::Create_Barricade()
{
	if (!m_Barricade.empty())
	{
		m_Barricade.back()->Get_Transform()->Set_World(WORLD_RIGHT, m_BarricadeMatrix.XMLoad().r[0]);
		m_Barricade.back()->Get_Transform()->Set_World(WORLD_UP, m_BarricadeMatrix.XMLoad().r[1]);
		m_Barricade.back()->Get_Transform()->Set_World(WORLD_LOOK, m_BarricadeMatrix.XMLoad().r[2]);
		m_Barricade.back()->Get_Transform()->Set_World(WORLD_POS, m_BarricadeMatrix.XMLoad().r[3]);

		ENABLE_GAMEOBJECT(m_Barricade.back());
		m_Barricade.pop_back();
	}
}

HRESULT CUnit_WarHammer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

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

	m_tUnitStatus.eClass = ENGINEER;
	m_tUnitStatus.fDashAttackSpeed *= 0.9f;
	m_tUnitStatus.fSprintAttackSpeed *= 0.9f;
	m_tUnitStatus.fSprintJumpSpeed *= 0.8f;
	m_tUnitStatus.fSprintSpeed *= 0.7f;
	m_tUnitStatus.fRunSpeed *= 0.6f;
	m_tUnitStatus.fWalkSpeed *= 0.7f;
	m_tUnitStatus.fRunBeginSpeed *= 0.8f;
	m_tUnitStatus.fJumpPower *= 0.9f;
	


	m_fCoolTime[SKILL1] = 10.f;
	m_fCoolTime[SKILL2] = 8.f;
	m_fCoolTime[SKILL3] = 0.f;

	//Enable_ModelParts(3, false);
	
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
	Enter_State(m_eCurState);
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
