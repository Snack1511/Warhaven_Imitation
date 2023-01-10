#include "stdafx.h"
#include "CIdle_Lancer.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"
#include "CScript_FollowCam.h"

CIdle_Lancer::CIdle_Lancer()
{
}

CIdle_Lancer::~CIdle_Lancer()
{
}

CIdle_Lancer* CIdle_Lancer::Create()
{
    CIdle_Lancer* pInstance = new CIdle_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Lancer::Initialize()
{


    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_JUMP_LANCER);
    m_vecAdjState.push_back(STATE_RUN_LANCER);

    m_vecAdjState.push_back(STATE_ATTACK_STING_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_DEAFULT_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_BREEZE_BEGIN_LANCER);


    
    return S_OK;
}

void CIdle_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{    
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_LANCER);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    
}

STATE_TYPE CIdle_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* LANCER�� Idle�� ���� ���� 
    1. ���� �������� �ִϸ��̼��� ������ ��
    */
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
