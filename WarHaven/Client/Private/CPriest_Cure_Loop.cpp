#include "stdafx.h"
#include "CPriest_Cure_Loop.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Priest.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"
#include "CUI_UnitHUD.h"
#include "CUI_UnitHP.h"

CPriest_Cure_Loop::CPriest_Cure_Loop()
{
}

CPriest_Cure_Loop::~CPriest_Cure_Loop()
{
}

CPriest_Cure_Loop* CPriest_Cure_Loop::Create()
{
	CPriest_Cure_Loop* pInstance = new CPriest_Cure_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPriest_Cure_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CPriest_Cure_Loop::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 6;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_CURE_LOOP_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.f;

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 0;


	m_iStopIndex = 712;
	m_iAttackEndIndex = 712;

	m_vecAdjState.push_back(STATE_CURE_END_PRIEST);

	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 1;
	m_iLandRightIndex = 16;
	m_iLandLeftIndex = 16;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 0;


	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 99;

	//m_iRunRightAnimIndex[STATE_DIRECTION_E] = 17;
	//m_iRunRightAnimIndex[STATE_DIRECTION_N] = 18;
	//m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 19;
	//m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 20;
	//m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	//m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	//m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	//m_iRunRightAnimIndex[STATE_DIRECTION_W] = 24;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 3;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 4;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 5;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 6;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 7;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 8;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 9;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 10;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 30;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 31;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 32;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 33;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 37;


	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 99;



	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 11; // 제자리
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99; // 의미없는값 채우기 (0이면 터지게 해놔서)
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;




	m_eWalkState = STATE_WALK_PRIEST;
	m_eJumpState = STATE_JUMP_PRIEST;
	m_eLandState = STATE_JUMP_LAND_PRIEST;
	m_eFallState = STATE_JUMPFALL_PRIEST;
	m_eRunState = STATE_WALK_PRIEST;
	m_eIdleState = STATE_IDLE_PRIEST;
	m_eBounceState = STATE_BOUNCE_PRIEST;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 2.f;

	m_bLandMove = true;

#define INDEXCHECK(index) if (index >= 9999) return E_FAIL;

	INDEXCHECK(m_iIdle_Index);
	INDEXCHECK(m_iJumpFallLeftIndex);
	INDEXCHECK(m_iJumpFallRightIndex);

	INDEXCHECK(m_iLandLeftIndex);
	INDEXCHECK(m_iLandRightIndex);

	INDEXCHECK(m_iFinishedFrame);
	INDEXCHECK(m_iAttackEndIndex);

#define STATECHECK(state) if (state >= STATE_END) return E_FAIL;

	STATECHECK(m_eWalkState);
	STATECHECK(m_eRunState);
	STATECHECK(m_eJumpState);
	STATECHECK(m_eFallState);
	STATECHECK(m_eLandState);
	STATECHECK(m_eIdleState);
	STATECHECK(m_eBounceState);

	for (_uint i = 0; i < STATE_DIRECTION_END; ++i)
	{
		if (m_iRunLeftAnimIndex[i] <= 0 ||
			m_iRunRightAnimIndex[i] <= 0 ||
			m_iWalkLeftAnimIndex[i] <= 0 ||
			m_iWalkRightAnimIndex[i] <= 0 ||
			m_iJumpLeftAnimIndex[i] <= 0 ||
			m_iJumpRightAnimIndex[i] <= 0
			)
			return E_FAIL;
	}




	/* Blend Stop Event*/
	Add_KeyFrame(79, 0);
	Add_KeyFrame(m_iStopIndex, 998);
	Add_KeyFrame(m_iAttackEndIndex, 999);
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.8f, 1000); // war 시작, 끝 사이
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.9f, 1001); // eng


	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.1f;

	m_fMaxTime = 0.5f;

	return S_OK;
}

void CPriest_Cure_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	if (m_eStateType == ePrevStateType)
		m_fInterPolationTime = 0.f;

	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	//pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed;

	__super::Enter(pOwner, pAnimator, ePrevStateType);

	Play_Voice(pOwner, L"Voice_Cure", 1.f);
}

void CPriest_Cure_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(false);

	m_pTargetUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_EFFECTS, m_iSndIdx);
}

STATE_TYPE CPriest_Cure_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_pPrvUnit = m_pCurUnit;
	m_pCurUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	if (m_pPrvUnit != m_pCurUnit)
	{
		if (m_pPrvUnit)
			m_pPrvUnit->Get_OwnerHUD()->Disable_HealBlur();
	}

	if (m_pCurUnit)
		m_pCurUnit->Get_OwnerHUD()->Enable_HealBlur();

	m_pTargetUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	if (!m_pTargetUnit)
		return STATE_CURE_END_PRIEST;

	if (pOwner->Get_SameNearObejct())
	{
		static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(true);
	}
	else
	{
		static_cast<CUnit_Priest*>(pOwner)->TurnOn_CureEffect(false);
	}


	// 타겟 유닛이 힐을 받고 있는 대상
	m_pTargetObject = m_pTargetUnit;

	_float fLength = (m_pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS)).Length();

	if (m_fSndTime <= 0.f)
		m_iSndIdx = CFunctor::Play_LoopSound(L"Effect_Cure_Priest", CHANNEL_EFFECTS);

	m_fSndTime += fDT(0);
	if (m_fSndTime >= m_fMaxSndTime)
		m_fSndTime = 0.f;

	if (fabs(fLength) > pOwner->Get_MaxDistance())
		return STATE_CURE_END_PRIEST;

	m_fTimeAcc += fDT(0);


	if (m_fTimeAcc > m_fMaxTime)
	{
		// 풀피가 아니면
		if (m_pTargetUnit->Get_Status().fHP < m_pTargetUnit->Get_Status().fMaxHP)
		{
			_float fPlusHp = 15.f;

			// UI 표시
			m_pTargetUnit->Get_Status().fHP += fPlusHp; // fPlusHp
			CUser::Get_Instance()->Enable_DamageFont(2, fPlusHp);

			if (pOwner->Get_OwnerPlayer()->Get_Gauge() < 100.f - 3.f)
				pOwner->Get_OwnerPlayer()->Get_Gauge() += 3.f;


			CUser::Get_Instance()->Enable_DamageFont(2, fPlusHp);

			// 풀피를 넘어서면
			if (m_pTargetUnit->Get_Status().fHP > m_pTargetUnit->Get_Status().fMaxHP)
				m_pTargetUnit->Get_Status().fHP = m_pTargetUnit->Get_Status().fMaxHP;

		}

		m_fTimeAcc = 0.f;
	}


	if (pOwner->Is_MainPlayer())
	{
		switch (m_eEnum)
		{
		case Client::CPriest_Cure_Loop::Enum::eWALK:
			Update_Walk(pOwner, pAnimator);

			break;
		case Client::CPriest_Cure_Loop::Enum::eRUN:
			Update_Run(pOwner, pAnimator);

			break;
		case Client::CPriest_Cure_Loop::Enum::eJUMP:
			Update_Jump(pOwner, pAnimator);

			break;
		case Client::CPriest_Cure_Loop::Enum::eFALL:
			Update_Fall(pOwner, pAnimator);

			break;
		case Client::CPriest_Cure_Loop::Enum::eLAND:
			if (m_bLandMove)
				DoMove(Get_Direction(), pOwner);

			Update_Land(pOwner, pAnimator);

			break;
		case Client::CPriest_Cure_Loop::Enum::eIDLE:
			Update_Idle(pOwner, pAnimator);

			break;
		default:
			break;
		}

		Follow_MouseLook(pOwner);
	}

	return __super::Tick(pOwner, pAnimator);
}


STATE_TYPE CPriest_Cure_Loop::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			if (m_eAnimLeftorRight == ANIM_BASE_L)
			{
				Move_Cycle(pAnimator, m_iWalkLeftAnimIndex, m_eAnimLeftorRight);
			}
			else
			{
				Move_Cycle(pAnimator, m_iWalkRightAnimIndex, m_eAnimLeftorRight);
			}

		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);

		}
	}
	else
	{
		//Move(Get_Direction(), pOwner);


	}

	if (pAnimator->Is_ActionFinished())
		return m_eWalkState;

	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(SPACE, TAP))
	{
		On_EnumChange(Enum::eJUMP, pAnimator);
	}


	return eType;
}

STATE_TYPE CPriest_Cure_Loop::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			if (m_eAnimLeftorRight == ANIM_BASE_L)
			{
				Move_Cycle(pAnimator, m_iRunLeftAnimIndex, m_eAnimLeftorRight);
			}
			else
			{
				Move_Cycle(pAnimator, m_iRunRightAnimIndex, m_eAnimLeftorRight);
			}


		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);
		}
	}
	else
	{
		//Move(Get_Direction(), pOwner);


	}

	if (pAnimator->Is_ActionFinished())
		return m_eRunState;

	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(SPACE, TAP))
	{
		On_EnumChange(Enum::eJUMP, pAnimator);
	}


	return eType;
}

STATE_TYPE CPriest_Cure_Loop::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CPriest_Cure_Loop::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CPriest_Cure_Loop::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	On_EnumChange(Enum::eIDLE, pAnimator);

	//Move(Get_Direction(), pOwner);

	//if (m_bLandMove || pAnimator->Is_CurAnimFinished())
	//	On_EnumChange(Enum::eIDLE, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eIdleState;



	return STATE_END;
}

STATE_TYPE CPriest_Cure_Loop::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
	{
		if (KEY(CTRL, HOLD))
		{
			On_EnumChange(Enum::eWALK, pAnimator);
		}
		else
		{
			On_EnumChange(Enum::eRUN, pAnimator);
		}
	}
	else if (KEY(SPACE, TAP))
		On_EnumChange(Enum::eJUMP, pAnimator);

	else
	{
		if (pAnimator->Is_ActionFinished())
			return m_eIdleState;

		On_EnumChange(Enum::eIDLE, pAnimator);
	}



	return STATE_END;
}

void CPriest_Cure_Loop::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}



void CPriest_Cure_Loop::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
{
	_uint iDirection = Get_Direction();

	if (iDirection == STATE_DIRECTION_END)
		return;

	//현재실행중인애만 다르면 댐

	if (m_iAnimIndex != arrDirectionAnimIndices[iDirection])//|| pAnimator->Get_CurCycleAnimIndex() != arrDirectionAnimIndices[iDirection])
	{
		m_iAnimIndex = arrDirectionAnimIndices[iDirection];

		pAnimator->Set_CurAnimIndex(eAnimType, m_iAnimIndex, ANIM_DIVIDE::eBODYLOWER);
		pAnimator->Set_AnimSpeed(eAnimType, m_iAnimIndex, m_fDirectionAnimSpeed[iDirection]);
		pAnimator->Set_InterpolationTime(eAnimType, m_iAnimIndex, 0.05f);
	}


	DoMove(iDirection, m_pOwner);
}



STATE_TYPE CPriest_Cure_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(RBUTTON, NONE))
		return STATE_CURE_END_PRIEST;

	return STATE_END;
}

void CPriest_Cure_Loop::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		pAnimator->Set_InterpolationTime(ANIM_ATTACK, 6, FLT_MAX);
		pAnimator->Set_AnimSpeed(ANIM_ATTACK, 6, FLT_MIN);

		break;

	case 1000:
		/*effect*/
		if (ARCHER == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;


	default:
		break;
	}
}



void CPriest_Cure_Loop::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_fAnimSpeed = 1.f;
	m_iAnimIndex = 0;

	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CPriest_Cure_Loop::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CPriest_Cure_Loop::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CPriest_Cure_Loop::Enum::eJUMP:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		m_pOwner->Get_PhysicsCom()->Set_Jump(m_pOwner->Get_Status().fJumpPower);
		{
			_uint iDirection = Get_Direction_Four();
			if (iDirection == STATE_DIRECTION_END)
				iDirection = STATE_DIRECTION_NW; // 제자리 점프

			if (m_eAnimLeftorRight == ANIM_BASE_L)
				pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpLeftAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
			else
				pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpRightAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
		}


		break;
	case Client::CPriest_Cure_Loop::Enum::eFALL:
		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallLeftIndex, ANIM_DIVIDE::eBODYLOWER);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallRightIndex, ANIM_DIVIDE::eBODYLOWER);

		break;
	case Client::CPriest_Cure_Loop::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;

		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandLeftIndex, ANIM_DIVIDE::eDEFAULT);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandRightIndex, ANIM_DIVIDE::eDEFAULT);

		Create_SoilEffect();
		break;

	case Client::CPriest_Cure_Loop::Enum::eIDLE:
		if (KEY(W, HOLD) && KEY(A, HOLD) && KEY(S, HOLD) && KEY(D, HOLD))
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		break;
	default:
		break;
	}


}
