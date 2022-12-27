#include "stdafx.h"
#include "CQanda_Aiming.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"

#include "CAnimWeapon.h"
#include "HIerarchyNode.h"
#include "CProjectile.h"
#include "CUnit_QANDA.h"

CQanda_Aiming::CQanda_Aiming()
{
}

CQanda_Aiming::~CQanda_Aiming()
{
}

CQanda_Aiming* CQanda_Aiming::Create()
{
    CQanda_Aiming* pInstance = new CQanda_Aiming();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CQanda_Aiming");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CQanda_Aiming::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 6;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_AIMING_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 1.5f;
    m_iStateChangeKeyFrame = 999;


    return __super::Initialize();
}

void CQanda_Aiming::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType, CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_ZOOM);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CQanda_Aiming::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
  
	if (KEY(LBUTTON, AWAY))
		return STATE_ATTACK_SHOOT_QANDA;

    if (KEY(RBUTTON, TAP))
        return STATE_ATTACK_CANCEL_QANDA;
		
	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Aiming::Exit(CUnit* pOwner, CAnimator* pAnimator)
{	
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CQanda_Aiming::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CQanda_Aiming::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
