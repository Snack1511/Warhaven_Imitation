#include "stdafx.h"
#include "CJump_Archer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Archer_R::CJump_Archer_R()
{
}

CJump_Archer_R::~CJump_Archer_R()
{
}

CJump_Archer_R* CJump_Archer_R::Create()
{
    CJump_Archer_R* pInstance = new CJump_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Archer_R::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 12;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;



    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_ARCHER);


    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_JUMPFALL_ARCHER_R);
    m_vecAdjState.push_back(STATE_JUMP_LAND_ARCHER_R);

   /* m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);*/


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 13;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 15;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 16;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 15;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 15;


	iPlaceJumpAnimIndex = 12;


    return S_OK;
}

void CJump_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
