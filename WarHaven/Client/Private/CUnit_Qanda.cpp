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

#include "CAnimWeapon_Crow.h"

#include "CCollider_Sphere.h"
#include "CColorController.h"

#include "CUI_Trail.h"
#include "CRectEffects.h"
#include "CEffect.h"

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
	m_pOwnerPlayer->On_FinishHero();
	m_pOwnerPlayer->Get_CurrentUnit()->On_Die();
	m_bDie = false;
	m_fDeadTimeAcc = 0.f;
	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;

	DISABLE_GAMEOBJECT(this);

	/*CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeadBody_QANDA", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
	vPos.y += 1.f;*/
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
		m_eSprintFallState = STATE_SPRINT_JUMPFALL_QANDA;


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
}

void CUnit_Qanda::Set_CrowAnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pAnimCrow->Set_AnimIndex(iAnimIndex, fInterpolateTime, fAnimSpeed);
}

void CUnit_Qanda::On_ChangePhase(_uint eCurPhase)
{
	m_pAnimCrow->On_ChangePhase(CAnimWeapon_Crow::ePhyxState(eCurPhase));
}

void CUnit_Qanda::Shoot_AnimCrow()
{
	_float4 vShootPos = m_pAnimCrow->Get_Transform()->Get_World(WORLD_POS);
	_float4 vShootDir = Get_FollowCamLook();

	m_pAnimCrow->Shoot_Crow(vShootPos, vShootDir);
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

void CUnit_Qanda::ReMap_Trail(_float4 vTargetPos)
{
	if (!m_pUI_Trail)
		return;

	m_pUI_Trail->Clear_Nodes();

	_float4 vHandPos = m_pAnimCrow->Get_Transform()->Get_World(WORLD_POS);//m_pModelCom->Get_BoneMatrix("0B_R_WP1").XMLoad().r[3];
	vHandPos.y += 0.05f;
	m_pUI_Trail->Add_Node(vHandPos);
	m_pUI_Trail->Add_Node(vTargetPos);
	m_pUI_Trail->ReMap_TrailBuffers();

}

CGameObject* CUnit_Qanda::Create_Meteor()
{
	/*if (m_pMeteor)
		DISABLE_GAMEOBJECT(m_pMeteor);*/

	CGameObject* pGameObject = nullptr;

	if (m_mapProjectilePool[HASHCODE(CQandaMeteor)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CQandaMeteor));
		//������ ���� �������
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CProjectile*>(pGameObject)->Reset(this);
		static_cast<CProjectile*>(pGameObject)->On_ChangePhase(CProjectile::eRANDOM);
	}
	else
	{
		CProjectile* pEffect = m_mapProjectilePool[HASHCODE(CQandaMeteor)].front();
		pEffect->Reset(this);
		pEffect->On_ChangePhase(CProjectile::eRANDOM);
		m_mapProjectilePool[HASHCODE(CQandaMeteor)].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;

//	m_pMeteor = static_cast<CProjectile*>(pGameObject);
}


void CUnit_Qanda::Turn_TransformParticle(_bool bOnOff)
{
	if (bOnOff)
	{
		if (m_TransformParticles.empty())
			m_TransformParticles = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Transform_Particle", this, ZERO_VECTOR);
	}
	else
	{
		if (!m_TransformParticles.empty())
		{
			for (auto& elem : m_TransformParticles)
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}

		m_TransformParticles.clear();
	}
}

void CUnit_Qanda::Turn_ChargeEffect(_bool bOnOff)
{
	if (bOnOff)
	{
		if (m_ChargeEffect.empty())
			m_ChargeEffect = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Charge_Test", m_pAnimCrow, ZERO_VECTOR);

		if (!m_pChargeParticle)
			m_pChargeParticle = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Charge_Particle_0"),
				m_pAnimCrow, ZERO_VECTOR);
	}
	else
	{
		if (!m_ChargeEffect.empty())
		{
			for (auto& elem : m_ChargeEffect)
				static_cast<CEffect*>(elem)->Set_FadeOut();
			m_ChargeEffect.clear();
		}
		if (m_pChargeParticle)
		{
			static_cast<CRectEffects*>(m_pChargeParticle)->Set_AllFadeOut();
			m_pChargeParticle = nullptr;
		}
	}
}

void CUnit_Qanda::Turn_FeatherEffect(_bool bOnOff)
{
	if (bOnOff)
	{
		if (!m_pFeathers)
			m_pFeathers = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Crow_Feathers_0"),
			m_pAnimCrow, ZERO_VECTOR);
	}
	else
	{
		if (m_pFeathers)
		{
			static_cast<CRectEffects*>(m_pFeathers)->Set_AllFadeOut(0.5f);
			m_pFeathers = nullptr;
		}
	}
	
}

void CUnit_Qanda::Turn_SteamEffect(_bool bOnOff)
{
	if (bOnOff)
	{
		if (m_SteamEffect.empty())
		{
			m_SteamEffect = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Crow_Steam", m_pAnimCrow, ZERO_VECTOR);
			//Create_Light(m_SteamEffect.back(), ZERO_VECTOR, 2.f, 0.f, 0.1f, 9999.f, 0.1f, RGB(255, 0, 0), false);
		}
	}
	else
	{
		if (!m_SteamEffect.empty())
		{
			for (auto& elem : m_SteamEffect)
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut(0.5f);
		}

		m_SteamEffect.clear();
	}
}

void CUnit_Qanda::TurnOff_AllEffect()
{
	Turn_TransformParticle(false);
	Turn_ChargeEffect(false);
	Turn_FeatherEffect(false);
	Turn_SteamEffect(false);
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

	_float3 vRadian = _float3(90.f, 180.f, 180.f);



	m_pAnimWeapon = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/Cane/Cane_60.fbx",
		L"", this, "0B_R_WP1", vRadian.x, vRadian.y, vRadian.z);

	vRadian = _float3(90.f, 180.f, 180.f);


	if (!m_pAnimWeapon)
		return E_FAIL;

	m_pAnimWeapon->Initialize();

	m_pAnimCrow = CAnimWeapon_Crow::Create(L"../bin/resources/meshes/weapons/Crow/Crow_40.fbx",
		L"../bin/resources/meshes/weapons/Crow/Crow_Anim.fbx", this, "0B_Head", vRadian.x, vRadian.y, vRadian.z);
	if (!m_pAnimCrow)
		return E_FAIL;

	m_pAnimCrow->Initialize();

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
	
	CREATE_GAMEOBJECT(m_pAnimCrow, GROUP_PLAYER);
	DISABLE_GAMEOBJECT(m_pAnimCrow);

	m_bForUseTeam = false;

	return S_OK;
}

HRESULT CUnit_Qanda::Start()
{
	__super::Start();

	if (m_pAnimWeapon)
		ENABLE_GAMEOBJECT(m_pAnimWeapon);

	if (m_pAnimCrow)
		ENABLE_GAMEOBJECT(m_pAnimCrow);

	if (m_pUI_Trail)
	{
		CREATE_GAMEOBJECT(m_pUI_Trail, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pUI_Trail);
	}
	

	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	m_TransformParticles.clear();

	return S_OK;
}

void CUnit_Qanda::OnEnable()
{
	__super::OnEnable();

	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;

	Turn_TransformParticle(true);
	
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos.y += 0.5f;

	Create_Light(vPos, 5.f, 0.f, 0.f, 0.f, 1.5f, RGB(255, 140, 40),
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn,
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);


	if (m_pAnimCrow)
	{
		ENABLE_GAMEOBJECT(m_pAnimCrow);
		Turn_FeatherEffect(true);
	}
	//
}

void CUnit_Qanda::OnDisable()
{
	__super::OnDisable();

	Turn_TransformParticle(false);
	Turn_ChargeEffect(false);
	Turn_SteamEffect(false);

	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos.y += 0.5f;
	Create_Light(vPos, 4.f, 0.f, 0.f, 0.f, 0.3f, RGB(255, 255, 255),
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn,
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);


	if (m_pAnimCrow)
	{
		DISABLE_GAMEOBJECT(m_pAnimCrow);
		Turn_FeatherEffect(false);
	}

	if (m_pUI_Trail)
		DISABLE_GAMEOBJECT(m_pUI_Trail);
}

void CUnit_Qanda::My_Tick()
{
	if (m_eCurState == STATE_ATTACK_BEGIN_SNIPING_QANDA ||
		m_eCurState == STATE_ATTACK_AIMING_SNIPING_QANDA)
	{

		__super::Check_MultipleObject_IsInFrustum();

	}
	else
	{

		if(!m_MultipleFrustumObject.empty())
			m_MultipleFrustumObject.clear();

	}

	__super::My_Tick();
}

void CUnit_Qanda::My_LateTick()
{
	__super::My_LateTick();
}
