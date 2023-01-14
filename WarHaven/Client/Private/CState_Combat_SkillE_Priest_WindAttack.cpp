#include "stdafx.h"
#include "CState_Combat_SkillE_Priest_WindAttack.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Combat_SkillE_Priest_WindAttack::CState_Combat_SkillE_Priest_WindAttack()
{
}

CState_Combat_SkillE_Priest_WindAttack::~CState_Combat_SkillE_Priest_WindAttack()
{
}

CState_Combat_SkillE_Priest_WindAttack* CState_Combat_SkillE_Priest_WindAttack::Create()
{
    CState_Combat_SkillE_Priest_WindAttack* pInstance = new CState_Combat_SkillE_Priest_WindAttack();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_Priest_WindAttack");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_SkillE_Priest_WindAttack::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eDOWN;
	m_tHitInfo.fKnockBackPower = 5.f;
	m_tHitInfo.fJumpPower = 4.f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 60;


    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_WINDATTACK_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 65;

	m_iAttackStartAnimIndex = 30;
	m_iAttackEndAnimIndex = 43;

	Add_KeyFrame(m_iAttackStartAnimIndex, 0);
	Add_KeyFrame(m_iAttackEndAnimIndex, 1);

	return __super::Initialize();
}

void CState_Combat_SkillE_Priest_WindAttack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_Status().fDamageMultiplier = 1.f;
	pOwner->On_Use(CUnit::SKILL2);

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 45; // ������ �������

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


	/* ColorControl*/
	tColorDesc;

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.2f;
	tColorDesc.fFadeOutStartTime = 0.f;
	tColorDesc.fFadeOutTime = 0.2f;
	tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

	tColorDesc.iMeshPartType = MODEL_PART_BODY;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = 45; // ������ �������

	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_Priest_WindAttack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
		return AI_STATE_COMBAT_DEFAULT_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_Priest_WindAttack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
	pOwner->Enable_UnitCollider(CUnit::FLYATTACK, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillE_Priest_WindAttack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Combat_SkillE_Priest_WindAttack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::FLYATTACK, true);

		if(pOwner->Is_Air())
			CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Wind_Attack_0"), pOwner->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE));
		else
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Wind_Attack", pOwner->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE));

		break;

	case 1:
		m_bAttackTrigger = false;
		m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
		pOwner->Enable_UnitCollider(CUnit::FLYATTACK, false);
		break;

	default:
		break;
	}
}
