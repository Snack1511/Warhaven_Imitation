#include "stdafx.h"
#include "CState_Combat_Attack_VerticalCut_Fiona.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Combat_Attack_VerticalCut_Fiona::CState_Combat_Attack_VerticalCut_Fiona()
{
}

CState_Combat_Attack_VerticalCut_Fiona::~CState_Combat_Attack_VerticalCut_Fiona()
{
}

CState_Combat_Attack_VerticalCut_Fiona* CState_Combat_Attack_VerticalCut_Fiona::Create()
{
    CState_Combat_Attack_VerticalCut_Fiona* pInstance = new CState_Combat_Attack_VerticalCut_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_Takedown");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_VerticalCut_Fiona::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 3.f;
	m_tHitInfo.fJumpPower = 0.f;

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 14;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_VERTICALCUT_FIONA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.3f;

    m_iStateChangeKeyFrame = 57;
   
    m_iStopIndex = 37;

	Add_KeyFrame(29, 1);
	Add_KeyFrame(m_iStopIndex, 2);
	Add_KeyFrame(30, 999);
	Add_KeyFrame(35, 998);

	m_eBounceState = AI_STATE_COMMON_BOUNCE_FIONA_R;

    return __super::Initialize();
}

void CState_Combat_Attack_VerticalCut_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_VerticalCut_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    STATE_TYPE eRandState = Enter_RandomState(pOwner, pAnimator);

    if (eRandState != STATE_END)
        return eRandState;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_VerticalCut_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

    //Exit���� ������ ���ܳ�����
    pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_VerticalCut_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_VerticalCut_Fiona::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
