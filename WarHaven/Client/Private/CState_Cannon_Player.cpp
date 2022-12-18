#include "stdafx.h"
#include "CState_Cannon_Player.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

#include "CCannon.h"

CState_Cannon_Player::CState_Cannon_Player()
{
}

CState_Cannon_Player::~CState_Cannon_Player()
{
}

CState_Cannon_Player* CState_Cannon_Player::Create()
{
    CState_Cannon_Player* pInstance = new CState_Cannon_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Cannon_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Cannon_Player::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_CANNON_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.5f;


    return S_OK;
}

void CState_Cannon_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    CCannon* pCannon = pOwner->Get_AdjCannon();

    if (pCannon)
    {
        pOwner->Get_PhysicsCom()->Set_Speed(0.f);
        pCannon->Control_Cannon(pOwner->Get_OwnerPlayer());
        pOwner->Get_Transform()->Set_LerpLook(pCannon->Get_Transform()->Get_World(WORLD_LOOK), 0.4f);
        pOwner->Teleport_Unit(pCannon->Get_ControlPos());
    }

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Cannon_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{



    return __super::Tick(pOwner, pAnimator);
}

void CState_Cannon_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_AdjCannon()->Control_Cannon(nullptr);
}

STATE_TYPE CState_Cannon_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */
    if (pOwner->Get_AdjCannon())
    {
        if (KEY(F, TAP))
        {
            return m_eStateType;
        }
    }

    return STATE_END;
}
