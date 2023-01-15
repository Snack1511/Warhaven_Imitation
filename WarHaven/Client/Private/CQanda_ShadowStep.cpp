#include "stdafx.h"
#include "CQanda_ShadowStep.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"
#include "CPlayer.h"

#include "Model.h"

#include "CUser.h"
#include "Renderer.h"
#include "CColorController.h"

#include "CEffects_Factory.h"
#include "CRectEffects.h"
#include "CAnimWeapon.h"
#include "Transform.h"
#include "Functor.h"

CQanda_ShadowStep::CQanda_ShadowStep()
{
}

CQanda_ShadowStep::~CQanda_ShadowStep()
{
}

CQanda_ShadowStep* CQanda_ShadowStep::Create()
{
	CQanda_ShadowStep* pInstance = new CQanda_ShadowStep();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_ShadowStep");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_ShadowStep::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SHADOWSTEP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_iStateChangeKeyFrame = 0;

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;


	// Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
	//m_vecAdjState.push_back(STATE_WALK_QANDA);
	//m_vecAdjState.push_back(STATE_JUMP_QANDA);

	//m_vecAdjState.push_back(STATE_ATTACK_BEGIN_QANDA);
	//m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_QANDA);

	//m_vecAdjState.push_back(STATE_SPRINT_BEGIN_QANDA);

	//m_vecAdjState.push_back(STATE_GUARD_QANDA);
	//m_vecAdjState.push_back(STATE_SHADOWSTEP_QANDA);

	/*m_vecAdjState.push_back(STATE_ATTACK_STING_QANDA);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);*/


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 8;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 9;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 10;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 11;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 12;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	return S_OK;
}

void CQanda_ShadowStep::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

	DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner->Get_AnimWeapon(), CRenderer));
	DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));

	pOwner->Enable_HitBoxColliders(false);

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dodge", pOwner, ZERO_VECTOR);
	static_cast<CUnit_Qanda*>(pOwner)->Turn_TransformParticle(false);
	static_cast<CUnit_Qanda*>(pOwner)->TurnOn_Trail(false);

	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

	m_fWalkSpeed = pOwner->Get_Status().fWalkSpeed; //기존 걷는 속도
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fSprintSpeed;
	//D3D11_RENDER_TARGET_BLEND_DESC

	Play_Voice(pOwner, L"Voice_Shadow", 1.f);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Sound(L"Effect_ShadowStep_Begin_Qanda", CHANNEL_EFFECTS);
}


STATE_TYPE CQanda_ShadowStep::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iDirection = Get_Direction();
	_bool bStop = false;

	if (iDirection != STATE_DIRECTION_END)
		DoMove(iDirection, pOwner);

	if (KEY(E, TAP))
		bStop = true;

	if (!pOwner->Is_Air() && KEY(SPACE, TAP))
	{
		pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);
	}

	if (m_fSndTime < 0.f)
		m_iSndIdx = CFunctor::Play_LoopSound(L"Effect_ShadowStep_Loop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	m_fSndTime += fDT(0);
	if (m_fSndTime >= 3.f)
		m_fSndTime = 0.f;

	m_fCurGuageMinusTime += fDT(0);

	if (m_fCurGuageMinusTime > m_fMaxGuageMinusTime)
	{
		--pOwner->Get_OwnerPlayer()->Get_Gauge();
		m_fCurGuageMinusTime = 0.f;
	}

	//_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;

	if (bStop)
	{
		if (pOwner->Is_Air())
			return STATE_JUMPFALL_QANDA;

		else if (
			KEY(W, NONE) &&
			KEY(A, NONE) &&
			KEY(S, NONE) &&
			KEY(D, NONE)
			)
		{
			_uint* pInt = new _uint;
			*pInt = m_iCurDirection;
			pOwner->Enter_State(STATE_STOP_QANDA, (void*)pInt);
			return STATE_END;

		}

		else
			return STATE_RUN_QANDA;
	}


	return __super::Tick(pOwner, pAnimator);
}

void CQanda_ShadowStep::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_Status().fWalkSpeed = m_fWalkSpeed;

	pOwner->Enable_HitBoxColliders(true);
	ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));
	ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner->Get_AnimWeapon(), CRenderer));


	TurnOff_DodgeEffect(pOwner);
	static_cast<CUnit_Qanda*>(pOwner)->Turn_TransformParticle(true);
	static_cast<CUnit_Qanda*>(pOwner)->TurnOn_Trail(true);
}

STATE_TYPE CQanda_ShadowStep::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(E, TAP))
		return m_eStateType;

	return STATE_END;
}

void CQanda_ShadowStep::TurnOff_DodgeEffect(CUnit* pOwner)
{
	for (auto& elem : m_EffectsList)
	{
		static_cast<CRectEffects*>(elem)->Set_AllFadeOut(0.3f);
	}
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Dodge_0"), pOwner, ZERO_VECTOR);
}
