#include "stdafx.h"
#include "CGuard_Dash_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Dash_Player::CGuard_Dash_Player()
{
}

CGuard_Dash_Player::~CGuard_Dash_Player()
{
}

CGuard_Dash_Player* CGuard_Dash_Player::Create()
{
    CGuard_Dash_Player* pInstance = new CGuard_Dash_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Dash_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Dash_Player::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARDDASH_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.4f;

	m_iStateChangeKeyFrame = 43;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);

	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);
	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 2;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 3;

    return S_OK;
}

void CGuard_Dash_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	_bool bRight = false;

	if (CUser::Get_Instance()->Get_LastKey() == KEY::A)
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];
		bRight = false;
	}

	if (CUser::Get_Instance()->Get_LastKey() == KEY::D)
	{
		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];
		bRight = true;
	}

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	Physics_Setting_Right(10.f, pOwner, true, bRight);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Dash_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);
}

void CGuard_Dash_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CGuard_Dash_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

   
    return STATE_END;
}
