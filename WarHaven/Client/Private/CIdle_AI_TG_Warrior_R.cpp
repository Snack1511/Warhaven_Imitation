#include "stdafx.h"
#include "CIdle_AI_TG_Warrior_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_AI_TG_Warrior_R::CIdle_AI_TG_Warrior_R()
{
}

CIdle_AI_TG_Warrior_R::~CIdle_AI_TG_Warrior_R()
{
}

CIdle_AI_TG_Warrior_R* CIdle_AI_TG_Warrior_R::Create()
{
    CIdle_AI_TG_Warrior_R* pInstance = new CIdle_AI_TG_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_AI_TG_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_AI_TG_Warrior_R::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_IDLE_WARRIOR_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;
    
    m_vecAdjState.push_back(AI_STATE_RUN_WARRIOR_R);
    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_R);
    m_vecAdjState.push_back(AI_STATE_GUARD_BEGIN_WARRIOR);

    return S_OK;
}

void CIdle_AI_TG_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    if (ePrevType == STATE_SWITCH_L_TO_R    ||
        ePrevType == STATE_SPRINT_END_PLAYER
        )
        m_fInterPolationTime = 0.f;
    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_AI_TG_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return AI_STATE_RUNBEGIN_WARRIOR_R;


    return __super::Tick(pOwner, pAnimator);
}

void CIdle_AI_TG_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_AI_TG_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ���� 
    1. ���� �������� �ִϸ��̼��� ������ ��
    */
	//if (
	//	KEY(W, NONE) &&
	//	KEY(A, NONE) &&
	//	KEY(S, NONE) &&
	//	KEY(D, NONE))
	//{
	//	if (pAnimator->Is_CurAnimFinished())
	//		return m_eStateType;
	//}

    return STATE_END;
}
