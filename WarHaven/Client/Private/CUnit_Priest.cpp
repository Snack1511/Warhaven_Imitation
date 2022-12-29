#include "stdafx.h"
#include "CUnit_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CAnimWeapon.h"
#include "CProjectile.h"

CUnit_Priest::CUnit_Priest()
{
}

CUnit_Priest::~CUnit_Priest()
{
}

CUnit_Priest* CUnit_Priest::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Priest* pInstance = new CUnit_Priest;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Priest");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Priest");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Priest::On_Die()
{
	__super::On_Die();
	_float4 vPos = Get_Transform()->Get_World(WORLD_POS);

	//_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MATRIX_IDENTITY);
	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	_float4x4 matWeapon = m_pModelCom->Find_HierarchyNode("0B_R_WP1")->Get_BoneMatrix();
	_float4 vBonePos = matWeapon.XMLoad().r[3];
	ZeroMemory(&matWeapon.m[3], sizeof(_float4));


	Add_DeathStones(CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle_Death(L"DeadBody_Warrior", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld));

	m_DeathStones.push_back(CEffects_Factory::Get_Instance()->Create_MeshParticle_Death(L"WarriorDead_Weapon", vBonePos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld));

}

void CUnit_Priest::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_BLUEHITBOX_BODY : COL_REDHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_BLUEHITBOX_HEAD : COL_REDHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_BLUEGUARD : COL_REDGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_BLUEATTACK : COL_REDATTACK;
	COL_GROUP_CLIENT	eCure = (bPlayer) ? COL_BLUECURE : COL_REDCURE;
	COL_GROUP_CLIENT	eCatch = (bPlayer) ? COL_BLUEPROJECTILECATCH  : COL_REDPROJECTILECATCH;
	COL_GROUP_CLIENT	eGuardBreakFlyAttack = (bPlayer) ? COL_BLUEFLYATTACKGUARDBREAK : COL_REDFLYATTACKGUARDBREAK;



	//CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	//{
	//	//Radius,	vOffsetPos.		eColType
	//	{0.6f, _float4(0.f, 0.5f, 0.f),eHitBoxBody },
	//	{0.6f, _float4(0.f, 1.f, 0.f),eHitBoxBody },
	//};

	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.f, 0.f), (_uint)eHitBoxBody },
	};

	//SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_COM"));
	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.7f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxGuard },
		{0.7f, _float4(0.f, 1.2f, 0.f),(_uint)eHitBoxGuard },

	};

//	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = (_uint)eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 4;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
		tWeaponUnitColDesc[i].eColType = (_uint)eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	//tUnitColDesc[0].fRadius = 2.5f;
	//tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.f, 0.f, 0.f);
	//tUnitColDesc[0].eColType = (_uint)eCatch;


	//SetUp_UnitCollider(CUnit::WEAPON_L, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true);

	tUnitColDesc[0].fRadius = 1.5f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.f, tUnitColDesc[0].fRadius * tUnitColDesc[0].fRadius, 0.f);
	tUnitColDesc[0].eColType = (_uint)eGuardBreakFlyAttack;

	SetUp_UnitCollider(CUnit::FLYATTACK, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1"));

	tUnitColDesc[0].fRadius = 1.f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 0.f, 1.f, 0.f);
	tUnitColDesc[0].eColType = (_uint)eCatch;
	
	SetUp_UnitCollider(CUnit::GUARD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1"));

}

void CUnit_Priest::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	/* 나중에 사망 후 부활 할 시 위치 저장할 때 사용해야 하므로 전체적으로 추가하도록 한다. */
	//m_tHitType.m_eDeathState  = STATE_DEAD;


	switch (eUnitType)
	{

	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_PRIEST;
		m_tHitType.eGuardState = STATE_GUARDHIT_PRIEST;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_PLAYER; // 가드 캔슬 없음
		m_tHitType.eGroggyState = STATE_GROGGYHIT_PRIEST;
		m_tHitType.eStingHitState = STATE_STINGHIT_PRIEST;
		m_tHitType.eFlyState = STATE_FLYHIT_PRIEST;
		m_tHitType.eBounce = STATE_BOUNCE_PRIEST;
		break;

	case Client::CUnit::UNIT_TYPE::eAI_TG:
		m_tHitType.eHitState = AI_STATE_TG_HIT_WARRIOR;
		m_tHitType.eGuardState = AI_STATE_TG_GUARDHIT_WARRIOR;
		m_tHitType.eGuardBreakState = AI_STATE_GUARD_CANCEL_WARRIOR;
		m_tHitType.eStingHitState = AI_STATE_TG_STINGHIT_WARRIOR;
		m_tHitType.eGroggyState = AI_STATE_TG_GROGGYHIT_WARRIOR;
		m_tHitType.eFlyState = AI_STATE_TG_FLYHIT_WARRIOR;
		m_tHitType.eBounce = AI_STATE_BOUNE_WARRIOR_L;
		break;

	case Client::CUnit::UNIT_TYPE::eSandbag:
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
		m_tHitType.eGuardState = STATE_GUARDHIT_ENEMY;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY;
		m_tHitType.eStingHitState = STATE_STINGHIT_ENEMY;
		m_tHitType.eGroggyState = STATE_GROGGY_ENEMY;
		m_tHitType.eFlyState = STATE_FLYHIT_ENEMY;
		m_tHitType.eBounce = STATE_BOUNCE_WARRIOR_L_AI_ENEMY;
		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:
		m_tHitType.eHitState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eGuardState = AI_STATE_COMMON_GUARDHIT_WARRIOR;
		m_tHitType.eGuardBreakState = AI_STATE_COMBAT_GUARDCANCEL_WARRIOR;
		m_tHitType.eStingHitState = AI_STATE_COMMON_STINGHIT_WARRIOR;
		m_tHitType.eGroggyState = AI_STATE_COMMON_GROGGYHIT_WARRIOR;
		m_tHitType.eFlyState = AI_STATE_COMMON_FLYHIT_WARRIOR;
		m_tHitType.eBounce = AI_STATE_COMMON_BOUNCE_WARRIOR_L;
		break;


	case Client::CUnit::UNIT_TYPE::eAI_idiot:
		m_tHitType.eHitState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eGuardState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eGuardBreakState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eStingHitState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eGroggyState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eFlyState = AI_STATE_COMMON_HIT_WARRIOR;
		m_tHitType.eBounce = AI_STATE_COMMON_HIT_WARRIOR;
		/*m_tUnitStatus.fMaxHP = 100000.f;
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;*/
		break;

		

	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}

		
}

void CUnit_Priest::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_PRIEST;
		m_eSprintEndState = STATE_SPRINT_END_PRIEST;
		m_eCureBeginType = STATE_CURE_BEGIN_PRIEST;
		m_eCureLoopType = STATE_CURE_LOOP_PRIEST;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_WARRIOR_R;
		m_eSprintEndState = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_idiot:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_WARRIOR_R;
		m_eSprintEndState = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;

		break;



	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}
}

void CUnit_Priest::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	__super::On_ChangeBehavior(pBehaviorDesc);

	if (nullptr == pBehaviorDesc)
		assert(0);

	STATE_TYPE	eNewState = STATE_END;

	switch (pBehaviorDesc->eCurType)
	{
	case eBehaviorType::ePatrol:
		//상태변경
		eNewState = AI_STATE_PATROL_DEFAULT_WARRIOR_R;
		break;
	case eBehaviorType::eFollow:
		//상태변경
		break;
	case eBehaviorType::eAttack:
		//상태변경
		eNewState = AI_STATE_COMBAT_DEFAULT_WARRIOR_L;

		break;
	case eBehaviorType::ePathNavigation:
		//상태변경
		eNewState = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;
		break;

	case eBehaviorType::eResurrect:
		//상태변경
		eNewState = AI_STATE_COMMON_REVIVE_AI;
		break;

	case eBehaviorType::eChange:
		//상태변경
		eNewState = AI_STATE_COMMON_CHANGE_HERO;
		break;
	default:
		assert(0);
		break;
	}

	if (eNewState != STATE_END)
		m_eReserveState = eNewState;
		
}

void CUnit_Priest::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : 맞은 쪽

	//때리는 사람 기준으로 나와야함

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	switch (m_eCurState)
	{

	case STATE_ATTACK_STING_PRIEST:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	default:
		break;

	}
}

HRESULT CUnit_Priest::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Priest/SKEL_Priest_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Priest/A_LobbyIdle_Priest_01.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Priest/SKEL_Priest_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Priest/SKEL_Priest_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/Priest/SKEL_Priest_ETC.fbx");



	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	Add_Component(pAnimator);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// 칼 길이
	tDesc.fHeight = 0.6f;
	// 칼 두께
	tDesc.fRadius = 0.2f;
	// 칼 붙일 뼈
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//칼 오프셋(로컬)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	m_fCoolTime[SKILL1] = 6.f;
	m_fCoolTime[SKILL2] = 5.f;
	m_fCoolTime[SKILL3] = 5.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; 
	m_fCoolAcc[SKILL3] = 0.f;


	m_tUnitStatus.eClass = PRIEST;

	m_tUnitStatus.fDashAttackSpeed = 9.f;
	m_fMaxDistance = 5.f;

	//m_pAnimWeapon = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/Staff/SK_WP_Staff0004.fbx",
	//	L"../bin/resources/meshes/weapons/Crow/Crow_Anim.fbx", this, "0B_R_WP1");

	//m_pAnimWeapon->Initialize();
	//m_pAnimWeapon->Set_AnimIndex(0, FLT_MAX, FLT_MIN);

	return S_OK;
}

HRESULT CUnit_Priest::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CUnit_Priest::Start()
{
	__super::Start();

	SetUp_TrailEffect(
		_float4(0.f, 0.f, -165.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -171.f, 1.f),	//Weapon High
		_float4(0.f, -2.f, -169.5f, 1.f), //Left
		_float4(0.f, 2.f, -169.5f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow
		_float4(1.f, 0.1f, 0.1f, 0.25f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_04.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		20,
		"0B_R_WP1"
	);
	

	/*Distortion Test*/
	SetUp_DistortionTrailEffect(
		_float4(0.f, 0.f, -130.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -171.f, 1.f),	//Weapon High
		_float4(0.f, -1.5f, -169.5f, 1.f), //Left
		_float4(0.f, 1.5f, -169.5f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow
		_float4(1.f, 0.1f, 0.1f, 0.25f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_09_M.dds",
		L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_09_M.dds",
		20,
		"0B_R_WP1"
	);

	return S_OK;
}

void CUnit_Priest::OnEnable()
{
	
	__super::OnEnable();
}

void CUnit_Priest::OnDisable()
{
	__super::OnDisable();
}

void CUnit_Priest::My_Tick()
{
	__super::My_Tick();

	if (m_eCureBeginType == m_eCurState || m_eCureLoopType == m_eCurState)
		__super::Check_NearObject_IsInFrustum();

	else
		m_pNearCureObject = nullptr;

}
void CUnit_Priest::My_LateTick()
{

	__super::My_LateTick();


	if (m_eCurState >= STATE_IDLE_WARRIOR_R_AI_ENEMY)
		return;

	if (KEY(NUM8, TAP))
	{
		GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(0.f, 0.f, 0.f));
		m_pTransform->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
	}
		//GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(50.f, 50.f, 50.f));

	/*if (KEY(SPACE, TAP))
	{
		m_pPhysics->Set_Jump(7.f);
	}*/
}
