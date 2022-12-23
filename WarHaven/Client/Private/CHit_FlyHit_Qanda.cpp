#include "stdafx.h"
#include "CHit_FlyHit_Qanda.h"

#include "GameInstance.h"
#include "Physics.h"
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
#include "CScript_FollowCam.h"

#include "CUtility_Transform.h"

CHit_FlyHit_Qanda::CHit_FlyHit_Qanda()
{
}

CHit_FlyHit_Qanda::~CHit_FlyHit_Qanda()
{
}

CHit_FlyHit_Qanda* CHit_FlyHit_Qanda::Create()
{
    CHit_FlyHit_Qanda* pInstance = new CHit_FlyHit_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_FlyHit_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHit_FlyHit_Qanda::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iFlyHitIndex;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_FLYHIT_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.f;


    return S_OK;
}

void CHit_FlyHit_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);

    pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
    pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

    /* �� �������� hit info�� �޾Ҵ�. */

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Fly_State();

    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_FlyHit_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_tHitInfo.iLandKeyFrame)
		return STATE_JUMPFALL_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_FlyHit_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CHit_FlyHit_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}
