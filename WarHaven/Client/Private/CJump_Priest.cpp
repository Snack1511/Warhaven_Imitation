#include "stdafx.h"
#include "CJump_Priest.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Priest::CJump_Priest()
{
}

CJump_Priest::~CJump_Priest()
{
}

CJump_Priest* CJump_Priest::Create()
{
    CJump_Priest* pInstance = new CJump_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Priest::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    m_vecAdjState.push_back(STATE_GLIDING);
    m_vecAdjState.push_back(STATE_JUMPFALL_PRIEST);
    m_vecAdjState.push_back(STATE_JUMP_LAND_PRIEST);
    m_vecAdjState.push_back(STATE_AIRDASH_PRIEST);

    Init_AttackState_Priest();
    Init_CommonState_Player();

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 12;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 15;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 14;


	iPlaceJumpAnimIndex = 11;


    return S_OK;
}

void CJump_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;


    /* Owner�� Animator Set Idle�� */

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    _uint iDirection = Get_Direction();

    if (iDirection == STATE_DIRECTION_END)
    {
        m_iAnimIndex = iPlaceJumpAnimIndex;
        m_fAnimSpeed = 1.5f;
    }
    else
    {
        m_iAnimIndex = m_iDirectionAnimIndex[iDirection];
        m_fAnimSpeed = 1.5f;

        DoMove(iDirection, pOwner);

        pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
        pOwner->Get_PhysicsCom()->Set_SpeedasMax();

    }

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
}

STATE_TYPE CJump_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� WASD �� �����ٸ�
    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }



    return STATE_END;

}
