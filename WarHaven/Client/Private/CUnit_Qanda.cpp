#include "stdafx.h"
#include "CUnit_Qanda.h"

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
#include "CQandaCrow.h"
#include "CQandaMeteor.h"

#include "CCollider_Sphere.h"
#include "CColorController.h"

#include "CUI_Trail.h"

#include "CState.h"

CUnit_Qanda::CUnit_Qanda()
{
}

CUnit_Qanda::~CUnit_Qanda()
{
}

CUnit_Qanda* CUnit_Qanda::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Qanda* pInstance = new CUnit_Qanda;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Qanda");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Qanda");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Qanda::On_Die()
{
	__super::On_Die();
	_float4 vPos = Get_Transform()->Get_World(WORLD_POS);

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MATRIX_IDENTITY);

	_float4x4 matWeapon = m_pModelCom->Find_HierarchyNode("0B_L_WP1")->Get_BoneMatrix();
	_float4 vBonePos = matWeapon.XMLoad().r[3];
	ZeroMemory(&matWeapon.m[3], sizeof(_float4));


	CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeadBody_QANDA", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
	vPos.y += 1.f;
	//CEffects_Factory::Get_Instance()->Create_MeshParticle(L"QANDADead_Weapon", vBonePos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);

}

void CUnit_Qanda::SetUp_Colliders(_bool bPlayer)
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

void CUnit_Qanda::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_QANDA;
		m_tHitType.eGuardState = STATE_GUARDHIT_QANDA;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_QANDA;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_QANDA;
		m_tHitType.eStingHitState = STATE_STINGHIT_QANDA;
		m_tHitType.eFlyState = STATE_FLYHIT_QANDA;
		m_tHitType.eBounce = STATE_HIT_QANDA;


		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:
		//m_tHitType.eHitState = AI_STATE_COMMON_HIT_QANDA;
		//m_tHitType.eGuardState = AI_STATE_COMMON_GUARDHIT_QANDA;
		//m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_QANDA; // �̰� ���� ����.
		//m_tHitType.eGroggyState = AI_STATE_COMMON_GROGGYHIT_QANDA;
		//m_tHitType.eStingHitState = AI_STATE_COMMON_STINGHIT_QANDA;
		//m_tHitType.eFlyState = AI_STATE_COMMON_FLYHIT_QANDA;
		//m_tHitType.eBounce = AI_STATE_COMMON_BOUNCE_QANDA;
		break;

	default:
		break;
	}
		
}

void CUnit_Qanda::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_QANDA;
		m_eSprintEndState = STATE_SPRINT_END_QANDA;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		//m_eDefaultState = AI_STATE_COMBAT_DEFAULT_QANDA_R;
		//m_eSprintEndState = AI_STATE_PATROL_DEFAULT_QANDA_R;

		break;



	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}
}

void CUnit_Qanda::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	__super::On_ChangeBehavior(pBehaviorDesc);

	if (nullptr == pBehaviorDesc)
		assert(0);

	STATE_TYPE	eNewState = STATE_END;

	switch (pBehaviorDesc->eCurType)
	{
	case eBehaviorType::ePatrol:
		//���º���
	//	eNewState = AI_STATE_PATROL_DEFAULT_QANDA_R;
		break;
	case eBehaviorType::eFollow:
		//���º���
		break;
	case eBehaviorType::eAttack:
		//���º���
	//	eNewState = AI_STATE_COMBAT_DEFAULT_QANDA_R;

		break;
	case eBehaviorType::ePathNavigation:
		//���º���
	//	eNewState = AI_STATE_PATROL_DEFAULT_QANDA_R;
		break;

	case eBehaviorType::eResurrect:
		//���º���
		eNewState = AI_STATE_COMMON_REVIVE_AI;
		break;

	case eBehaviorType::eChange:
		//���º���
		eNewState = AI_STATE_COMMON_CHANGE_HERO;
		break;
	default:
		assert(0);
		break;
	}

	if (eNewState != STATE_END)
		m_eReserveState = eNewState;
}

void CUnit_Qanda::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : ���� ��

	//������ ��� �������� ���;���

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

void CUnit_Qanda::Enable_Crow(_bool bEnable)
{
	if (bEnable)
		ENABLE_GAMEOBJECT(m_pCrow);
	else
		DISABLE_GAMEOBJECT(m_pCrow);
}

void CUnit_Qanda::Enable_Trail(_bool bEnable)
{
	if (!m_pUI_Trail)
		return;

	if (bEnable)
		ENABLE_GAMEOBJECT(m_pUI_Trail);
	else
		DISABLE_GAMEOBJECT(m_pUI_Trail);
}
void CUnit_Qanda::Enable_AnimWeapon(_bool bEnable)
{
	if (bEnable)
		ENABLE_GAMEOBJECT(m_pAnimWeapon);
	else
		DISABLE_GAMEOBJECT(m_pAnimWeapon);
}


void CUnit_Qanda::ReMap_Trail(_float4 vTargetPos)
{
	if (!m_pUI_Trail)
		return;

	m_pUI_Trail->Clear_Nodes();

	_float4 vHandPos = m_pModelCom->Get_BoneMatrix("0B_R_WP1").XMLoad().r[3];
	m_pUI_Trail->Add_Node(vHandPos);
	m_pUI_Trail->Add_Node(vTargetPos);
	m_pUI_Trail->ReMap_TrailBuffers();

}

void CUnit_Qanda::Set_ColorController(_uint iMeshPartType)
{
	if (!m_pCrow)
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

	GET_COMPONENT_FROM(m_pCrow, CColorController)->Add_ColorControll(tColorDesc);
}

void CUnit_Qanda::Create_Crow()
{
	if (m_pCrow) 
		DISABLE_GAMEOBJECT(m_pCrow);

	CGameObject* pGameObject = nullptr;

	if (m_mapProjectilePool[HASHCODE(CQandaCrow)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CQandaCrow));
		//������ ���� �������
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CProjectile*>(pGameObject)->Reset(this);
	}
	else
	{
		CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CQandaCrow)].front();
		pEffect->Reset(this);
		m_mapProjectilePool[HASHCODE(CQandaCrow)].pop_front();
		pGameObject = pEffect;
	}

	m_pCrow = static_cast<CProjectile*>(pGameObject);
}

void CUnit_Qanda::Create_Meteor()
{
	//if (m_pMeteor)
	//	DISABLE_GAMEOBJECT(m_pMeteor);

	//CGameObject* pGameObject = nullptr;

	//if (m_mapProjectilePool[HASHCODE(CQandaMeteor)].empty())
	//{
	//	pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CQandaMeteor));
	//	//������ ���� �������
	//	pGameObject->Initialize();
	//	CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	//	static_cast<CProjectile*>(pGameObject)->Reset(this);
	//}
	//else
	//{
	//	CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CQandaMeteor)].front();
	//	pEffect->Reset(this);
	//	m_mapProjectilePool[HASHCODE(CQandaMeteor)].pop_front();
	//	pGameObject = pEffect;
	//}

	//m_pMeteor = static_cast<CProjectile*>(pGameObject);
}



void CUnit_Qanda::Change_CrowPhase(_uint iPhase)
{
	if (!m_pCrow)
		return;

	m_pCrow->On_ChangePhase(CProjectile::ePROJECTILE_PHASE(iPhase));
}

void CUnit_Qanda::Shoot_Crow()
{
	if (!m_pCrow)
		return;

	ENABLE_COMPONENT(GET_COMPONENT_FROM(m_pCrow, CModel));
	m_pCrow->On_ShootProjectile();
	m_pCrow->Get_Transform()->Set_World(WORLD_POS, Get_Transform()->Get_World(WORLD_POS));
	m_pCrow = nullptr;
}

void CUnit_Qanda::Collect_QandaProjectile(_hashcode _hcCode, CProjectile* pEffect)
{
	m_mapProjectilePool[_hcCode].push_back(pEffect);
}

HRESULT CUnit_Qanda::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Qanda/SKEL_Qanda_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Qanda/SKEL_Qanda_Attack.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Qanda/SKEL_Qanda_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Qanda/SKEL_Qanda_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/Qanda/SKEL_Qanda_ETC.fbx");



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


	m_tUnitStatus.eClass = QANDA;

	m_pAnimWeapon = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/Crow/SKEL_Crow_A00_15.fbx",
		L"../bin/resources/meshes/weapons/Crow/Crow_Anim.fbx", this, "0B_C_Hat_02");

	_float3 vRadian = _float3(90.f, 180.f, 180.f);

	m_pCane = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/Cane/Cane_50.fbx",
		L"", this, "0B_R_WP1", vRadian.x, vRadian.y, vRadian.z);


	if (!m_pAnimWeapon)
		return E_FAIL;

	if (!m_pCane)
		return E_FAIL;

	m_pAnimWeapon->Initialize();
	m_pCane->Initialize();

	m_tUnitStatus.fRunSpeed *= 0.95f;
	


	return S_OK;
}

HRESULT CUnit_Qanda::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	//Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	/* UI_TRAIL */
	if (m_pOwnerPlayer->IsMainPlayer())
	{
		//CUI_Trail* pUI_Trail = CUI_Trail::Create(CP_BEFORE_RENDERER, 2, 0.1f, -0.1f, 10.f, ZERO_VECTOR, _float4(1.f, 1.f, 1.f, 1.f),
		//	L"../bin/resources/textures/effects/warhaven/texture/T_CrowUI_01_FX.dds",
		//	L"../bin/resources/textures/White.png"
		//);

		//if (!pUI_Trail)
		//	return E_FAIL;

		//m_pUI_Trail = pUI_Trail;
	}

	CREATE_GAMEOBJECT(m_pAnimWeapon, GROUP_PLAYER);
	DISABLE_GAMEOBJECT(m_pAnimWeapon);
	
	CREATE_GAMEOBJECT(m_pCane, GROUP_PLAYER);
	DISABLE_GAMEOBJECT(m_pCane);

	return S_OK;
}

HRESULT CUnit_Qanda::Start()
{
	__super::Start();

	ENABLE_GAMEOBJECT(m_pAnimWeapon);
	ENABLE_GAMEOBJECT(m_pCane);

	//if (m_pUI_Trail)
	//{
	//	CREATE_GAMEOBJECT(m_pUI_Trail, GROUP_EFFECT);
	//	DISABLE_GAMEOBJECT(m_pUI_Trail);
	//}
	

	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);


	return S_OK;
}

void CUnit_Qanda::OnEnable()
{
	__super::OnEnable();
	//Create_DefaultCrow();

}

void CUnit_Qanda::OnDisable()
{
	__super::OnDisable();
	//if (m_pCrow)
	//	DISABLE_GAMEOBJECT(m_pCrow);
}

void CUnit_Qanda::My_Tick()
{
	CState::HIT_INFO tHitInfo;
	ZeroMemory(&tHitInfo, sizeof(CState::HIT_INFO));

	tHitInfo.eHitType = CState::HIT_TYPE::eUP;
	tHitInfo.fKnockBackPower = 1.f;
	tHitInfo.fJumpPower = 3.f;
	tHitInfo.bFly = true;
	tHitInfo.iLandKeyFrame = 60;
	tHitInfo.bNoneHeadAttack = true;

	m_pCurState->Get_HitInfo() = tHitInfo;


	__super::My_Tick();
}

void CUnit_Qanda::My_LateTick()
{
	__super::My_LateTick();
}
