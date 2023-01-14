#include "stdafx.h"
#include "CState_Combat_Attack_Default_Begin_Qanda.h"

#include "UsefulHeaders.h"

#include "CAnimWeapon_Crow.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_QANDA.h"
#include "CDefaultArrow.h"
#include "CAnimWeapon.h"
#include "CUtility_PhysX.h"
#include "HIerarchyNode.h"

CState_Combat_Attack_Default_Begin_Qanda::CState_Combat_Attack_Default_Begin_Qanda()
{
}

CState_Combat_Attack_Default_Begin_Qanda::~CState_Combat_Attack_Default_Begin_Qanda()
{
}

CState_Combat_Attack_Default_Begin_Qanda* CState_Combat_Attack_Default_Begin_Qanda::Create()
{
    CState_Combat_Attack_Default_Begin_Qanda* pInstance = new CState_Combat_Attack_Default_Begin_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Default_Begin_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Default_Begin_Qanda::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_BEGIN_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 1.3f;
    m_iStateChangeKeyFrame = 999;
   
	m_fAIMyLength = 2.f;

    return __super::Initialize();
}

void CState_Combat_Attack_Default_Begin_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Attack_Begin(pOwner);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Default_Begin_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_AIMING_QANDA;


	if (pAnimator->Get_CurAnimFrame() > 40)
	{
		if(Get_TargetLook_Length(pOwner) < m_fAIMyLength)
			return AI_STATE_COMBAT_SHOOT_QANDA;
	}

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Default_Begin_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(16, 0.f, m_fAnimSpeed);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);

	m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	if (!m_pCoreBone)
		assert(0);

	m_pCoreBone->Set_PrevMatrix(static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat());

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Default_Begin_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Default_Begin_Qanda::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 1:

		break;

	case 2:

		break;

	default:
		break;
	}
}