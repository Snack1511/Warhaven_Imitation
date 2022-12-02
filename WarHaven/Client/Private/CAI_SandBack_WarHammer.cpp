#include "stdafx.h"
#include "CAI_SandBack_WarHammer.h"

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

CAI_SandBack_WarHammer::CAI_SandBack_WarHammer()
{
}

CAI_SandBack_WarHammer::~CAI_SandBack_WarHammer()
{
}

CAI_SandBack_WarHammer* CAI_SandBack_WarHammer::Create()
{
    CAI_SandBack_WarHammer* pInstance = new CAI_SandBack_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAI_SandBack_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CAI_SandBack_WarHammer::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
   /* if (iOtherColType == COL_PLAYERATTACK)
    {
        m_pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);
        m_pOwner->Get_PhysicsCom()->Set_SpeedasMax();

       
        m_pOwner->Get_PhysicsCom()->Set_Dir(CUtility_Transform::Get_Dir_2D(pOtherObject->Get_Transform(), m_pOwner->Get_Transform()));
    }*/
}
HRESULT CAI_SandBack_WarHammer::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 14;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = SANDBAG_STATE_IDLE_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;


    return S_OK;
}

void CAI_SandBack_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CAI_SandBack_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CAI_SandBack_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
}

STATE_TYPE CAI_SandBack_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
