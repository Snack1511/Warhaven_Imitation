#include "stdafx.h"
#include "CState_PathNavigation_Jump_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Jump_Lancer::CState_PathNavigation_Jump_Lancer()
{
}

CState_PathNavigation_Jump_Lancer::~CState_PathNavigation_Jump_Lancer()
{
}

CState_PathNavigation_Jump_Lancer* CState_PathNavigation_Jump_Lancer::Create()
{
    CState_PathNavigation_Jump_Lancer* pInstance = new CState_PathNavigation_Jump_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Jump_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_PathNavigation_Jump_Lancer::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 7;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_JUMP_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 7;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 7;


	iPlaceJumpAnimIndex = 7;

    m_eFallState = AI_STATE_PATHNAVIGATION_DEFAULT_LANCER;


    return S_OK;
}

void CState_PathNavigation_Jump_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Jump_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return __super::Tick(pOwner, pAnimator);
    
}

void CState_PathNavigation_Jump_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CState_PathNavigation_Jump_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}

void CState_PathNavigation_Jump_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
