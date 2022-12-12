#include "stdafx.h"
#include "CState_Combat_Attack_HorizontalMiddle_Fiona_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CState_Combat_Attack_HorizontalMiddle_Fiona_R::CState_Combat_Attack_HorizontalMiddle_Fiona_R()
{
}

CState_Combat_Attack_HorizontalMiddle_Fiona_R::~CState_Combat_Attack_HorizontalMiddle_Fiona_R()
{
}

CState_Combat_Attack_HorizontalMiddle_Fiona_R* CState_Combat_Attack_HorizontalMiddle_Fiona_R::Create()
{
	CState_Combat_Attack_HorizontalMiddle_Fiona_R* pInstance = new CState_Combat_Attack_HorizontalMiddle_Fiona_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_HorizontalMiddle_Fiona_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CState_Combat_Attack_HorizontalMiddle_Fiona_R::Initialize()
{

	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;

	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	m_eBounceState = AI_STATE_COMMON_BOUNCE_FIONA_R;

	return __super::Initialize();
}

void CState_Combat_Attack_HorizontalMiddle_Fiona_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Set_BounceState(STATE_BOUNCE_VALKYRIE_R);

	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Fiona_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_HorizontalMiddle_Fiona_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	

}

STATE_TYPE CState_Combat_Attack_HorizontalMiddle_Fiona_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* VALKYRIE�� Attack ���� ���� ����
	1.  LBuutton �� �̿��� �����Ѵ�.
	*/
	//if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	//{
	//	return m_eStateType;
	//}

	//return STATE_END;
}

void CState_Combat_Attack_HorizontalMiddle_Fiona_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
