#include "stdafx.h"
#include "CState_Patrol_Cure_Begin_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Patrol_Cure_Begin_Priest::CState_Patrol_Cure_Begin_Priest()
{
}

CState_Patrol_Cure_Begin_Priest::~CState_Patrol_Cure_Begin_Priest()
{
}

CState_Patrol_Cure_Begin_Priest* CState_Patrol_Cure_Begin_Priest::Create()
{
    CState_Patrol_Cure_Begin_Priest* pInstance = new CState_Patrol_Cure_Begin_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Cure_Begin_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Cure_Begin_Priest::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_CURE_BEGIN_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 99;

	return __super::Initialize();
}

void CState_Patrol_Cure_Begin_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Cure_Begin_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CGameObject* pCureObject = pOwner->Get_CureObject();

    if (!pCureObject)
        return AI_STATE_PATROL_CURE_END_PRIEST;

    _float4 vTargetLook = pCureObject->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
    vTargetLook.y = 0.f;

    pOwner->Get_Transform()->Set_LerpLook(vTargetLook, 0.4f);

	m_fTimeAcc += fDT(0);

	if (pAnimator->Is_CurAnimFinished() || m_fTimeAcc > 0.5f)
	{
		if (pCureObject)
			return AI_STATE_PATROL_CURE_LOOP_PRIEST;
		else
			return AI_STATE_PATROL_CURE_END_PRIEST;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Cure_Begin_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Cure_Begin_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pOtherUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	if (!pOtherUnit)
		return STATE_END;

    //if (KEY(RBUTTON, TAP))
    //    return m_eStateType;

    return STATE_END;
}


void CState_Patrol_Cure_Begin_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
