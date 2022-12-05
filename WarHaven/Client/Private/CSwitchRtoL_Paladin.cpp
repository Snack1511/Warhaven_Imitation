#include "stdafx.h"
#include "CSwitchRtoL_Paladin.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CSwitchRtoL_Paladin::CSwitchRtoL_Paladin()
{
}

CSwitchRtoL_Paladin::~CSwitchRtoL_Paladin()
{
}

CSwitchRtoL_Paladin* CSwitchRtoL_Paladin::Create()
{
    CSwitchRtoL_Paladin* pInstance = new CSwitchRtoL_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchRtoL_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchRtoL_Paladin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 44;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_R_TO_L_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);

    m_vecAdjState.push_back(STATE_IDLE_PALADIN_L);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_L);
    m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
    m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    ///*���߿� �߰�*/
    //m_vecAdjState.push_back(STATE_COUNTER_PALADIN);
    //m_vecAdjState.push_back(STATE_SHIELDATTACK_PALADIN);
    //m_vecAdjState.push_back(STATE_SPINATTACK_PALADIN);


    return S_OK;
}

void CSwitchRtoL_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchRtoL_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	/*if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
        return STATE_CHARGE_FRONT_PALADIN_L;


    if(MOUSE_MOVE(MMS_WHEEL) > 0)
        return STATE_VERTICALATTACK_PALADIN_L;*/

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchRtoL_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSwitchRtoL_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::CTRL)
        return m_eStateType;

    return STATE_END;
}
