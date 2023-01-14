#include "stdafx.h"
#include "CPaladin_ShieldWall_Begin.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CPaladin_ShieldWall_Begin::CPaladin_ShieldWall_Begin()
{
}

CPaladin_ShieldWall_Begin::~CPaladin_ShieldWall_Begin()
{
}

CPaladin_ShieldWall_Begin* CPaladin_ShieldWall_Begin::Create()
{
	CPaladin_ShieldWall_Begin* pInstance = new CPaladin_ShieldWall_Begin();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPaladin_ShieldWall_Begin");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CPaladin_ShieldWall_Begin::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.5f;
	m_tHitInfo.fJumpPower = 1.2f;
	m_tHitInfo.bFly = true;
	m_tHitInfo.iLandKeyFrame = 20;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHIELDWALL_BEGIN_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.5f;

	Add_KeyFrame(30, 1);
	Add_KeyFrame(35, 2);
	//Add_KeyFrame(, 222);
	

	return S_OK;
}

void CPaladin_ShieldWall_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CPaladin_ShieldWall_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	
	if (pAnimator->Is_CurAnimFinished())
	{
		return STATE_SHIELDWALL_LOOP_PALADIN;
	}


	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();
	return __super::Tick(pOwner, pAnimator);
}

void CPaladin_ShieldWall_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
}

STATE_TYPE CPaladin_ShieldWall_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (!pOwner->Can_Use(CUnit::SKILL2))
		return STATE_END;

	if (KEY(E, HOLD))
		return m_eStateType;


	return STATE_END;
}

void CPaladin_ShieldWall_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, true);
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ShieldWall_0"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		Play_Voice(pOwner, L"Voice_Wall", 1.f);
		break;
	case 2:
		pOwner->Enable_GuardBreakCollider(CUnit::GUARDBREAK_R, false);
		break;
	default:
		break;
	}
}
