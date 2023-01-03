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

#include "CAnimWeapon.h"

#include "CProjectile.h"
#include "CDefaultArrow.h"
#include "CPurpleArrow.h"
#include "CSnipeArrow.h"

#include "CCollider_Sphere.h"
#include "CColorController.h"

#include "CUI_Trail.h"

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

	_float4x4 matWeapon = m_pModelCom->Find_HierarchyNode("0B_L_WP1")->Get_BoneMatrix();
	_float4 vBonePos = matWeapon.XMLoad().r[3];
	ZeroMemory(&matWeapon.m[3], sizeof(_float4));


	CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeadBody_Archer", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
	vPos.y += 1.f;
	//CEffects_Factory::Get_Instance()->Create_MeshParticle(L"ArcherDead_Weapon", vBonePos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);

}

void CUnit_Archer::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_BLUEHITBOX_BODY : COL_REDHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_BLUEHITBOX_HEAD : COL_REDHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_BLUEGUARD : COL_REDGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_BLUEATTACK : COL_REDATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_BLUEGUARDBREAK : COL_REDGUARDBREAK;
	COL_GROUP_CLIENT	eFlyAttack = (bPlayer) ? COL_BLUEFLYATTACK : COL_REDFLYATTACK;



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

	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.7f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxGuard },
		{0.7f, _float4(0.f, 1.2f, 0.f),(_uint)eHitBoxGuard },
	};

	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = (_uint)eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 2;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum] = 
	{
		//Radius,	vOffsetPos.		eColType
		{0.4f, _float4(0.f, 5.f, 0.f),	(_uint)eAttack },
		{0.4f, _float4(0.f, 0.f, 0.f),	(_uint)eAttack },
	};


	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	//for (_uint i = 0; i < iWeaponSphereNum; ++i)
	//	tWeaponUnitColDesc[i].eColType = eGuardBreak;

	//SetUp_UnitCollider(CUnit::GUARDBREAK_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));

}

void CUnit_Archer::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_ARCHER;
		m_tHitType.eGuardState = STATE_GUARDHIT_ARCHER;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_ARCHER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_ARCHER;
		m_tHitType.eStingHitState = STATE_STINGHIT_ARCHER;
		m_tHitType.eFlyState = STATE_FLYHIT_ARCHER;
		m_tHitType.eBounce = STATE_BOUNCE_ARCHER;


		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:
		m_tHitType.eHitState = AI_STATE_COMMON_HIT_ARCHER;
		m_tHitType.eGuardState = AI_STATE_COMMON_GUARDHIT_ARCHER;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_ARCHER; // 이거 아직 안함.
		m_tHitType.eGroggyState = AI_STATE_COMMON_GROGGYHIT_ARCHER;
		m_tHitType.eStingHitState = AI_STATE_COMMON_STINGHIT_ARCHER;
		m_tHitType.eFlyState = AI_STATE_COMMON_FLYHIT_ARCHER;
		m_tHitType.eBounce = AI_STATE_COMMON_BOUNCE_ARCHER;
		break;

	default:
		break;
	}
		
}

void CUnit_Archer::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_ARCHER_R;
		m_eSprintEndState = STATE_SPRINT_END_ARCHER;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_ARCHER_R;
		m_eSprintEndState = AI_STATE_PATROL_DEFAULT_ARCHER_R;

		break;



	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}
}

void CUnit_Archer::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	__super::On_ChangeBehavior(pBehaviorDesc);

	if (nullptr == pBehaviorDesc)
		assert(0);

	STATE_TYPE	eNewState = STATE_END;

	switch (pBehaviorDesc->eCurType)
	{
	case eBehaviorType::ePatrol:
		//상태변경
		eNewState = AI_STATE_PATROL_DEFAULT_ARCHER_R;
		break;
	case eBehaviorType::eFollow:
		//상태변경
		break;
	case eBehaviorType::eAttack:
		//상태변경
		eNewState = AI_STATE_COMBAT_DEFAULT_ARCHER_R;

		break;
	case eBehaviorType::ePathNavigation:
		//상태변경
		eNewState = AI_STATE_PATROL_DEFAULT_ARCHER_R;
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

	case STATE_ATTACK_SWING_ARCHER:
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

void CUnit_Archer::Enable_Arrow(_bool bEnable)
{
	if (bEnable)
		ENABLE_GAMEOBJECT(m_pCurArrow);
	else
		DISABLE_GAMEOBJECT(m_pCurArrow);
}

void CUnit_Archer::Enable_Trail(_bool bEnable)
{
	if (!m_pUI_Trail)
		return;

	if (bEnable)
		ENABLE_GAMEOBJECT(m_pUI_Trail);
	else
		DISABLE_GAMEOBJECT(m_pUI_Trail);
}

void CUnit_Archer::ReMap_Trail(_float4 vTargetPos)
{
	if (!m_pUI_Trail)
		return;

	m_pUI_Trail->Clear_Nodes();

	_float4 vHandPos = m_pModelCom->Get_BoneMatrix("0B_R_WP1").XMLoad().r[3];
	m_pUI_Trail->Add_Node(vHandPos);
	m_pUI_Trail->Add_Node(vTargetPos);
	m_pUI_Trail->ReMap_TrailBuffers();

}

void CUnit_Archer::Set_ColorController(_uint iMeshPartType)
{
	if (!m_pCurArrow)
		return;

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (140.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = iMeshPartType;

	GET_COMPONENT_FROM(m_pCurArrow, CColorController)->Add_ColorControll(tColorDesc);
}

void CUnit_Archer::Create_DefaultArrow()
{
	if (m_pCurArrow) 
		DISABLE_GAMEOBJECT(m_pCurArrow);

	CGameObject* pGameObject = nullptr;

	if (m_mapProjectilePool[HASHCODE(CDefaultArrow)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CDefaultArrow));
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CProjectile*>(pGameObject)->Reset(this);
	}
	else
	{
		CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CDefaultArrow)].front();
		pEffect->Reset(this);
		m_mapProjectilePool[HASHCODE(CDefaultArrow)].pop_front();
		pGameObject = pEffect;
	}

	m_pCurArrow = static_cast<CProjectile*>(pGameObject);
}

void CUnit_Archer::Create_PurpleArrow()
{
	if (m_pCurArrow)
		DISABLE_GAMEOBJECT(m_pCurArrow);

	CGameObject* pGameObject = nullptr;



	if (m_mapProjectilePool[HASHCODE(CPurpleArrow)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CPurpleArrow));
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CProjectile*>(pGameObject)->Reset(this);
	}
	else
	{
		CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CPurpleArrow)].front();
		pEffect->Reset(this);
		m_mapProjectilePool[HASHCODE(CPurpleArrow)].pop_front();
		pGameObject = pEffect;
	}

	m_pCurArrow = static_cast<CProjectile*>(pGameObject);
}

void CUnit_Archer::Create_SnipeArrow()
{
	if (m_pCurArrow)
		DISABLE_GAMEOBJECT(m_pCurArrow);

	CGameObject* pGameObject = nullptr;

	if (m_mapProjectilePool[HASHCODE(CSnipeArrow)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CSnipeArrow));
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CProjectile*>(pGameObject)->Reset(this);
	}
	else
	{
		CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CSnipeArrow)].front();
		pEffect->Reset(this);
		m_mapProjectilePool[HASHCODE(CSnipeArrow)].pop_front();
		pGameObject = pEffect;
	}

	m_pCurArrow = static_cast<CProjectile*>(pGameObject);
}



void CUnit_Archer::Change_ArrowPhase(_uint iPhase)
{
	if (!m_pCurArrow)
		return;

	m_pCurArrow->On_ChangePhase(CProjectile::ePROJECTILE_PHASE(iPhase));
}

void CUnit_Archer::Shoot_Arrow()
{
	if (!m_pCurArrow)
		return;

	m_pCurArrow->On_ShootProjectile();
	m_pCurArrow = nullptr;
}

void CUnit_Archer::Collect_Arrow(_hashcode _hcCode, CProjectile* pEffect)
{
	m_mapProjectilePool[_hcCode].push_back(pEffect);
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


	m_fCoolTime[SKILL1] = 3.f;
	m_fCoolTime[SKILL2] = 5.f;
	m_fCoolTime[SKILL3] = 3.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; 
	m_fCoolAcc[SKILL3] = 0.f;


	m_tUnitStatus.eClass = ARCHER;

	m_pAnimWeapon = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/longbow/SK_WP_LongBow0005_A00_30.fbx",
		L"../bin/resources/meshes/weapons/longbow/LongBow_Anim2.fbx", this, "0B_L_WP1");

	if (!m_pAnimWeapon)
		return E_FAIL;

	m_pAnimWeapon->Initialize();


	
	


	return S_OK;
}

HRESULT CUnit_Archer::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	/* UI_TRAIL */
	if (m_pOwnerPlayer->IsMainPlayer())
	{
		CUI_Trail* pUI_Trail = CUI_Trail::Create(CP_BEFORE_RENDERER, 2, 0.1f, -0.1f, 10.f, ZERO_VECTOR, _float4(1.f, 1.f, 1.f, 1.f),
			L"../bin/resources/textures/effects/warhaven/texture/T_ArrowUI_01_FX.dds",
			L"../bin/resources/textures/White.png"
		);

		if (!pUI_Trail)
			return E_FAIL;

		m_pUI_Trail = pUI_Trail;
	}

	CREATE_GAMEOBJECT(m_pAnimWeapon, GROUP_PLAYER);
	DISABLE_GAMEOBJECT(m_pAnimWeapon);

	return S_OK;
}

HRESULT CUnit_Archer::Start()
{
	__super::Start();

	ENABLE_GAMEOBJECT(m_pAnimWeapon);

	if (m_pUI_Trail)
	{
		CREATE_GAMEOBJECT(m_pUI_Trail, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pUI_Trail);
	}
	


	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	SetUp_TrailEffect(
		_float4(0.f, 0.f, -10.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -11.f, 1.f),	//Weapon High
		_float4(0.f, -1.5f, -10.f, 1.f), //Left
		_float4(0.f, 1.5f, -10.f, 1.f), //Right
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
	Create_DefaultArrow();

}

void CUnit_Archer::OnDisable()
{
	__super::OnDisable();
	if (m_pCurArrow)
		DISABLE_GAMEOBJECT(m_pCurArrow);

	if (m_pUI_Trail)
		DISABLE_GAMEOBJECT(m_pUI_Trail);
}

void CUnit_Archer::My_Tick()
{
	__super::My_Tick();
}

void CUnit_Archer::My_LateTick()
{
	__super::My_LateTick();
}
