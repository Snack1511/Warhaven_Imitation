#include "stdafx.h"
#include "CQanda_Attack_Begin.h"

#include "UsefulHeaders.h"

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

CQanda_Attack_Begin::CQanda_Attack_Begin()
{
}

CQanda_Attack_Begin::~CQanda_Attack_Begin()
{
}

CQanda_Attack_Begin* CQanda_Attack_Begin::Create()
{
    CQanda_Attack_Begin* pInstance = new CQanda_Attack_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CQanda_Attack_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CQanda_Attack_Begin::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_BEGIN_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 1.3f;
    m_iStateChangeKeyFrame = 999;
   
	m_iAnimFrame = 35;

	Add_KeyFrame(10, 1);
	Add_KeyFrame(m_iAnimFrame, 2);

    return __super::Initialize();
}

void CQanda_Attack_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Attack_Begin(pOwner);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
	
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Charge_Test", (CGameObject*)pAnimCrow
		, ZERO_VECTOR);
}

STATE_TYPE CQanda_Attack_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(LBUTTON, AWAY))
		m_bKeyInputable = true;

	if (pAnimator->Get_CurAnimFrame() > m_iAnimFrame)
		return STATE_ATTACK_AIMING_QANDA;

	if (m_bKeyInput)
	{
		if (pAnimator->Get_CurAnimFrame() > m_iMinCancelAnimIndex)
		{
			if (!m_bKeyInputable || m_bAttackTrigger)
				return STATE_ATTACK_SHOOT_QANDA;
			else
				return STATE_ATTACK_CANCEL_QANDA;
		}
	}

    return __super::Tick(pOwner, pAnimator);
}

void CQanda_Attack_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//Prevent_Oneframe(pOwner);
	//Prevent_Oneframe(pOwner);
	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(16, 0.f, m_fAnimSpeed);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);
	m_pCoreBone->Set_PrevMatrix(static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat());

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CQanda_Attack_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(LBUTTON, TAP))
		return m_eStateType;

    return STATE_END;
}

void CQanda_Attack_Begin::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 1:
		m_bAttackTrigger = true;
		break;

	case 2:
		m_bMoveTrigger = true;
		break;

	default:
		break;
	}
}