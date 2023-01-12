#include "stdafx.h"
#include "CJump_Archer_Land_R.h"


#include "UsefulHeaders.h"


CJump_Archer_Land_R::CJump_Archer_Land_R()
{
}

CJump_Archer_Land_R::~CJump_Archer_Land_R()
{
}

CJump_Archer_Land_R* CJump_Archer_Land_R::Create()
{
    CJump_Archer_Land_R* pInstance = new CJump_Archer_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.15f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
	m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
	m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
	m_vecAdjState.push_back(STATE_RUN_ARCHER_R);


    Init_CommonState_Player();

    return S_OK;
}

void CJump_Archer_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

    _uint iKeyFrame = 20;

    if(ePrevType != STATE_GLIDING_ARCHER)
        pAnimator->Set_CurFrame(iKeyFrame);
    else
        m_iStateChangeKeyFrame = iKeyFrame;
}

STATE_TYPE CJump_Archer_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Archer_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Archer_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
