#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player.h"
#include "CWalk_Player.h"
#include "CWarrior_Attack_01.h"
#include "CRun_Player.h"
#include "CSlide_Player.h"
#include "CJump_Player.h"
#include "CSprint_Player.h"

IMPLEMENT_SINGLETON(CState_Manager);

CState_Manager::CState_Manager()
{
}

CState_Manager::~CState_Manager()
{
	for (_uint i = 0; i < STATE_END; ++i)
	{
		SAFE_DELETE(m_arrStates[i]);
	}
}

HRESULT CState_Manager::Initialize()
{
#pragma region Warrior State pattern
	m_arrStates[STATE_IDLE_PLAYER] = CIdle_Player::Create();
	m_arrStates[STATE_WALK_PLAYER] = CWalk_Player::Create();
	m_arrStates[STATE_ATTACK_WARRIOR] = CWarrior_Attack_01::Create();
	m_arrStates[STATE_RUN_PLAYER] = CRun_Player::Create();
	m_arrStates[STATE_SLIDE_PLAYER] = CSlide_Player::Create();
	m_arrStates[STATE_JUMP_PLAYER] = CJump_Player::Create();
	m_arrStates[STATE_SPRINT_PLAYER] = CSprint_Player::Create();
#pragma endregion

#pragma endregion

	
	

	for (_uint i = 0; i < STATE_END; ++i)
	{
		if (!m_arrStates[i])
		{
			Call_MsgBox(L"태호야 상태추가하랬지");
			return E_FAIL;

		}
	}

	return S_OK;
}
