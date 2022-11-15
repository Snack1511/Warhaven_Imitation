#include "stdafx.h"
#include "CIdle_SpearMan_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_SpearMan_R::CIdle_SpearMan_R()
{
}

CIdle_SpearMan_R::~CIdle_SpearMan_R()
{
}

CIdle_SpearMan_R* CIdle_SpearMan_R::Create()
{
    CIdle_SpearMan_R* pInstance = new CIdle_SpearMan_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_SpearMan_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_SpearMan_R::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_SPEARMAN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    // m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);
    

	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_SPEARMAN_R);
	
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
 //   m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);

 //   m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

 //   m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	//m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);



    //m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
    //m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);


    //m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);
    

    


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

void CIdle_SpearMan_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));


    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CIdle_SpearMan_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_SpearMan_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_SpearMan_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ���� 
    1. ���� �������� �ִϸ��̼��� ������ ��
    */
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
