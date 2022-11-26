#include "stdafx.h"
#include "CState_AI_Warrior_Blendable.h"

#include "UsefulHeaders.h"

#include "CSword_Effect.h"
#include "CColorController.h"

CState_AI_Warrior_Blendable::CState_AI_Warrior_Blendable()
{
}

CState_AI_Warrior_Blendable::~CState_AI_Warrior_Blendable()
{
}

HRESULT CState_AI_Warrior_Blendable::Initialize()
{
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

	return S_OK;
}

void	CState_AI_Warrior_Blendable::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
}

void	CState_AI_Warrior_Blendable::OnCollisionStay(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType)
{
}

void CState_AI_Warrior_Blendable::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData )
{
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::KEYFRAME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (222.f / 255.f), (42.f / 255.f), 0.f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
	tColorDesc.iStartKeyFrame = 2;
	tColorDesc.iEndKeyFrame = m_iStopIndex; // 프레임 맞춰놓음

	GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(tColorDesc);

	//pOwner->CallBack_CollisionEnter += bind(&CState_AI_Warrior_Blendable::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	//pOwner->CallBack_CollisionStay += bind(&CState_AI_Warrior_Blendable::OnCollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);

	__super::Enter(pOwner, pAnimator, ePrevStateType);

}

void CState_AI_Warrior_Blendable::Exit(CUnit * pOwner, CAnimator * pAnimator)
{
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->TurnOn_TrailEffect(false);

	//pOwner->CallBack_CollisionEnter -= bind(&CState_AI_Warrior_Blendable::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	//pOwner->CallBack_CollisionStay -= bind(&CState_AI_Warrior_Blendable::OnCollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);

}

STATE_TYPE CState_AI_Warrior_Blendable::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eStateType = STATE_END;

	// Create_SwordAfterEffect();

	//switch (m_eEnum)
	//{
	//case Client::CState_AI_Warrior_Blendable::Enum::eWALK:
	//	if ((eStateType = Update_Walk(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//case Client::CState_AI_Warrior_Blendable::Enum::eRUN:
	//	if ((eStateType = Update_Run(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//case Client::CState_AI_Warrior_Blendable::Enum::eJUMP:
	//	if ((eStateType = Update_Jump(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//case Client::CState_AI_Warrior_Blendable::Enum::eFALL:
	//	if ((eStateType = Update_Fall(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//case Client::CState_AI_Warrior_Blendable::Enum::eLAND:
	//	if ((eStateType = Update_Land(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//case Client::CState_AI_Warrior_Blendable::Enum::eIDLE:
	//	if ((eStateType = Update_Idle(pOwner, pAnimator)) != STATE_END)
	//		return eStateType;
	//	break;
	//default:
	//	break;
	//}

	//Follow_MouseLook_Turn(pOwner);

	return __super::Tick(pOwner, pAnimator);
}

STATE_TYPE CState_AI_Warrior_Blendable::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CState_AI_Warrior_Blendable::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if(m_bAIMove)
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

STATE_TYPE CState_AI_Warrior_Blendable::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CState_AI_Warrior_Blendable::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CState_AI_Warrior_Blendable::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	//Move(Get_Direction(), pOwner);

	if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eIDLE, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eIdleState;



	return STATE_END;
}

STATE_TYPE CState_AI_Warrior_Blendable::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (!m_bAIMove)
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
		if (pAnimator->Is_ActionFinished())
			return m_eIdleState;


	return STATE_END;
}

void CState_AI_Warrior_Blendable::Create_SwordAfterEffect()
{
	m_fCreateTimeAcc += fDT(0);

	_float fTargetTime = m_fCreateTimeSlow;

	if (m_bAfterEffect)
		fTargetTime = m_fCreateTime;


	if (m_fCreateTimeAcc >= fTargetTime)
	{
		m_fCreateTimeAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CSword_Effect), m_pOwner,
			m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	}


	
}

void CState_AI_Warrior_Blendable::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}

void CState_AI_Warrior_Blendable::Create_BloodEffect()
{
	

	_float fTargetTime = 0.02f;

	if (m_bBlood)
	{
		m_fCreateTimeAcc += fDT(0);
		if (m_fCreateTimeAcc >= fTargetTime)
		{
			m_fCreateTimeAcc = 0.f;
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BloodEffect", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));

		}
	}

}



void CState_AI_Warrior_Blendable::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
{
	_uint iDirection = Get_Direction();

	if (iDirection == STATE_DIRECTION_END)
		return;

	//현재실행중인애만 다르면 댐

	if (m_iAnimIndex != arrDirectionAnimIndices[iDirection] )//|| pAnimator->Get_CurCycleAnimIndex() != arrDirectionAnimIndices[iDirection])
	{
		m_iAnimIndex = arrDirectionAnimIndices[iDirection];

		pAnimator->Set_CurAnimIndex(eAnimType, m_iAnimIndex, ANIM_DIVIDE::eBODYLOWER);
		pAnimator->Set_AnimSpeed(eAnimType, m_iAnimIndex, m_fDirectionAnimSpeed[iDirection]);
		pAnimator->Set_InterpolationTime(eAnimType, m_iAnimIndex, m_fInterPolationTime);
	}


	DoMove(iDirection, m_pOwner);
}

void CState_AI_Warrior_Blendable::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 998:

		m_bAfterEffect = true;
		m_bHitEffect = true;
		pOwner->TurnOn_TrailEffect(true);

		/* dash Front */
		if (!pOwner->Is_Air())
		{
			pOwner->Set_DirAsLook();
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();
			pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
			
		}

		

		m_bBlendable = false;
		if (m_eEnum == Enum::eWALK || m_eEnum == Enum::eRUN)
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		break;

	case 999:

		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
		m_bAfterEffect = false;
		m_bBlood = false;
		pOwner->TurnOn_TrailEffect(false);

		//m_bBlendable = true;
		if (m_eAnimLeftorRight == ANIM_BASE_L)
			m_eAnimLeftorRight = ANIM_BASE_R;
		else
			m_eAnimLeftorRight = ANIM_BASE_L;


		break;

	case 1000:
		/*effect*/
		if(CUnit::CLASS_TYPE::WARRIOR == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;
	case 1001:
		/*effect*/
		if (CUnit::CLASS_TYPE::ENGINEER == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;

	default:
		break;
	}
}

void CState_AI_Warrior_Blendable::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CState_AI_Warrior_Blendable::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CState_AI_Warrior_Blendable::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CState_AI_Warrior_Blendable::Enum::eJUMP:
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
	case Client::CState_AI_Warrior_Blendable::Enum::eFALL:
		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallLeftIndex, ANIM_DIVIDE::eBODYLOWER);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallRightIndex, ANIM_DIVIDE::eBODYLOWER);

		break;
	case Client::CState_AI_Warrior_Blendable::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;

		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandLeftIndex, ANIM_DIVIDE::eDEFAULT);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandRightIndex, ANIM_DIVIDE::eDEFAULT);

		Create_SoilEffect();
		break;

	case Client::CState_AI_Warrior_Blendable::Enum::eIDLE:
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);
		break;
	default:
		break;
	}
}
