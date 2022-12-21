#include "stdafx.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"

#include "CColorController.h"

CState_Combat_SkillE_Paladin_ShieldWall_Loop::CState_Combat_SkillE_Paladin_ShieldWall_Loop()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_Loop::~CState_Combat_SkillE_Paladin_ShieldWall_Loop()
{
}

CState_Combat_SkillE_Paladin_ShieldWall_Loop* CState_Combat_SkillE_Paladin_ShieldWall_Loop::Create()
{
	CState_Combat_SkillE_Paladin_ShieldWall_Loop* pInstance = new CState_Combat_SkillE_Paladin_ShieldWall_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_Paladin_ShieldWall_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_SkillE_Paladin_ShieldWall_Loop::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 21;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_SHIELDWALL_LOOP_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_fMyMaxLerp = 1.2f;

	m_iStateChangeKeyFrame = 99;

	return S_OK;
}

void CState_Combat_SkillE_Paladin_ShieldWall_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fRand = frandom(0.5, 3.f);

	pOwner->Enable_GuardCollider(true);

	
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (140.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON_L;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pUnit = pOwner->Get_TargetUnit();

	m_fTimeAcc += fDT(0);

	if (!pUnit || m_fTimeAcc > m_fRand)
		return AI_STATE_COMBAT_SHIELDWALL_END_PALADIN;
	
	CTransform* pMyTransform = pOwner->Get_Transform();

	_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
	vLook.y = 0.f;
	pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_Paladin_ShieldWall_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

	pOwner->On_Use(CUnit::SKILL2);
}

STATE_TYPE CState_Combat_SkillE_Paladin_ShieldWall_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}
