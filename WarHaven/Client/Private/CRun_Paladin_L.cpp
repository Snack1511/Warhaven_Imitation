#include "stdafx.h"
#include "CRun_Paladin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Paladin_L::CRun_Paladin_L()
{
}

CRun_Paladin_L::~CRun_Paladin_L()
{
}

CRun_Paladin_L* CRun_Paladin_L::Create()
{
    CRun_Paladin_L* pInstance = new CRun_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Paladin_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 19;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_PALADIN_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);

    m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
    m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);



   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;


    return S_OK;
}

void CRun_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */
  /*  CColorController::COLORDESC m_tColorDesc;
    ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

    m_tColorDesc.eFadeStyle = CColorController::TIME;
    m_tColorDesc.fFadeInStartTime = 1.f;
    m_tColorDesc.fFadeInTime = 1.f;
    m_tColorDesc.fFadeOutStartTime = 1.f;
    m_tColorDesc.fFadeOutTime = 1.f;

    m_tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 0.f);

    m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    m_tColorDesc.iStartKeyFrame = 3;
    m_tColorDesc.iEndKeyFrame = 6;


    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(m_tColorDesc);*/



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if (pOwner->Is_Air())
        return STATE_JUMPFALL_PALADIN_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        _uint*   pInt = new _uint;
        *pInt = m_iCurDirection;
        pOwner->Enter_State(STATE_STOP_PALADIN_L, (void*)pInt);
        return STATE_END;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Walk�� ���� ����
    2. WAD �� ���� ����
    */

	return __super::Check_Condition(pOwner, pAnimator);
}
