#include "stdafx.h"
#include "CJump_Qanda.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Qanda::CJump_Qanda()
{
}

CJump_Qanda::~CJump_Qanda()
{
}

CJump_Qanda* CJump_Qanda::Create()
{
    CJump_Qanda* pInstance = new CJump_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;



    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_ARCHER);


    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_JUMPFALL_QANDA);
    m_vecAdjState.push_back(STATE_JUMP_LAND_QANDA);

   /* m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);*/


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 3;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 6;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 5;


	iPlaceJumpAnimIndex = 2;


    return S_OK;
}

void CJump_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
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

STATE_TYPE CJump_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
}

STATE_TYPE CJump_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���� WASD �� �����ٸ�
    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }



    return STATE_END;

}
