#include "stdafx.h"
#include "CWarrior_GuardBreak.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CWarrior_GuardBreak::CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak::~CWarrior_GuardBreak()
{
}

CWarrior_GuardBreak* CWarrior_GuardBreak::Create()
{
    CWarrior_GuardBreak* pInstance = new CWarrior_GuardBreak();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_GuardBreak::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 15;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PLAYER_SKILL2;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 77;
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);
    //ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    //tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    //tColorDesc.fFadeInStartTime = 1.f;
    //tColorDesc.fFadeInTime = 1.f;
    //tColorDesc.fFadeOutStartTime = 1.f;
    //tColorDesc.fFadeOutTime = 1.f;
    //tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 1.f);
    //tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    //tColorDesc.iStartKeyFrame = 10;
    //tColorDesc.iEndKeyFrame = 30;

    return S_OK;
}

void CWarrior_GuardBreak::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
{
    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CWarrior_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : ��� ����(R Attack)
    // 23 : Ȳ�� ����(SKill)
    //Create_SwordEffect(pOwner);

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PLAYER_R;


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWarrior_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  ��ų��ư �� �̿��� �����Ѵ�.
    */

    if (KEY(T, TAP))
        return m_eStateType;


    return STATE_END;
}
