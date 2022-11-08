#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player.h"
#include "CWalk_Player.h"
#include "CWarrior_Attack_01.h"

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
	m_arrStates[STATE_IDLE_PLAYER] = CIdle_Player::Create();
	m_arrStates[STATE_WALK_PLAYER] = CWalk_Player::Create();
	m_arrStates[STATE_ATTACK_WARRIOR] = CWarrior_Attack_01::Create();

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
