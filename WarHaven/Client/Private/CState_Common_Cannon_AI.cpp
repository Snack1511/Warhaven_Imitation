#include "stdafx.h"
#include "CState_Common_Cannon_AI.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

#include "CCannon.h"

CState_Common_Cannon_AI::CState_Common_Cannon_AI()
{
}

CState_Common_Cannon_AI::~CState_Common_Cannon_AI()
{
}

CState_Common_Cannon_AI* CState_Common_Cannon_AI::Create()
{
    CState_Common_Cannon_AI* pInstance = new CState_Common_Cannon_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Cannon_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Cannon_AI::Initialize()
{
    // ����ü �и�.
    // 
    // __super::Initialize();

    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_CANNON_AI;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_tHitInfo.bFly = true;
    m_tHitInfo.iLandKeyFrame = 100;
    m_tHitInfo.fJumpPower = 6.f;
    m_tHitInfo.bGuardBreak = true;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.5f;
    m_fDamagePumping = 7.f;

    return S_OK;
}

void CState_Common_Cannon_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    CCannon* pCannon = pOwner->Get_AdjCannon();
    CGameObject* pTargetObj = pOwner->Get_OwnerPlayer()->Get_TargetObject();

    // ���� ������ Ÿ�ٿ�����Ʈ�� ������ �ƴ� ��
    if (pCannon != pTargetObj)
    {
        m_bNoAdjTrigger = true;
        return;
    }

    if (pCannon)
    {
        pOwner->Get_PhysicsCom()->Set_Speed(0.f);
        pCannon->Control_Cannon(pOwner->Get_OwnerPlayer());
        pOwner->Get_Transform()->Set_LerpLook(pCannon->Get_Transform()->Get_World(WORLD_LOOK), 0.4f);
        pOwner->Teleport_Unit(pCannon->Get_ControlPos());
    }


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Cannon_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_bNoAdjTrigger)
    {
        if (m_ePreStateType != STATE_END)
            return m_ePreStateType;
        else
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }
    }
        
    CCannon* pCannon = pOwner->Get_AdjCannon();     
    
    m_fTimeAcc += fDT(0);

    if(m_fTimeAcc > 5.f)
        pCannon->Shoot_Cannon();


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Cannon_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_AdjCannon()->Exit_Cannon();
}

STATE_TYPE CState_Common_Cannon_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
