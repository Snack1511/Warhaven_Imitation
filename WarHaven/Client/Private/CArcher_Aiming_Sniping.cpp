#include "stdafx.h"
#include "CArcher_Aiming_Sniping.h"

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


CArcher_Aiming_Sniping::CArcher_Aiming_Sniping()
{
}

CArcher_Aiming_Sniping::~CArcher_Aiming_Sniping()
{
}

CArcher_Aiming_Sniping* CArcher_Aiming_Sniping::Create()
{
    CArcher_Aiming_Sniping* pInstance = new CArcher_Aiming_Sniping();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Aiming_Sniping");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Aiming_Sniping::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_AIMING_SNIPING_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.03f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 0;
    

	m_vecAdjState.push_back(STATE_ATTACK_SHOOT_SNIPING_ARCHER);


	m_fDamagePumping = 4.f;

    return __super::Initialize();
}

void CArcher_Aiming_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Aiming(pOwner, pAnimator, ePrevType, CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_ZOOMMAX);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
	GAMEINSTANCE->Start_RadialBlur(0.01f);

}

STATE_TYPE CArcher_Aiming_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(LBUTTON, AWAY))
		return STATE_ATTACK_SHOOT_SNIPING_ARCHER;

    if (KEY(RBUTTON, TAP))
        return STATE_ATTACK_CANCEL_ARCHER;

	return __super::Tick(pOwner, pAnimator);
}

void CArcher_Aiming_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit_Aiming(pOwner, pAnimator);
	__super::Exit(pOwner, pAnimator);
	GAMEINSTANCE->Stop_RadialBlur();

}

STATE_TYPE CArcher_Aiming_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CArcher_Aiming_Sniping::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	 __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
