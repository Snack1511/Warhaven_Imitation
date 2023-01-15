#include "stdafx.h"
#include "CUnit_Lancer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CLancerNeedle.h"
#include "CUnit_Lancer_Head.h"
#include "CRectEffects.h"

#include "CRectEffects.h"

CUnit_Lancer::CUnit_Lancer()
{
}

CUnit_Lancer::~CUnit_Lancer()
{
}

CUnit_Lancer* CUnit_Lancer::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CUnit_Lancer* pInstance = new CUnit_Lancer;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Lancer");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Lancer");
		return nullptr;
	}

	return pInstance;
}

void CUnit_Lancer::On_Die()
{
	m_pOwnerPlayer->On_FinishHero();
	m_pOwnerPlayer->Get_CurrentUnit()->On_Die();
	m_bDie = false;
	m_fDeadTimeAcc = 0.f;
	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;

}

void CUnit_Lancer::TurnOn_Trail(_bool bOn)
{
	if (!m_pTrail_R)
		return;

	m_pTrail_R->TurnOn_TrailEffect(bOn);
	m_pTrail_R2->TurnOn_TrailEffect(bOn);
	m_pTrail_L->TurnOn_TrailEffect(bOn);
	m_pTrail_L2->TurnOn_TrailEffect(bOn);


	m_pLowerTrail_R->TurnOn_TrailEffect(bOn);
	m_pLowerTrail_R2->TurnOn_TrailEffect(bOn);
	m_pLowerTrail_L->TurnOn_TrailEffect(bOn);
	m_pLowerTrail_L2->TurnOn_TrailEffect(bOn);

	m_pHorseTrail_R->TurnOn_TrailEffect(bOn);
	m_pHorseTrail_R2->TurnOn_TrailEffect(bOn);
	m_pHorseTrail_L->TurnOn_TrailEffect(bOn);
	m_pHorseTrail_L2->TurnOn_TrailEffect(bOn);

	m_pEyeTrail_R->TurnOn_TrailEffect(bOn);
	m_pEyeTrail_R2->TurnOn_TrailEffect(bOn);
	m_pEyeTrail_L->TurnOn_TrailEffect(bOn);
	m_pEyeTrail_L2->TurnOn_TrailEffect(bOn);
}

void CUnit_Lancer::SetUp_Colliders(_bool bPlayer)
{
	__super::SetUp_Colliders(bPlayer);

	COL_GROUP_CLIENT	eHitBoxBody = (bPlayer) ? COL_BLUEHITBOX_BODY : COL_REDHITBOX_BODY;
	COL_GROUP_CLIENT	eHitBoxHead = (bPlayer) ? COL_BLUEHITBOX_HEAD : COL_REDHITBOX_HEAD;
	COL_GROUP_CLIENT	eAttack = (bPlayer) ? COL_BLUEATTACK : COL_REDATTACK;
	COL_GROUP_CLIENT	eGroggy = (bPlayer) ? COL_BLUEGROGGYATTACK : COL_REDGROGGYATTACK;


	const _uint iBodySphereNum = 8;

	CUnit::UNIT_COLLIDERDESC tUnitColDesc[iBodySphereNum] =
	{
		//Radius,	vOffsetPos.		eColType
		{0.5f, _float4(0.f, 0.5f, -0.7f),(_uint)eHitBoxBody },
		{0.5f, _float4(0.f, 0.5f, 0.7f),(_uint)eHitBoxBody },
		{0.5f, _float4(0.f, 1.f, -0.7f),(_uint)eHitBoxBody },
		{0.5f, _float4(0.f, 1.f, 0.7f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.4f, -0.7f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.4f, 0.f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.4f, 0.7f),(_uint)eHitBoxBody },
		{0.6f, _float4(0.f, 1.9f, 0.f),(_uint)eHitBoxBody },
		//{0.6f, _float4(0.f, 1.5f, 0.f), (_uint)eHitBoxBody },
	};

	SetUp_UnitCollider(CUnit::BODY, tUnitColDesc, iBodySphereNum);


	tUnitColDesc[0].fRadius = 0.4f;
	tUnitColDesc[0].vOffsetPos = _float4(0.f, 2.5f, 0.f, 0.f);
	tUnitColDesc[0].eColType = (_uint)eHitBoxHead;


	SetUp_UnitCollider(CUnit::HEAD, tUnitColDesc, 1, DEFAULT_TRANS_MATRIX, true, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"));


	const _uint iWeaponSphereNum = 22;

	CUnit::UNIT_COLLIDERDESC tWeaponUnitColDesc[iWeaponSphereNum];

	for (_uint i = 0; i < iWeaponSphereNum; ++i)
	{
		tWeaponUnitColDesc[i].fRadius = 0.3f;
		tWeaponUnitColDesc[i].vOffsetPos.z = -10.f * _float(i) - 10.f;
		tWeaponUnitColDesc[i].eColType = (_uint)eAttack;
	}

	SetUp_UnitCollider(CUnit::WEAPON_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


	for (_uint i = 0; i < iWeaponSphereNum; ++i)
		tWeaponUnitColDesc[i].eColType = (_uint)eGroggy;

	SetUp_UnitCollider(CUnit::GUARDBREAK_R, tWeaponUnitColDesc, iWeaponSphereNum, DEFAULT_TRANS_MATRIX, false, GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1"));


}

void	CUnit_Lancer::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	/* 나중에 사망 후 부활 할 시 위치 저장할 때 사용해야 하므로 전체적으로 추가하도록 한다. */
	//m_tHitType.m_eDeathState  = STATE_DEAD;


	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:
		m_tHitType.eHitState = STATE_HIT_LANCER;
		m_tHitType.eGuardState = STATE_BOUNCE_LANCER;
		m_tHitType.eGuardBreakState = STATE_BOUNCE_LANCER;
		m_tHitType.eGroggyState = STATE_GROGGYHIT_LANCER;
		m_tHitType.eStingHitState = STATE_GROGGYHIT_LANCER;
		m_tHitType.eFlyState = STATE_FLYHIT_LANCER;
		m_tHitType.eBounce = STATE_BOUNCE_LANCER;
		break;


	case Client::CUnit::UNIT_TYPE::eAI_Default:
		m_tHitType.eHitState = AI_STATE_COMMON_HIT_LANCER;
		m_tHitType.eGuardState = AI_STATE_COMMON_GROGGYHIT_LANCER;
		m_tHitType.eGuardBreakState = AI_STATE_COMMON_GROGGYHIT_LANCER;
		m_tHitType.eStingHitState = AI_STATE_COMMON_GROGGYHIT_LANCER;
		m_tHitType.eGroggyState = AI_STATE_COMMON_GROGGYHIT_LANCER;
		m_tHitType.eFlyState = AI_STATE_COMMON_FLYHIT_LANCER;
		m_tHitType.eBounce = AI_STATE_COMMON_BOUNCE_LANCER;
		break;

		

	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}

		
}

void CUnit_Lancer::SetUp_ReserveState(UNIT_TYPE eUnitType)
{
	m_eSprintEndState = NO_PATTERN;

	switch (eUnitType)
	{
	case Client::CUnit::UNIT_TYPE::ePlayer:

		m_eDefaultState = STATE_IDLE_LANCER;
		m_eBreezeBegin = STATE_ATTACK_BREEZE_BEGIN_LANCER;
		m_eBreezeLoop = STATE_ATTACK_BREEZE_LOOP_LANCER;

		m_eSprintEndState = STATE_STOP_LANCER;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_Default:

		m_eDefaultState = AI_STATE_COMBAT_DEAFULT_LANCER; 
		m_eBreezeBegin = AI_STATE_COMBAT_BREEZE_BEGIN_LANCER;
		m_eBreezeLoop = AI_STATE_COMBAT_BREEZE_LOOP_LANCER;

		m_eSprintEndState = AI_STATE_PATHNAVIGATION_STOP_LANCER;

		m_tAIChangeType.eAIPathFindDefaultState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;
		m_tAIChangeType.eAICommbatDefaultState = AI_STATE_COMBAT_DEAFULT_LANCER;
		m_tAIChangeType.eAIPatrolDefaultState = AI_STATE_PATROL_DEFAULT_LANCER;
		m_tAIChangeType.eAIGoTirrgerDefaultState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;

		break;

	case Client::CUnit::UNIT_TYPE::eAI_idiot:

		m_eDefaultState = AI_STATE_COMBAT_DEFAULT_WARRIOR_R;
		//m_eSprintEndState = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;

		break;



	case Client::CUnit::UNIT_TYPE::eUNIT_TYPE_END:
		break;

	default:
		break;
	}
}

void CUnit_Lancer::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	__super::On_ChangeBehavior(pBehaviorDesc);

	if (nullptr == pBehaviorDesc)
		assert(0);

	STATE_TYPE	eNewState = STATE_END;

	switch (pBehaviorDesc->eCurType)
	{
	case eBehaviorType::ePatrol:
		//상태변경
		eNewState = AI_STATE_PATROL_DEFAULT_LANCER;
		break;
	case eBehaviorType::ePadenCannonInteract:
		//상태변경
		break;
	case eBehaviorType::eCombat:
		//상태변경
		eNewState = AI_STATE_COMBAT_DEAFULT_LANCER;

		break;
	 
	case eBehaviorType::ePathFinding:
		//상태변경
		eNewState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;
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

void CUnit_Lancer::Turn_TransformParticle(_bool bOnOff)
{
	if (bOnOff)
	{
		if (m_TransformParticles.empty())
			m_TransformParticles = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Transform_Particle", this, ZERO_VECTOR);

		if (m_EyeFlares.empty())
			m_EyeFlares = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Eye_Flare", this, ZERO_VECTOR);
	}
	else
	{
		if (!m_TransformParticles.empty())
		{
			for (auto& elem : m_TransformParticles)
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut();

			m_TransformParticles.clear();
		}
		

		if (!m_EyeFlares.empty())
		{
			for (auto& elem : m_EyeFlares)
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut();

			m_EyeFlares.clear();
		}
		
	}
}

void CUnit_Lancer::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
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
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALDOWN_L:
	case STATE_SPRINTATTACK_PLAYER:
	case AI_STATE_COMBAT_GUARDBREAK_WARRIOR:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, matWorld);
		break;

	case AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR:
	case STATE_WARRIOR_OXEN_LOOPATTACK:
	case STATE_ATTACK_HORIZONTALUP_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, matWorld);
		break;

	case STATE_ATTACK_HORIZONTALMIDDLE_R:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY:
	case AI_STATE_ATTACK_HORIZONTALMIDDLE_R:
	case AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R:
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

void CUnit_Lancer::SetUp_Trail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrail_R2, CMesh))->Set_NoCurve();

	m_pTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pTrail_R->TurnOn_TrailEffect(false);
	m_pTrail_R2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_Trail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrail_L2, CMesh))->Set_NoCurve();

	m_pTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pTrail_L->TurnOn_TrailEffect(false);
	m_pTrail_L2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_LowerTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pLowerTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pLowerTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pLowerTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pLowerTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLowerTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pLowerTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLowerTrail_R2, CMesh))->Set_NoCurve();

	m_pLowerTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pLowerTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pLowerTrail_R->TurnOn_TrailEffect(false);
	m_pLowerTrail_R2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_LowerTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pLowerTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pLowerTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pLowerTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pLowerTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLowerTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pLowerTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLowerTrail_L2, CMesh))->Set_NoCurve();

	m_pLowerTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pLowerTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pLowerTrail_L->TurnOn_TrailEffect(false);
	m_pLowerTrail_L2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_HorseTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pHorseTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pHorseTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pHorseTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pHorseTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pHorseTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pHorseTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pHorseTrail_R2, CMesh))->Set_NoCurve();

	m_pHorseTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pHorseTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pHorseTrail_R->TurnOn_TrailEffect(false);
	m_pHorseTrail_R2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_HorseTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pHorseTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pHorseTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pHorseTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pHorseTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pHorseTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pHorseTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pHorseTrail_L2, CMesh))->Set_NoCurve();

	m_pHorseTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pHorseTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pHorseTrail_L->TurnOn_TrailEffect(false);
	m_pHorseTrail_L2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_EyeTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pEyeTrail_R = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pEyeTrail_R2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pEyeTrail_R)
		return;

	CREATE_GAMEOBJECT(m_pEyeTrail_R, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail_R, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pEyeTrail_R2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail_R2, CMesh))->Set_NoCurve();

	m_pEyeTrail_R->Set_EffectFlag(m_vTrailShader);
	m_pEyeTrail_R2->Set_EffectFlag(m_vTrailShader);

	m_pEyeTrail_R->TurnOn_TrailEffect(false);
	m_pEyeTrail_R2->TurnOn_TrailEffect(false);
}

void CUnit_Lancer::SetUp_EyeTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pEyeTrail_L = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pEyeTrail_L2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pEyeTrail_L)
		return;

	CREATE_GAMEOBJECT(m_pEyeTrail_L, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail_L, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pEyeTrail_L2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail_L2, CMesh))->Set_NoCurve();

	m_pEyeTrail_L->Set_EffectFlag(m_vTrailShader);
	m_pEyeTrail_L2->Set_EffectFlag(m_vTrailShader);

	m_pEyeTrail_L->TurnOn_TrailEffect(false);
	m_pEyeTrail_L2->TurnOn_TrailEffect(false);
}

HRESULT CUnit_Lancer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	
	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Lancer/SKEL_Lancer_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;
	

	
	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/Lancer/SKEL_Lancer_Base_R.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/Lancer/SKEL_Lancer_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/Lancer/SKEL_Lancer_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/Lancer/SKEL_Lancer_ETC.fbx");



	/* PhysX Collider */

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	Add_Component(pAnimator);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// 칼 길이
	tDesc.fHeight = 1.2f;
	// 칼 두께
	tDesc.fRadius = 0.1f;
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

	m_tUnitStatus.fSprintSpeed = 10.f;
	m_tUnitStatus.fRunSpeed = m_tUnitStatus.fSprintSpeed;
	m_tUnitStatus.fWalkSpeed = m_tUnitStatus.fSprintSpeed;
	



	m_tUnitStatus.eClass = LANCER;

	m_pMyHead = CUnit_Lancer_Head::Create(L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0000_Face_A00_20.fbx",
		GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"), this);

	for (_int i = 0; i < eNeedle_Max; ++i)
	{
		m_pNeedle[i] = CLancerNeedle::Create(L"../bin/resources/meshes/Weapons/Needle/SM_MagicLancer_04.fbx",
		GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Spine"), this, i);

		if (!m_pNeedle[i])
			return E_FAIL;

		m_pNeedle[i]->Initialize();
		
	}

	//m_pMyHead = CLancerNeedle::Create(L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0000_Face_A00_20.fbx",
	//	GET_COMPONENT(CModel)->Find_HierarchyNode("0B_Head"), this);

	//CLancerNeedle::



	if (!m_pMyHead)
		return E_FAIL;

	m_pMyHead->Initialize();

	return S_OK;
}

HRESULT CUnit_Lancer::Initialize()
{
	__super::Initialize();

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;


	
	return S_OK;
}

HRESULT CUnit_Lancer::Start()
{
	__super::Start();

	if (m_pMyHead)
	{
		CREATE_GAMEOBJECT(m_pMyHead, GROUP_PLAYER);
		//ENABLE_GAMEOBJECT(m_pMyHead);
	}
	

	for (_int i = 0; i < eNeedle_Max; ++i)
	{
		CREATE_GAMEOBJECT(m_pNeedle[i], GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pNeedle[i]);
	}

	

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
		60,
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
		60,
		"0B_R_WP1"
	);

	m_vTrailShader = SH_EFFECT_NONE;
	wstring strMask = L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_04.dds";
	_float fAlpha = 0.5f;
	_float fUpperSize = 10.f;

	SetUp_Trail_R(
		_float4(0.f, 0.f, fUpperSize, 1.f),	//Weapon R
		_float4(-0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		_float4(1.f, 0.8f, 0.5f, fAlpha), //vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"0B_R_ShoulderP1_01"
	);

	SetUp_Trail_L(
		_float4(0.f, 0.f, -fUpperSize, 1.f),	//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		_float4(1.f, 0.8f, 0.5f, fAlpha), //vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"0B_L_ShoulderP1_01"
	);
	//lower
	fUpperSize *= 0.5f;

	SetUp_LowerTrail_R(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"0B_R_Knee_02"
	);
	SetUp_LowerTrail_L(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"0B_L_Knee_02"
	);

	SetUp_HorseTrail_R(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"1B_R_Thigh2"
	);
	SetUp_HorseTrail_L(
		_float4(0.f, 0.f, -fUpperSize, 1.f),					//Weapon R
		_float4(-0.f, 0.f, fUpperSize, 1.f),					//Weapon R
		_float4(-fUpperSize, 0.f, 0.f, 1.f),					 //Left	L
		_float4(fUpperSize, 0.f, 0.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.8f, 0.5f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		60,
		"1B_L_Thigh2"
	);

	SetUp_EyeTrail_R(
		_float4(10.f, -10.f, 25.f, 1.f),					//Weapon R
		_float4(10.f, -10.f, 5.f, 1.f),					//Weapon R
		_float4(-fUpperSize + 10.f, -10.f, 25.f, 1.f),					 //Left	L
		_float4(fUpperSize + 10.f, -10.f, 5.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.f, 0.f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		20,
		"1B_Jaw"
	);
	SetUp_EyeTrail_L(
		_float4(10.f, -10.f, -25.f, 1.f),					//Weapon R
		_float4(10.f, -10.f, -5.f, 1.f),					//Weapon R
		_float4(-fUpperSize + 10.f, -10.f, -25.f, 1.f),					 //Left	L
		_float4(fUpperSize + 10.f, -10.f, -5.f, 1.f),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f),					// GlowFlag
		_float4(1.f, 0.f, 0.f, fAlpha),					//vColor
		0.f,
		strMask,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		20,
		"1B_Jaw"
	);

	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	m_pModelCom->Set_RimLightFlag(RGB(50, 30, 0));

	m_TransformParticles.clear();
	m_EyeFlares.clear();

	return S_OK;
}

void CUnit_Lancer::OnEnable()
{
	__super::OnEnable();

	Turn_TransformParticle(true);
	TurnOn_Trail(true);
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos.y += 0.5f;

	Create_Light(vPos, 5.f, 0.f, 0.f, 0.f, 1.5f, RGB(255, 140, 40),
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn,
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);


	ENABLE_GAMEOBJECT(m_pMyHead);
	m_iNeedleNums = 0;
	m_fTimeAcc = 0.f;
}

void CUnit_Lancer::OnDisable()
{
	__super::OnDisable();

	Turn_TransformParticle(false);
	TurnOn_Trail(false);
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos.y += 0.5f;
	Create_Light(vPos, 4.f, 0.f, 0.f, 0.f, 0.3f, RGB(255, 255, 255),
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseIn,
		LIGHTDESC::EASING_TYPE::EAS_BounceEaseOut);

	for (_int i = 0; i < eNeedle_Max; ++i)
	{
		if(m_pNeedle[i])
			m_pNeedle[i]->Enable_Needle(false);
	}

	DISABLE_GAMEOBJECT(m_pMyHead);
}

void CUnit_Lancer::My_LateTick()
{
	__super::My_LateTick();


	if (Get_CurState() != m_eBreezeBegin &&
		Get_CurState() != m_eBreezeLoop)
	{
		if (m_fCoolAcc[CUnit::SKILL1] <= 0.f)
		{
			if (m_iNeedleNums < eNeedle_Max)
			{
				if(m_bIsMainPlayer)
					CUser::Get_Instance()->Set_LancerGauge(m_iNeedleNums, m_fTimeAcc, m_fNeedleCreateTime);
				
				m_fTimeAcc += fDT(0);

				if (m_fTimeAcc > m_fNeedleCreateTime)
				{
					m_pNeedle[m_iNeedleNums]->Enable_Needle(true);
					++m_iNeedleNums;
					m_fTimeAcc = 0.f;

				}
			}

			

		}
	}
	else
	{
		m_fTimeAcc = 0.f;
	}

	//if (KEY(Q, TAP))
	//{
	//	for(_int i = 0 ; i < eNeedle_Max; ++i)
	//		m_pNeedle[i]->Enable_Needle(true);

	//	m_fTimeAcc = 0.f;
	//	m_iNeedleNums = 0;
	//}

	if (KEY(NUM8, TAP))
	{
		GET_COMPONENT(CPhysXCharacter)->Set_Position(_float4(0.f, 0.f, 0.f));
		m_pTransform->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
	}

}
