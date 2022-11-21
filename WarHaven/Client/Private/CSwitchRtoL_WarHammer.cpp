#include "stdafx.h"
#include "CSwitchRtoL_WarHammer.h"

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

CSwitchRtoL_WarHammer::CSwitchRtoL_WarHammer()
{
}

CSwitchRtoL_WarHammer::~CSwitchRtoL_WarHammer()
{
}

CSwitchRtoL_WarHammer* CSwitchRtoL_WarHammer::Create()
{
    CSwitchRtoL_WarHammer* pInstance = new CSwitchRtoL_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchRtoL_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchRtoL_WarHammer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 54;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_R_TO_L_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 20;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_RUN_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);


    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);


    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


    return S_OK;
}

void CSwitchRtoL_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchRtoL_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
        return STATE_CHARGE_FRONT_WARHAMMER_L;


    if(MOUSE_MOVE(MMS_WHEEL) > 0)
        return STATE_VERTICALATTACK_WARHAMMER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchRtoL_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSwitchRtoL_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::CTRL)
        return m_eStateType;

    return STATE_END;
}
