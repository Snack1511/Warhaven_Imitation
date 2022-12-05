#include "stdafx.h"
#include "CIdle_Paladin_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_Paladin_R::CIdle_Paladin_R()
{
}

CIdle_Paladin_R::~CIdle_Paladin_R()
{
}

CIdle_Paladin_R* CIdle_Paladin_R::Create()
{
    CIdle_Paladin_R* pInstance = new CIdle_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Paladin_R::Initialize()
{


    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PALADIN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);

    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_PALADIN);

    m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
    m_vecAdjState.push_back(STATE_JUMP_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_PALADIN_R);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    ///*���߿� �߰�*/
    //m_vecAdjState.push_back(STATE_COUNTER_PALADIN);
    //m_vecAdjState.push_back(STATE_SHIELDATTACK_PALADIN);
    //m_vecAdjState.push_back(STATE_SPINATTACK_PALADIN);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);




    
    return S_OK;
}

void CIdle_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    if (ePrevType == STATE_SWITCH_L_TO_R    ||
        ePrevType == STATE_SPRINT_END_PALADIN
        )
        m_fInterPolationTime = 0.f;
    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Idle�� ���� ���� 
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
