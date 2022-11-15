#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player_R.h"
#include "CIdle_Player_L.h"

#pragma region Warrior

#include "CWalk_Player.h"
#include "CRun_Player.h"
#include "CSlide_Player.h"
#include "CJump_Player.h"
#include "CSprint_Player.h"

#include "CWalk_Player_L.h"
#include "CWalk_Player_R.h"

#include "CSwitchLtoR.h"
#include "CSwitchRtoL.h"

#include "CRun_Player_L.h"
#include "CRun_Player_R.h"

#include "CRun_Player_Begin_L.h"
#include "CRun_Player_Begin_R.h"

#include "CJump_Player_L.h"
#include "CJump_Player_R.h"

#include "CJump_Player_Fall_L.h"
#include "CJump_Player_Fall_R.h"

#include "CJump_Player_Land_L.h"
#include "CJump_Player_Land_R.h"


#include "CWarrior_Attack_HorizontalDown_R.h"
#include "CWarrior_Attack_HorizontalDown_L.h"

#include "CWarrior_Attack_HorizontalUp_R.h"
#include "CWarrior_Attack_HorizontalUp_L.h"

#include "CWarrior_Attack_HorizontalMiddle_R.h"
#include "CWarrior_Attack_HorizontalMiddle_L.h"

#include "CWarrior_Attack_Sting_L.h"
#include "CWarrior_Attack_Sting_R.h"

#include "CWarrior_Attack_VerticalCut.h"

#include "CSprint_Begin.h"
#include "CSprint_Loop.h"
#include "CSprint_End.h"

#include "CSprintAttack_Player_Begin.h"
#include "CSprintAttack_Player.h"

#include "CSlide_Begin_Player.h"
#include "CSlide_Loop_Player.h"
#include "CSlide_End_Player.h"

#include "CSprint_Jump_Player.h"
#include "CSprint_Jump_Fall_Player.h"

#include "CWarrior_GuardBreak.h"

#include  "CWarrior_Oxen_Begin.h"
#include  "CWarrior_Oxen_Loop_Attack.h"
#include  "CWarrior_Oxen_Loop.h"
#include  "CWarrior_Oxen_Cancel.h"

#include "CGuard_Cancel_Player.h"
#include "CGuard_Begin_Player.h"
#include "CGuard_Loop_Player.h"
#include "CGuard_End_Player.h"

#include "CBounce_Player_L.h"
#include "CBounce_Player_R.h"

#include "CStop_Player_L.h"
#include "CStop_Player_R.h"

#pragma endregion

#pragma region Spearman

#include "CIdle_SpearMan_L.h"
#include "CIdle_SpearMan_R.h"

#include "CWalk_SpearMan_L.h"
#include "CWalk_SpearMan_R.h"

#include "CRun_SpearmMan_Begin_L.h"
#include "CRun_SpearmMan_Begin_R.h"

#include "CRun_SpearMan_L.h"
#include "CRun_SpearMan_R.h"

#include "CJump_SpearMan_L.h"
#include "CJump_SpearMan_R.h"

#include "CJump_SpearMan_Fall_L.h"
#include "CJump_SpearMan_Fall_R.h"

#include "CJump_SpearMan_Land_L.h"
#include "CJump_SpearMan_Land_R.h"

#include "CStop_SpearMan_L.h"
#include "CStop_SpearMan_R.h"


#pragma endregion

#include "CHit_Player.h"

#include "CAI_SandBack.h"


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
	Warrior_State();

	m_arrStates[STATE_IDLE_SPEARMAN_L] = CIdle_SpearMan_L::Create();
	m_arrStates[STATE_IDLE_SPEARMAN_R] = CIdle_SpearMan_R::Create();

	m_arrStates[STATE_WALK_SPEARMAN_L] = CWalk_SpearMan_L::Create();
	m_arrStates[STATE_WALK_SPEARMAN_R] = CWalk_SpearMan_R::Create();

	m_arrStates[STATE_RUNBEGIN_SPEARMAN_L] = CRun_SpearmMan_Begin_L::Create();
	m_arrStates[STATE_RUNBEGIN_SPEARMAN_R] = CRun_SpearmMan_Begin_R::Create();

	m_arrStates[STATE_RUN_SPEARMAN_L] = CRun_SpearMan_L::Create();
	m_arrStates[STATE_RUN_SPEARMAN_R] = CRun_SpearMan_R::Create();



	m_arrStates[STATE_JUMP_SPEARMAN_L] = CJump_SpearMan_L::Create();
	m_arrStates[STATE_JUMP_SPEARMAN_R] = CJump_SpearMan_R::Create();

	m_arrStates[STATE_JUMPFALL_SPEARMAN_L] = CJump_SpearMan_Fall_L::Create();
	m_arrStates[STATE_JUMPFALL_SPEARMAN_R] = CJump_SpearMan_Fall_R::Create();

	m_arrStates[STATE_JUMP_LAND_SPEARMAN_L] = CJump_SpearMan_Land_L::Create();
	m_arrStates[STATE_JUMP_LAND_SPEARMAN_R] = CJump_SpearMan_Land_R::Create();


	m_arrStates[STATE_STOP_SPEARMAN_L] = CStop_SpearMan_L::Create();
	m_arrStates[STATE_STOP_SPEARMAN_R] = CStop_SpearMan_R::Create();


	

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

void CState_Manager::Warrior_State()
{
#pragma region Warrior State pattern

	m_arrStates[STATE_IDLE_PLAYER_L] = CIdle_Player_L::Create();
	m_arrStates[STATE_IDLE_PLAYER_R] = CIdle_Player_R::Create();

	m_arrStates[STATE_SWITCH_R_TO_L] = CSwitchRtoL::Create();
	m_arrStates[STATE_SWITCH_L_TO_R] = CSwitchLtoR::Create();

	m_arrStates[STATE_WALK_PLAYER_L] = CWalk_Player_L::Create();
	m_arrStates[STATE_WALK_PLAYER_R] = CWalk_Player_R::Create();

	m_arrStates[STATE_RUN_BEGIN_PLAYER_R] = CRun_Player_Begin_L::Create();
	m_arrStates[STATE_RUN_BEGIN_PLAYER_L] = CRun_Player_Begin_R::Create();

	m_arrStates[STATE_RUN_PLAYER_L] = CRun_Player_L::Create();
	m_arrStates[STATE_RUN_PLAYER_R] = CRun_Player_R::Create();

	m_arrStates[STATE_JUMP_PLAYER_L] = CJump_Player_L::Create();
	m_arrStates[STATE_JUMPFALL_PLAYER_L] = CJump_Player_Fall_L::Create();
	m_arrStates[STATE_JUMP_LAND_PLAYER_L] = CJump_Player_Land_L::Create();

	m_arrStates[STATE_JUMP_PLAYER_R] = CJump_Player_R::Create();
	m_arrStates[STATE_JUMPFALL_PLAYER_R] = CJump_Player_Fall_R::Create();
	m_arrStates[STATE_JUMP_LAND_PLAYER_R] = CJump_Player_Land_R::Create();


	m_arrStates[STATE_ATTACK_HORIZONTALUP_L] = CWarrior_Attack_HorizontalUp_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALUP_R] = CWarrior_Attack_HorizontalUp_R::Create();

	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_L] = CWarrior_Attack_HorizontalMiddle_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_R] = CWarrior_Attack_HorizontalMiddle_R::Create();

	m_arrStates[STATE_ATTACK_HORIZONTALDOWN_L] = CWarrior_Attack_HorizontalDown_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALDOWN_R] = CWarrior_Attack_HorizontalDown_R::Create();

	m_arrStates[STATE_ATTACK_STING_PLAYER_L] = CWarrior_Attack_Sting_L::Create();
	m_arrStates[STATE_ATTACK_STING_PLAYER_R] = CWarrior_Attack_Sting_R::Create();

	m_arrStates[STATE_ATTACK_VERTICALCUT] = CWarrior_Attack_VerticalCut::Create();

	m_arrStates[STATE_SPRINT_BEGIN_PLAYER] = CSprint_Begin::Create();
	m_arrStates[STATE_SPRINT_LOOP_PLAYER] = CSprint_Loop::Create();
	m_arrStates[STATE_SPRINT_END_PLAYER] = CSprint_End::Create();

	m_arrStates[STATE_SLIDE_BEGIN_PLAYER] = CSlide_Begin_Player::Create();
	m_arrStates[STATE_SLIDE_LOOP_PLAYER] = CSlide_Loop_Player::Create();
	m_arrStates[STATE_SLIDE_END_PLAYER] = CSlide_End_Player::Create();

	m_arrStates[STATE_SPRINT_JUMPFALL_PLAYER] = CSprint_Jump_Fall_Player::Create();

	m_arrStates[STATE_SPRINT_JUMP_PLAYER] = CSprint_Jump_Player::Create();

	m_arrStates[STATE_SPRINTATTACK_BEGIN_PLAYER] = CSprintAttack_Player_Begin::Create();
	m_arrStates[STATE_SPRINTATTACK_PLAYER] = CSprintAttack_Player::Create();


	m_arrStates[STATE_GUARD_BEGIN_PLAYER] = CGuard_Begin_Player::Create();
	m_arrStates[STATE_GUARD_LOOP_PLAYER] = CGuard_Loop_Player::Create();
	m_arrStates[STATE_GUARD_END_PLAYER] = CGuard_End_Player::Create();
	m_arrStates[STATE_GUARD_CANCEL_PLAYER] = CGuard_Cancel_Player::Create();

	m_arrStates[STATE_BOUNCE_PLAYER_L] = CBounce_Player_L::Create();
	m_arrStates[STATE_BOUNCE_PLAYER_R] = CBounce_Player_R::Create();

	m_arrStates[STATE_STOP_PLAYER_L] = CStop_Player_L::Create();
	m_arrStates[STATE_STOP_PLAYER_R] = CStop_Player_R::Create();


	m_arrStates[STATE_HIT] = CHit_Player::Create();

	//	m_arrStates[STATE_WARRIOR_OXEN] = CWarrior_Oxen::Create();
	m_arrStates[STATE_WARRIOR_GUARDBREAK] = CWarrior_GuardBreak::Create();

	//	m_arrStates[STATE_ATTACK_UPPER_MIDDLE_PLAYER_L] = CWarrior_Attack_01::Create();

	m_arrStates[STATE_WARRIOR_OXEN_BEGIN] = CWarrior_Oxen_Begin::Create();
	m_arrStates[STATE_WARRIOR_OXEN_LOOP] = CWarrior_Oxen_Loop::Create();
	m_arrStates[STATE_WARRIOR_OXEN_LOOPATTACK] = CWarrior_Oxen_Loop_Attack::Create();
	m_arrStates[STATE_WARRIOR_OXEN_END] = CWarrior_Oxen_Cancel::Create();



	m_arrStates[STATE_IDLE_WARRIOR_R_AI_ENEMY] = CAI_SandBack::Create();

	//STATE_HIT





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
//	m_arrStates[STATE_JUMP_LAND_PLAYER_R] = CIdle_Player_L::Create();
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
//	m_arrStates[STATE_JUMP_LAND_PLAYER_L] = CIdle_Player_L::Create();
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
//	m_arrStates[STATE_ATTACK_VERTICAL_CUT] = CIdle_Player_L::Create();
//
//	m_arrStates[STATE_WARRIOR_OXEN] = CIdle_Player_L::Create();
//	m_arrStates[STATE_WARRIOR_GUARDBREAK] = CIdle_Player_L::Create();
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
