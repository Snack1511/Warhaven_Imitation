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

    m_fMyAccel = 10.f;
    m_fMaxSpeed = 5.f;
    m_fMyMaxLerp = 0.4f;

    return S_OK;
}

void CState_Common_Cannon_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

    m_ePreStateType = ePrevType;

    {
        m_eAnimType = ANIM_BASE_R;

        switch (pOwner->Get_OwnerPlayer()->Get_CurClass())
        {
        case WARRIOR:
            m_iAnimIndex = 27;
            break;
        case ENGINEER:
            m_iAnimIndex = 30;
            break;
        case ARCHER:
            m_iAnimIndex = 27;
            break;
        case PALADIN:
            m_iAnimIndex = 19;
            break;
        case PRIEST:
            m_iAnimIndex = 19;
            break;
        default:
            break;
        }

        m_eCannonState = CANNON_FOLLOW;

    }
        

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Cannon_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    CCannon* pCannon = pOwner->Get_AdjCannon();
    CGameObject* pTargetObject = pOwner->Get_TargetObject();

    if (!pTargetObject)
    {
        STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
        pOwner->Force_ChangeBehavior();

        return eDefaultState;
    }



    switch (m_eCannonState)
    {
    case Client::CState_Common_Cannon_AI::CANNON_FOLLOW:

        if (pCannon)
        {
            m_eCannonState = CANNON_CONTROL;
            if (!pCannon->Can_ControlCannon(pOwner->Get_OwnerPlayer()))
            {
                pOwner->Force_ChangeBehavior();

                return pOwner->Get_DefaultState();
            }

            _uint iAnimIndex = 0;

            Change_Animation(pAnimator, ANIM_ETC, iAnimIndex);

            pOwner->Get_PhysicsCom()->Set_Speed(0.f);

            pCannon->Control_Cannon(pOwner->Get_OwnerPlayer());
            pOwner->Get_Transform()->Set_LerpLook(pCannon->Get_Transform()->Get_World(WORLD_LOOK), 0.4f);
            pOwner->Teleport_Unit(pCannon->Get_ControlPos());
            m_bControlCannon = true;

            m_fTimeAcc = 0.f;
        }
            

        // Ÿ�� ������Ʈ�� ������ ? Ÿ�������� ã�ư���.
        else
        {
            if (!dynamic_cast<CCannon*>(pTargetObject)->Can_ControlCannon(pOwner->Get_OwnerPlayer()))
                pOwner->Force_ChangeBehavior();

            _float4 vLook = pTargetObject->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
            vLook.y = 0.f;
            vLook.Normalize();

            CTransform* pMyTransform = pOwner->Get_Transform();

            CPhysics* pMyPhysicsCom = nullptr;
            pMyPhysicsCom = pOwner->Get_PhysicsCom();

            vLook.y = 0.f;

            if (!vLook.Is_Zero())
                pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

            pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);
            pMyPhysicsCom->Set_Dir(vLook);
            pMyPhysicsCom->Set_Accel(m_fMyAccel);

            //Follow_Move(pOwner, pTargetObject);

        }

        

        break;

    case Client::CState_Common_Cannon_AI::CANNON_ENTER:

        //m_fTimeAcc += fDT(0);

        if (pCannon)
        {
               
                m_eCannonState = CANNON_CONTROL;

                Change_Animation(pAnimator, ANIM_ETC, 0);
        }
        else
            m_eCannonState = CANNON_FOLLOW;

        break;

    case Client::CState_Common_Cannon_AI::CANNON_CONTROL:

        if (pCannon)
            pCannon->Shoot_Cannon();
        else
            m_eCannonState = CANNON_STOP;

        break;

    case Client::CState_Common_Cannon_AI::CANNON_STOP:
    case Client::CState_Common_Cannon_AI::CANNON_NOENTER:

        if (m_ePreStateType != STATE_END)
            return m_ePreStateType;
        else
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }

    default:
        break;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Cannon_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_eCannonState != CANNON_NOENTER)
    {
        if (m_bControlCannon)
        {
            if (pOwner->Get_AdjCannon())
            {
                pOwner->Get_AdjCannon()->Exit_Cannon();
            }
        }
            
    }
}

STATE_TYPE CState_Common_Cannon_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Common_Cannon_AI::Change_Animation(CAnimator* pAnimator, ANIM_TYPE eAnimType, _uint iAnimIndex)
{
    pAnimator->Set_CurAnimIndex(eAnimType, iAnimIndex, m_eAnimDivide);
    pAnimator->Set_InterpolationTime(eAnimType, iAnimIndex, m_fInterPolationTime);
    pAnimator->Set_AnimSpeed(eAnimType, iAnimIndex, m_fAnimSpeed);
}
