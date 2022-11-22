#include "stdafx.h"
#include "CAI_SandBack_L.h"

#include "GameInstance.h"
#include "Physics.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

#include "CUtility_Transform.h"

CAI_SandBack_L::CAI_SandBack_L()
{
}

CAI_SandBack_L::~CAI_SandBack_L()
{
}

CAI_SandBack_L* CAI_SandBack_L::Create()
{
    CAI_SandBack_L* pInstance = new CAI_SandBack_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAI_SandBack_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CAI_SandBack_L::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
   /* if (iOtherColType == COL_PLAYERATTACK)
    {
        m_pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);
        m_pOwner->Get_PhysicsCom()->Set_SpeedasMax();

       
        m_pOwner->Get_PhysicsCom()->Set_Dir(CUtility_Transform::Get_Dir_2D(pOtherObject->Get_Transform(), m_pOwner->Get_Transform()));
    }*/
}
HRESULT CAI_SandBack_L::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_WARRIOR_L_AI_ENEMY;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;


    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_JUMP);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);

    //m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);


    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    //m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    //m_vecAdjState.push_back(STATE_SPRINT_PLAYER);

    return S_OK;
}

void CAI_SandBack_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CAI_SandBack_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CUnit* pTargetUnit = pOwner->Get_TargetUnit();

    if (pTargetUnit)
          CUtility_Transform::LookAt(pOwner->Get_Transform(), pTargetUnit->Get_Transform()->Get_World(WORLD_POS), true);


    return __super::Tick(pOwner, pAnimator);
}

void CAI_SandBack_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
}

STATE_TYPE CAI_SandBack_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
