#include "stdafx.h"
#include "CArcher_Attack_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Archer.h"
#include "CDefaultArrow.h"
#include "CAnimWeapon.h"
#include "CUtility_PhysX.h"
#include "HIerarchyNode.h"

CArcher_Attack_Begin::CArcher_Attack_Begin()
{
}

CArcher_Attack_Begin::~CArcher_Attack_Begin()
{
}

CArcher_Attack_Begin* CArcher_Attack_Begin::Create()
{
    CArcher_Attack_Begin* pInstance = new CArcher_Attack_Begin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CArcher_Attack_Begin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CArcher_Attack_Begin::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_BEGIN_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 999;
   

	Add_KeyFrame(31, 1);
	Add_KeyFrame(90, 2);



    return __super::Initialize();
}

void CArcher_Attack_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Attack_Begin(pOwner);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CArcher_Attack_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(LBUTTON, AWAY))
		m_bKeyInputable = true;

	if (m_bMoveTrigger)
		return STATE_ATTACK_AIMING_ARCHER;

	if (m_bKeyInput)
	{
		if (pAnimator->Get_CurAnimFrame() > m_iMinCancelAnimIndex)
		{
			if (!m_bKeyInputable || m_bAttackTrigger)
				return STATE_ATTACK_SHOOT_ARCHER;
			else
				return STATE_ATTACK_CANCEL_ARCHER;
		}
	}

    return __super::Tick(pOwner, pAnimator);
}

void CArcher_Attack_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	Prevent_Oneframe(pOwner);
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLOOP, FLT_MAX, FLT_MIN);
	m_pCoreBone->Set_PrevMatrix(static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat());

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CArcher_Attack_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
	{
		if (KEY(LBUTTON, TAP))
			return m_eStateType;
	}

    return STATE_END;
}

void CArcher_Attack_Begin::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
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