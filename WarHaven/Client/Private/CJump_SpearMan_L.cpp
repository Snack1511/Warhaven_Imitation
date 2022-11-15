#include "stdafx.h"
#include "CJump_SpearMan_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_SpearMan_L::CJump_SpearMan_L()
{
}

CJump_SpearMan_L::~CJump_SpearMan_L()
{
}

CJump_SpearMan_L* CJump_SpearMan_L::Create()
{
    CJump_SpearMan_L* pInstance = new CJump_SpearMan_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_SpearMan_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_SPEARMAN_L; // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.05f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;



    m_vecAdjState.push_back(STATE_JUMPFALL_SPEARMAN_L);
    m_vecAdjState.push_back(STATE_JUMP_LAND_SPEARMAN_L);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 7;
    

	iPlaceJumpAnimIndex = 3;


    return S_OK;
}

void CJump_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
