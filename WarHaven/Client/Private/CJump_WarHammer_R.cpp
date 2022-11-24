#include "stdafx.h"
#include "CJump_WarHammer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_WarHammer_R::CJump_WarHammer_R()
{
}

CJump_WarHammer_R::~CJump_WarHammer_R()
{
}

CJump_WarHammer_R* CJump_WarHammer_R::Create()
{
    CJump_WarHammer_R* pInstance = new CJump_WarHammer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_WarHammer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_WarHammer_R::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 15;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_WARHAMMER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;



    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_WARHAMMER);


    m_vecAdjState.push_back(STATE_JUMPFALL_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_LAND_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 16;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 19;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 18;


	iPlaceJumpAnimIndex = 15;


    return S_OK;
}

void CJump_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
