#include "stdafx.h"
#include "CState_Common_Hit_Warrior.h"

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

CState_Common_Hit_Warrior::CState_Common_Hit_Warrior()
{
}

CState_Common_Hit_Warrior::~CState_Common_Hit_Warrior()
{
}

CState_Common_Hit_Warrior* CState_Common_Hit_Warrior::Create()
{
    CState_Common_Hit_Warrior* pInstance = new CState_Common_Hit_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Hit_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_Hit_Warrior::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iHitIndex[0];                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_HIT_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;
    
    return S_OK;
}

void CState_Common_Hit_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Hit_State(pOwner);

    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    m_iRand = random(0, 2);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Hit_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    //
    //if (pAnimator->Get_CurAnimFrame() > 43)
    //{
    //    if (m_iRand != 0)
    //    {
    //        pOwner->Set_LookToTarget();
    //        return AI_STATE_DEAFULT_IDLE_WARRIOR_R;
    //    }
    //}
        
    if (pAnimator->Is_CurAnimFinished())
        return m_ePreStateType;



    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Hit_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Common_Hit_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}
