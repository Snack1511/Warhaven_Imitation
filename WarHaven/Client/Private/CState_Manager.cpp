#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player.h"

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

	for (_uint i = 0; i < STATE_END; ++i)
	{
		if (!m_arrStates[i])
			return E_FAIL;
	}

	return S_OK;
}
