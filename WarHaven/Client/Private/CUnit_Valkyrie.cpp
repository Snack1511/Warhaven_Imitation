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

	//pOtherUnit : ���� ��

	//������ ��� �������� ���;���

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS);

	switch (m_eCurState)
	{
	case STATE_ATTACK_HORIZONTALUP_VALKYRIE_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L:
	case STATE_SPINATTACK_VALKYRIE:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L:
	case STATE_SPRINTATTACK_VALKYRIE:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALUP_VALKYRIE_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", vHitPos, matWorld);
		break;

	case STATE_ATTACK_VERTICALCUT_VALKYRIE:
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


	//�߰������� Animator �����ߴ�.

	//  attack, hit, etc, parkour, L_Base, R_Base �� �⺻������ fbx�� �߰��մϴ�.
	//  �⺻������ L_Base �� ���� Unit Mesh �� ������ L_Base �� �����ϰ� Add_Animation �� ��������.
	

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

	// Į ����
	tDesc.fHeight = 0.9f;
	// Į �β�
	tDesc.fRadius = 0.1f;
	// Į ���� ��
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	//Į ������(����)
	tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	m_pWeaponCollider_R = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pWeaponCollider_R);

	//// Į ����
	//tDesc.fHeight = 0.1f;
	//// Į �β�
	//tDesc.fRadius = 0.2f;
	//// Į ���� ��
	//tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_L_WP1");

	////Į ������(����)
	//tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	//m_pWeaponCollider_L = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	//Add_Component(m_pWeaponCollider_L);

	m_tUnitStatus.fSprintAttackSpeed = 13.f;
	m_tUnitStatus.fDashAttackSpeed = 4.f;

	m_tUnitStatus.eClass = FIONA;
	m_tUnitStatus.fHP = 100000.f;

	m_tUnitStatus.fRunSpeed *= 0.8f;


	m_fCoolTime[SKILL1] = 45.f;
	m_fCoolTime[SKILL2] = 0.f; // ȭ�ŷ� �Ҹ� ��ų
	m_fCoolTime[SKILL3] = 5.f;

	m_fCoolAcc[SKILL1] = 0.f;
	m_fCoolAcc[SKILL2] = 0.f; // ȭ�ŷ� �Ҹ� ��ų
	m_fCoolAcc[SKILL3] = 0.f;

	return S_OK;
}

HRESULT CUnit_Valkyrie::Initialize()
{
	__super::Initialize();
	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	return S_OK;
}

HRESULT CUnit_Valkyrie::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

	SetUp_TrailEffect(
		_float4(0.f, 0.f, -100.f, 1.f),	//Weapon Low
		_float4(0.f, 0.f, -103.f, 1.f),	//Weapon High
		_float4(0.f, -1.5f, -101.5f, 1.f), //Left
		_float4(0.f, 1.5f, -101.5f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow
		_float4(1.f, 0.1f, 0.1f, 0.25f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/T_EFF_Blur_05_M.dds",
		L"../bin/resources/Textures/Effects/WarHaven/T_EFF_Blur_05_M.dds",
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

	m_pModelCom->Set_RimLightFlag(_float4((255.f / 255.f), (140.f / 255.f), (42.f / 255.f), 1.f));

	return S_OK;
}

void CUnit_Valkyrie::OnEnable()
{
	TurnOn_ValkyrieTrail(true);
	__super::OnEnable();
}

void CUnit_Valkyrie::OnDisable()
{
	TurnOn_ValkyrieTrail(false);
	__super::OnDisable();
}

void CUnit_Valkyrie::My_Tick()
{
	__super::My_Tick();
}

void CUnit_Valkyrie::My_LateTick()
{

	if (KEY(NUM8, TAP))
		GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(20.f, 2.f, 20.f));

}


