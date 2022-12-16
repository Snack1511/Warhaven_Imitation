#include "stdafx.h"
#include "CState_Combat_Default_Paladin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CPlayer.h"

#include "CUser.h"

CState_Combat_Default_Paladin_R::CState_Combat_Default_Paladin_R()
{
}

CState_Combat_Default_Paladin_R::~CState_Combat_Default_Paladin_R()
{
}

CState_Combat_Default_Paladin_R* CState_Combat_Default_Paladin_R::Create()
{
    CState_Combat_Default_Paladin_R* pInstance = new CState_Combat_Default_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Paladin_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEAFULT_PALADIN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    m_fAnimSpeed = 2.5f;

    // ���ĺ� �� �ִϸ��̼� ����
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;

    m_iAINextState = AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R;

    return S_OK;
}

void CState_Combat_Default_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) < m_fAIMyLength)
    {

        switch (m_iRand)
        {
        case 0:
        case 1:


            return AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R;

        case 2:
        case 3:

            return AI_STATE_COMBAT_VERTICALCUT_PALADIN;

        case 4:



            if (m_ePreStateType != AI_STATE_COMBAT_GUARDBEGIN_PALADIN)
                return AI_STATE_COMBAT_GUARDBEGIN_PALADIN;
            else
                return m_eStateType;

        case 5:

            if (pOwner->Can_Use(CUnit::SKILL1))
                return AI_STATE_COMBAT_RUSH_BEGIN_PALADIN;
            else
                return AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R;

        case 6:

            if (pOwner->Can_Use(CUnit::SKILL2))
                return AI_STATE_COMBAT_SHIELDWALL_BEGIN_PALADIN;
            else
                return AI_STATE_COMBAT_GUARDBEGIN_PALADIN;

        case 7:

            if (pOwner->Can_Use(CUnit::SKILL2))
                return AI_STATE_COMBAT_SHIELDSLAM_PALADIN;
            else
                return AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_L;


        default:
            break;
        }
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
