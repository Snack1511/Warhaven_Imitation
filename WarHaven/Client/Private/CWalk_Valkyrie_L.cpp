#include "stdafx.h"
#include "CWalk_Valkyrie_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Valkyrie_L::CWalk_Valkyrie_L()
{
}

CWalk_Valkyrie_L::~CWalk_Valkyrie_L()
{
}

CWalk_Valkyrie_L* CWalk_Valkyrie_L::Create()
{
    CWalk_Valkyrie_L* pInstance = new CWalk_Valkyrie_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Valkyrie_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_Valkyrie_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_VALKYRIE_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_VALKYRIE);

    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_RUN_VALKYRIE_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_L);




	// ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 31;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 32;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 36;



    


    return S_OK;
}

void CWalk_Valkyrie_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Valkyrie_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_VALKYRIE_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_VALKYRIE_L;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Valkyrie_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Valkyrie_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);
   
}
