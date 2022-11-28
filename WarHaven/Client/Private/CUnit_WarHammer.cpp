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
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_PLAYERHITBOX_BODY : COL_ENEMYHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_PLAYERHITBOX_HEAD : COL_ENEMYHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_PLAYERGUARD : COL_ENEMYGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_PLAYERATTACK : COL_ENEMYATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_PLAYERGUARDBREAK : COL_ENEMYGUARDBREAK;
	COL_GROUP_CLIENT	eGroggy = (bPlayer) ? COL_PLAYERGROGGYATTACK : COL_ENEMYGROGGYATTACK;
	COL_GROUP_CLIENT	eFlyAttack = (bPlayer) ? COL_PLAYERFLYATTACKGUARDBREAK : COL_ENEMYFLYATTACKGUARDBREAK;

	CUnit::UNIT_COLLIDREINFODESC tUnitInfoDesc;


	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),	eHitBoxBody },
		{0.6f, _float4(0.f, 1.f, 0.f),	eHitBoxBody },
	};



	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));

	const _uint iWeaponSphereNum = 5;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum - 1; ++i)
	{
		if (i == iWeaponSphereNum - 1)
		{
			tWeaponUnitColDesc[i].fRadius = 0.35f;
			tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
			tWeaponUnitColDesc[i].eColType = eAttack;

			tWeaponUnitColDesc[i + 1].fRadius = tWeaponUnitColDesc[i].fRadius;
			tWeaponUnitColDesc[i + 1].vOffsetPos = tWeaponUnitColDesc[i].vOffsetPos;
			tWeaponUnitColDesc[i + 1].eColType = tWeaponUnitColDesc[i].eColType;

			tWeaponUnitColDesc[i].vOffsetPos.x -= 10.f;
			tWeaponUnitColDesc[i].vOffsetPos.x += 20.f;

		}
		else
		{
			tWeaponUnitColDesc[i].fRadius = 0.2f;
			tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
			tWeaponUnitColDesc[i].eColType = eAttack;
		}
	}




	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, 4, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.7f, _float4(0.f, 0.5f, 0.f), eHitBoxGuard },
		{0.7f, _float4(0.f, 1.2f, 0.f), eHitBoxGuard },
	};

	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	CUnit::UNIT_COLLIDERDESC tGroggyColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),	eGroggy },
		{0.6f, _float4(0.f, 1.f, 0.f),	eGroggy },
	};

	SetUp_UnitCollider(CUnit::GROGGY, tGroggyColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tUnitColDesc[0].fRadius = 2.f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eFlyAttack;

	SetUp_UnitCollider(CUnit::FLYATTACK, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, false);
}

void CUnit_WarHammer::SetUp_HitStates(_bool bPlayer)
{
	
	if (!bPlayer)
	{
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
		m_tHitType.eGuardState = STATE_GUARDHIT_ENEMY;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY;
		m_tHitType.eStingHitState = STATE_STINGHIT_ENEMY;
		m_tHitType.eGroggyState = STATE_GROGGY_ENEMY;
		m_tHitType.eFlyState = STATE_FLYHIT_ENEMY;
		m_tHitType.eBounce = STATE_BOUNCE_WARHAMMER_L;

	}
	else
	{
		m_tHitType.eHitState = STATE_HIT_WARHAMMER;
		m_tHitType.eGuardState = STATE_GUARDHIT_WARHAMMER;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARHAMMER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_WARHAMMER;
		m_tHitType.eStingHitState = STATE_STINGHIT_WARHAMMER;
		m_tHitType.eFlyState = STATE_FLYHIT_WARHAMMER;
		m_tHitType.eBounce = STATE_BOUNCE_WARHAMMER_L;
	}
}

void CUnit_WarHammer::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : ���� ��

	//������ ��� �������� ���;���

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	if (STATE_GROGGYATTACK_WARHAMMER != m_eCurState)
	{
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SmashSpark", vHitPos, matWorld);
	}

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"StoneSpark_1"), vHitPos, matWorld);

	switch (m_eCurState)
	{
	case STATE_SPRINTATTACK_WARHAMMER:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_D", vHitPos, matWorld);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_Left", vHitPos, matWorld);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_Right", vHitPos, matWorld);
		break;
	case STATE_VERTICALATTACK_WARHAMMER_L:
	case STATE_VERTICALATTACK_WARHAMMER_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smash_D", vHitPos, matWorld);
		break;
	case STATE_ATTACK_STING_WARHAMMER_L:
	case STATE_ATTACK_STING_WARHAMMER_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Heading", vHitPos, matWorld);
		break;
	case STATE_GROGGYATTACK_WARHAMMER: 
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Heading", vHitPos, matWorld);
		break;
	case STATE_AIRSPIKE_END_WARHAMMER:
		break;
	}

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

	tDesc.fHeight = 1.f;
	// Į �β�
	tDesc.fRadius = 0.2f;
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
	m_tUnitStatus.fGuardDashSpeed *= 0.8f;


	m_fCoolTime[SKILL1] = 5.f;
	m_fCoolTime[SKILL2] = 4.f;
	m_fCoolTime[SKILL3] = 0.f;

	//Enable_ModelParts(3, false);
	

	int iCnt = (int)CUnit_WarHammer::BARRICADE_CNT::BARRICADE_CNT_END;

	/* �ٸ�����Ʈ ���� */
	for (int i = 0; i < iCnt; ++i)
	{
		CBarricade* pBarricade = CBarricade::Create(this, L"../Bin/Resources/Meshes/Map/Environments/Install/SM_Prop_Installation_Trap01a.fbx");


		Disable_Barricade(pBarricade);
		CREATE_GAMEOBJECT(pBarricade, GROUP_DEFAULT);
		DISABLE_GAMEOBJECT(pBarricade);
		
	}

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
