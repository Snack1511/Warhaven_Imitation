#include "stdafx.h"
#include "CValkyrie_Attack_HorizontalMiddle_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CValkyrie_Attack_HorizontalMiddle_R::CValkyrie_Attack_HorizontalMiddle_R()
{
}

CValkyrie_Attack_HorizontalMiddle_R::~CValkyrie_Attack_HorizontalMiddle_R()
{
}

CValkyrie_Attack_HorizontalMiddle_R* CValkyrie_Attack_HorizontalMiddle_R::Create()
{
	CValkyrie_Attack_HorizontalMiddle_R* pInstance = new CValkyrie_Attack_HorizontalMiddle_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CValkyrie_Attack_HorizontalMiddle_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CValkyrie_Attack_HorizontalMiddle_R::Initialize()
{
	/* HIT_INFO */
	/*�����ʿ��� �������� �ֵθ��ϱ� LEFT*/
	m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;







	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_L);

	/* Setting for Blendable */
	m_eAnimLeftorRight = ANIM_BASE_R;

	m_eWalkState = STATE_WALK_VALKYRIE_L;
	m_eJumpState = STATE_JUMP_VALKYRIE_L;
	m_eLandState = STATE_JUMP_LAND_VALKYRIE_L;
	m_eFallState = STATE_JUMPFALL_VALKYRIE_L;
	m_eRunState = STATE_RUN_VALKYRIE_L;
	m_eIdleState = STATE_IDLE_VALKYRIE_L;
	m_eBounceState = STATE_BOUNCE_VALKYRIE_R;


	return __super::Initialize();
}

void CValkyrie_Attack_HorizontalMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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

STATE_TYPE CValkyrie_Attack_HorizontalMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Attack_HorizontalMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	

}

STATE_TYPE CValkyrie_Attack_HorizontalMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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

void CValkyrie_Attack_HorizontalMiddle_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
