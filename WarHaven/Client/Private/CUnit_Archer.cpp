#include "stdafx.h"
#include "CUnit_Archer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CUnit_Archer::CUnit_Archer()
{
}

CUnit_Archer::~CUnit_Archer()
{
}

CUnit_Archer* CUnit_Archer::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Archer* pInstance = new CUnit_Archer;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Archer");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Archer");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Archer::On_Die()
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

void CUnit_Archer::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_PLAYERHITBOX_BODY : COL_ENEMYHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_PLAYERHITBOX_HEAD : COL_ENEMYHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_PLAYERGUARD : COL_ENEMYGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_PLAYERATTACK : COL_ENEMYATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_PLAYERGUARDBREAK : COL_ENEMYGUARDBREAK;
	COL_GROUP_CLIENT	eFlyAttack = (bPlayer) ? COL_PLAYERFLYATTACK : COL_ENEMYFLYATTACK;



	//CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	//{
	//	//Radius,	vOffsetPos.		eColType
	//	{0.6f, _float4(0.f, 0.5f, 0.f),eHitBoxBody },
	//	{0.6f, _float4(0.f, 1.f, 0.f),eHitBoxBody },
	//};

	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),eHitBoxBody },
		{0.6f, _float4(0.f, 1.f, 0.f),eHitBoxBody },
	};

	//SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_COM"));
	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.7f, _float4(0.f, 0.5f, 0.f),eHitBoxGuard },
		{0.7f, _float4(0.f, 1.2f, 0.f),eHitBoxGuard },
	};

	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 6;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
		tWeaponUnitColDesc[i].eColType = eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = eGuardBreak;

	SetUp_UnitCollider(CUnit::GUARDBREAK_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = eFlyAttack;

	SetUp_UnitCollider(CUnit::FLYATTACK, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));
	


}

void	CUnit_Archer::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_WARHAMMER;
		m_tHitType.eGuardState = STATE_GUARDHIT_WARHAMMER;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARHAMMER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_WARHAMMER;
		m_tHitType.eStingHitState = STATE_STINGHIT_WARHAMMER;
		m_tHitType.eFlyState = STATE_FLYHIT_WARHAMMER;
		m_tHitType.eBounce = STATE_BOUNCE_WARHAMMER_L;
		break;

	case Client::CUnit::UNIT_TYPE::eAI_TG:
	case Client::CUnit::UNIT_TYPE::eSandbag:
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
		m_tHitType.eGuardState = STATE_GUARDHIT_ENEMY;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY;
		m_tHitType.eStingHitState = STATE_STINGHIT_ENEMY;
		m_tHitType.eGroggyState = STATE_GROGGY_ENEMY;
		m_tHitType.eFlyState = STATE_FLYHIT_ENEMY;
		m_tHitType.eBounce = STATE_BOUNCE_WARHAMMER_L;
		break;

	default:
		break;
	}
		
}

void CUnit_Archer::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : 맞은 쪽

	//때리는 사람 기준으로 나와야함

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	switch (m_eCurState)
	{
	case STATE_ATTACK_HORIZONTALUP_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_L:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY:
	case AI_STATE_ATTACK_HORIZONTALMIDDLE_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_L:
	case STATE_SPRINTATTACK_PLAYER:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, matWorld);
		break;

	case STATE_WARRIOR_OXEN_LOOPATTACK:
	case STATE_ATTACK_HORIZONTALUP_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_R:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY:
	case AI_STATE_ATTACK_HORIZONTALMIDDLE_R:
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", vHitPos, matWorld);
			break;

	case STATE_ATTACK_HORIZONTALDOWN_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_VERTICALCUT:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_D", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_PLAYER_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_PLAYER_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	default:
		break;

	}
}

HRESULT CUnit_Archer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Archer/SKEL_Archer_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Archer/SKEL_Archer_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Archer/SKEL_Archer_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Archer/SKEL_Archer_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/Archer/SKEL_Archer_ETC.fbx");



	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	Add_Component(pAnimator);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// 칼 길이
	tDesc.fHeight = 0.9f;
	// 칼 두께
	tDesc.fRadius = 0.2f;
	// 칼 붙일 뼈
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//칼 오프셋(로컬)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	m_fCoolTime[SKILL1] = 3.f;
	m_fCoolTime[SKILL2] = 5.f;
	m_fCoolTime[SKILL3] = 0.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; 
	m_fCoolAcc[SKILL3] = 0.f;


	m_tUnitStatus.eClass = WARRIOR;



	return S_OK;
}

HRESULT CUnit_Archer::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	for (_uint i = 0; i < MODEL_PART_END; ++i)
	{
		_int iTemp = 0;
		iTemp = m_tModelData.strModelPaths[i].find(L"SK_Warrior_Helmet_Rabbit_50");

		if (iTemp > 0)
			m_pModelCom->Set_ShaderFlag(i, SH_LIGHT_NOSPEC);
	}


	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CUnit_Archer::Start()
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

void CUnit_Archer::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Archer::OnDisable()
{
	__super::OnDisable();
}

void CUnit_Archer::My_LateTick()
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
