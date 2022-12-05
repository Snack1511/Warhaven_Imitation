#include "stdafx.h"
#include "CJump_Archer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Archer_L::CJump_Archer_L()
{
}

CJump_Archer_L::~CJump_Archer_L()
{
}

CJump_Archer_L* CJump_Archer_L::Create()
{
    CJump_Archer_L* pInstance = new CJump_Archer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_ARCHER_L; // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;



    m_vecAdjState.push_back(STATE_JUMPFALL_ARCHER_L);
    m_vecAdjState.push_back(STATE_JUMP_LAND_ARCHER_L);

  /*  m_vecAdjState.push_back(STATE_ATTACK_STING_ARCHER_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);*/

 
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 7;


    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 6;

	iPlaceJumpAnimIndex = 3;


    return S_OK;
}

void CJump_Archer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Archer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_Archer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Archer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
