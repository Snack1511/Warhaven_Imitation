#include "stdafx.h"
#include "CPriest_Catching.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Priest.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"
#include "CScript_FollowCam.h"

#include "CProjectile.h"
#include "Easing_Utillity.h"
#include "CCannonBall.h"
CPriest_Catching::CPriest_Catching()
{
}

CPriest_Catching::~CPriest_Catching()
{
}

CPriest_Catching* CPriest_Catching::Create()
{
	CPriest_Catching* pInstance = new CPriest_Catching();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPriest_Catching");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CPriest_Catching::Initialize()
{
	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 10;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_PROJECTILECATCH_HIT_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = FLT_MIN;

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 0;


	m_iStopIndex = 712;
	m_iAttackEndIndex = 712;

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

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 17;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 18;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 34;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 24;


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

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.f;

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
	Add_KeyFrame(m_iStopIndex, 998);
	Add_KeyFrame(m_iAttackEndIndex, 999);
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.8f, 1000); // war 시작, 끝 사이
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.9f, 1001); // eng


	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.1f;

	m_fMaxTime = 0.05f;

	return S_OK;
}

void CPriest_Catching::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(true);
	//GAMEINSTANCE->Start_RadialBlur(0.017f);
	GAMEINSTANCE->Start_ChromaticAberration(15.f);
	pOwner->Lerp_Camera(5);

	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed;

	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

void CPriest_Catching::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(false);
	//GAMEINSTANCE->Stop_RadialBlur();
	GAMEINSTANCE->Stop_ChromaticAberration();
	pOwner->Lerp_Camera(0);

	pOwner->On_Use(CUnit::SKILL3);

	if (pOwner->Get_CatchProjectileObject())
		pOwner->Get_CatchProjectileObject()->Use_Collect(false);

	if (pOwner->Get_CatchedBall())
		DISABLE_GAMEOBJECT(pOwner->Get_CatchedBall());

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CPriest_Catching::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Shake_Camera(0.2f, 0.1f);

	//if (!pProjectile)
	//	return STATE_IDLE_PRIEST;
	//else
	//{
	//	pProjectile->Get_Transform()->Set_World(WORLD_LOOK, pOwner->Get_Transform()->Get_World(WORLD_LOOK));
	//}

	if (KEY(Q, AWAY))
		return STATE_PROJECTILECATCH_SHOOT_PRIEST;
		

	if (pOwner->Is_MainPlayer())
	{
		switch (m_eEnum)
		{
		case Client::CPriest_Catching::Enum::eWALK:
			Update_Walk(pOwner, pAnimator);

			break;
		case Client::CPriest_Catching::Enum::eRUN:
			Update_Run(pOwner, pAnimator);

			break;
		case Client::CPriest_Catching::Enum::eJUMP:
			Update_Jump(pOwner, pAnimator);

			break;
		case Client::CPriest_Catching::Enum::eFALL:
			Update_Fall(pOwner, pAnimator);

			break;
		case Client::CPriest_Catching::Enum::eLAND:
			if (m_bLandMove)
				DoMove(Get_Direction(), pOwner);

			Update_Land(pOwner, pAnimator);

			break;
		case Client::CPriest_Catching::Enum::eIDLE:
			Update_Idle(pOwner, pAnimator);

			break;
		default:
			break;
		}

		Follow_MouseLook_Turn(pOwner);
	}

	return __super::Tick(pOwner, pAnimator);
}

STATE_TYPE CPriest_Catching::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CPriest_Catching::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CPriest_Catching::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CPriest_Catching::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CPriest_Catching::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	//Move(Get_Direction(), pOwner);


	if (m_bLandMove || pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eWALK, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eIdleState;



	return STATE_END;
}

STATE_TYPE CPriest_Catching::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
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

void CPriest_Catching::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}



void CPriest_Catching::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
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
		pAnimator->Set_InterpolationTime(eAnimType, m_iAnimIndex, m_fInterPolationTime);
	}


	DoMove(iDirection, m_pOwner);
}



STATE_TYPE CPriest_Catching::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(RBUTTON, NONE))
		return STATE_CURE_END_PRIEST;

	return STATE_END;
}

void CPriest_Catching::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
		//case 998:

		//	m_bAfterEffect = true;
		//	m_bHitEffect = true;
		//	pOwner->TurnOn_TrailEffect(true);

		//	/* dash Front */
		//	if (!pOwner->Is_Air())
		//	{
		//		pOwner->Set_DirAsLook();
		//		pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
		//		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		//		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
		//		
		//	}

		//	

		//	m_bBlendable = false;
		//	if (m_eEnum == Enum::eWALK || m_eEnum == Enum::eRUN)
		//	pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		//	break;

		//case 999:

		//	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
		//	m_bAfterEffect = false;
		//	m_bBlood = false;
		//	pOwner->TurnOn_TrailEffect(false);

		//	//m_bBlendable = true;
		//	if (m_eAnimLeftorRight == ANIM_BASE_L)
		//		m_eAnimLeftorRight = ANIM_BASE_R;
		//	else
		//		m_eAnimLeftorRight = ANIM_BASE_L;


		//	break;

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



void CPriest_Catching::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_fAnimSpeed = 1.f;

	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CPriest_Catching::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CPriest_Catching::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CPriest_Catching::Enum::eJUMP:
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
	case Client::CPriest_Catching::Enum::eFALL:
		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallLeftIndex, ANIM_DIVIDE::eBODYLOWER);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallRightIndex, ANIM_DIVIDE::eBODYLOWER);

		break;
	case Client::CPriest_Catching::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;

		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandLeftIndex, ANIM_DIVIDE::eDEFAULT);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandRightIndex, ANIM_DIVIDE::eDEFAULT);

		Create_SoilEffect();
		break;

	case Client::CPriest_Catching::Enum::eIDLE:
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);
		break;
	default:
		break;
	}

	
}
