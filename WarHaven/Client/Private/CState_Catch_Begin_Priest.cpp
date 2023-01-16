#include "stdafx.h"
#include "CState_Catch_Begin_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CUnit_Priest.h"


CState_Catch_Begin_Priest::CState_Catch_Begin_Priest()
{
}

CState_Catch_Begin_Priest::~CState_Catch_Begin_Priest()
{
}

CState_Catch_Begin_Priest* CState_Catch_Begin_Priest::Create()
{
    CState_Catch_Begin_Priest* pInstance = new CState_Catch_Begin_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Catch_Begin_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Catch_Begin_Priest::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 8;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_CATCH_CANNON_BEGIN_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 99;
    m_fMyMaxLerp = 0.4f;

	return __super::Initialize();
}

void CState_Catch_Begin_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Catch_Begin_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc > 0.5f)
		return AI_STATE_CATCH_CANNON_LOOP_PRIEST;


    CUnit* pUnit = pOwner->Get_TargetUnit();
    CTransform* pMyTransform = pOwner->Get_Transform();

    _float4 vLook = ZERO_VECTOR;

    if (pUnit)
    {
        vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
        vLook.y = 0.f;
    }
    else
        vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);


    pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Catch_Begin_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Catch_Begin_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Catch_Begin_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
