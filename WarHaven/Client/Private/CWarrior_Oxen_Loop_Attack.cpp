#include "stdafx.h"
#include "CWarrior_Oxen_Loop_Attack.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Loop_Attack::CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack::~CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack* CWarrior_Oxen_Loop_Attack::Create()
{
    CWarrior_Oxen_Loop_Attack* pInstance = new CWarrior_Oxen_Loop_Attack();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Loop_Attack");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Loop_Attack::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 23;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOPATTACK;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 87;

    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);


    Add_KeyFrame(10, 0);


    return S_OK;
}

void CWarrior_Oxen_Loop_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    pOwner->TurnOn_TrailEffect(true);

    if (ePrevType == STATE_WARRIOR_OXEN_LOOPATTACK)
    {
        m_fInterPolationTime = 0.f;
    }
    else
    {
        m_fInterPolationTime = 0.1f;
    }




    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Loop_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;


}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
        */

    if (KEY(Q, NONE))
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Oxen_Loop_Attack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
    {
        CTransform* pMyTransform = pOwner->Get_Transform();
        CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();



        //�ӽ�
        pMyPhysicsCom->Get_Physics().bAir = false;

        _float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
        vCamLook.y = 0.f;

        //1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
        pMyTransform->Set_LerpLook(vCamLook, 0.4f);

        //���� �����̴� ����
        pMyPhysicsCom->Set_Dir(vCamLook);

        //�ִ�ӵ� ����
        pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed);
        pMyPhysicsCom->Set_SpeedasMax();


        //���� �����ϱ�
        //*���� : ����ϰ��� Exit���� �ݵ�� 1�� �ǵ����ֱ�
        pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    }
       

    default:
        break;
    }


}


