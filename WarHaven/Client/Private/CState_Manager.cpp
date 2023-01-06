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

#pragma region Archer

#include "CIdle_Archer_L.h"
#include "CIdle_Archer_R.h"

#include "CWalk_Archer_L.h"
#include "CWalk_Archer_R.h"

#include "CRun_Archer_Begin_L.h"
#include "CRun_Archer_Begin_R.h"

#include "CRun_Archer_L.h"
#include "CRun_Archer_R.h"

#include "CJump_Archer_L.h"
#include "CJump_Archer_R.h"

#include "CJump_Archer_Fall_L.h"
#include "CJump_Archer_Fall_R.h"

#include "CJump_Archer_Land_L.h"
#include "CJump_Archer_Land_R.h"

#include "CStop_Archer_L.h"
#include "CStop_Archer_R.h"

#include "CSprint_Jump_Archer.h"
#include "CSprint_Jump_Fall_Archer.h"
#include "CSprint_Begin_Archer.h"
#include "CSprint_Loop_Archer.h"
#include "CSprint_End_Archer.h"

#include "CArcher_ClawAttack.h"

#include "CArcher_Attack_Begin.h"
#include "CArcher_Aiming.h"
#include "CArcher_Shoot.h"

#include "CArcher_Attack_Begin_Sniping.h"
#include "CArcher_Aiming_Sniping.h"
#include "CArcher_Shoot_Sniping.h"

#include "CArcher_Attack_Begin_Poison.h"
#include "CArcher_Aiming_Poison.h"
#include "CArcher_Shoot_Poison.h"

#include "CArcher_Swap.h"

#include "CBounce_Archer_R.h"

#include "CArcher_Attack_Cancel.h"

#include "CGuard_Loop_Archer.h"
#include "CGuard_Cancel_Archer.h"


#include "CHit_Archer.h"
#include "CHit_GuardHit_Archer.h"
#include "CHit_Groggy_Archer.h"
#include "CHit_Sting_Archer.h"
#include "CHit_Fly_Archer.h"



#pragma endregion

#pragma region Paladin

#include "CSwitchLtoR_Paladin.h"
#include "CSwitchRtoL_Paladin.h"

#include "CIdle_Paladin_L.h"
#include "CIdle_Paladin_R.h"

#include "CWalk_Paladin_L.h"
#include "CWalk_Paladin_R.h"

#include "CRun_Paladin_Begin_L.h"
#include "CRun_Paladin_Begin_R.h"

#include "CRun_Paladin_L.h"
#include "CRun_Paladin_R.h"

#include "CJump_Paladin_L.h"
#include "CJump_Paladin_R.h"

#include "CJump_Paladin_Fall_L.h"
#include "CJump_Paladin_Fall_R.h"

#include "CJump_Paladin_Land_L.h"
#include "CJump_Paladin_Land_R.h"

#include "CStop_Paladin_L.h"
#include "CStop_Paladin_R.h"

#include "CSprint_Begin_Paladin.h"
#include "CSprint_Loop_Paladin.h"
#include "CSprint_End_Paladin.h"

#include "CSprint_Jump_Paladin.h"
#include "CSprint_Jump_Fall_Paladin.h"
#include "CSprintAttack_Paladin_Begin.h"
#include "CSprintAttack_Paladin.h"

#include "CPaladin_Attack_HorizontalMiddle_L.h"
#include "CPaladin_Attack_HorizontalMiddle_R.h"
#include "CPaladin_Attack_VerticalCut.h"

#include "CPaladin_Rush_Begin.h"
#include "CPaladin_Rush_Loop.h"
#include "CPaladin_Rush_End.h"

#include "CPaladin_ShieldWall_Begin.h"
#include "CPaladin_ShieldWall_Loop.h"
#include "CPaladin_ShieldWall_End.h"
#include "CPaladin_ShieldWall_Hit.h"

#include "CPaladin_Attack_ShieldSlam.h"

#include "CState_Cannon_Player.h"
#include "CState_Gliding.h"
#include "CState_Gliding_Archer.h"

#include "CGuard_Begin_Paladin.h"
#include "CGuard_Loop_Paladin.h"
#include "CGuard_End_Paladin.h"
#include "CGuard_Cancel_Paladin.h"

#include "CBounce_Paladin_L.h"
#include "CBounce_Paladin_R.h"

#include "CHit_Paladin.h"
#include "CHit_GuardHit_Paladin.h"
#include "CHit_Groggy_Paladin.h"
#include "CHit_Sting_Paladin.h"
#include "CHit_Fly_Paladin.h"


#pragma endregion 

#pragma region Qanda


#include "CIdle_Qanda.h"
#include "CWalk_Qanda.h"
#include "CRun_Qanda.h"
#include "CJump_Qanda.h"
#include "CJump_Archer_Fall_Qanda.h"
#include "CJump_Archer_Land_Qanda.h"
#include "CStop_Qanda.h"

#include "CSprint_Begin_Qanda.h"
#include "CSprint_Loop_Qanda.h"
#include "CSprint_End_Qanda.h"

#include "CSprint_Jump_Qanda.h"
#include "CSprint_Jump_Fall_Qanda.h"


#include "CQanda_Attack_Begin.h"
#include "CQanda_Aiming.h"
#include "CQanda_Shoot.h"
#include "CQanda_Attack_Begin_Sniping.h"
#include "CQanda_Aiming_Sniping.h"
#include "CQanda_Shoot_Sniping.h"

#include "CQanda_Attack_Cancel.h"
#include "CQanda_ShadowStep.h"

#include "CGuard_Loop_Qanda.h"
#include "CGuard_Cancel_Qanda.h"

#include "CHit_Qanda.h"
#include "CHit_GuardHit_Qanda.h"
#include "CHit_Groggy_Qanda.h"
#include "CHit_Sting_Qanda.h"
#include "CHit_FlyHit_Qanda.h"




#pragma endregion 

#pragma region Priest

#include "CIdle_Priest.h"
#include "CWalk_Priest.h"
#include "CRun_Priest.h"
#include "CJump_Priest.h"
#include "CJump_Priest_Fall.h"
#include "CJump_Priest_Land.h"
#include "CStop_Priest.h"
#include "CBounce_Priest.h"

#include "CSprint_Begin_Priest.h"
#include "CSprint_Loop_Priest.h"
#include "CSprint_End_Priest.h"

#include "CSprint_Jump_Priest.h"
#include "CSprint_Jump_Fall_Priest.h"
#include "CSprintAttack_Priest_Begin.h"
#include "CSprintAttack_Priest.h"

#include "CPriest_Attack_Sting.h"
#include "CPriest_WindAttack.h"
#include "CAirDash_Priest.h"

#include "CPriest_Cure_Begin.h"
#include "CPriest_Cure_Loop.h"
#include "CPriest_Cure_End.h"

#include "CPriest_Catch_Begin.h"
#include "CPriest_Catch_Loop.h"
#include "CPriest_Catching.h"
#include "CPriest_Catch_Shoot.h"
  
#include "CHit_Priest.h"
#include "CHit_GuardHit_Priest.h"
#include "CHit_Groggy_Priest.h"
#include "CHit_Sting_Priest.h"
#include "CHit_Fly_Priest.h"


#pragma endregion 

#pragma region Priest

#include "CIdle_Lancer.h"
#include "CRun_Lancer.h"
#include "CStop_Lancer.h"
#include "CBounce_Lancer.h"


#include "CJump_Lancer.h"
#include "CJump_Lancer_Fall.h"
#include "CJump_Lancer_Land.h"

#include "CLancer_Attack_Default.h"
#include "CLancer_Attack_Sting.h"
#include "CLancer_Breeze_Begin.h"
#include "CLancer_Breeze_Loop.h"



#pragma endregion 


#pragma region AI_TG

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

#include "CState_PathNavigation_Test.h"

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

#pragma endregion 


#pragma region Patrol

#include "CState_Patrol_Default_Warrior_L.h"
#include "CState_Patrol_Default_Warrior_R.h"
#include "CState_Patrol_Switch_Warrior_CSwitchRtoL.h"
#include "CState_Patrol_Switch_Warrior_CSwitchLtoR.h"
#include "CState_Patrol_Walk_Warrior_L.h"
#include "CState_Patrol_Walk_Warrior_R.h"


#include "CState_Patrol_Default_Engineer_L.h"
#include "CState_Patrol_Default_Engineer_R.h"
#include "CState_Patrol_Switch_Engineer_CSwitchRtoL.h"
#include "CState_Patrol_Switch_Engineer_CSwitchLtoR.h"
#include "CState_Patrol_Walk_Engineer_L.h"
#include "CState_Patrol_Walk_Engineer_R.h"


#include "CState_Patrol_Default_Fiona_L.h"
#include "CState_Patrol_Default_Fiona_R.h"
#include "CState_Patrol_Switch_Fiona_CSwitchRtoL.h"
#include "CState_Patrol_Switch_Fiona_CSwitchLtoR.h"
#include "CState_Patrol_Walk_Fiona_L.h"
#include "CState_Patrol_Walk_Fiona_R.h"

#include "CState_Patrol_Default_Paladin_L.h"
#include "CState_Patrol_Default_Paladin_R.h"
#include "CState_Patrol_Switch_Paladin_CSwitchRtoL.h"
#include "CState_Patrol_Switch_Paladin_CSwitchLtoR.h"
#include "CState_Patrol_Walk_Paladin_L.h"
#include "CState_Patrol_Walk_Paladin_R.h"

#include "CState_Patrol_Default_Archer_L.h"
#include "CState_Patrol_Default_Archer_R.h"
#include "CState_Patrol_Walk_Archer_L.h"
#include "CState_Patrol_Walk_Archer_R.h"

#include "CState_Patrol_Walk_Priest.h"
#include "CState_Patrol_Default_Priest.h"
#include "CState_Patrol_Cure_Begin_Priest.h"
#include "CState_Patrol_Cure_Loop_Priest.h"
#include "CState_Patrol_Cure_End_Priest.h"


#pragma endregion 

#pragma region Combat

#include "CState_Combat_Run_Warrior_L.h"
#include "CState_Combat_Run_Warrior_R.h"
#include "CState_Combat_Attack_HorizontalUp_Warrior_L.h"
#include "CState_Combat_Attack_HorizontalUp_Warrior_R.h"
#include "CState_Combat_Attack_HorizontalMiddle_Warrior_L.h"
#include "CState_Combat_Attack_HorizontalMiddle_Warrior_R.h"
#include "CState_Combat_Attack_HorizontalDown_Warrior_L.h"
#include "CState_Combat_Attack_HorizontalDown_Warrior_R.h"
#include "CState_Combat_Attack_Warrior_VerticalCut.h"
#include "CState_Combat_Oxen_Begin_Warrior.h"
#include "CState_Combat_Oxen_Loop_Warrior.h"
#include "CState_Combat_Oxen_Cancel_Warrior.h"
#include "CState_Combat_Oxen_Loop_Attack_Warrior.h"
#include "CState_Combat_GuardBreak_Warrior.h"
#include "CState_Combat_GuardBegin_Warrior.h"
#include "CState_Combat_GuardLoop_Warrior.h"
#include "CState_Combat_GuardEnd_Warrior.h"
#include "CState_Combat_GuardDash_Warrior.h"
#include "CState_Combat_GuardCancel_Warrior.h"


#include "CState_Combat_Default_Engineer_L.h"
#include "CState_Combat_Default_Engineer_R.h"
#include "CState_Combat_SkillQ_AirSpike_Begin.h"
#include "CState_Combat_SkillQ_AirSpike_Loop.h"
#include "CState_Combat_SkillQ_AirSpike_End.h"
#include "CState_Combat_GuardBegin_Engineer.h"
#include "CState_Combat_GuardLoop_Engineer.h"
#include "CState_Combat_GuardDash_Engineer.h"
#include "CState_Combat_GuardEnd_Engineer.h"
#include "CState_Combat_GuardCancel_Engineer.h"
#include "CState_Combat_SkillE_Engineer_GroggyAttack.h"
#include "CState_Combat_Attack_HorizontalMiddle_Engineer_L.h"
#include "CState_Combat_Attack_HorizontalMiddle_Engineer_R.h"
#include "CState_Combat_Attack_VerticalCut_Engineer_L.h"
#include "CState_Combat_Attack_VerticalCut_Engineer_R.h"


#include "CState_Combat_Default_Fiona_L.h"
#include "CState_Combat_Default_Fiona_R.h"
#include "CState_Combat_SkillQ_Counter_Fiona.h"
#include "CState_Combat_SkillE_ShieldAttack_Fiona.h"
#include "CState_Combat_SkillR_SpinAttack_Fiona.h"
#include "CState_Combat_GuardBegin_Fiona.h"
#include "CState_Combat_GuardLoop_Fiona.h"
#include "CState_Combat_GuardDash_Fiona.h"
#include "CState_Combat_GuardEnd_Fiona.h"
#include "CState_Combat_GuardCancel_Fiona.h"
#include "CState_Combat_Attack_HorizontalUp_Fiona_L.h"
#include "CState_Combat_Attack_HorizontalUp_Fiona_R.h"
#include "CState_Combat_Attack_HorizontalMiddle_Fiona_L.h"
#include "CState_Combat_Attack_HorizontalMiddle_Fiona_R.h"
#include "CState_Combat_Attack_HorizontalDown_Fiona_L.h"
#include "CState_Combat_Attack_HorizontalDown_Fiona_R.h"
#include "CState_Combat_Attack_VerticalCut_Fiona.h"



#include "CState_Combat_Default_Paladin_L.h"
#include "CState_Combat_Default_Paladin_R.h"
#include "CState_Combat_SkillQ_Paladin_Rush_Begin.h"
#include "CState_Combat_SkillQ_Paladin_Rush_Loop.h"
#include "CState_Combat_SkillQ_Paladin_Rush_End.h"
#include "CState_Combat_SkillQ_Paladin_Rush_Begin.h"
#include "CState_Combat_SkillQ_Paladin_Rush_Loop.h"
#include "CState_Combat_SkillQ_Paladin_Rush_End.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_Begin.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_Loop.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_End.h"
#include "CState_Combat_SkillE_Paladin_ShieldWall_Hit.h"
#include "CState_Combat_GuardBegin_Paladin.h"
#include "CState_Combat_GuardLoop_Paladin.h"
#include "CState_Combat_GuardEnd_Paladin.h"
#include "CState_Combat_GuardCancel_Paladin.h"
#include "CState_Combat_Attack_HorizontalMiddle_Paladin_L.h"
#include "CState_Combat_Attack_HorizontalMiddle_Paladin_R.h"
#include "CState_Combat_Attack_VerticalCut_Paladin.h"
#include "CState_Combat_SkillR_Paladin_ShieldSlam.h"


#include "CState_Combat_Default_Archer_L.h"
#include "CState_Combat_Default_Archer_R.h"
#include "CState_Combat_Attack_Archer_Begin.h"
#include "CState_Combat_Attack_Archer_Aiming.h"
#include "CState_Combat_Attack_Archer_Shoot.h"
#include "CState_Combat_Attack_Archer_Begin_Poison.h"
#include "CState_Combat_Attack_Archer_Aiming_Poison.h"
#include "CState_Combat_Attack_Archer_Shoot_Poison.h"
#include "CState_Combat_Attack_Archer_Begin_Sniping.h"
#include "CState_Combat_Attack_Archer_Aiming_Sniping.h"
#include "CState_Combat_Attack_Archer_Shoot_Sniping.h"

#include "CState_Combat_Default_Priest.h"
#include "CState_Combat_Cure_Begin_Priest.h"
#include "CState_Combat_Cure_Loop_Priest.h"
#include "CState_Combat_Cure_End_Priest.h"
#include "CState_Combat_Attack_Sting_Priest.h"
#include "CState_Combat_SkillR_AirDash_Priest.h"
#include "CState_Combat_SkillE_Priest_WindAttack.h"


#pragma endregion

#pragma region PathNavi

#include "CState_PathNavigation_Run_Warrior_L.h"
#include "CState_PathNavigation_Run_Warrior_R.h"
#include "CState_PathNavigation_Walk_Warrior_L.h"
#include "CState_PathNavigation_Walk_Warrior_R.h"
#include "CState_PathNavigation_Sprint_Warrior_Begin.h"
#include "CState_PathNavigation_Sprint_Warrior_Loop.h"
#include "CState_PathNavigation_Sprint_Warrior_End.h"
#include "CState_PathNavigation_Sprint_Warrior_Fall.h"
#include "CState_PathNavigation_Sprint_Warrior_Jump.h"
#include "CState_PathNavigation_Jump_Warrior_L.h"
#include "CState_PathNavigation_Jump_Warrior_R.h"


#include "CState_PathNavigation_Run_Engineer_L.h"
#include "CState_PathNavigation_Run_Engineer_R.h"
#include "CState_PathNavigation_Walk_Engineer_L.h"
#include "CState_PathNavigation_Walk_Engineer_R.h"
#include "CState_PathNavigation_Sprint_Engineer_Begin.h"
#include "CState_PathNavigation_Sprint_Engineer_Loop.h"
#include "CState_PathNavigation_Sprint_Engineer_End.h"
#include "CState_PathNavigation_Sprint_Engineer_Fall.h"
#include "CState_PathNavigation_Sprint_Engineer_Jump.h"
#include "CState_PathNavigation_Jump_Engineer_L.h"
#include "CState_PathNavigation_Jump_Engineer_R.h"


#include "CState_PathNavigation_Default_Fiona_L.h"
#include "CState_PathNavigation_Default_Fiona_R.h"
#include "CState_PathNavigation_Walk_Fiona_L.h"
#include "CState_PathNavigation_Walk_Fiona_R.h"
#include "CState_PathNavigation_Sprint_Fiona_Begin.h"
#include "CState_PathNavigation_Sprint_Fiona_Loop.h"
#include "CState_PathNavigation_Sprint_Fiona_End.h"
#include "CState_PathNavigation_Sprint_Fiona_Fall.h"
#include "CState_PathNavigation_Sprint_Fiona_Jump.h"
#include "CState_PathNavigation_Jump_Fiona_L.h"
#include "CState_PathNavigation_Jump_Fiona_R.h"

#include "CState_PathNavigation_Default_Paladin_L.h"
#include "CState_PathNavigation_Default_Paladin_R.h"
#include "CState_PathNavigation_Walk_Paladin_L.h"
#include "CState_PathNavigation_Walk_Paladin_R.h"
#include "CState_PathNavigation_Sprint_Paladin_Begin.h"
#include "CState_PathNavigation_Sprint_Paladin_Loop.h"
#include "CState_PathNavigation_Sprint_Paladin_End.h"
#include "CState_PathNavigation_Sprint_Paladin_Fall.h"
#include "CState_PathNavigation_Sprint_Paladin_Jump.h"
#include "CState_PathNavigation_Jump_Paladin_L.h"
#include "CState_PathNavigation_Jump_Paladin_R.h"


#include "CState_PathNavigation_Default_Priest.h"
#include "CState_PathNavigation_Sprint_Priest_Jump.h"
#include "CState_PathNavigation_Sprint_Priest_Fall.h"
#include "CState_PathNavigation_Sprint_Priest_Begin.h"
#include "CState_PathNavigation_Sprint_Priest_Loop.h"
#include "CState_PathNavigation_Sprint_Priest_End.h"
#include "CState_PathNavigation_Jump_Priest.h"
#include "CState_PathNavigation_Walk_Priest.h"


#pragma endregion

#pragma region Common

#include "CState_Common_ChangeHero_AI.h"


#include "CState_Common_Hit_Warrior.h"
#include "CState_Common_GuardHit_Warrior.h"
#include "CState_Common_Groggy_Warrior.h"
#include "CState_Common_Sting_Warrior.h"
#include "CState_Common_FlyHit_Warrior.h"
#include "CState_Common_Land_Warrior_L.h"
#include "CState_Common_Land_Warrior_R.h"
#include "CState_Common_Fall_Warrior_L.h"
#include "CState_Common_Fall_Warrior_R.h"
#include "CState_Common_Bounce_Warrior_L.h"
#include "CState_Common_Bounce_Warrior_R.h"


#include "CState_Common_Hit_Engineer.h"
#include "CState_Common_GuardHit_Engineer.h"
#include "CState_Common_Groggy_Engineer.h"
#include "CState_Common_Sting_Engineer.h"
#include "CState_Common_FlyHit_Engineer.h"
#include "CState_Common_Fall_Engineer_L.h"
#include "CState_Common_Fall_Engineer_R.h"
#include "CState_Common_Land_Engineer_L.h"
#include "CState_Common_Land_Engineer_R.h"
#include "CState_Common_Bounce_Engineer_L.h"
#include "CState_Common_Bounce_Engineer_R.h"


#include "CState_Common_Land_Fiona_L.h"
#include "CState_Common_Land_Fiona_R.h"
#include "CState_Common_Fall_Fiona_L.h"
#include "CState_Common_Fall_Fiona_R.h"
#include "CState_Common_Hit_Fiona.h"
#include "CState_Common_GuardHit_Fiona.h"
#include "CState_Common_Groggy_Fiona.h"
#include "CState_Common_Sting_Fiona.h"
#include "CState_Common_FlyHit_Fiona.h"
#include "CState_Common_Bounce_Fiona_L.h"
#include "CState_Common_Bounce_Fiona_R.h"


#include "CState_Common_Land_Paladin_L.h"
#include "CState_Common_Land_Paladin_R.h"
#include "CState_Common_Fall_Paladin_L.h"
#include "CState_Common_Fall_Paladin_R.h"
#include "CState_Common_Hit_Paladin.h"
#include "CState_Common_GuardHit_Paladin.h"
#include "CState_Common_Groggy_Paladin.h"
#include "CState_Common_Sting_Paladin.h"
#include "CState_Common_FlyHit_Paladin.h"
#include "CState_Common_Bounce_Paladin_L.h"
#include "CState_Common_Bounce_Paladin_R.h"

#include "CState_Common_Land_Archer_L.h"
#include "CState_Common_Land_Archer_R.h"
#include "CState_Common_Fall_Archer_L.h"
#include "CState_Common_Fall_Archer_R.h"
#include "CState_Common_Hit_Archer.h"
#include "CState_Common_GuardHit_Archer.h"
#include "CState_Common_Groggy_Archer.h"
#include "CState_Common_Sting_Archer.h"
#include "CState_Common_FlyHit_Archer.h"
#include "CState_Common_Bounce_Archer.h"


#include "CState_Common_Fall_Priest.h"
#include "CState_Common_Land_Priest.h"
#include "CState_Common_Bounce_Priest.h"
#include "CState_Common_Hit_Priest.h"
#include "CState_Common_Groggy_Priest.h"
#include "CState_Common_Sting_Priest.h"
#include "CState_Common_FlyHit_Priest.h"

#pragma endregion


#include "CState_NoPattern.h"

#pragma region ETC

#include "CRevive_Player.h"
#include "CState_Victory.h"
#include "CState_Defeat.h"
#include "CState_Common_Revive_AI.h"

#include "CState_Common_Slide_Begin_AI.h"
#include "CState_Common_Slide_Loop_AI.h"
#include "CState_Common_Slide_End_AI.h"

#pragma endregion


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
	m_arrStates[NO_PATTERN] = CState_NoPattern::Create();
	m_arrStates[AI_STATE_COMMON_CHANGE_HERO] = CState_Common_ChangeHero_AI::Create();



	m_arrStates[STATE_REVIVE_PLAYER] = CRevive_Player::Create();
	m_arrStates[STATE_CHANGE_PLAYER] = CChangeHero_Player::Create();
	m_arrStates[STATE_VICTORY] = CState_Victory::Create();
	m_arrStates[STATE_DEFEAT] = CState_Defeat::Create();
	m_arrStates[STATE_CANNON_PLAYER] = CState_Cannon_Player::Create();
	m_arrStates[STATE_GLIDING] = CState_Gliding::Create();
	m_arrStates[STATE_GLIDING_ARCHER] = CState_Gliding_Archer::Create();
	m_arrStates[STATE_TRANSFORM] = CState_Victory::Create();


	m_arrStates[AI_STATE_COMMON_REVIVE_AI] = CState_Common_Revive_AI::Create();
	m_arrStates[AI_STATE_COMMON_SLIDEBEGIN_AI] = CState_Common_Slide_Begin_AI::Create();
	m_arrStates[AI_STATE_COMMON_SLIDELOOP_AI] = CState_Common_Slide_Loop_AI::Create();
	m_arrStates[AI_STATE_COMMON_SLIDEEND_AI] = CState_Common_Slide_End_AI::Create();
	m_arrStates[AI_STATE_COMMON_TRANSFORM_AI] = CState_Victory::Create();


	Spear_State();

	// 기본
	Warrior_State();
	WarHammer_State();
	Archer_State();
	Paladin_State();
	Priest_State();

	// 영웅
	Valkyrie_State();
	Qanda_State();
	Lancer_State();

	// AI 기본
	Warrior_SandBagState();
	
	Warrior_State_AI();
	WarHammer_State_AI();
	Valkyrie_State_AI();
	Paladin_State_AI();
	Archer_State_AI();
	Priest_State_AI();
	Lancer_State_AI();

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

void CState_Manager::Archer_State()
{

	/* Idle */
	m_arrStates[STATE_IDLE_ARCHER_L] = CIdle_Archer_L::Create();
	m_arrStates[STATE_IDLE_ARCHER_R] = CIdle_Archer_R::Create();

	/* Walk */
	m_arrStates[STATE_WALK_ARCHER_L] = CWalk_Archer_L::Create();
	m_arrStates[STATE_WALK_ARCHER_R] = CWalk_Archer_R::Create();

	/* RunBegin */
	m_arrStates[STATE_RUNBEGIN_ARCHER_L] = CRun_Archer_Begin_L::Create();
	m_arrStates[STATE_RUNBEGIN_ARCHER_R] = CRun_Archer_Begin_R::Create();

	/* Run */
	m_arrStates[STATE_RUN_ARCHER_L] = CRun_Archer_L::Create();
	m_arrStates[STATE_RUN_ARCHER_R] = CRun_Archer_R::Create();

	/* Jump */
	m_arrStates[STATE_JUMP_ARCHER_L] = CJump_Archer_L::Create();
	m_arrStates[STATE_JUMPFALL_ARCHER_L] = CJump_Archer_Fall_L::Create();
	m_arrStates[STATE_JUMP_LAND_ARCHER_L] = CJump_Archer_Land_L::Create();

	m_arrStates[STATE_JUMP_ARCHER_R] = CJump_Archer_R::Create();
	m_arrStates[STATE_JUMPFALL_ARCHER_R] = CJump_Archer_Fall_R::Create();
	m_arrStates[STATE_JUMP_LAND_ARCHER_R] = CJump_Archer_Land_R::Create();

	/* Stop */
	m_arrStates[STATE_STOP_ARCHER_L] = CStop_Archer_L::Create();
	m_arrStates[STATE_STOP_ARCHER_R] = CStop_Archer_R::Create();


	m_arrStates[STATE_SPRINT_JUMP_ARCHER] = CSprint_Jump_Archer::Create();
	m_arrStates[STATE_SPRINT_JUMPFALL_ARCHER] = CSprint_Jump_Fall_Archer::Create();
	m_arrStates[STATE_SPRINT_BEGIN_ARCHER] = CSprint_Begin_Archer::Create();
	m_arrStates[STATE_SPRINT_LOOP_ARCHER] = CSprint_Loop_Archer::Create();
	m_arrStates[STATE_SPRINT_END_ARCHER] = CSprint_End_Archer::Create();

	m_arrStates[STATE_ATTACK_SWING_ARCHER] = CArcher_ClawAttack::Create();

	m_arrStates[STATE_ATTACK_BEGIN_ARCHER] = CArcher_Attack_Begin::Create();
	m_arrStates[STATE_ATTACK_AIMING_ARCHER] = CArcher_Aiming::Create();
	m_arrStates[STATE_ATTACK_SHOOT_ARCHER] = CArcher_Shoot::Create();

	m_arrStates[STATE_ATTACK_BEGIN_SNIPING_ARCHER] = CArcher_Attack_Begin_Sniping::Create();
	m_arrStates[STATE_ATTACK_AIMING_SNIPING_ARCHER] = CArcher_Aiming_Sniping::Create();
	m_arrStates[STATE_ATTACK_SHOOT_SNIPING_ARCHER] = CArcher_Shoot_Sniping::Create();

	m_arrStates[STATE_ATTACK_BEGIN_POISION_ARCHER] = CArcher_Attack_Begin_Poison::Create();
	m_arrStates[STATE_ATTACK_AIMING_POISION_ARCHER] = CArcher_Aiming_Poison::Create();
	m_arrStates[STATE_ATTACK_SHOOT_POISION_ARCHER] = CArcher_Shoot_Poison::Create();

	m_arrStates[STATE_SWAP_ARCHER] = CArcher_Swap::Create();

	m_arrStates[STATE_BOUNCE_ARCHER] = CBounce_Archer_R::Create();

	m_arrStates[STATE_ATTACK_CANCEL_ARCHER] = CArcher_Attack_Cancel::Create();

	m_arrStates[STATE_GUARD_ARCHER] = CGuard_Loop_Archer::Create();
	m_arrStates[STATE_GUARD_CANCEL_ARCHER] = CGuard_Cancel_Archer::Create();
		

	m_arrStates[STATE_HIT_ARCHER] = CHit_Archer::Create();
	m_arrStates[STATE_GUARDHIT_ARCHER] = CHit_GuardHit_Archer::Create();
	m_arrStates[STATE_GROGGYHIT_ARCHER] = CHit_Groggy_Archer::Create();
	m_arrStates[STATE_STINGHIT_ARCHER] = CHit_Sting_Archer::Create();
	m_arrStates[STATE_FLYHIT_ARCHER] = CHit_Fly_Archer::Create();


	//switch (i)
	//{
	//case 0:
	//	strKey = "EnemyTrio_1";
	//	wstrInfoKey = L"EnemyTrio_0";
	//	eEnemyState = SANDBAG_STATE_IDLE_WARHAMMER_R;

	//	break;
	//case 1:
	//	strKey = "EnemyTrio_2";
	//	wstrInfoKey = L"EnemyTrio_1";
	//	eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;

	//	break;
	//case 2:
	//	strKey = "q";
	//	wstrInfoKey = L"EnemyTrio_2";
	//	eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
	//	break;
	//case 3:
	//	strKey = "EnemyHall";
	//	wstrInfoKey = L"EnemyHall";
	//	break;
	//case 4:
	//	strKey = "EnemyBlock";
	//	wstrInfoKey = L"EnemyBlock";
	//	eEnemyState = STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY;
	//	break;
	//case 5:
	//	strKey = "EnemyHeadShot";
	//	wstrInfoKey = L"EnemyHeadShot";
	//	eEnemyState = STATE_IDLE_WARRIOR_R_AI_ENEMY;
	//	break;
	//case 6:
	//	strKey = "EnemyGuardBreak";
	//	wstrInfoKey = L"EnemyGuardBreak";
	//	eEnemyState = STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY;

	//	break;

	//case 7:
	//	strKey = "EnemyFinal";
	//	wstrInfoKey = L"EnemyFinal";
	//	eEnemyState = AI_STATE_IDLE_WARRIOR_L;

	//	break;

	//default:
	//	break;
	//}

}
void CState_Manager::Paladin_State()
{




	m_arrStates[STATE_IDLE_PALADIN_L] = CIdle_Paladin_L::Create();
	m_arrStates[STATE_IDLE_PALADIN_R] = CIdle_Paladin_R::Create();

	/* Walk */
	m_arrStates[STATE_WALK_PALADIN_L] = CWalk_Paladin_L::Create();
	m_arrStates[STATE_WALK_PALADIN_R] = CWalk_Paladin_R::Create();

	/* RunBegin */
	m_arrStates[STATE_RUNBEGIN_PALADIN_L] = CRun_Paladin_Begin_L::Create();
	m_arrStates[STATE_RUNBEGIN_PALADIN_R] = CRun_Paladin_Begin_R::Create();

	/* Run */
	m_arrStates[STATE_RUN_PALADIN_L] = CRun_Paladin_L::Create();
	m_arrStates[STATE_RUN_PALADIN_R] = CRun_Paladin_R::Create();

	/* Jump */
	m_arrStates[STATE_JUMP_PALADIN_L] = CJump_Paladin_L::Create();
	m_arrStates[STATE_JUMPFALL_PALADIN_L] = CJump_Paladin_Fall_L::Create();
	m_arrStates[STATE_JUMP_LAND_PALADIN_L] = CJump_Paladin_Land_L::Create();

	m_arrStates[STATE_JUMP_PALADIN_R] = CJump_Paladin_R::Create();
	m_arrStates[STATE_JUMPFALL_PALADIN_R] = CJump_Paladin_Fall_R::Create();
	m_arrStates[STATE_JUMP_LAND_PALADIN_R] = CJump_Paladin_Land_R::Create();

	/* Stop */
	m_arrStates[STATE_STOP_PALADIN_L] = CStop_Paladin_L::Create();
	m_arrStates[STATE_STOP_PALADIN_R] = CStop_Paladin_R::Create();


	/* Sprint */
	m_arrStates[STATE_SPRINT_BEGIN_PALADIN] = CSprint_Begin_Paladin::Create();
	m_arrStates[STATE_SPRINT_LOOP_PALADIN] = CSprint_Loop_Paladin::Create();
	m_arrStates[STATE_SPRINT_END_PALADIN] = CSprint_End_Paladin::Create();


	m_arrStates[STATE_SPRINT_JUMPFALL_PALADIN] = CSprint_Jump_Fall_Paladin::Create();

	m_arrStates[STATE_SPRINT_JUMP_PALADIN] = CSprint_Jump_Paladin::Create();

	m_arrStates[STATE_SPRINTATTACK_BEGIN_PALADIN] = CSprintAttack_Paladin_Begin::Create();
	m_arrStates[STATE_SPRINTATTACK_PALADIN] = CSprintAttack_Paladin::Create();


	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L] = CPaladin_Attack_HorizontalMiddle_L::Create();
	m_arrStates[STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R] = CPaladin_Attack_HorizontalMiddle_R::Create();
	m_arrStates[STATE_ATTACK_VERTICALCUT_PALADIN] = CPaladin_Attack_VerticalCut::Create();
	
	 
	m_arrStates[STATE_RUSH_BEGIN_PALADIN] = CPaladin_Rush_Begin::Create();
	m_arrStates[STATE_RUSH_LOOP_PALADIN] = CPaladin_Rush_Loop::Create();
	m_arrStates[STATE_RUSH_END_PALADIN] = CPaladin_Rush_End::Create();




	m_arrStates[STATE_SHIELDWALL_BEGIN_PALADIN] = CPaladin_ShieldWall_Begin::Create();
	m_arrStates[STATE_SHIELDWALL_LOOP_PALADIN] = CPaladin_ShieldWall_Loop::Create();
	m_arrStates[STATE_SHIELDWALL_END_PALADIN] = CPaladin_ShieldWall_End::Create();
	m_arrStates[STATE_SHIELDWALL_HIT_PALADIN] = CPaladin_ShieldWall_Hit::Create();

	m_arrStates[STATE_SHIELDSLAM_PALADIN] = CPaladin_Attack_ShieldSlam::Create();

	m_arrStates[STATE_BOUNCE_PALADIN_L] = CBounce_Paladin_L::Create();
	m_arrStates[STATE_BOUNCE_PALADIN_R] = CBounce_Paladin_R::Create();


	m_arrStates[STATE_GUARD_BEGIN_PALADIN] = CGuard_Begin_Paladin::Create();
	m_arrStates[STATE_GUARD_LOOP_PALADIN] = CGuard_Loop_Paladin::Create();
	m_arrStates[STATE_GUARD_END_PALADIN] = CGuard_End_Paladin::Create();
	m_arrStates[STATE_GUARD_CANCEL_PALADIN] = CGuard_Cancel_Paladin::Create();


	m_arrStates[STATE_SWITCH_R_TO_L_PALADIN] = CSwitchLtoR_Paladin::Create();
	m_arrStates[STATE_SWITCH_L_TO_R_PALADIN] = CSwitchRtoL_Paladin::Create();




	m_arrStates[STATE_HIT_PALADIN] = CHit_Paladin::Create();
	m_arrStates[STATE_GUARDHIT_PALADIN] = CHit_GuardHit_Paladin::Create();
	m_arrStates[STATE_GROGGYHIT_PALADIN] = CHit_Groggy_Paladin::Create();
	m_arrStates[STATE_STINGHIT_PALADIN] = CHit_Sting_Paladin::Create();
	m_arrStates[STATE_FLYHIT_PALADIN] = CHit_Fly_Paladin::Create();

}

void CState_Manager::Priest_State()
{
	/*
		CIdle_Priest
		CWalk_Priest
		CRun_Priest
		CJump_Priest
		CJump_Priest_Fall
		CJump_Priest_Land 
		CStop_Priest
		CBounce_Priest
		CSprint_Begin_Priest
		CSprint_Loop_Priest
		CSprint_End_Priest
		CSprint_Jump_Fall_Priest
		CSprint_Jump_Priest
		CSprintAttack_Priest_Begin
		CSprintAttack_Priest
	*/

	m_arrStates[STATE_IDLE_PRIEST] = CIdle_Priest::Create();
	m_arrStates[STATE_WALK_PRIEST] = CWalk_Priest::Create();
	m_arrStates[STATE_RUN_PRIEST] = CRun_Priest::Create();


	m_arrStates[STATE_JUMP_PRIEST] = CJump_Priest::Create();
	m_arrStates[STATE_JUMPFALL_PRIEST] = CJump_Priest_Fall::Create();
	m_arrStates[STATE_JUMP_LAND_PRIEST] = CJump_Priest_Land::Create();

	m_arrStates[STATE_STOP_PRIEST] = CStop_Priest::Create();


	m_arrStates[STATE_BOUNCE_PRIEST] = CBounce_Priest::Create();


	/* Sprint */
	m_arrStates[STATE_SPRINT_BEGIN_PRIEST] = CSprint_Begin_Priest::Create();
	m_arrStates[STATE_SPRINT_LOOP_PRIEST] = CSprint_Loop_Priest::Create();
	m_arrStates[STATE_SPRINT_END_PRIEST] = CSprint_End_Priest::Create();


	m_arrStates[STATE_SPRINT_JUMPFALL_PRIEST] = CSprint_Jump_Fall_Priest::Create();

	m_arrStates[STATE_SPRINT_JUMP_PRIEST] = CSprint_Jump_Priest::Create();

	m_arrStates[STATE_SPRINTATTACK_BEGIN_PRIEST] = CSprintAttack_Priest_Begin::Create();
	m_arrStates[STATE_SPRINTATTACK_PRIEST] = CSprintAttack_Priest::Create();


	m_arrStates[STATE_CURE_BEGIN_PRIEST] = CPriest_Cure_Begin::Create();
	m_arrStates[STATE_CURE_LOOP_PRIEST] = CPriest_Cure_Loop::Create();
	m_arrStates[STATE_CURE_END_PRIEST] = CPriest_Cure_End::Create();

	
	m_arrStates[STATE_WINDATTACK_PRIEST] = CPriest_WindAttack::Create();
	m_arrStates[STATE_ATTACK_STING_PRIEST] = CPriest_Attack_Sting::Create();
	m_arrStates[STATE_AIRDASH_PRIEST] = CAirDash_Priest::Create();

	// 수정
	m_arrStates[STATE_PROJECTILECATCH_BEGIN_PRIEST] = CPriest_Catch_Begin::Create();
	m_arrStates[STATE_PROJECTILECATCH_LOOP_PRIEST] = CPriest_Catch_Loop::Create();
	m_arrStates[STATE_PROJECTILECATCH_HIT_PRIEST] = CPriest_Catching::Create();
	m_arrStates[STATE_PROJECTILECATCH_SHOOT_PRIEST] = CPriest_Catch_Shoot::Create();
	//

	m_arrStates[STATE_HIT_PRIEST] = CHit_Priest::Create();
	m_arrStates[STATE_GUARDHIT_PRIEST] = CHit_GuardHit_Priest::Create();
	m_arrStates[STATE_GROGGYHIT_PRIEST] = CHit_Groggy_Priest::Create();
	m_arrStates[STATE_STINGHIT_PRIEST] = CHit_Sting_Priest::Create();
	m_arrStates[STATE_FLYHIT_PRIEST] = CHit_Fly_Priest::Create();





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

void CState_Manager::Qanda_State()
{


	/* Idle */
	m_arrStates[STATE_IDLE_QANDA] = CIdle_Qanda::Create();

	/* Walk */
	m_arrStates[STATE_WALK_QANDA] = CWalk_Qanda::Create();

	/* Run */
	m_arrStates[STATE_RUN_QANDA] = CRun_Qanda::Create();

	/* Jump */
	m_arrStates[STATE_JUMP_QANDA] = CJump_Qanda::Create();
	m_arrStates[STATE_JUMPFALL_QANDA] = CJump_Archer_Fall_Qanda::Create();
	m_arrStates[STATE_JUMP_LAND_QANDA] = CJump_Archer_Land_Qanda::Create();


	/* Stop */
	m_arrStates[STATE_STOP_QANDA] = CStop_Qanda::Create();

	/* Sprint */
	m_arrStates[STATE_SPRINT_BEGIN_QANDA] = CSprint_Begin_Qanda::Create();
	m_arrStates[STATE_SPRINT_LOOP_QANDA] = CSprint_Loop_Qanda::Create();
	m_arrStates[STATE_SPRINT_END_QANDA] = CSprint_End_Qanda::Create();

	m_arrStates[STATE_SPRINT_JUMPFALL_QANDA] = CSprint_Jump_Fall_Qanda::Create();

	m_arrStates[STATE_SPRINT_JUMP_QANDA] = CSprint_Jump_Qanda::Create();

	m_arrStates[STATE_ATTACK_BEGIN_QANDA] = CQanda_Attack_Begin::Create(); 
	m_arrStates[STATE_ATTACK_AIMING_QANDA] = CQanda_Aiming::Create();
	m_arrStates[STATE_ATTACK_SHOOT_QANDA] = CQanda_Shoot::Create();


	m_arrStates[STATE_ATTACK_BEGIN_SNIPING_QANDA] = CQanda_Attack_Begin_Sniping::Create();
	m_arrStates[STATE_ATTACK_AIMING_SNIPING_QANDA] = CQanda_Aiming_Sniping::Create();
	m_arrStates[STATE_ATTACK_SHOOT_SNIPING_QANDA] = CQanda_Shoot_Sniping::Create();

	m_arrStates[STATE_ATTACK_CANCEL_QANDA] = CQanda_Attack_Cancel::Create();

	m_arrStates[STATE_SHADOWSTEP_QANDA] = CQanda_ShadowStep::Create();



	m_arrStates[STATE_GUARD_QANDA] = CGuard_Loop_Qanda::Create();
	m_arrStates[STATE_GUARD_CANCEL_QANDA] = CGuard_Cancel_Qanda::Create();




	m_arrStates[STATE_HIT_QANDA] = CHit_Qanda::Create();
	m_arrStates[STATE_GUARDHIT_QANDA] = CHit_GuardHit_Qanda::Create();
	m_arrStates[STATE_GROGGYHIT_QANDA] = CHit_Groggy_Qanda::Create();
	m_arrStates[STATE_STINGHIT_QANDA] = CHit_Sting_Qanda::Create();
	m_arrStates[STATE_FLYHIT_QANDA] = CHit_FlyHit_Qanda::Create();

	/* Base_R */

}


void CState_Manager::Lancer_State()
{
	m_arrStates[STATE_IDLE_LANCER] = CIdle_Lancer::Create();
	m_arrStates[STATE_RUN_LANCER] = CRun_Lancer::Create();

	m_arrStates[STATE_JUMP_LANCER] = CJump_Lancer::Create();
	m_arrStates[STATE_JUMPFALL_LANCER] = CJump_Lancer_Fall::Create();
	m_arrStates[STATE_JUMP_LAND_LANCER] = CIdle_Lancer::Create();
	//m_arrStates[STATE_JUMP_LAND_LANCER] = CJump_Lancer_Land::Create();

	m_arrStates[STATE_STOP_LANCER] = CStop_Lancer::Create();
	m_arrStates[STATE_BOUNCE_LANCER] = CBounce_Lancer::Create();
	

	m_arrStates[STATE_ATTACK_DEAFULT_LANCER] = CLancer_Attack_Default::Create();
	m_arrStates[STATE_ATTACK_STING_LANCER] = CLancer_Attack_Sting::Create();

	m_arrStates[STATE_ATTACK_BREEZE_BEGIN_LANCER] = CLancer_Breeze_Begin::Create();
	m_arrStates[STATE_ATTACK_BREEZE_LOOP_LANCER] = CLancer_Breeze_Loop::Create();


	// 수정 ㄱㄱ
	m_arrStates[STATE_HIT_LANCER] = CIdle_Lancer::Create();
	m_arrStates[STATE_GUARDHIT_LANCER] = CRun_Lancer::Create();
	m_arrStates[STATE_GROGGYHIT_LANCER] = CIdle_Lancer::Create();
	m_arrStates[STATE_STINGHIT_LANCER] = CRun_Lancer::Create();
	m_arrStates[STATE_FLYHIT_LANCER] = CStop_Lancer::Create();

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

	//---------------------------------------------------------------------------------//
	
#pragma region Patrol
	m_arrStates[AI_STATE_PATROL_DEFAULT_WARRIOR_L] = CState_Patrol_Default_Warrior_L::Create();
	m_arrStates[AI_STATE_PATROL_DEFAULT_WARRIOR_R] = CState_Patrol_Default_Warrior_R::Create();
						 
	m_arrStates[AI_STATE_PATROL_WARRIOR_SWITCH_R_TO_L] = CState_Patrol_Switch_Warrior_CSwitchRtoL::Create();
	m_arrStates[AI_STATE_PATROL_WARRIOR_SWITCH_L_TO_R] = CState_Patrol_Switch_Warrior_CSwitchLtoR::Create();
	m_arrStates[AI_STATE_PATROL_WALK_WARRIOR_L] = CState_Patrol_Walk_Warrior_L::Create();
	m_arrStates[AI_STATE_PATROL_WALK_WARRIOR_R] = CState_Patrol_Walk_Warrior_R::Create();

#pragma endregion

#pragma region PathNavi
	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_L] = CState_PathNavigation_Run_Warrior_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R] = CState_PathNavigation_Run_Warrior_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_WARRIOR_L] = CState_PathNavigation_Walk_Warrior_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_WARRIOR_R] = CState_PathNavigation_Walk_Warrior_R::Create();

	
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTBEGIN_WARRIOR] = CState_PathNavigation_Sprint_Warrior_Begin::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTLOOP_WARRIOR] = CState_PathNavigation_Sprint_Warrior_Loop::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR] = CState_PathNavigation_Sprint_Warrior_End::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMP_WARRIOR] = CState_PathNavigation_Sprint_Warrior_Jump::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_WARRIOR] = CState_PathNavigation_Sprint_Warrior_Fall::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_WARRIOR_L] = CState_PathNavigation_Jump_Warrior_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_WARRIOR_R] = CState_PathNavigation_Jump_Warrior_R::Create();

#pragma endregion

#pragma region Combat
	m_arrStates[AI_STATE_COMBAT_DEFAULT_WARRIOR_L] = CState_Combat_Run_Warrior_L::Create();
	m_arrStates[AI_STATE_COMBAT_DEFAULT_WARRIOR_R] = CState_Combat_Run_Warrior_R::Create();
	

	m_arrStates[AI_STATE_COMBAT_OXEN_BEGIN_WARRIOR] = CState_Combat_Oxen_Begin_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_OXEN_LOOP_WARRIOR] = CState_Combat_Oxen_Loop_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR] = CState_Combat_Oxen_Loop_Attack_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_OXEN_END_WARRIOR] = CState_Combat_Oxen_Cancel_Warrior::Create();


	m_arrStates[AI_STATE_COMBAT_GUARDBEGIN_WARRIOR] = CState_Combat_GuardBegin_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDLOOP_WARRIOR] = CState_Combat_GuardLoop_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDDASH_WARRIOR] = CState_Combat_GuardDash_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDEND_WARRIOR] = CState_Combat_GuardEnd_Warrior::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDCANCEL_WARRIOR] = CState_Combat_GuardCancel_Warrior::Create();

	m_arrStates[AI_STATE_COMBAT_GUARDBREAK_WARRIOR] = CState_Combat_GuardBreak_Warrior::Create();
	
	m_arrStates[AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_L] = CState_Combat_Attack_HorizontalUp_Warrior_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_R] = CState_Combat_Attack_HorizontalUp_Warrior_R::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L] = CState_Combat_Attack_HorizontalMiddle_Warrior_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R] = CState_Combat_Attack_HorizontalMiddle_Warrior_R::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALDOWN_WARRIOR_L] = CState_Combat_Attack_HorizontalDown_Warrior_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALDOWN_WARRIOR_R] = CState_Combat_Attack_HorizontalDown_Warrior_R::Create();

	m_arrStates[AI_STATE_COMBAT_VERTICALCUT_WARRIOR] = CState_Combat_Attack_Warrior_VerticalCut::Create();

#pragma endregion

#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_WARRIOR_L] = CState_Common_Fall_Warrior_L::Create();
	m_arrStates[AI_STATE_COMMON_FALL_WARRIOR_R] = CState_Common_Fall_Warrior_R::Create();
	m_arrStates[AI_STATE_COMMON_LAND_WARRIOR_L] = CState_Common_Land_Warrior_L::Create();
	m_arrStates[AI_STATE_COMMON_LAND_WARRIOR_R] = CState_Common_Land_Warrior_R::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_WARRIOR_L] = CState_Common_Bounce_Warrior_L::Create();
	m_arrStates[AI_STATE_COMMON_BOUNCE_WARRIOR_R] = CState_Common_Bounce_Warrior_R::Create();

	m_arrStates[AI_STATE_COMMON_HIT_WARRIOR] = CState_Common_Hit_Warrior::Create();
	m_arrStates[AI_STATE_COMMON_GUARDHIT_WARRIOR] = CState_Common_GuardHit_Warrior::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_WARRIOR] = CState_Common_Groggy_Warrior::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_WARRIOR] = CState_Common_Sting_Warrior::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_WARRIOR] = CState_Common_FlyHit_Warrior::Create();

#pragma endregion

}

void CState_Manager::WarHammer_State_AI()
{

#pragma region Patrol

	m_arrStates[AI_STATE_PATROL_DEFAULT_ENGINEER_L] = CState_Patrol_Default_Engineer_L::Create();
	m_arrStates[AI_STATE_PATROL_DEFAULT_ENGINEER_R] = CState_Patrol_Default_Engineer_R::Create();

	m_arrStates[AI_STATE_PATROL_ENGINEER_SWITCH_R_TO_L] = CState_Patrol_Switch_Engineer_CSwitchRtoL::Create();
	m_arrStates[AI_STATE_PATROL_ENGINEER_SWITCH_L_TO_R] = CState_Patrol_Switch_Engineer_CSwitchLtoR::Create();
	m_arrStates[AI_STATE_PATROL_WALK_ENGINEER_L] = CState_Patrol_Walk_Engineer_L::Create();
	m_arrStates[AI_STATE_PATROL_WALK_ENGINEER_R] = CState_Patrol_Walk_Engineer_R::Create();

#pragma endregion



#pragma region PathNavi

	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_L] = CState_PathNavigation_Run_Engineer_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_R] = CState_PathNavigation_Run_Engineer_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_ENGINEER_L] = CState_PathNavigation_Walk_Engineer_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_ENGINEER_R] = CState_PathNavigation_Walk_Engineer_R::Create();


	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ENGINEER] = CState_PathNavigation_Sprint_Engineer_Begin::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTLOOP_ENGINEER] = CState_PathNavigation_Sprint_Engineer_Loop::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTEND_ENGINEER] = CState_PathNavigation_Sprint_Engineer_End::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMP_ENGINEER] = CState_PathNavigation_Sprint_Engineer_Jump::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_ENGINEER] = CState_PathNavigation_Sprint_Engineer_Fall::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_L] = CState_PathNavigation_Jump_Engineer_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_R] = CState_PathNavigation_Jump_Engineer_R::Create();

#pragma endregion


#pragma region Combat


	m_arrStates[AI_STATE_COMBAT_DEFAULT_ENGINEER_L] = CState_Combat_Default_Engineer_L::Create();
	m_arrStates[AI_STATE_COMBAT_DEFAULT_ENGINEER_R] = CState_Combat_Default_Engineer_R::Create();


	m_arrStates[AI_STATE_COMBAT_AIRSPIKE_BEGIN_ENGINEER] = CState_Combat_SkillQ_AirSpike_Begin::Create();
	m_arrStates[AI_STATE_COMBAT_AIRSPIKE_LOOP_ENGINEER] = CState_Combat_SkillQ_AirSpike_Loop::Create();
	m_arrStates[AI_STATE_COMBAT_AIRSPIKE_END_ENGINEER] = CState_Combat_SkillQ_AirSpike_End::Create();

	m_arrStates[AI_STATE_COMBAT_GUARDBEGIN_ENGINEER] = CState_Combat_GuardBegin_Engineer::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDLOOP_ENGINEER] = CState_Combat_GuardLoop_Engineer::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDDASH_ENGINEER] = CState_Combat_GuardDash_Engineer::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDEND_ENGINEER] = CState_Combat_GuardEnd_Engineer::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDCANCEL_ENGINEER] = CState_Combat_GuardCancel_Engineer::Create();

	m_arrStates[AI_STATE_COMBAT_GROGGYATTACK_ENGINEER] = CState_Combat_SkillE_Engineer_GroggyAttack::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_L] = CState_Combat_Attack_HorizontalMiddle_Engineer_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R] = CState_Combat_Attack_HorizontalMiddle_Engineer_R::Create();

	m_arrStates[AI_STATE_COMBAT_VERTICALCUT_ENGINEER_L] = CState_Combat_Attack_VerticalCut_Engineer_L::Create();
	m_arrStates[AI_STATE_COMBAT_VERTICALCUT_ENGINEER_R] = CState_Combat_Attack_VerticalCut_Engineer_R::Create();



#pragma endregion


#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_ENGINEER_L] = CState_Common_Fall_Engineer_L::Create();
	m_arrStates[AI_STATE_COMMON_FALL_ENGINEER_R] = CState_Common_Fall_Engineer_R::Create();
	m_arrStates[AI_STATE_COMMON_LAND_ENGINEER_L] = CState_Common_Land_Engineer_L::Create();
	m_arrStates[AI_STATE_COMMON_LAND_ENGINEER_R] = CState_Common_Land_Engineer_R::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_ENGINEER_L] = CState_Common_Bounce_Engineer_L::Create();
	m_arrStates[AI_STATE_COMMON_BOUNCE_ENGINEER_R] = CState_Common_Bounce_Engineer_R::Create();

	m_arrStates[AI_STATE_COMMON_HIT_ENGINEER] = CState_Common_Hit_Engineer::Create();
	m_arrStates[AI_STATE_COMMON_GUARDHIT_ENGINEER] = CState_Common_GuardHit_Engineer::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_ENGINEER] = CState_Common_Groggy_Engineer::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_ENGINEER] = CState_Common_Sting_Engineer::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_ENGINEER] = CState_Common_FlyHit_Engineer::Create();

#pragma endregion


}

void CState_Manager::Valkyrie_State_AI()
{


#pragma region Patrol

	m_arrStates[AI_STATE_PATROL_DEFAULT_FIONA_L] = CState_Patrol_Default_Fiona_L::Create();
	m_arrStates[AI_STATE_PATROL_DEFAULT_FIONA_R] = CState_Patrol_Default_Fiona_R::Create();

	m_arrStates[AI_STATE_PATROL_FIONA_SWITCH_R_TO_L] = CState_Patrol_Switch_Fiona_CSwitchRtoL::Create();
	m_arrStates[AI_STATE_PATROL_FIONA_SWITCH_L_TO_R] = CState_Patrol_Switch_Fiona_CSwitchLtoR::Create();
	m_arrStates[AI_STATE_PATROL_WALK_FIONA_L] = CState_Patrol_Walk_Fiona_L::Create();
	m_arrStates[AI_STATE_PATROL_WALK_FIONA_R] = CState_Patrol_Walk_Fiona_R::Create();

#pragma endregion


#pragma region PathNavi



	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_L] = CState_PathNavigation_Default_Fiona_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_R] = CState_PathNavigation_Default_Fiona_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_FIONA_L] = CState_PathNavigation_Walk_Fiona_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_FIONA_R] = CState_PathNavigation_Walk_Fiona_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTBEGIN_FIONA] = CState_PathNavigation_Sprint_Fiona_Begin::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTLOOP_FIONA] = CState_PathNavigation_Sprint_Fiona_Loop::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTEND_FIONA] = CState_PathNavigation_Sprint_Fiona_End::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMP_FIONA] = CState_PathNavigation_Sprint_Fiona_Jump::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_FIONA] = CState_PathNavigation_Sprint_Fiona_Fall::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_FIONA_L] = CState_PathNavigation_Jump_Fiona_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_FIONA_R] = CState_PathNavigation_Jump_Fiona_R::Create();

#pragma endregion

#pragma region Combat

	m_arrStates[AI_STATE_COMBAT_DEFAULT_FIONA_L] = CState_Combat_Default_Fiona_L::Create();
	m_arrStates[AI_STATE_COMBAT_DEFAULT_FIONA_R] = CState_Combat_Default_Fiona_R::Create();

	
	m_arrStates[AI_STATE_COMBAT_COUNTER_FIONA] = CState_Combat_SkillQ_Counter_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_SHIELDATTACK_FIONA] = CState_Combat_SkillE_ShieldAttack_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_SPINATTACK_FIONA] = CState_Combat_SkillR_SpinAttack_Fiona::Create();

	m_arrStates[AI_STATE_COMBAT_GUARDBEGIN_FIONA] = CState_Combat_GuardBegin_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDLOOP_FIONA] = CState_Combat_GuardLoop_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDDASH_FIONA] = CState_Combat_GuardDash_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDEND_FIONA] = CState_Combat_GuardEnd_Fiona::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDCANCEL_FIONA] = CState_Combat_GuardCancel_Fiona::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALUP_FIONA_L] = CState_Combat_Attack_HorizontalUp_Fiona_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALUP_FIONA_R] = CState_Combat_Attack_HorizontalUp_Fiona_R::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L] = CState_Combat_Attack_HorizontalMiddle_Fiona_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R] = CState_Combat_Attack_HorizontalMiddle_Fiona_R::Create();

	m_arrStates[AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_L] = CState_Combat_Attack_HorizontalDown_Fiona_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R] = CState_Combat_Attack_HorizontalDown_Fiona_R::Create();

	m_arrStates[AI_STATE_COMBAT_VERTICALCUT_FIONA] = CState_Combat_Attack_VerticalCut_Fiona::Create();



#pragma endregion

#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_FIONA_L] = CState_Common_Fall_Fiona_L::Create();
	m_arrStates[AI_STATE_COMMON_FALL_FIONA_R] = CState_Common_Fall_Fiona_R::Create();
	m_arrStates[AI_STATE_COMMON_LAND_FIONA_L] = CState_Common_Land_Fiona_L::Create();
	m_arrStates[AI_STATE_COMMON_LAND_FIONA_R] = CState_Common_Land_Fiona_R::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_FIONA_L] = CState_Common_Bounce_Fiona_L::Create();
	m_arrStates[AI_STATE_COMMON_BOUNCE_FIONA_R] = CState_Common_Bounce_Fiona_R::Create();

	m_arrStates[AI_STATE_COMMON_HIT_FIONA] = CState_Common_Hit_Fiona::Create();
	m_arrStates[AI_STATE_COMMON_GUARDHIT_FIONA] = CState_Common_GuardHit_Fiona::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_FIONA] = CState_Common_Groggy_Fiona::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_FIONA] = CState_Common_Sting_Fiona::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_FIONA] = CState_Common_FlyHit_Fiona::Create();

#pragma endregion



}

void CState_Manager::Paladin_State_AI()
{
#pragma region Patrol

	m_arrStates[AI_STATE_PATROL_DEFAULT_PALADIN_L] = CState_Patrol_Default_Paladin_L::Create();
	m_arrStates[AI_STATE_PATROL_DEFAULT_PALADIN_R] = CState_Patrol_Default_Paladin_R::Create();

	m_arrStates[AI_STATE_PATROL_PALADIN_SWITCH_R_TO_L] = CState_Patrol_Switch_Paladin_CSwitchRtoL::Create();
	m_arrStates[AI_STATE_PATROL_PALADIN_SWITCH_L_TO_R] = CState_Patrol_Switch_Paladin_CSwitchLtoR::Create();
	m_arrStates[AI_STATE_PATROL_WALK_PALADIN_L] = CState_Patrol_Walk_Paladin_L::Create();
	m_arrStates[AI_STATE_PATROL_WALK_PALADIN_R] = CState_Patrol_Walk_Paladin_R::Create();

#pragma endregion

#pragma region Combat


	m_arrStates[AI_STATE_COMBAT_DEFAULT_PALADIN_L] = CState_Combat_Default_Paladin_L::Create();
	m_arrStates[AI_STATE_COMBAT_DEFAULT_PALADIN_R] = CState_Combat_Default_Paladin_R::Create();

	m_arrStates[AI_STATE_COMBAT_GUARDBEGIN_PALADIN] = CState_Combat_GuardBegin_Paladin::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDLOOP_PALADIN] = CState_Combat_GuardLoop_Paladin::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDEND_PALADIN] = CState_Combat_GuardEnd_Paladin::Create();
	m_arrStates[AI_STATE_COMBAT_GUARDCANCEL_PALADIN] = CState_Combat_GuardCancel_Paladin::Create();

	m_arrStates[AI_STATE_COMBAT_SHIELDWALL_BEGIN_PALADIN] = CState_Combat_SkillE_Paladin_ShieldWall_Begin::Create();
	m_arrStates[AI_STATE_COMBAT_SHIELDWALL_LOOP_PALADIN] = CState_Combat_SkillE_Paladin_ShieldWall_Loop::Create();
	m_arrStates[AI_STATE_COMBAT_SHIELDWALL_END_PALADIN] = CState_Combat_SkillE_Paladin_ShieldWall_End::Create();
	m_arrStates[AI_STATE_COMBAT_SHIELDWALL_HIT_PALADIN] = CState_Combat_SkillE_Paladin_ShieldWall_Hit::Create();

	m_arrStates[AI_STATE_COMBAT_RUSH_BEGIN_PALADIN] = CState_Combat_SkillQ_Paladin_Rush_Begin::Create();
	m_arrStates[AI_STATE_COMBAT_RUSH_LOOP_PALADIN] = CState_Combat_SkillQ_Paladin_Rush_Loop::Create();
	m_arrStates[AI_STATE_COMBAT_RUSH_END_PALADIN] = CState_Combat_SkillQ_Paladin_Rush_End::Create();

	m_arrStates[AI_STATE_COMBAT_SHIELDSLAM_PALADIN] = CState_Combat_SkillR_Paladin_ShieldSlam::Create();

	m_arrStates[AI_STATE_COMBAT_VERTICALCUT_PALADIN] = CState_Combat_Attack_VerticalCut_Paladin::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_L] = CState_Combat_Attack_HorizontalMiddle_Paladin_L::Create();
	m_arrStates[AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R] = CState_Combat_Attack_HorizontalMiddle_Paladin_R::Create();


#pragma endregion

#pragma region PathNavigation

	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_L] = CState_PathNavigation_Default_Paladin_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_R] = CState_PathNavigation_Default_Paladin_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_PALADIN_L] = CState_PathNavigation_Walk_Paladin_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_PALADIN_R] = CState_PathNavigation_Walk_Paladin_R::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PALADIN] = CState_PathNavigation_Sprint_Paladin_Begin::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTLOOP_PALADIN] = CState_PathNavigation_Sprint_Paladin_Loop::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTEND_PALADIN] = CState_PathNavigation_Sprint_Paladin_End::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMP_PALADIN] = CState_PathNavigation_Sprint_Paladin_Jump::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_PALADIN] = CState_PathNavigation_Sprint_Paladin_Fall::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_PALADIN_L] = CState_PathNavigation_Jump_Paladin_L::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_PALADIN_R] = CState_PathNavigation_Jump_Paladin_R::Create();

#pragma endregion

#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_PALADIN_L] = CState_Common_Fall_Paladin_L::Create();
	m_arrStates[AI_STATE_COMMON_FALL_PALADIN_R] = CState_Common_Fall_Paladin_R::Create();
	m_arrStates[AI_STATE_COMMON_LAND_PALADIN_L] = CState_Common_Land_Paladin_L::Create();
	m_arrStates[AI_STATE_COMMON_LAND_PALADIN_R] = CState_Common_Land_Paladin_R::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_PALADIN_L] = CState_Common_Bounce_Paladin_L::Create();
	m_arrStates[AI_STATE_COMMON_BOUNCE_PALADIN_R] = CState_Common_Bounce_Paladin_R::Create();

	m_arrStates[AI_STATE_COMMON_HIT_PALADIN] = CState_Common_Hit_Paladin::Create();
	m_arrStates[AI_STATE_COMMON_GUARDHIT_PALADIN] = CState_Common_GuardHit_Paladin::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_PALADIN] = CState_Common_Groggy_Paladin::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_PALADIN] = CState_Common_Sting_Paladin::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_PALADIN] = CState_Common_FlyHit_Paladin::Create();

#pragma endregion



}

void CState_Manager::Archer_State_AI()
{


#pragma region Patrol

	m_arrStates[AI_STATE_PATROL_DEFAULT_ARCHER_L] = CState_Patrol_Default_Archer_L::Create();
	m_arrStates[AI_STATE_PATROL_DEFAULT_ARCHER_R] = CState_Patrol_Default_Archer_R::Create();
	m_arrStates[AI_STATE_PATROL_WALK_ARCHER_L] = CState_Patrol_Walk_Archer_L::Create();
	m_arrStates[AI_STATE_PATROL_WALK_ARCHER_R] = CState_Patrol_Walk_Archer_R::Create();

#pragma endregion

#pragma region Combat


	m_arrStates[AI_STATE_COMBAT_DEFAULT_ARCHER_L] = CState_Combat_Default_Archer_L::Create();
	m_arrStates[AI_STATE_COMBAT_DEFAULT_ARCHER_R] = CState_Combat_Default_Archer_R::Create();


	m_arrStates[AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER] = CState_Combat_Attack_Archer_Begin::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_AIMING_ARCHER] = CState_Combat_Attack_Archer_Aiming::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_SHOOT_ARCHER] = CState_Combat_Attack_Archer_Shoot::Create();

	m_arrStates[AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER] = CState_Combat_Attack_Archer_Begin_Poison::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_AIMING_POISION_ARCHER] = CState_Combat_Attack_Archer_Aiming_Poison::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_SHOOT_POISION_ARCHER] = CState_Combat_Attack_Archer_Shoot_Poison::Create();

	m_arrStates[AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER] = CState_Combat_Attack_Archer_Begin_Sniping::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_AIMING_SNIPING_ARCHER] = CState_Combat_Attack_Archer_Aiming_Sniping::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_SHOOT_SNIPING_ARCHER] = CState_Combat_Attack_Archer_Shoot_Sniping::Create();

	m_arrStates[AI_STATE_COMBAT_GUARDLOOP_ARCHER] = CState_Combat_Default_Archer_R::Create();
	m_arrStates[AI_STATE_COMBAT_ATTACK_SWING_ARCHER] = CState_Combat_Default_Archer_R::Create();
	m_arrStates[AI_STATE_COMBAT_STATE_SWAP_ARCHER] = CState_Combat_Default_Archer_R::Create();

#pragma endregion

#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_ARCHER_L] = CState_Common_Fall_Archer_L::Create();
	m_arrStates[AI_STATE_COMMON_FALL_ARCHER_R] = CState_Common_Fall_Archer_R::Create();
	m_arrStates[AI_STATE_COMMON_LAND_ARCHER_L] = CState_Common_Land_Archer_L::Create();
	m_arrStates[AI_STATE_COMMON_LAND_ARCHER_R] = CState_Common_Land_Archer_R::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_ARCHER] = CState_Common_Bounce_Archer::Create();

	m_arrStates[AI_STATE_COMMON_HIT_ARCHER] = CState_Common_Hit_Archer::Create();
	m_arrStates[AI_STATE_COMMON_GUARDHIT_ARCHER] = CState_Common_GuardHit_Archer::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_ARCHER] = CState_Common_Groggy_Archer::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_ARCHER] = CState_Common_Sting_Archer::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_ARCHER] = CState_Common_FlyHit_Archer::Create();

#pragma endregion


}

void CState_Manager::Qanda_State_AI()
{

}


void CState_Manager::Priest_State_AI()
{



#pragma region Patrol

	m_arrStates[AI_STATE_PATROL_DEFAULT_PRIEST] = CState_Patrol_Default_Priest::Create();
	m_arrStates[AI_STATE_PATROL_WALK_PRIEST] = CState_Patrol_Walk_Priest::Create();

	m_arrStates[AI_STATE_PATROL_CURE_BEGIN_PRIEST] = CState_Patrol_Cure_Begin_Priest::Create();
	m_arrStates[AI_STATE_PATROL_CURE_LOOP_PRIEST] = CState_Patrol_Cure_Loop_Priest::Create();
	m_arrStates[AI_STATE_PATROL_CURE_END_PRIEST] = CState_Patrol_Cure_End_Priest::Create();


#pragma endregion

#pragma region Combat

	m_arrStates[AI_STATE_COMBAT_DEFAULT_PRIEST] = CState_Combat_Default_Priest::Create();
	m_arrStates[AI_STATE_COMBAT_STINGATTACK_PRIEST] = CState_Combat_Attack_Sting_Priest::Create();
	m_arrStates[AI_STATE_COMBAT_AIRDASH_PRIEST] = CState_Combat_SkillR_AirDash_Priest::Create();

	m_arrStates[AI_STATE_COMBAT_CURE_BEGIN_PRIEST] = CState_Combat_Cure_Begin_Priest::Create();
	m_arrStates[AI_STATE_COMBAT_CURE_LOOP_PRIEST] = CState_Combat_Cure_Loop_Priest::Create();
	m_arrStates[AI_STATE_COMBAT_CURE_END_PRIEST] = CState_Combat_Cure_End_Priest::Create();

	m_arrStates[AI_STATE_COMBAT_WINDATTACK_PRIEST] = CState_Combat_SkillE_Priest_WindAttack::Create();


#pragma endregion



#pragma region PathNavigation

	m_arrStates[AI_STATE_PATHNAVIGATION_DEFAULT_PRIEST] = CState_PathNavigation_Default_Priest::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_WALK_PRIEST] = CState_PathNavigation_Walk_Priest::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PRIEST] = CState_PathNavigation_Sprint_Priest_Begin::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTLOOP_PRIEST] = CState_PathNavigation_Sprint_Priest_Loop::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTEND_PRIEST] = CState_PathNavigation_Sprint_Priest_End::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMP_PRIEST] = CState_PathNavigation_Sprint_Priest_Jump::Create();
	m_arrStates[AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_PRIEST] = CState_PathNavigation_Sprint_Priest_Fall::Create();

	m_arrStates[AI_STATE_PATHNAVIGATION_JUMP_PRIEST] = CState_PathNavigation_Jump_Priest::Create();

#pragma endregion

#pragma region Common

	m_arrStates[AI_STATE_COMMON_FALL_PRIEST] = CState_Common_Fall_Priest::Create();
	m_arrStates[AI_STATE_COMMON_LAND_PRIEST] = CState_Common_Land_Priest::Create();

	m_arrStates[AI_STATE_COMMON_BOUNCE_PRIEST] = CState_Common_Bounce_Priest::Create();


	m_arrStates[AI_STATE_COMMON_HIT_PRIEST] = CState_Common_Hit_Priest::Create();
	m_arrStates[AI_STATE_COMMON_GROGGYHIT_PRIEST] = CState_Common_Groggy_Priest::Create();
	m_arrStates[AI_STATE_COMMON_STINGHIT_PRIEST] = CState_Common_Sting_Priest::Create();
	m_arrStates[AI_STATE_COMMON_FLYHIT_PRIEST] = CState_Common_FlyHit_Priest::Create();

#pragma endregion
}

void CState_Manager::Lancer_State_AI()
{

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
