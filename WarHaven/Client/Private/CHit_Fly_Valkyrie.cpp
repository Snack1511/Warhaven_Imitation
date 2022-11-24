#include "stdafx.h"
#include "CHit_Fly_Valkyrie.h"

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

#include "CUtility_Transform.h"

CHit_Fly_Valkyrie::CHit_Fly_Valkyrie()
{
}

CHit_Fly_Valkyrie::~CHit_Fly_Valkyrie()
{
}

CHit_Fly_Valkyrie* CHit_Fly_Valkyrie::Create()
{
    CHit_Fly_Valkyrie* pInstance = new CHit_Fly_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Fly_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHit_Fly_Valkyrie::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iFlyHitIndex;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_FLYHIT_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.f;


    return S_OK;
}

void CHit_Fly_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Fly_State();

    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Fly_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Get_CurAnimFrame() > m_tHitInfo.iLandKeyFrame)
		return STATE_JUMPFALL_VALKYRIE_R;

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Fly_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CHit_Fly_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ����
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}
