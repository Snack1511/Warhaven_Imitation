#include "stdafx.h"
#include "CState_Blendable.h"

#include "UsefulHeaders.h"

CState_Blendable::CState_Blendable()
{
}

CState_Blendable::~CState_Blendable()
{
}

HRESULT CState_Blendable::Initialize()
{
#define INDEXCHECK(index) if (index >= 9999) return E_FAIL;

	INDEXCHECK(m_iIdle_Index);
	INDEXCHECK(m_iJumpFallIndex);
	INDEXCHECK(m_iFinishedFrame);
	INDEXCHECK(m_iLandIndex);

#define STATECHECK(state) if (state >= STATE_END) return E_FAIL;

	STATECHECK(m_eWalkState);
	STATECHECK(m_eRunState);
	STATECHECK(m_eJumpState);
	STATECHECK(m_eFallState);
	STATECHECK(m_eLandState);
	STATECHECK(m_eIdleState);

	for (_uint i = 0; i < STATE_DIRECTION_END; ++i)
	{
		if (m_iRunAnimIndex[i] <= 0 ||
			m_iWalkAnimIndex[i] <= 0 ||
			m_iJumpAnimIndex[i] <= 0
			)
			return E_FAIL;
	}


	/* Blend Stop Event*/
	Add_KeyFrame(m_iStopIndex, 0);

	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.1f;

	return S_OK;
}

void CState_Blendable::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData )
{
#ifdef _DEBUG
	if (FAILED(Initialize()))
	{
		Call_MsgBox(L"Index 누락 : CState_Blendable");
	}

#endif // _DEBUG

	__super::Enter(pOwner, pAnimator, ePrevStateType);

}

STATE_TYPE CState_Blendable::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eStateType = STATE_END;

	switch (m_eEnum)
	{
	case Client::CState_Blendable::Enum::eWALK:
		if ((eStateType = Update_Walk(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	case Client::CState_Blendable::Enum::eRUN:
		if ((eStateType = Update_Run(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	case Client::CState_Blendable::Enum::eJUMP:
		if ((eStateType = Update_Jump(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	case Client::CState_Blendable::Enum::eFALL:
		if ((eStateType = Update_Fall(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	case Client::CState_Blendable::Enum::eLAND:
		if ((eStateType = Update_Land(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	case Client::CState_Blendable::Enum::eIDLE:
		if ((eStateType = Update_Idle(pOwner, pAnimator)) != STATE_END)
			return eStateType;
		break;
	default:
		break;
	}

	return __super::Tick(pOwner, pAnimator);
}

STATE_TYPE CState_Blendable::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			Move_Cycle(pAnimator, m_iWalkAnimIndex, m_eAnimLeftorRight);
		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);

		}
	}
	else
	{
		Move(Get_Direction(), pOwner);

		if (pAnimator->Is_ActionFinished())
			return m_eWalkState;
	}

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

STATE_TYPE CState_Blendable::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			Move_Cycle(pAnimator, m_iRunAnimIndex, m_eAnimLeftorRight);
		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);
		}
	}
	else
	{
		Move(Get_Direction(), pOwner);

		if (pAnimator->Is_ActionFinished())
			return m_eRunState;
	}

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

STATE_TYPE CState_Blendable::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;
	vCamLook.Normalize();

	pOwner->Get_Transform()->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CState_Blendable::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;
	vCamLook.Normalize();

	pOwner->Get_Transform()->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CState_Blendable::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	Move(Get_Direction(), pOwner);

	if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eIDLE, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eLandState;



	return STATE_END;
}

STATE_TYPE CState_Blendable::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
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
	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else
		if (pAnimator->Is_ActionFinished())
			return m_eIdleState;


	return STATE_END;
}

void CState_Blendable::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
{
	_uint iDirection = Get_Direction();

	if (iDirection == STATE_DIRECTION_END)
		return;

	if (m_iAnimIndex != arrDirectionAnimIndices[iDirection])
	{
		m_iAnimIndex = arrDirectionAnimIndices[iDirection];

		pAnimator->Set_CurAnimIndex(eAnimType, m_iAnimIndex, ANIM_DIVIDE::eBODYLOWER);
		pAnimator->Set_AnimSpeed(eAnimType, m_iAnimIndex, m_iDirectionAnimSpeed[iDirection]);
		pAnimator->Set_InterpolationTime(eAnimType, m_iAnimIndex, m_fInterPolationTime);
	}


	Move(iDirection, m_pOwner);
}

void CState_Blendable::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		m_bBlendable = false;
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		break;


	default:
		break;
	}
}

void CState_Blendable::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CState_Blendable::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CState_Blendable::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CState_Blendable::Enum::eJUMP:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		m_pOwner->Get_PhysicsCom()->Set_Jump(m_pOwner->Get_Status().fJumpPower);
		{
			_uint iDirection = Get_Direction_Four();
			if (iDirection == STATE_DIRECTION_END)
				iDirection = STATE_DIRECTION_NW; // 제자리 점프

			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
			pAnimator->Set_AnimSpeed(m_eAnimLeftorRight, m_iJumpAnimIndex[iDirection], 2.f);
		}
		

		break;
	case Client::CState_Blendable::Enum::eFALL:
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallIndex, ANIM_DIVIDE::eBODYLOWER);
		break;
	case Client::CState_Blendable::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandIndex, ANIM_DIVIDE::eBODYLOWER);
		break;

	case Client::CState_Blendable::Enum::eIDLE:
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);
		break;
	default:
		break;
	}
}
