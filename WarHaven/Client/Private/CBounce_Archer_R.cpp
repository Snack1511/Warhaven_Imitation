#include "stdafx.h"
#include "CBounce_Archer_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CBounce_Archer_R::CBounce_Archer_R()
{
}

CBounce_Archer_R::~CBounce_Archer_R()
{
}

CBounce_Archer_R* CBounce_Archer_R::Create()
{
    CBounce_Archer_R* pInstance = new CBounce_Archer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBounce_Archer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CBounce_Archer_R::Initialize()
{
	__super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_BOUNCE_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_vecAdjState.push_back(STATE_IDLE_ARCHER_R);
    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_ARCHER_R);
    m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);

    return S_OK;
}

void CBounce_Archer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CBounce_Archer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CBounce_Archer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CBounce_Archer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  �����ϴ� ���� ƨ���
    */

	return __super::Check_Condition(pOwner, pAnimator);

}