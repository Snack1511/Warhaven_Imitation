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

#include "CChargeTest.h"

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

#include "CHit_Player.h"
#include "CHit_GuardHit_Warrior.h"
#include "CHit_Groggy_Warrior.h"
#include "CHit_Sting_Warrior.h"
#include "CHit_Fly_Warrior.h"

#include "CGuard_Dash_Player.h"
#include "CChangeHero_Player.h"

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

#include "CSpearMan_Attack_Sting.h"
#include "CBounce_SpearMan.h"

#include "CSpearMan_GuardBreak.h"
#include "CSpearMan_SpecialGuard.h"
#include "CSpearMan_BackStep.h"

#pragma endregion

#pragma region WarHammer

#include "CIdle_WarHammer_L.h"
#include "CIdle_WarHammer_R.h"

#include "CWalk_WarHammer_L.h"
#include "CWalk_WarHammer_R.h"

#include "CRun_WarHammer_Begin_L.h"
#include "CRun_WarHammer_Begin_R.h"

#include "CRun_WarHammer_L.h"
#include "CRun_WarHammer_R.h"

#include "CJump_WarHammer_L.h"
#include "CJump_WarHammer_R.h"

#include "CJump_WarHammer_Fall_L.h"
#include "CJump_WarHammer_Fall_R.h"

#include "CJump_WarHammer_Land_L.h"
#include "CJump_WarHammer_Land_R.h"

#include "CStop_WarHammer_L.h"
#include "CStop_WarHammer_R.h"

#include "CSprint_Jump_WarHammer.h"
#include "CSprint_Jump_Fall_WarHammer.h"
#include "CSprintAttack_WarHammer_Begin.h"
#include "CSprintAttack_WarHammer.h"
#include "CSprint_Begin_WarHammer.h"
#include "CSprint_Loop_WarHammer.h"
#include "CSprint_End_WarHammer.h"

#include "CSwitchLtoR_WarHammer.h"
#include "CSwitchRtoL_WarHammer.h"

#include "CWarHammer_Attack_HorizontalMiddle_L.h"
#include "CWarHammer_Attack_HorizontalMiddle_R.h"

#include "CWarHammer_Attack_VerticalAttack_L.h"
#include "CWarHammer_Attack_VerticalAttack_R.h"

#include "CWarHammer_GroggyAttack.h"

#include "CWarHammer_AirSpike_Begin.h"
#include "CWarHammer_AirSpike_Loop.h"
#include "CWarHammer_AirSpike_End.h"

#include "CWarHammer_Attack_Sting_L.h"
#include "CWarHammer_Attack_Sting_R.h"


#include "CBounce_WarHammer_L.h"
#include "CBounce_WarHammer_R.h"

#include "CGuard_Begin_WarHammer.h"
#include "CGuard_Loop_WarHammer.h"
#include "CGuard_End_WarHammer.h"
#include "CGuard_Cancel_WarHammer.h"

#include "CInstall_Begin_WarHammer.h"
#include "CInstall_Loop_WarHammer.h"
#include "CInstall_End_WarHammer.h"

#include "CCharge_WarHammer_L.h"
#include "CCharge_WarHammer_R.h"

#include "CWarHammer_ChargeAttack_L.h"
#include "CWarHammer_ChargeAttack_R.h"

#include "CCharge_WarHammer_Front_L.h"
#include "CCharge_WarHammer_Front_R.h"

#include "CWarHammer_Attack_VerticalAttack_L.h"
#include "CWarHammer_Attack_VerticalAttack_R.h"

#include "CHit_WarHammer.h"
#include "CHit_GuardHit_WarHammer.h"
#include "CHit_Groggy_WarHammer.h"
#include "CHit_Sting_WarHammer.h"
#include "CHit_Fly_WarHammer.h"

#include "CGuard_Dash_WarHammer.h"



#pragma endregion

#pragma region Valkyrie

#include "CSwitchLtoR_Valkyrie.h"
#include "CSwitchRtoL_Valkyrie.h"

#include "CIdle_Valkyrie_L.h"
#include "CIdle_Valkyrie_R.h"

#include "CWalk_Valkyrie_L.h"
#include "CWalk_Valkyrie_R.h"

#include "CRun_Valkyrie_Begin_L.h"
#include "CRun_Valkyrie_Begin_R.h"

#include "CRun_Valkyrie_L.h"
#include "CRun_Valkyrie_R.h"

#include "CJump_Valkyrie_L.h"
#include "CJump_Valkyrie_R.h"

#include "CJump_Valkyrie_Fall_L.h"
#include "CJump_Valkyrie_Fall_R.h"

#include "CJump_Valkyrie_Land_L.h"
#include "CJump_Valkyrie_Land_R.h"

#include "CStop_Valkyrie_L.h"
#include "CStop_Valkyrie_R.h"

#include "CSprint_Begin_Valkyrie.h"
#include "CSprint_Loop_Valkyrie.h"
#include "CSprint_End_Valkyrie.h"

#include "CSprint_Jump_Valkyrie.h"
#include "CSprint_Jump_Fall_Valkyrie.h"
#include "CSprintAttack_Valkyrie_Begin.h"
#include "CSprintAttack_Valkyrie.h"

#include "CValkyrie_Attack_HorizontalDown_L.h"
#include "CValkyrie_Attack_HorizontalDown_R.h"
#include "CValkyrie_Attack_HorizontalMiddle_L.h"
#include "CValkyrie_Attack_HorizontalMiddle_R.h"
#include "CValkyrie_Attack_HorizontalUp_L.h"
#include "CValkyrie_Attack_HorizontalUp_R.h"

#include "CValkyrie_Attack_Sting_L.h"
#include "CValkyrie_Attack_Sting_R.h"
#include "CValkyrie_Attack_VerticalCut.h"


#include "CGuard_Begin_Valkyrie.h"
#include "CGuard_Loop_Valkyrie.h"
#include "CGuard_End_Valkyrie.h"
#include "CGuard_Cancel_Valkyrie.h"
#include "CGuard_Dash_Valkyrie.h"

#include "CBounce_Valkyrie_L.h"
#include "CBounce_Valkyrie_R.h"


#include "CValkyrie_Counter.h"
#include "CValkyrie_SpinAttack.h"
#include "CValkyrie_ShieldAttack.h"


#pragma endregion 



#include "CAI_SandBack.h"
#include "CAI_SandBack_L.h"

#include "CAI_SandBack_Hit.h"
#include "CAI_SandBack_GuardHit.h"
#include "CAI_SandBack_Groggy.h"
#include "CAI_SandBack_StingHit.h"
#include "CAI_SandBack_FlyHit.h"
//#include "CGuard_Cancel_Warrior_AI.h"

#include "CAI_CWarrior_Attack_HorizontalMiddle_L.h"
#include "CAI_CWarrior_Attack_HorizontalMiddle_R.h"

#include "CJump_Warrior_AI_Fall_L.h"
#include "CJump_Warrior_AI_Fall_R.h"
#include "CJump_Warrior_AI_Land_L.h"
#include "CJump_Warrior_AI_Land_R.h"

#include "CBounce_Warrior_AI_R.h"
#include "CBounce_Warrior_AI_L.h"

#include "CGuard_Begin_Player_Warrior_AI.h"
#include "CGuard_Loop_Warrior_AI.h"
#include "CGuard_Cancel_Warrior_AI.h"

#include "CAI_SandBack_WarHammer.h"

#include "CHit_Valkyrie.h"
#include "CHit_GuardHit_Valkyrie.h"
#include "CHit_Groggy_Valkyrie.h"
#include "CHit_Sting_Valkyrie.h"
#include "CHit_Fly_Valkyrie.h"

#include "CIdle_AI_TG_Warrior_L.h"
#include "CIdle_AI_TG_Warrior_R.h"

#include "CWalk_AI_TG_Warrior_L.h"
#include "CWalk_AI_TG_Warrior_R.h"


#include "CRun_AI_TG_Warrior_L.h"
#include "CRun_AI_TG_Warrior_R.h"

#include "CRun_AI_TG_Warrior_Begin_L.h"
#include "CRun_AI_TG_Warrior_Begin_R.h"

#include "CGuard_Cancel_AI_TG_Warrior.h"
#include "CGuard_End_AI_TG_Warrior.h"
#include "CGuard_Loop_AI_TG_Warrior.h"
#include "CGuard_Begin_AI_TG_Warrior.h"

#include "CBounce_AI_TG_Warrior_L.h"
#include "CBounce_AI_TG_Warrior_R.h"

#include "CAI_TG_Warrior_Attack_HorizontalMiddle_L.h"
#include "CAI_TG_Warrior_Attack_HorizontalMiddle_R.h"

#include "CAI_TG_Hit.h"
#include "CAI_TG_GuardHit.h"
#include "CAI_TG_StingHit.h"
#include "CAI_TG_FlyHit.h"
#include "CAI_TG_Groggy.h"

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
	Spear_State();


	Warrior_State();
	WarHammer_State();
	Valkyrie_State();

	Warrior_SandBagState();
	Warrior_State_AI();

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
	m_arrStates[STATE_GUARDHIT_WARRIOR] = CHit_GuardHit_Warrior::Create();
	m_arrStates[STATE_GROGGYHIT_WARRIOR] = CHit_Groggy_Warrior::Create();
	m_arrStates[STATE_STINGHIT_WARRIOR] = CHit_Sting_Warrior::Create();
	m_arrStates[STATE_FLYHIT_WARRIOR] = CHit_Fly_Warrior::Create();

	

	//	m_arrStates[STATE_WARRIOR_OXEN] = CWarrior_Oxen::Create();
	m_arrStates[STATE_WARRIOR_GUARDBREAK] = CWarrior_GuardBreak::Create();

	//	m_arrStates[STATE_ATTACK_UPPER_MIDDLE_PLAYER_L] = CWarrior_Attack_01::Create();

	m_arrStates[STATE_WARRIOR_OXEN_BEGIN] = CWarrior_Oxen_Begin::Create();
	m_arrStates[STATE_WARRIOR_OXEN_LOOP] = CWarrior_Oxen_Loop::Create();
	m_arrStates[STATE_WARRIOR_OXEN_LOOPATTACK] = CWarrior_Oxen_Loop_Attack::Create();
	m_arrStates[STATE_WARRIOR_OXEN_END] = CWarrior_Oxen_Cancel::Create();



	m_arrStates[STATE_GUARDDASH_WARRIOR] = CGuard_Dash_Player::Create();


	/* Charge Test */
	m_arrStates[STATE_CHARGETEST] = CChargeTest::Create();

	m_arrStates[STATE_CHANGE_PLAYER] = CChangeHero_Player::Create();



#pragma endregion
}

void CState_Manager::WarHammer_State()
{

	/* Idle */
	m_arrStates[STATE_IDLE_WARHAMMER_L] = CIdle_WarHammer_L::Create();
	m_arrStates[STATE_IDLE_WARHAMMER_R] = CIdle_WarHammer_R::Create();

	/* Walk */
	m_arrStates[STATE_WALK_WARHAMMER_L] = CWalk_WarHammer_L::Create();
	m_arrStates[STATE_WALK_WARHAMMER_R] = CWalk_WarHammer_R::Create();

	/* RunBegin */
	m_arrStates[STATE_RUNBEGIN_WARHAMMER_L] = CRun_WarHammer_Begin_L::Create();
	m_arrStates[STATE_RUNBEGIN_WARHAMMER_R] = CRun_WarHammer_Begin_R::Create();

	/* Run */
	m_arrStates[STATE_RUN_WARHAMMER_L] = CRun_WarHammer_L::Create();
	m_arrStates[STATE_RUN_WARHAMMER_R] = CRun_WarHammer_R::Create();

	/* Jump */
	m_arrStates[STATE_JUMP_WARHAMMER_L] = CJump_WarHammer_L::Create();
	m_arrStates[STATE_JUMPFALL_WARHAMMER_L] = CJump_WarHammer_Fall_L::Create();
	m_arrStates[STATE_JUMP_LAND_WARHAMMER_L] = CJump_WarHammer_Land_L::Create();

	m_arrStates[STATE_JUMP_WARHAMMER_R] = CJump_WarHammer_R::Create();
	m_arrStates[STATE_JUMPFALL_WARHAMMER_R] = CJump_WarHammer_Fall_R::Create();
	m_arrStates[STATE_JUMP_LAND_WARHAMMER_R] = CJump_WarHammer_Land_R::Create();
	
	/* Stop */
	m_arrStates[STATE_STOP_WARHAMMER_L] = CStop_WarHammer_L::Create();
	m_arrStates[STATE_STOP_WARHAMMER_R] = CStop_WarHammer_R::Create();

	/* Sprint */
	m_arrStates[STATE_SPRINT_BEGIN_WARHAMMER] = CSprint_Begin_WarHammer::Create();
	m_arrStates[STATE_SPRINT_LOOP_WARHAMMER] = CSprint_Loop_WarHammer::Create();
	m_arrStates[STATE_SPRINT_END_WARHAMMER] = CSprint_End_WarHammer::Create();


	m_arrStates[STATE_SPRINT_JUMPFALL_WARHAMMER] = CSprint_Jump_Fall_WarHammer::Create();

	m_arrStates[STATE_SPRINT_JUMP_WARHAMMER] = CSprint_Jump_WarHammer::Create();

	m_arrStates[STATE_SPRINTATTACK_BEGIN_WARHAMMER] = CSprintAttack_WarHammer_Begin::Create();
	m_arrStates[STATE_SPRINTATTACK_WARHAMMER] = CSprintAttack_WarHammer::Create();


	m_arrStates[STATE_SWITCH_R_TO_L_WARHAMMER] = CSwitchLtoR_WarHammer::Create();
	m_arrStates[STATE_SWITCH_L_TO_R_WARHAMMER] = CSwitchRtoL_WarHammer::Create();
	
	/* Attack */
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L] = CWarHammer_Attack_HorizontalMiddle_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R] = CWarHammer_Attack_HorizontalMiddle_R::Create();


	m_arrStates[STATE_VERTICALATTACK_WARHAMMER_L] = CWarHammer_Attack_VerticalAttack_L::Create();
	m_arrStates[STATE_VERTICALATTACK_WARHAMMER_R] = CWarHammer_Attack_VerticalAttack_R::Create();

	m_arrStates[STATE_GROGGYATTACK_WARHAMMER] = CWarHammer_GroggyAttack::Create();

	m_arrStates[STATE_AIRSPIKE_BEGIN_WARHAMMER] = CWarHammer_AirSpike_Begin::Create();
	m_arrStates[STATE_AIRSPIKE_LOOP_WARHAMMER] = CWarHammer_AirSpike_Loop::Create();
	m_arrStates[STATE_AIRSPIKE_END_WARHAMMER] = CWarHammer_AirSpike_End::Create();


	m_arrStates[STATE_ATTACK_STING_WARHAMMER_L] = CWarHammer_Attack_Sting_L::Create();
	m_arrStates[STATE_ATTACK_STING_WARHAMMER_R] = CWarHammer_Attack_Sting_R::Create();

	/* Bounce */
	m_arrStates[STATE_BOUNCE_WARHAMMER_L] = CBounce_WarHammer_L::Create();
	m_arrStates[STATE_BOUNCE_WARHAMMER_R] = CBounce_WarHammer_R::Create();


	/* Guard */
	m_arrStates[STATE_GUARD_BEGIN_WARHAMMER] = CGuard_Begin_WarHammer::Create();
	m_arrStates[STATE_GUARD_LOOP_WARHAMMER] = CGuard_Loop_WarHammer::Create();
	m_arrStates[STATE_GUARD_END_WARHAMMER] = CGuard_End_WarHammer::Create();
	m_arrStates[STATE_GUARD_CANCEL_WARHAMMER] = CGuard_Cancel_WarHammer::Create();
	
	m_arrStates[STATE_INSTALL_BEIGN_WARHAMMER] = CInstall_Begin_WarHammer::Create();
	m_arrStates[STATE_INSTALL_LOOP_WARHAMMER] = CInstall_Loop_WarHammer::Create();
	m_arrStates[STATE_INSTALL_END_WARHAMMER] = CInstall_End_WarHammer::Create();

	m_arrStates[STATE_CHARGE_WARHAMMER_L] = CCharge_WarHammer_L::Create();
	m_arrStates[STATE_CHARGE_WARHAMMER_R] = CCharge_WarHammer_R::Create();

	m_arrStates[STATE_CHARGEATTACK_WARHAMMER_L] = CWarHammer_ChargeAttack_L::Create();
	m_arrStates[STATE_CHARGEATTACK_WARHAMMER_R] = CWarHammer_ChargeAttack_R::Create();

	m_arrStates[STATE_CHARGE_FRONT_WARHAMMER_L] = CCharge_WarHammer_Front_L::Create();
	m_arrStates[STATE_CHARGE_FRONT_WARHAMMER_R] = CCharge_WarHammer_Front_R::Create();

	m_arrStates[STATE_CHARGEATTACK_FRONT_WARHAMMER_L] = CWarHammer_Attack_VerticalAttack_L::Create();
	m_arrStates[STATE_CHARGEATTACK_FRONT_WARHAMMER_R] = CWarHammer_Attack_VerticalAttack_R::Create();


	m_arrStates[STATE_HIT_WARHAMMER] = CHit_WarHammer::Create();
	m_arrStates[STATE_GUARDHIT_WARHAMMER] = CHit_GuardHit_WarHammer::Create();
	m_arrStates[STATE_GROGGYHIT_WARHAMMER] = CHit_Groggy_WarHammer::Create();
	m_arrStates[STATE_STINGHIT_WARHAMMER] = CHit_Sting_WarHammer::Create();
	m_arrStates[STATE_FLYHIT_WARHAMMER] = CHit_Fly_WarHammer::Create();

	m_arrStates[STATE_GUARDDASH_WARHAMMER] = CGuard_Dash_WarHammer::Create();

}

void CState_Manager::Valkyrie_State()
{
	/* Idle */
	m_arrStates[STATE_IDLE_VALKYRIE_L] =					CIdle_Valkyrie_L::Create();
	m_arrStates[STATE_IDLE_VALKYRIE_R] =					CIdle_Valkyrie_R::Create();

	/* Walk */
	m_arrStates[STATE_WALK_VALKYRIE_L] =					CWalk_Valkyrie_L::Create();
	m_arrStates[STATE_WALK_VALKYRIE_R] =					CWalk_Valkyrie_R::Create();

	/* RunBegin */
	m_arrStates[STATE_RUNBEGIN_VALKYRIE_L] =				CRun_Valkyrie_Begin_L::Create();
	m_arrStates[STATE_RUNBEGIN_VALKYRIE_R] =				CRun_Valkyrie_Begin_R::Create();

	/* Run */
	m_arrStates[STATE_RUN_VALKYRIE_L] =						CRun_Valkyrie_L::Create();
	m_arrStates[STATE_RUN_VALKYRIE_R] =						CRun_Valkyrie_R::Create();

	/* Jump */
	m_arrStates[STATE_JUMP_VALKYRIE_L] =					CJump_Valkyrie_L::Create();
	m_arrStates[STATE_JUMPFALL_VALKYRIE_L] =				CJump_Valkyrie_Fall_L::Create();
	m_arrStates[STATE_JUMP_LAND_VALKYRIE_L] =				CJump_Valkyrie_Land_L::Create();

	m_arrStates[STATE_JUMP_VALKYRIE_R] =					CJump_Valkyrie_R::Create();
	m_arrStates[STATE_JUMPFALL_VALKYRIE_R] =				CJump_Valkyrie_Fall_R::Create();
	m_arrStates[STATE_JUMP_LAND_VALKYRIE_R] =				CJump_Valkyrie_Land_R::Create();

	/* Stop */
	m_arrStates[STATE_STOP_VALKYRIE_L] =					CStop_Valkyrie_L::Create();
	m_arrStates[STATE_STOP_VALKYRIE_R] =					CStop_Valkyrie_R::Create();

	/* Sprint */
	m_arrStates[STATE_SPRINT_BEGIN_VALKYRIE] =				CSprint_Begin_Valkyrie::Create();
	m_arrStates[STATE_SPRINT_LOOP_VALKYRIE] =				CSprint_Loop_Valkyrie::Create();
	m_arrStates[STATE_SPRINT_END_VALKYRIE] =				CSprint_End_Valkyrie::Create();


	m_arrStates[STATE_SPRINT_JUMPFALL_VALKYRIE] =			CSprint_Jump_Fall_Valkyrie::Create();

	m_arrStates[STATE_SPRINT_JUMP_VALKYRIE] =				CSprint_Jump_Valkyrie::Create();

	m_arrStates[STATE_SPRINTATTACK_BEGIN_VALKYRIE] =		CSprintAttack_Valkyrie_Begin::Create();
	m_arrStates[STATE_SPRINTATTACK_VALKYRIE] =				CSprintAttack_Valkyrie::Create();

	
		
		
	m_arrStates[STATE_ATTACK_HORIZONTALUP_VALKYRIE_L] = CValkyrie_Attack_HorizontalUp_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L] = CValkyrie_Attack_HorizontalMiddle_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L] = CValkyrie_Attack_HorizontalDown_L::Create();
	m_arrStates[STATE_ATTACK_STING_VALKYRIE_L] = CValkyrie_Attack_Sting_L::Create();

	m_arrStates[STATE_ATTACK_HORIZONTALUP_VALKYRIE_R] = CValkyrie_Attack_HorizontalUp_R::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R] = CValkyrie_Attack_HorizontalMiddle_R::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R] = CValkyrie_Attack_HorizontalDown_R::Create();
	m_arrStates[STATE_ATTACK_STING_VALKYRIE_R] = CValkyrie_Attack_Sting_R::Create();

	m_arrStates[STATE_ATTACK_VERTICALCUT_VALKYRIE] = CValkyrie_Attack_VerticalCut::Create();

	m_arrStates[STATE_BOUNCE_VALKYRIE_L] = CBounce_Valkyrie_L::Create();
	m_arrStates[STATE_BOUNCE_VALKYRIE_R] = CBounce_Valkyrie_R::Create();

	m_arrStates[STATE_GUARD_BEGIN_VALKYRIE] =				CGuard_Begin_Valkyrie::Create();
	m_arrStates[STATE_GUARD_LOOP_VALKYRIE] =				CGuard_Loop_Valkyrie::Create();
	m_arrStates[STATE_GUARD_END_VALKYRIE] =					CGuard_End_Valkyrie::Create();
	m_arrStates[STATE_GUARD_CANCEL_VALKYRIE] =				CGuard_Cancel_Valkyrie::Create();
	m_arrStates[STATE_GUARDDASH_VALKYRIE] =					CGuard_Dash_Valkyrie::Create();


	m_arrStates[STATE_SWITCH_R_TO_L_VALKYRIE] =				CSwitchLtoR_Valkyrie::Create();
	m_arrStates[STATE_SWITCH_L_TO_R_VALKYRIE] =				CSwitchRtoL_Valkyrie::Create();


	m_arrStates[STATE_COUNTER_VALKYRIE] =					CValkyrie_Counter::Create();
	m_arrStates[STATE_SHIELDATTACK_VALKYRIE] =				CValkyrie_ShieldAttack::Create();
	m_arrStates[STATE_SPINATTACK_VALKYRIE] =				CValkyrie_SpinAttack::Create();


	m_arrStates[STATE_HIT_VALKYRIE] =						CHit_Valkyrie::Create();
	m_arrStates[STATE_GUARDHIT_VALKYRIE] =					CHit_GuardHit_Valkyrie::Create();
	m_arrStates[STATE_GROGGYHIT_VALKYRIE] =					CHit_Groggy_Valkyrie::Create();
	m_arrStates[STATE_STINGHIT_VALKYRIE] =					CHit_Sting_Valkyrie::Create();
	m_arrStates[STATE_FLYHIT_VALKYRIE] =					CHit_Fly_Valkyrie::Create();

	
		
		
}

void CState_Manager::Warrior_SandBagState()
{
	/*AI TEST*/
	m_arrStates[STATE_IDLE_WARRIOR_R_AI_ENEMY] = CAI_SandBack::Create();
	m_arrStates[STATE_IDLE_WARRIOR_L_AI_ENEMY] = CAI_SandBack_L::Create();


	m_arrStates[STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY] = CGuard_Begin_Player_Warrior_AI::Create();
	m_arrStates[STATE_GUARD_LOOP_WARRIOR_AI_ENEMY] = CGuard_Loop_Warrior_AI::Create();
	m_arrStates[STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY] = CGuard_Cancel_Warrior_AI::Create();
	m_arrStates[STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY] = CAI_CWarrior_Attack_HorizontalMiddle_L::Create();
	m_arrStates[STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY] = CAI_CWarrior_Attack_HorizontalMiddle_R::Create();



	m_arrStates[STATE_FALL_WARRIOR_L_AI_ENEMY] = CJump_Warrior_AI_Fall_L::Create();
	m_arrStates[STATE_FALL_WARRIOR_R_AI_ENEMY] = CJump_Warrior_AI_Fall_R::Create();

	m_arrStates[STATE_LAND_WARRIOR_L_AI_ENEMY] = CJump_Warrior_AI_Land_L::Create();
	m_arrStates[STATE_LAND_WARRIOR_R_AI_ENEMY] = CJump_Warrior_AI_Land_R::Create();

	m_arrStates[STATE_BOUNCE_WARRIOR_L_AI_ENEMY] = CBounce_Warrior_AI_L::Create();
	m_arrStates[STATE_BOUNCE_WARRIOR_R_AI_ENEMY] = CBounce_Warrior_AI_R::Create();


	m_arrStates[STATE_HIT_TEST_ENEMY] = CAI_SandBack_Hit::Create();
	m_arrStates[STATE_GUARDHIT_ENEMY] = CAI_SandBack_GuardHit::Create();
	m_arrStates[STATE_GROGGY_ENEMY] = CAI_SandBack_Groggy::Create();
	m_arrStates[STATE_STINGHIT_ENEMY] = CAI_SandBack_StingHit::Create();
	m_arrStates[STATE_FLYHIT_ENEMY] = CAI_SandBack_FlyHit::Create();

	m_arrStates[SANDBAG_STATE_IDLE_WARHAMMER_R] = CAI_SandBack_WarHammer::Create();
}

void CState_Manager::Warrior_State_AI()
{
	
	m_arrStates[AI_STATE_IDLE_WARRIOR_L] = CIdle_AI_TG_Warrior_L::Create();
	m_arrStates[AI_STATE_IDLE_WARRIOR_R] = CIdle_AI_TG_Warrior_R::Create();

	m_arrStates[AI_STATE_WALK_WARRIOR_L] = CWalk_AI_TG_Warrior_L::Create();
	m_arrStates[AI_STATE_WALK_WARRIOR_R] = CWalk_AI_TG_Warrior_R::Create();

	m_arrStates[AI_STATE_RUN_WARRIOR_L] = CRun_AI_TG_Warrior_L::Create();
	m_arrStates[AI_STATE_RUN_WARRIOR_R] = CRun_AI_TG_Warrior_R::Create();

	m_arrStates[AI_STATE_RUNBEGIN_WARRIOR_L] = CRun_AI_TG_Warrior_Begin_L::Create();
	m_arrStates[AI_STATE_RUNBEGIN_WARRIOR_R] = CRun_AI_TG_Warrior_Begin_R::Create();

	m_arrStates[AI_STATE_GUARD_BEGIN_WARRIOR] = CGuard_Begin_AI_TG_Warrior::Create();
	m_arrStates[AI_STATE_GUARD_LOOP_WARRIOR] = CGuard_Loop_AI_TG_Warrior::Create();
	m_arrStates[AI_STATE_GUARD_CANCEL_WARRIOR] = CGuard_Cancel_AI_TG_Warrior::Create();
	m_arrStates[AI_STATE_GUARD_END_WARRIOR] = CGuard_End_AI_TG_Warrior::Create();

	m_arrStates[AI_STATE_BOUNE_WARRIOR_L] = CBounce_AI_TG_Warrior_L::Create();
	m_arrStates[AI_STATE_BOUNE_WARRIOR_R] = CBounce_AI_TG_Warrior_R::Create();

	m_arrStates[AI_STATE_ATTACK_HORIZONTALMIDDLE_L] = CAI_TG_Warrior_Attack_HorizontalMiddle_L::Create();
	m_arrStates[AI_STATE_ATTACK_HORIZONTALMIDDLE_R] = CAI_TG_Warrior_Attack_HorizontalMiddle_R::Create();

	    
	m_arrStates[AI_STATE_TG_HIT_WARRIOR] = CAI_TG_Hit::Create();
	m_arrStates[AI_STATE_TG_GUARDHIT_WARRIOR] = CAI_TG_GuardHit::Create();
	m_arrStates[AI_STATE_TG_GROGGYHIT_WARRIOR] = CAI_TG_Groggy::Create();
	m_arrStates[AI_STATE_TG_STINGHIT_WARRIOR] = CAI_TG_StingHit::Create();
	m_arrStates[AI_STATE_TG_FLYHIT_WARRIOR] = CAI_TG_FlyHit::Create();


	//CRun_AI_TG_FlyHit
}

void CState_Manager::Spear_State()
{
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


	m_arrStates[STATE_ATTACK_STING_SPEARMAN] = CSpearMan_Attack_Sting::Create();
	m_arrStates[STATE_BOUNCE_SPEARMAN] = CBounce_SpearMan::Create();

	m_arrStates[STATE_SPEARMAN_GUARDBREAK] = CSpearMan_GuardBreak::Create();
	m_arrStates[STATE_SPEARMAN_SPECIALGUARD] = CSpearMan_SpecialGuard::Create();
	m_arrStates[STATE_SPEARMAN_BACKSTEP] = CSpearMan_BackStep::Create();
}

//void CState_Manager::Warrior_Base_R()
