#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player_R.h"
#include "CIdle_Player_L.h"

#include "CWalk_Player.h"
#include "CWarrior_Attack_01.h"
#include "CRun_Player.h"
#include "CSlide_Player.h"
#include "CJump_Player.h"
#include "CSprint_Player.h"

#include "CWalk_Player_L.h"
#include "CWalk_Player_R.h"

#include "CRun_Player_L.h"
#include "CRun_Player_R.h"

#include "CWarrior_Attack_01.h"

#include "CWarrior_Oxen.h"
#include "CWarrior_GuardBreak.h"


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

	m_arrStates[STATE_IDLE_PLAYER_L] = (CState*)CIdle_Player_L::Create();
	m_arrStates[STATE_IDLE_PLAYER_R] = (CState*)CIdle_Player_R::Create();

	m_arrStates[STATE_WALK_PLAYER_L] = (CState*)CWalk_Player_L::Create();
	m_arrStates[STATE_WALK_PLAYER_R] = (CState*)CWalk_Player_R::Create();

	m_arrStates[STATE_RUN_PLAYER_L] = (CState*)CRun_Player_L::Create();
	m_arrStates[STATE_RUN_PLAYER_R] = (CState*)CRun_Player_R::Create();

	m_arrStates[STATE_PLAYER_SKILL1] = (CState*)CWarrior_Oxen::Create();
	m_arrStates[STATE_PLAYER_SKILL2] = (CState*)CWarrior_GuardBreak::Create();

	m_arrStates[STATE_ATTACK_UPPER_MIDDLE_PLAYER_L] = (CState*)CWarrior_Attack_01::Create();

	



	//Warrior_Base_R();
	//Warrior_Base_L();

	//Warrior_Base_Attack_R();
	//Warrior_Base_Attack_L();
	//Warrior_Base_Attack_Default();
	//
	//Warrior_Base_Hit();
	//Warrior_Base_ETC();

	//Warrior_Others();


#pragma endregion


	
	

	for (_uint i = 0; i < STATE_END; ++i)
	{
		if (!m_arrStates[i])
		{
			STATE_TYPE iType = (STATE_TYPE)i;
			Call_MsgBox(L"태호야 상태추가하랬지");
			return E_FAIL;

		}
	}

	return S_OK;
}

//void CState_Manager::Warrior_Base_R()
//{
//	m_arrStates[STATE_IDLE_PLAYER_R] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_WALK_PLAYER_R] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_RUN_PLAYER_R] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_JUMP_PLAYER_R] = CIdle_Player_L::Create();
//	m_arrStates[STATE_JUMPFALL_PLAYER_R] = CIdle_Player_L::Create();
//	m_arrStates[STATE_JUMP_RAND_PLAYER_R] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_BOUNCE_PLAYER_R] = CIdle_Player_L::Create();
//}
//
//void CState_Manager::Warrior_Base_L()
//{
//	m_arrStates[STATE_IDLE_PLAYER_L] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_WALK_PLAYER_L] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_RUN_PLAYER_L] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_JUMP_PLAYER_L] = CIdle_Player_L::Create();
//	m_arrStates[STATE_JUMP_FALL_PLAYER_L] = CIdle_Player_L::Create();
//	m_arrStates[STATE_JUMP_RAND_PLAYER_L] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_BOUNCE_PLAYER_L] = CIdle_Player_L::Create();
//}
//
//void CState_Manager::Warrior_Base_Attack_L()
//{
//	m_arrStates[STATE_ATTACK_UPPER_MIDDLE_PLAYER_L] = CIdle_Player_L::Create();
//	m_arrStates[STATE_ATTACK_BOTTOM_DIAGONAL_PLAYER_L] = CIdle_Player_L::Create();
//	m_arrStates[STATE_ATTACK_TOP_DIAGONAL_PLAYER_L] = CIdle_Player_L::Create();
//	
//	
//	
//	
//}
//
//void CState_Manager::Warrior_Base_Attack_R()
//{
//	m_arrStates[STATE_ATTACK_UPPER_MIDDLE_PLAYER_R] = CIdle_Player_L::Create();
//	m_arrStates[STATE_ATTACK_BOTTOM_DIAGONAL_PLAYER_R] = CIdle_Player_L::Create();
//	m_arrStates[STATE_ATTACK_TOP_DIAGONAL_PLAYER_R] = CIdle_Player_L::Create();
//}
//
//void CState_Manager::Warrior_Base_Attack_Default()
//{
//	m_arrStates[STATE_SPRINTATTACK_PLAYER] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_ATTACK_STING_PLAYER_L] = CIdle_Player_L::Create();
//	m_arrStates[STATE_ATTACK_STING_PLAYER_R] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_ATTACK_TAKEDOWN] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_PLAYER_SKILL1] = CIdle_Player_L::Create();
//	m_arrStates[STATE_PLAYER_SKILL2] = CIdle_Player_L::Create();
//}
//
//void CState_Manager::Warrior_Base_Hit()
//{
//}
//
//void CState_Manager::Warrior_Base_ETC()
//{
//}
//
//void CState_Manager::Warrior_Others()
//{
//	m_arrStates[STATE_SLIDE_PLAYER] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_SPRINT_RUN_PLAYER] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_SPRINT_JUMP_PLAYER] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_SPRINT_JUMPFALL_PLAYER] = CIdle_Player_L::Create();
//	m_arrStates[STATE_SPRINT_LAND_PLAYER] = CIdle_Player_L::Create();
//
//
//	m_arrStates[STATE_IDLE_PLAYER] = CWalk_Player::Create();
//	m_arrStates[STATE_WALK_PLAYER] = CWalk_Player::Create();
//	m_arrStates[STATE_ATTACK_WARRIOR] = CWarrior_Attack_01::Create();
//	m_arrStates[STATE_RUN_PLAYER] = CRun_Player::Create();
//	m_arrStates[STATE_SLIDE_PLAYER] = CSlide_Player::Create();
//	m_arrStates[STATE_JUMP_PLAYER] = CJump_Player::Create();
//	m_arrStates[STATE_SPRINT_PLAYER] = CSprint_Player::Create();
//}
