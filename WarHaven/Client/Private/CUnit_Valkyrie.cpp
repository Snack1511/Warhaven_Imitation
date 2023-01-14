#include "stdafx.h"
#include "CUnit_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CPlayer.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"
#include "CRectEffects.h"
#include "HIerarchyNode.h"
#include "CState.h"


CUnit_Valkyrie::CUnit_Valkyrie()
{
}

CUnit_Valkyrie::~CUnit_Valkyrie()
{
	m_TransformParticles.clear();
}

CUnit_Valkyrie* CUnit_Valkyrie::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Valkyrie* pInstance = new CUnit_Valkyrie;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Valkyrie");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Valkyrie");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Valkyrie::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_BLUEHITBOX_BODY : COL_REDHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_BLUEHITBOX_HEAD : COL_REDHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_BLUEGUARD : COL_REDGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_BLUEATTACK : COL_REDATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_BLUEGUARDBREAK : COL_REDGUARDBREAK;
	COL_GROUP_CLIENT	eGroggy = (bPlayer) ? COL_BLUEGROGGYATTACK : COL_REDGROGGYATTACK;

	CUnit::UNIT_COLLIDERDESC tUnitColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.6f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.f, 0.f),(_uint)eHitBoxBody },
	};

	//SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_COM"));
	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2);

	CUnit::UNIT_COLLIDERDESC tGuardColDesc[2] =
	{
		//Radius,	vOffsetPos.		eColType
		{1.1f, _float4(0.f, 0.5f, 0.f),(_uint)eHitBoxGuard },
		{1.1f, _float4(0.f, 1.2f, 0.f),(_uint)eHitBoxGuard },
	};


	SetUp_UnitCollider(CUnit::GUARD, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false);


	tGuardColDesc[0].eColType = eGuardBreak;
	tGuardColDesc[1].eColType = eGuardBreak;

	SetUp_UnitCollider(CUnit::GUARDBREAK_R, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1"));


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = (_uint)eHitBoxHead;

	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 4;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 38.f;
		tWeaponUnitColDesc[i].eColType = (_uint)eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));
	
	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = (_uint)eGroggy;
	

	SetUp_UnitCollider(CUnit::GROGGY, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));

}

void CUnit_Valkyrie::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_VALKYRIE;
		m_tHitType.eGuardState = STATE_GUARDHIT_VALKYRIE;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_VALKYRIE;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_VALKYRIE;
		m_tHitType.eStingHitState = STATE_STINGHIT_VALKYRIE;
		m_tHitType.eFlyState = STATE_FLYHIT_VALKYRIE;
		m_tHitType.eBounce = STATE_BOUNCE_VALKYRIE_L;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_TG:
	case Client::CUnit::UNIT_TYPE::eSandbag:
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
		m_tHitType.eGuardState = STATE_GUARDHIT_ENEMY;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY;
		m_tHitType.eStingHitState = STATE_STINGHIT_ENEMY;
		m_tHitType.eGroggyState = STATE_GROGGY_ENEMY;
		m_tHitType.eFlyState = STATE_FLYHIT_ENEMY;
		m_tHitType.eBounce = STATE_BOUNCE_VALKYRIE_L;
		break;


	case Client::CUnit::UNIT_TYPE::eAI_Default:
		m_tHitType.eHitState = AI_STATE_COMMON_HIT_FIONA;
		m_tHitType.eGuardState = AI_STATE_COMMON_GUARDHIT_FIONA;
		m_tHitType.eGuardBreakState = AI_STATE_COMBAT_GUARDBEGIN_FIONA;
		m_tHitType.eStingHitState = AI_STATE_COMMON_STINGHIT_FIONA;
		m_tHitType.eGroggyState = AI_STATE_COMMON_GROGGYHIT_FIONA;
		m_tHitType.eFlyState = AI_STATE_COMMON_FLYHIT_FIONA;
		m_tHitType.eBounce = AI_STATE_COMMON_BOUNCE_FIONA_L;
		break;

	default:
		break;
	}

}

void CUnit_Valkyrie::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_VALKYRIE_R;
		m_eSprintEndState = STATE_SPRINT_END_VALKYRIE;
		m_eSprintFallState = STATE_SPRINT_JUMPFALL_VALKYRIE;
		
		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_FIONA_R;
		m_eSprintEndState = AI_STATE_PATHNAVIGATION_SPRINTEND_FIONA;
		m_eSprintFallState = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_FIONA;

		break;



	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}
}

void CUnit_Valkyrie::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	__super::On_ChangeBehavior(pBehaviorDesc);

	if (nullptr == pBehaviorDesc)
		assert(0);

	STATE_TYPE	eNewState = STATE_END;

	switch (pBehaviorDesc->eCurType)
	{
	case eBehaviorType::ePatrol:
		//상태변경
		eNewState = AI_STATE_PATROL_DEFAULT_FIONA_R;
		break;
	case eBehaviorType::ePadenCannonInteract:
		//상태변경
		break;
	case eBehaviorType::eCombat:
		//상태변경
		eNewState = AI_STATE_COMBAT_DEFAULT_FIONA_R;

		break;
	case eBehaviorType::eGoToTrigger:
		//상태변경
		eNewState = AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_R;
		break;

	case eBehaviorType::eRevive:
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

void CUnit_Valkyrie::On_Die()
{
	m_pOwnerPlayer->On_FinishHero();
	m_pOwnerPlayer->Get_CurrentUnit()->On_Die();
	m_bDie = false;
	m_fDeadTimeAcc = 0.f;
	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
}

void CUnit_Valkyrie::TurnOn_ValkyrieTrail(_bool bOn)
{
	if (!m_pValkyrieTrail_R)
		return;

	m_pValkyrieTrail_R->TurnOn_TrailEffect(bOn);
	m_pValkyrieTrail_R2->TurnOn_TrailEffect(bOn);
	m_pValkyrieTrail_L->TurnOn_TrailEffect(bOn);
	m_pValkyrieTrail_L2->TurnOn_TrailEffect(bOn);


	m_pValkyrieLowerTrail_R->TurnOn_TrailEffect(bOn);
	m_pValkyrieLowerTrail_R2->TurnOn_TrailEffect(bOn);
	m_pValkyrieLowerTrail_L->TurnOn_TrailEffect(bOn);
	m_pValkyrieLowerTrail_L2->TurnOn_TrailEffect(bOn);
	
}

void CUnit_Valkyrie::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	__super::Effect_Hit(pOtherUnit, vHitPos);

	/*_float fUnitDist = pUnit->Get_Transform()->Get_World(WORLD_POS)
	_float fHitDist = m_pTransform->Get_World(WORLD_POS)*/

	//pOtherUnit : 맞은 쪽

	//때리는 사람 기준으로 나와야함

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	switch (m_eCurState)
	{
	case STATE_ATTACK_HORIZONTALUP_VALKYRIE_L:
	case AI_STATE_COMBAT_HORIZONTALUP_FIONA_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L:
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L:
	case STATE_SPINATTACK_VALKYRIE:
	case AI_STATE_COMBAT_SPINATTACK_FIONA:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L:
	case AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_L:
	case STATE_SPRINTATTACK_VALKYRIE:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALUP_VALKYRIE_R:
	case AI_STATE_COMBAT_HORIZONTALUP_FIONA_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R:
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R:
	case AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_VERTICALCUT_VALKYRIE:
	case AI_STATE_COMBAT_VERTICALCUT_FIONA:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_D", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_VALKYRIE_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_VALKYRIE_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	default:
		break;

	}
}

void CUnit_Valkyrie::SetUp_ValkyrieTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pValkyrieTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pValkyrieTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);
	
	if (!m_pValkyrieTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pValkyrieTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pValkyrieTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieTrail_R2, CMesh))->Set_NoCurve();

	m_pValkyrieTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pValkyrieTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pValkyrieTrail_R->TurnOn_TrailEffect(false);
	m_pValkyrieTrail_R2->TurnOn_TrailEffect(false);

}

void CUnit_Valkyrie::SetUp_ValkyrieTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pValkyrieTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pValkyrieTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pValkyrieTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pValkyrieTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pValkyrieTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieTrail_L2, CMesh))->Set_NoCurve();

	m_pValkyrieTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pValkyrieTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pValkyrieTrail_L->TurnOn_TrailEffect(false);
	m_pValkyrieTrail_L2->TurnOn_TrailEffect(false);
}

void CUnit_Valkyrie::SetUp_ValkyrieLowerTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pValkyrieLowerTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pValkyrieLowerTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pValkyrieLowerTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pValkyrieLowerTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieLowerTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pValkyrieLowerTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieLowerTrail_R2, CMesh))->Set_NoCurve();

	m_pValkyrieLowerTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pValkyrieLowerTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pValkyrieLowerTrail_R->TurnOn_TrailEffect(false);
	m_pValkyrieLowerTrail_R2->TurnOn_TrailEffect(false);
}

void CUnit_Valkyrie::SetUp_ValkyrieLowerTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pValkyrieLowerTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pValkyrieLowerTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pValkyrieLowerTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pValkyrieLowerTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieLowerTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pValkyrieLowerTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pValkyrieLowerTrail_L2, CMesh))->Set_NoCurve();

	m_pValkyrieLowerTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pValkyrieLowerTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pValkyrieLowerTrail_L->TurnOn_TrailEffect(false);
	m_pValkyrieLowerTrail_L2->TurnOn_TrailEffect(false);
}

HRESULT CUnit_Valkyrie::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Valkyrie/SKEL_Fiona_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Valkyrie/SKEL_Fiona_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Valkyrie/SKEL_Fiona_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Valkyrie/SKEL_Fiona_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/Valkyrie/SKEL_Fiona_ETC.fbx");


	Add_Component(pAnimator);

	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);


	CBoneCollider::BONECOLLIDERDESC tDesc;

	// 칼 길이
	tDesc.fHeight = 0.7f;
	// 칼 두께
	tDesc.fRadius = 0.15f;
	// 칼 붙일 뼈
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//칼 오프셋(로컬)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	//// 칼 길이
	//tDesc.fHeight = 0.1f;
	//// 칼 두께
	//tDesc.fRadius = 0.2f;
	//// 칼 붙일 뼈
	//tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1");

	////칼 오프셋(로컬)
	//tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	//m_pWeaponCollider_L = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	//Add_Component(m_pWeaponCollider_L);

	m_tUnitStatus.fSprintAttackSpeed = 13.f;
	m_tUnitStatus.fDashAttackSpeed = 4.f;

	m_tUnitStatus.eClass = FIONA;
	m_tUnitStatus.fHP = 500.f;
	m_tUnitStatus.fMaxHP = 500.f;

	m_tUnitStatus.fRunSpeed *= 0.8f;


	m_fCoolTime[SKILL1] = 45.f; //45
	m_fCoolTime[SKILL2] = 0.f; // 화신력 소모 스킬
	m_fCoolTime[SKILL3] = 5.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; // 화신력 소모 스킬
	m_fCoolAcc[SKILL3] = 0.f;

	return S_OK;
}

HRESULT CUnit_Valkyrie::Initialize()
{
	__super::Initialize();
	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	Set_ShaderNoSpec(L"SK_Flona_Helm_50");

	return S_OK;
}

HRESULT CUnit_Valkyrie::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	SetUp_TrailEffect(
		_float4(0.f, 0.f, -115.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -120.f, 1.f),	//Weapon High
		_float4(-5.f, 0.f, -117.5f, 1.f), //Left
		_float4(5.f, 0.f, -117.5f, 1.f), //Right
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
		_float4(0.f, 0.f, -60.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -120.f, 1.f),	//Weapon High
		_float4(-5.f, 0.f, -120.f, 1.f), //Left
		_float4(5.f, 0.f, -120.f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow 
		_float4(1.f, 0.1f, 0.1f, 0.25f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_09_M.dds",
		L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_09_M.dds",
		10,
		"0B_R_WP1"
	);

	


	//upper
	m_vTrailShader = SH_EFFECT_NONE;
	wstring strMask = L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_04.dds";
	_float fAlpha = 0.5f;
	_float fUpperSize = 10.f;

	SetUp_ValkyrieTrail_R(
		_float4(0.f, 0.f, fUpperSize, 1.f),	//Weapon R
		_float4(-0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		_float4(1.f, 0.8f, 0.5f, fAlpha), //vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		4,
		"0B_R_ShoulderP3_01"
	);

	SetUp_ValkyrieTrail_L(
		_float4(0.f, 0.f, -fUpperSize, 1.f),	//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		_float4(1.f, 0.8f, 0.5f, fAlpha), //vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		6,
		"0B_L_ShoulderP3_01"
	);
	//lower
	fUpperSize *= 0.5f;

	SetUp_ValkyrieLowerTrail_R(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		8,
		"0B_R_Chain02"
	);
	SetUp_ValkyrieLowerTrail_L(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		12,
		"0B_L_Chain02"
	);

	m_pModelCom->Set_RimLightFlag(RGB(50, 30, 0));



	/* 모델 파츠중 하나 따로 회전시킨느거 */
	_float4x4 matTrans = XMMatrixRotationZ(XMConvertToRadians(90.0f));
	m_pModelCom->Set_TransformMatrix(MODEL_PART_WEAPON, matTrans);

	return S_OK;
}

void CUnit_Valkyrie::OnEnable()
{
	TurnOn_ValkyrieTrail(true);
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	
	m_TransformParticles.clear();

	m_TransformParticles = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Transform_Particle", this, vPos);

	vPos.y += 0.5f;

	Create_Light(vPos, 5.f, 0.f, 0.f, 0.f, 1.5f, RGB(255, 140, 40), 
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn, 
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);

	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;

	__super::OnEnable();
}

void CUnit_Valkyrie::OnDisable()
{
	TurnOn_ValkyrieTrail(false);

	if (!m_TransformParticles.empty())
	{
		for(auto& elem : m_TransformParticles)
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();

		m_TransformParticles.clear();
	}


	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos.y += 0.5f;
	Create_Light(vPos, 4.f, 0.f, 0.f, 0.f, 0.3f, RGB(255, 255, 255), 
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn, 
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);
	
	__super::OnDisable();
}

void CUnit_Valkyrie::My_Tick()
{
	__super::My_Tick();
}

void CUnit_Valkyrie::My_LateTick()
{
	__super::My_LateTick();

}

void CUnit_Valkyrie::On_FinishGame(_bool bWin)
{
	/* 변신 해제 */
	m_pOwnerPlayer->On_FinishHero();
	m_pOwnerPlayer->Get_CurrentUnit()->On_FinishGame(bWin);
}


