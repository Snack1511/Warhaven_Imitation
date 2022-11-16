#include "stdafx.h"
#include "CRun_SpearMan_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_SpearMan_R::CRun_SpearMan_R()
{
}

CRun_SpearMan_R::~CRun_SpearMan_R()
{
}

CRun_SpearMan_R* CRun_SpearMan_R::Create()
{
    CRun_SpearMan_R* pInstance = new CRun_SpearMan_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_SpearMan_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_SpearMan_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 30;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_SPEARMAN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
	

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 26;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 27;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 28;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 29;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 41;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 42;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 43;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 30;


    return S_OK;
}

void CRun_SpearMan_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */
    CColorController::COLORDESC m_tColorDesc;
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


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);



    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_SpearMan_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{
		return STATE_STOP_SPEARMAN_R;
	}

    

    return __super::Tick(pOwner, pAnimator);
}

void CRun_SpearMan_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_SpearMan_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. CTRL �� ������ ���� ����
    2. WASD �� ���� ����
    */

	return __super::Check_Condition(pOwner, pAnimator);
}

