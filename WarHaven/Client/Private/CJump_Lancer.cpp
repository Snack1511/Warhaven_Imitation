#include "stdafx.h"
#include "CJump_Lancer.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Lancer::CJump_Lancer()
{
}

CJump_Lancer::~CJump_Lancer()
{
}

CJump_Lancer* CJump_Lancer::Create()
{
    CJump_Lancer* pInstance = new CJump_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 7;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    iFallKeyFrame = 49;

    Add_KeyFrame(iFallKeyFrame, 0);

    m_vecAdjState.push_back(STATE_ATTACK_STING_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_DEAFULT_LANCER);
    m_vecAdjState.push_back(STATE_JUMPFALL_LANCER);
    m_vecAdjState.push_back(STATE_JUMP_LAND_LANCER);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 12;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 15;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 14;


	iPlaceJumpAnimIndex = 7;


    return S_OK;
}

void CJump_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;


    /* Owner�� Animator Set Idle�� */

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

    m_iAnimIndex = iPlaceJumpAnimIndex;


    DoMove(STATE_DIRECTION_N, pOwner);
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    pOwner->Get_PhysicsCom()->Set_SpeedasMax();

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return STATE_RUN_LANCER;

    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
}

STATE_TYPE CJump_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� WASD �� �����ٸ�
    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }



    return STATE_END;

}

void CJump_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {

    case 0:
        m_fAnimSpeed = FLT_MIN;
        break;

    default:
        break;
    }
}