#include "stdafx.h"
#include "CUnit_Paladin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CUnit_Paladin::CUnit_Paladin()
{
}

CUnit_Paladin::~CUnit_Paladin()
{
}

CUnit_Paladin* CUnit_Paladin::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Paladin* pInstance = new CUnit_Paladin;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Paladin");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Paladin");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Paladin::On_Die()
{
	__super::On_Die();
	_float4 vPos = Get_Transform()->Get_World(WORLD_POS);

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MATRIX_IDENTITY);

	_float4x4 matWeapon = m_pModelCom->Find_HierarchyNode("0B_R_WP1")->Get_BoneMatrix();
	_float4 vBonePos = matWeapon.XMLoad().r[3];
	ZeroMemory(&matWeapon.m[3], sizeof(_float4));


	CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeadBody_Warrior", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
	vPos.y += 1.f;
	CEffects_Factory::Get_Instance()->Create_MeshParticle(L"WarriorDead_Weapon", vBonePos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);

}

void CUnit_Paladin::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_BLUEHITBOX_BODY : COL_REDHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_BLUEHITBOX_HEAD : COL_REDHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_BLUEGUARD : COL_REDGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_BLUEATTACK : COL_REDATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_BLUEGUARDBREAK : COL_REDGUARDBREAK;
	COL_GROUP_CLIENT	eFlyAttack = (bPlayer) ? COL_BLUEFLYATTACK : COL_REDFLYATTACK;
	COL_GROUP_CLIENT	eFlyGuardBreakAttack = (bPlayer) ? COL_BLUEFLYATTACKGUARDBREAK : COL_REDFLYATTACKGUARDBREAK;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxBody },
		{0.7f, _float4(0.f, 1.2f, 0.f),(_uint)eHitBoxBody },
	};

	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.9f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxGuard },
		{0.9f, _float4(0.f, 1.2f, 0.f),(_uint)eHitBoxGuard },
	};

	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 8;


	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -10.f * _float(i);
		tWeaponUnitColDesc[i].eColType = (_uint)eAttack;
	}

	tWeaponUnitColDesc[iWeaponSphereNum - 1].fRadius = 0.4f;

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = (_uint)eGuardBreak;


	const _uint iShieldSphereNum = 25;

	_uint iDivide = 5;

	iDivide = iShieldSphereNum / iDivide;

	CUnit::UNIT_COLLIDERDESC tShieldUnitColDesc[iShieldSphereNum];

	for (_uint i = 0; i < iDivide; ++i)
	{
		_float fOffSetY = -10.f;

		fOffSetY += (5.f * i);


		for (_uint j = 0; j < iDivide; ++j)
		{
			_float fOffSetX = -10.f;

			fOffSetX += (5.f * j);

			_int iIndex = i * (iDivide) + j;

			tShieldUnitColDesc[iIndex].fRadius = 0.4f;
			tShieldUnitColDesc[iIndex].vOffsetPos.x = fOffSetX * (j + 1);
			tShieldUnitColDesc[iIndex].vOffsetPos.z = fOffSetY * (i + 1);
			tShieldUnitColDesc[iIndex].eColType = (_uint)eFlyGuardBreakAttack;
		}

	}

	// ���� ���� ��ų
	SetUp_UnitCollider(CUnit::GROGGY, tShieldUnitColDesc, iShieldSphereNum, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1"));

	for (_uint i = 0; i < iShieldSphereNum; ++i)
		tShieldUnitColDesc[i].eColType = (_uint)eFlyAttack;

	// �� ��ų
	SetUp_UnitCollider(CUnit::FLYATTACK, tShieldUnitColDesc, iShieldSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1"));

	

	tUnitColDesc[0].fRadius = 1.3f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.f, 1.3f, 0.f);
	tUnitColDesc[0].eColType = eGuardBreak;

	// ���� ��� ��ų
	SetUp_UnitCollider(CUnit::GUARDBREAK_R, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, false);

}

void	CUnit_Paladin::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_PALADIN;
		m_tHitType.eGuardState = STATE_GUARDHIT_PALADIN;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_PALADIN;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_PALADIN;
		m_tHitType.eStingHitState = STATE_STINGHIT_PALADIN;
		m_tHitType.eFlyState = STATE_FLYHIT_PALADIN;
		m_tHitType.eBounce = STATE_BOUNCE_PALADIN_L;
		break;

	
	case Client::CUnit::UNIT_TYPE::eAI_Default:

		break;

		

		break;

	default:
		break;
	}
		
}

void CUnit_Paladin::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : ���� ��

	//������ ��� �������� ���;���

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	switch (m_eCurState)
	{
	case STATE_SPRINTATTACK_PALADIN:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_D", vHitPos, matWorld);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_Left", vHitPos, matWorld);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_Right", vHitPos, matWorld);
		break;

	case STATE_ATTACK_VERTICALCUT_PALADIN:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_D", vHitPos, matWorld);
		break;

	default:

		break;

	}
}

HRESULT CUnit_Paladin::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/paladin/SKEL_Paladin_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/paladin/SKEL_Paladin_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/paladin/SKEL_Paladin_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/paladin/SKEL_Paladin_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/paladin/SKEL_Paladin_ETC.fbx");



	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	Add_Component(pAnimator);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// Į ����
	tDesc.fHeight = 0.6f;
	// Į �β�
	tDesc.fRadius = 0.2f;
	// Į ���� ��
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//Į ������(����)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	m_fCoolTime[SKILL1] = 3.f;
	m_fCoolTime[SKILL2] = 10.f;
	m_fCoolTime[SKILL3] = 10.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; 
	m_fCoolAcc[SKILL3] = 0.f;


	m_tUnitStatus.eClass = PALADIN;
	m_tUnitStatus.fDashAttackSpeed *= 0.9f;
	m_tUnitStatus.fSprintAttackSpeed *= 0.9f;
	m_tUnitStatus.fSprintJumpSpeed *= 0.8f;
	m_tUnitStatus.fSprintSpeed *= 0.75f;
	m_tUnitStatus.fRunSpeed *= 0.75f;
	m_tUnitStatus.fWalkSpeed *= 0.8f;
	m_tUnitStatus.fRunBeginSpeed *= 0.8f;
	m_tUnitStatus.fJumpPower *= 0.9f;
	m_tUnitStatus.fGuardDashSpeed *= 0.8f;


	return S_OK;
}

HRESULT CUnit_Paladin::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	//Set_ShaderNoSpec(L"SK_Engineer1002_Helm_50");

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CUnit_Paladin::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	SetUp_TrailEffect(
		_float4(0.f, 0.f, -168.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -171.f, 1.f),	//Weapon High
		_float4(0.f, -1.5f, -169.5f, 1.f), //Left
		_float4(0.f, 1.5f, -169.5f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow
		_float4(1.f, 0.1f, 0.1f, 0.25f), //vColor
		0.f,
		L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds",
		L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds",
		10,
		"0B_R_WP1"
	);

	_float4x4 matTrans = XMMatrixRotationZ(XMConvertToRadians(270.f));
	m_pModelCom->Set_TransformMatrix(MODEL_PART_WEAPON_L, matTrans);

	return S_OK;
}

void CUnit_Paladin::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Paladin::OnDisable()
{
	__super::OnDisable();
}

void CUnit_Paladin::My_LateTick()
{
	__super::My_LateTick();

	if (m_eCurState >= STATE_IDLE_WARRIOR_R_AI_ENEMY)
		return;

	if (KEY(NUM8, TAP))
		GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(0.f, 2.f, 0.f));
		//GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(50.f, 50.f, 50.f));

	/*if (KEY(SPACE, TAP))
	{
		m_pPhysics->Set_Jump(7.f);
	}*/
}
