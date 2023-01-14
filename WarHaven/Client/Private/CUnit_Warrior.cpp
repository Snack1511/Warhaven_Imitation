#include "stdafx.h"
#include "CUnit_Warrior.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CUnit_Warrior::CUnit_Warrior()
{
}

CUnit_Warrior::~CUnit_Warrior()
{
}

CUnit_Warrior* CUnit_Warrior::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Warrior* pInstance = new CUnit_Warrior;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Warrior");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Warrior");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Warrior::On_Die()
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

void CUnit_Warrior::SetUp_Colliders(_bool bPlayer)
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

	//SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, 2, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_COM"));
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


	const _uint iWeaponSphereNum = 6;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 40.f;
		tWeaponUnitColDesc[i].eColType = (_uint)eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = (_uint)eGuardBreak;

	SetUp_UnitCollider(CUnit::GUARDBREAK_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = (_uint)eFlyAttack;

	SetUp_UnitCollider(CUnit::FLYATTACK, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));



}

void	CUnit_Warrior::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	/* 나중에 사망 후 부활 할 시 위치 저장할 때 사용해야 하므로 전체적으로 추가하도록 한다. */
	//m_tHitType.m_eDeathState  = STATE_DEAD;


	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT;
		m_tHitType.eGuardState = STATE_GUARDHIT_WARRIOR;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_PLAYER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_WARRIOR;
		m_tHitType.eStingHitState = STATE_STINGHIT_WARRIOR;
		m_tHitType.eFlyState = STATE_FLYHIT_WARRIOR;
		m_tHitType.eBounce = STATE_BOUNCE_PLAYER_L;
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
		m_tUnitStatus.fMaxHP = 100000.f;
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
		break;

		

	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}

		
}

void CUnit_Warrior::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_PLAYER_R;
		m_eSprintEndState = STATE_SPRINT_END_PLAYER;
		m_eSprintFallState = STATE_SPRINT_JUMPFALL_PLAYER;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_WARRIOR_R;
		m_eSprintEndState = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;
		m_eSprintFallState = AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_WARRIOR;

		m_tAIChangeType.eAIPathFindDefaultState = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;
		m_tAIChangeType.eAICommbatDefaultState = AI_STATE_COMBAT_DEFAULT_WARRIOR_R;
		m_tAIChangeType.eAIReviveDefaultState = AI_STATE_COMMON_REVIVE_AI;
		m_tAIChangeType.eAICannonDefaultState = AI_STATE_CANNON_AI;
		m_tAIChangeType.eAIGlidingDefaultState = AI_STATE_GLIDING_AI;
		m_tAIChangeType.eAIPatrolDefaultState = AI_STATE_PATROL_DEFAULT_WARRIOR_R;
		m_tAIChangeType.eAIGoTirrgerDefaultState = AI_STATE_PATHNAVIGATION_SPRINTBEGIN_WARRIOR;
		m_tAIChangeType.eAIChangeDeafultState = AI_STATE_COMMON_CHANGE_HERO;

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

void CUnit_Warrior::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
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
	case eBehaviorType::ePadenCannonInteract:
		//상태변경
		break;
	case eBehaviorType::eCombat:
		//상태변경
		eNewState = AI_STATE_COMBAT_DEFAULT_WARRIOR_L;

		break;
	case eBehaviorType::eGoToTrigger:
		//상태변경
		eNewState = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;
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

void CUnit_Warrior::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
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
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_L:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY:
	case AI_STATE_ATTACK_HORIZONTALMIDDLE_L:
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L:
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_L:
	case STATE_SPRINTATTACK_PLAYER:
	case AI_STATE_COMBAT_GUARDBREAK_WARRIOR:
		Play_Sound(L"Effect_GuardBreak_Warrior", CHANNEL_EFFECTS);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, matWorld);
		break;

	case AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR:
	case STATE_WARRIOR_OXEN_LOOPATTACK:
	case STATE_ATTACK_HORIZONTALUP_R:
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_R:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY:
	case AI_STATE_ATTACK_HORIZONTALMIDDLE_R:
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R:
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", vHitPos, matWorld);
			break;

	case STATE_ATTACK_HORIZONTALDOWN_R:
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_VERTICALCUT:
		Play_Sound(L"Effect_Swing_Warrior", CHANNEL_EFFECTS);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_D", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_PLAYER_L:
		Play_Sound(L"Effect_Sting_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	case STATE_ATTACK_STING_PLAYER_R:
		Play_Sound(L"Effect_Sting_Warrior", CHANNEL_EFFECTS, 1.f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, matWorld);
		break;

	default:
		break;

	}
}

HRESULT CUnit_Warrior::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/warrior/SKEL_Warrior_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_ETC.fbx");



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

	m_fCoolTime[SKILL1] = 6.f;
	m_fCoolTime[SKILL2] = 5.f;
	m_fCoolTime[SKILL3] = 0.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; 
	m_fCoolAcc[SKILL3] = 0.f;


	m_tUnitStatus.eClass = WARRIOR;



	return S_OK;
}

HRESULT CUnit_Warrior::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CUnit_Warrior::Start()
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

void CUnit_Warrior::OnEnable()
{
	__super::OnEnable();

	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Qanda_Sniping", this, ZERO_VECTOR);
}

void CUnit_Warrior::OnDisable()
{
	__super::OnDisable();
}

void CUnit_Warrior::My_LateTick()
{
	__super::My_LateTick();

	if (m_eCurState >= STATE_IDLE_WARRIOR_R_AI_ENEMY)
	{		
		//if (CUser::Get_Instance()->Get_CurLevel() == LEVEL_TEST)
		{
			if (KEY(J, TAP))
			{
				Enter_State(AI_STATE_COMMON_CHANGE_HERO, nullptr);
			}
		}

		return;
	}
		

	if (KEY(CTRL, HOLD))
	{
		if (KEY(NUM8, TAP))
		{
			GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(0.f, 0.f, 0.f));
			m_pTransform->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
		}
		if (KEY(NUM9, TAP))
		{
			DISABLE_COMPONENT(GET_COMPONENT(CPhysXCharacter));
		}
	}

		//GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(50.f, 50.f, 50.f));


}
