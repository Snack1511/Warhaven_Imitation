#include "stdafx.h"
#include "CUnit_Valkyrie.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CPlayer.h"


CUnit_Valkyrie::CUnit_Valkyrie()
{
}

CUnit_Valkyrie::~CUnit_Valkyrie()
{
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
	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_PLAYERHITBOX_BODY : COL_ENEMYHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_PLAYERHITBOX_HEAD : COL_ENEMYHITBOX_HEAD;
	COL_GROUP_CLIENT	eHitBoxGuard = (bPlayer) ? COL_PLAYERGUARD : COL_ENEMYGUARD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_PLAYERATTACK : COL_ENEMYATTACK;
	COL_GROUP_CLIENT	eGuardBreak = (bPlayer) ? COL_PLAYERGUARDBREAK : COL_ENEMYGUARDBREAK;
	COL_GROUP_CLIENT	eGroggy = (bPlayer) ? COL_PLAYERGROGGYATTACK : COL_ENEMYGROGGYATTACK;

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


	tGuardColDesc[0].eColType = eGuardBreak;
	tGuardColDesc[1].eColType = eGuardBreak;


	SetUp_UnitCollider(CUnit::GUARDBREAK_L, tGuardColDesc, 2, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 1.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = eHitBoxHead;

	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 4;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.2f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -25.f * _float(i) - 38.f;
		tWeaponUnitColDesc[i].eColType = eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));
	
	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = eGroggy;
	

	SetUp_UnitCollider(CUnit::GROGGY, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));

}

void CUnit_Valkyrie::SetUp_HitStates(_bool bPlayer)
{
	if (!bPlayer)
	{
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
		m_tHitType.eGuardState = STATE_GUARDHIT_ENEMY;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY;
		m_tHitType.eStingHitState = STATE_STINGHIT_ENEMY;
		m_tHitType.eGroggyState = STATE_GROGGY_ENEMY;
		m_tHitType.eFlyState = STATE_FLYHIT_ENEMY;
		m_tHitType.eBounce = STATE_BOUNCE_VALKYRIE_L;
	}
	else
	{
		m_tHitType.eHitState = STATE_HIT_VALKYRIE;
		m_tHitType.eGuardState = STATE_GUARDHIT_VALKYRIE;
		m_tHitType.eGuardBreakState = STATE_GUARD_CANCEL_PLAYER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_VALKYRIE;
		m_tHitType.eStingHitState = STATE_STINGHIT_VALKYRIE;
		m_tHitType.eFlyState = STATE_FLYHIT_VALKYRIE;
		m_tHitType.eBounce = STATE_BOUNCE_VALKYRIE_L;
	}

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
	pAnimator->Add_Animations(L"../bin/resources/animations/Valkyrie/SKEL_Fiona_HIT.fbx");

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
	tDesc.fHeight = 0.9f;
	// 칼 두께
	tDesc.fRadius = 0.1f;
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
	m_tUnitStatus.fHP = 100000.f;


	m_fCoolTime[SKILL3] = 5.f;
	m_fCoolTime[SKILL2] = 0.f; // 화신력 소모 스킬
	m_fCoolTime[SKILL1] = 45.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; // 화신력 소모 스킬
	m_fCoolAcc[SKILL3] = 10.f;

	return S_OK;
}

HRESULT CUnit_Valkyrie::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CUnit_Valkyrie::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	return S_OK;
}

void CUnit_Valkyrie::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Valkyrie::OnDisable()
{
	__super::OnDisable();
}

void CUnit_Valkyrie::My_Tick()
{
	__super::My_Tick();

	if (KEY(NUM7, TAP))
	{
		CPlayer::CLASS_DEFAULT eDefaultClass = m_pOwnerPlayer->Get_CurrentDefaultClass();
		m_pOwnerPlayer->Change_DefaultUnit(eDefaultClass);
	}
}

void CUnit_Valkyrie::My_LateTick()
{

	if (KEY(NUM8, TAP))
		GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(20.f, 2.f, 20.f));

}


