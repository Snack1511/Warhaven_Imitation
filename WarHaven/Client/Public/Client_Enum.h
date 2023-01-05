#pragma once
namespace Client
{
	enum class eTEAM_TYPE
	{
		eRED,
		eBLUE,
		eCOUNT
	};

	enum CLASS_TYPE
	{
		WARRIOR,
		SPEAR,
		ARCHER,
		PALADIN,
		PRIEST,
		ENGINEER,
		CT_DEFAULT_END = 6,
		FIONA = 6,
		QANDA,
		HOEDT,
		LANCER,
		CLASS_END
	};

	enum ANIM_TYPE
	{
		ANIM_BASE_R,
		ANIM_BASE_L,
		ANIM_ATTACK,
		ANIM_HIT,
		ANIM_ETC,
		ANIM_END,
	};

	enum UNIT_EYE_TYPE
	{
		EYE_DEFAULT,
		EYE_END

	};

	enum MODEL_PART_TYPE
	{
		MODEL_PART_SKEL,
		MODEL_PART_BODY,
		MODEL_PART_FACE,
		MODEL_PART_HEAD,
		MODEL_PART_WEAPON,
		MODEL_PART_WEAPON_L,
		MODEL_PART_SUBWEAPON,
		MODEL_PART_END,
	};

	enum CHANNEL_CLIENT
	{
		CHANNEL_BGM,
		CHANNEL_UI,
		CHANNEL_ENVIRONMENT,
		CHANNEL_EFFECTS,
		CHANNEL_VOICE,
		CHANNEL_END = CH_GROUP_END
	};

	enum COL_GROUP_CLIENT
	{
		COL_DEFAULT,

		COL_TRIGGER,

		COL_BODY,
		COL_REVIVE,

		COL_BLUETEAM,
		COL_BLUEHITBOX_HEAD,
		COL_BLUEHITBOX_BODY,
		COL_BLUEGUARD,

		COL_BLUEATTACK,
		COL_BLUETHROW,
		COL_BLUEGUARDBREAK, // 가드 브레이크 
		COL_BLUEGROGGYATTACK, // 가드 브레이크 or 가드 안했을 시 그로기 상태 
		COL_BLUEFLYATTACK, // 공중에 뜨는 공격 -> 방어 가능(블레이드 Q)
		COL_BLUEFLYATTACKGUARDBREAK, // 공중에 뜨는 공격-> 방어 불가(워해머 Q)
		COL_BLUECURE,
		COL_BLUEPROJECTILECATCH, // 스모크 전용 투사체 잡기 컬라이더.


		COL_REDTEAM,

		COL_REDHITBOX_HEAD,
		COL_REDHITBOX_BODY,
		COL_REDGUARD,

		COL_REDATTACK,
		COL_REDGUARDBREAK,
		COL_REDGROGGYATTACK,
		COL_REDFLYATTACK,
		COL_REDFLYATTACKGUARDBREAK,
		COL_REDPROJECTILECATCH, // 스모크 전용 투사체 잡기 컬라이더.
		COL_REDCURE,
		COL_REDTHROW,


		COL_WALL,
		COL_CANNON,

		COL_SIGHTRANGE,

		COL_END = 32,
	};

	enum OBJ_GROUP_CLIENT
	{
		GROUP_STATIC,
		GROUP_DEFAULT,
		GROUP_ENEMY,
		GROUP_DECORATION,
		GROUP_PROP,
		GROUP_UI,
		GROUP_PLAYER,
		GROUP_LOADING,
		GROUP_EFFECT,
		GROUP_DEBUG,
		GROUP_USER,
		GROUP_CAMERA,
		GROUP_TRIGGER,
		GROUP_PHYSX,

		GROUP_END = GR_END
	};

	enum LEVEL_TYPE_CLIENT
	{
		LEVEL_LOGO,
		LEVEL_LOADING,
		LEVEL_MAINMENU,
		LEVEL_TEST,
		LEVEL_BOOTCAMP,
		LEVEL_PADEN,
		LEVEL_HWARA,
		LEVEL_END
	};

	enum COMPONENT_PIPELINE
	{
		CP_BEFORE_NAVIGATION,
		CP_NAVIGATION,
		CP_BEFORE_TRANSFORM,
		CP_TRANSFORM,
		CP_AFTER_TRANSFORM,

		CP_BEFORE_RENDERER,
		CP_RIGHTBEFORE_RENDERER,
		CP_RENDERER,
		CP_AFTER_RENDERER,

		CP_END,
	};

	enum SHADER_FILE_ID
	{
		SHADER_DEFERRED,
		SHADER_BLUR,
		SHADER_GLOW,
		SHADER_SHADOW,
		SHADER_VTXTEX,
		SHADER_VTXCUBETEX,
		SHADER_VTXNOR,
		SHADER_VTXMODEL,
		SHADER_VTXANIMMODEL,
		SHADER_VTXMODELINSTANCE,
		SHADER_VTXRECTINSTANCE,
		SHADER_VTXTRIINSTANCE,
		SHADER_VTXEFFECTS,
		SHADER_VTXDECALCUBE,
		SHADER_END
	};

	enum VTXMODEL_PASS_TYPE
	{
		VTXMODEL_PASS_DEFAULT,
		VTXMODEL_PASS_CW,
		VTXMODEL_PASS_NONE,
		VTXMODEL_PASS_BUILDING,
		VTXMODEL_PASS_GROUND,
		VTXMODEL_PASS_GROUNDEXAM,
		VTXMODEL_PASS_SAMPLE,
		VTXMODEL_PASS_NORMALMAPPING,
		VTXMODEL_PASS_SHADOW,
		VTXMODEL_PASS_GROUNDBREAK,
		VTXMODEL_PASS_STATICSHADOW,
		VTXMODEL_PASS_END,
	};

	enum VTXEFFECT_PASS_TYPE
	{
		VTXEFFECT_PASS_DEFAULT,
		VTXEFFECT_PASS_DISTORTION,
		VTXEFFECT_PASS_DISSOLVE,
		VTXEFFECT_PASS_CLAMP,
		VTXEFFECT_PASS_BORDER,
		VTXEFFECT_PASS_LIGHTNING,
		VTXEFFECT_PASS_DOMINION,
		VTXEFFECT_PASS_AURA,
		VTXEFFECT_PASS_END,
	};

	enum VTXANIM_PASS_TYPE
	{
		VTXANIM_PASS_DEFAULT,
		VTXANIM_PASS_FACE,
		VTXANIM_PASS_EYE,
		VTXANIM_PASS_HIT,
		VTXANIM_PASS_SHADOW,
		VTXANIM_PASS_NORMAL,

		VTXANIM_PASS_END,
	};

	enum VTXINSTANCE_PASS_TYPE
	{
		VTXINSTANCE_PASS_DEFAULT,
		VTXINSTANCE_PASS_NORMALMAPPING,
		VTXINSTANCE_PASS_PARTICLE,
		VTXINSTANCE_PASS_SHADOW,
		VTXINSTANCE_PASS_STATICSHADOW,

		VTXINSTANCE_PASS_END,
	};

	enum VTXRECTINSTANCE_PASS_TYPE
	{
		VTXRECTINSTANCE_PASS_DEFAULT,
		VTXRECTINSTANCE_PASS_COLORMAP,
		VTXRECTINSTANCE_PASS_DEBUG,
		VTXRECTINSTANCE_PASS_ANIMATION,
		VTXRECTINSTANCE_PASS_ANIMATIONALPHA,
		VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE,
		VTXRECTINSTANCE_PASS_BLACKBACKGROUNDTEXTURE,
		VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR,
		VTXRECTINSTANCE_PASS_UVTEXTURESELECT,
		VTXRECTINSTANCE_PASS_FLARE,
		VTXRECTINSTANCE_PASS_UVCOLORSELECT,
		VTXRECTINSTANCE_PASS_COLORPOWERDECREASE,
		VTXRECTINSTANCE_PASS_ANIMATIONZFALSE,
		VTXRECTINSTANCE_PASS_END,
	};

	enum VTXTRIINSTANCE_PASS_TYPE
	{
		VTXTRIINSTANCE_PASS_DEFAULT,

		VTXTRIINSTANCE_PASS_END,
	};

	enum VTXDECALCUBE_PASS_TYPE
	{
		VTXDECALCUBE_PASS_DEFAULT,

		VTXDECALCUBE_PASS_END,
	};

	enum VTXTEX_PASS_TYPE
	{
		VTXTEX_PASS_DEFAULT,
		VTXTEX_PASS_UI_Color,
		VTXTEX_PASS_UI_LoadingIcon,
		VTXTEX_PASS_UI_HealthGauge,
		VTXTEX_PASS_UI_HpBar,
		VTXTEX_PASS_UI_HeroGauge,
		VTXTEX_PASS_UI_RELIC,
		VTXTEX_PASS_UI_PortEffect,
		VTXTEX_PASS_UI_HeroKeySkill,
		VTXTEX_PASS_UI_MainEffect,
		VTXTEX_PASS_UI_LobbyEffect,
		VTXTEX_PASS_UI_VerticalGauge,
		VTXTEX_PASS_UI_HorizontalGauge,
		VTXTEX_PASS_UI_OperSmoke,
		VTXTEX_PASS_UI_GlowLine,
		VTXTEX_PASS_UI_CircleGauge,
		VTXTEX_PASS_UI_SelectEffect,

		VTXTEX_PASS_ALPHA,
		VTXTEX_PASS_TRAIL,
		VTXTEX_PASS_BLOODOVERLAY,
		VTXTEX_PASS_DEBUG,
		VTXTEX_PASS_UI_ANIMATION,
		VTXTEX_PASS_TRAILDISTORTION,
		VTXTEX_PASS_UVFIRE,
		VTXTEX_PASS_PROFILE,
		VTXTEX_PASS_UITRAIL,
		VTXTEX_PASS_END,
	};

	enum VTXCUBETEX_PASS_TYPE
	{
		VTXCUBETEX_PASS_SKYBOX,
		VTXCUBETEX_PASS_DEBUG,
		VTXCUBETEX_PASS_END,
	};

	enum VTXNOR_PASS_TYPE
	{
		VTXNOR_PASS_TERRAIN,
		VTXNOR_PASS_NAVIGATION,
		VTXNOR_PASS_TEXTUREARRAY,
		VTXNOR_PASS_END,
	};

	enum MESH_TYPE
	{
		MESH_RECT,
		MESH_END,
	};


	enum STATE_TYPE
	{
		NO_PATTERN, // 터진다.



		STATE_CANNON_PLAYER,
		STATE_VICTORY,
		STATE_DEFEAT,
		STATE_GLIDING,
		STATE_GLIDING_ARCHER,
		STATE_TRANSFORM,

		AI_STATE_COMMON_REVIVE_AI,
		AI_STATE_COMMON_SLIDEBEGIN_AI,
		AI_STATE_COMMON_SLIDELOOP_AI,
		AI_STATE_COMMON_SLIDEEND_AI,
		AI_STATE_COMMON_TRANSFORM_AI,

#pragma region Warrior
//#pragma region Base_R
//
//		/* Idle */
		STATE_IDLE_PLAYER_R,
//
//		/* Walk */
		STATE_WALK_PLAYER_R,
//
//		/* Run */
		STATE_RUN_BEGIN_PLAYER_R,
		STATE_RUN_PLAYER_R,
//
//		/* Jump */
		STATE_JUMP_PLAYER_R,
		STATE_JUMPFALL_PLAYER_R,
		STATE_JUMP_LAND_PLAYER_R,
//
//		/* Bounece */
		STATE_BOUNCE_PLAYER_R,

		/* Revive */
		STATE_REVIVE_PLAYER,
//
//
//#pragma endregion
//
//#pragma region Base_L_Player
//
//		/* Idle */
		STATE_SWITCH_R_TO_L,
		STATE_SWITCH_L_TO_R,

		STATE_IDLE_PLAYER_L,
//
//		/* Walk */
		STATE_WALK_PLAYER_L,
//
//		/* Run */
		STATE_RUN_BEGIN_PLAYER_L,
		STATE_RUN_PLAYER_L,
//
//		/* Jump */
		STATE_JUMP_PLAYER_L,
		STATE_JUMPFALL_PLAYER_L,
		STATE_JUMP_LAND_PLAYER_L,
//
//		/* Bounece */
		STATE_BOUNCE_PLAYER_L,

		/*Stop*/
		STATE_STOP_PLAYER_L,
//
//#pragma endregion
//
//#pragma region Default
//		/* Slide */
		STATE_SLIDE_BEGIN_PLAYER,
		STATE_SLIDE_LOOP_PLAYER,
		STATE_SLIDE_END_PLAYER,
//
//		/* Sprint */
		STATE_SPRINT_BEGIN_PLAYER,
		STATE_SPRINT_LOOP_PLAYER, 
		STATE_SPRINT_END_PLAYER, 

//		STATE_SPRINT_RUN_PLAYER,
//
		STATE_SPRINT_JUMP_PLAYER,
//
		STATE_SPRINT_JUMPFALL_PLAYER,
//		STATE_SPRINT_LAND_PLAYER,
//
//#pragma endregion
//
//#pragma region Attack_Player
//
//		/* L */
		STATE_ATTACK_HORIZONTALUP_L,
		STATE_ATTACK_HORIZONTALMIDDLE_L, // 중상단 공격
		STATE_ATTACK_HORIZONTALDOWN_L,

//
//		/* R */
		STATE_ATTACK_HORIZONTALUP_R,
		STATE_ATTACK_HORIZONTALMIDDLE_R, // 중상단 공격
		STATE_ATTACK_HORIZONTALDOWN_R,

//



//
//		/*Sprint*/
		STATE_SPRINTATTACK_BEGIN_PLAYER,
		STATE_SPRINTATTACK_PLAYER,
//
//		/* Sting */
		STATE_ATTACK_STING_PLAYER_L,
		STATE_ATTACK_STING_PLAYER_R,
//
//		/* Take_Down */
		STATE_ATTACK_VERTICALCUT,
			/*Test*/
		STATE_CHARGETEST,

		/* Stop */
		STATE_STOP_PLAYER_R,
//
//#pragma endregion
//
//
//#pragma region SKill
//
//		/*Skill*/

		STATE_WARRIOR_OXEN_BEGIN,
		STATE_WARRIOR_OXEN_LOOP,
		STATE_WARRIOR_OXEN_LOOPATTACK,
		STATE_WARRIOR_OXEN_END,



		STATE_WARRIOR_GUARDBREAK, // 가드 파괴

		STATE_HIT,
		STATE_GUARDHIT_WARRIOR,
		STATE_GROGGYHIT_WARRIOR,
		STATE_STINGHIT_WARRIOR,
		STATE_FLYHIT_WARRIOR,

		STATE_GUARDDASH_WARRIOR,


//
//
//#pragma endregion
//
//
//		// TEST
//		STATE_IDLE_PLAYER,
//
//		STATE_WALK_PLAYER,
//		STATE_RUN_PLAYER,
//
//		STATE_SPRINT_PLAYER,
//
//
//		/* ATtack */
//		STATE_ATTACK_WARRIOR,
//
//		/* ETC */
//		//STATE_SLIDE_PLAYER,
//		STATE_JUMP_PLAYER,

		STATE_GUARD_BEGIN_PLAYER,
		STATE_GUARD_END_PLAYER,
		STATE_GUARD_LOOP_PLAYER,
		STATE_GUARD_CANCEL_PLAYER,

		STATE_CHANGE_PLAYER,


#pragma endregion


#pragma region SpearMan



		/* Base_R */
		STATE_WALK_SPEARMAN_R,
		STATE_IDLE_SPEARMAN_R,
		STATE_RUNBEGIN_SPEARMAN_R,
		STATE_RUN_SPEARMAN_R,

		STATE_JUMP_SPEARMAN_R,
		STATE_JUMPFALL_SPEARMAN_R,
		STATE_JUMP_LAND_SPEARMAN_R,

		STATE_STOP_SPEARMAN_R,

		/* Base_L */
		STATE_WALK_SPEARMAN_L,
		STATE_IDLE_SPEARMAN_L,
		STATE_RUNBEGIN_SPEARMAN_L,
		STATE_RUN_SPEARMAN_L,

		STATE_JUMP_SPEARMAN_L,
		STATE_JUMPFALL_SPEARMAN_L,
		STATE_JUMP_LAND_SPEARMAN_L,

		STATE_STOP_SPEARMAN_L,


		/*Attack*/
		STATE_ATTACK_STING_SPEARMAN,
		STATE_BOUNCE_SPEARMAN,
		STATE_SPEARMAN_GUARDBREAK,
		STATE_SPEARMAN_SPECIALGUARD,
		STATE_SPEARMAN_BACKSTEP,


#pragma endregion


#pragma region WarHammer

		/* 보간용 자세 스위칭*/
		STATE_SWITCH_R_TO_L_WARHAMMER,
		STATE_SWITCH_L_TO_R_WARHAMMER,

		/* Base_R */
		STATE_IDLE_WARHAMMER_R,
		STATE_WALK_WARHAMMER_R,
		STATE_RUNBEGIN_WARHAMMER_R,
		STATE_RUN_WARHAMMER_R,

		STATE_JUMP_WARHAMMER_R,
		STATE_JUMPFALL_WARHAMMER_R,
		STATE_JUMP_LAND_WARHAMMER_R,

		STATE_STOP_WARHAMMER_R,

		/* Base_L */
		STATE_IDLE_WARHAMMER_L,
		STATE_WALK_WARHAMMER_L,
		STATE_RUNBEGIN_WARHAMMER_L,
		STATE_RUN_WARHAMMER_L,

		STATE_JUMP_WARHAMMER_L,
		STATE_JUMPFALL_WARHAMMER_L,
		STATE_JUMP_LAND_WARHAMMER_L,

		STATE_STOP_WARHAMMER_L,


		/*Sprint*/
		STATE_SPRINT_JUMP_WARHAMMER,
		STATE_SPRINT_JUMPFALL_WARHAMMER,
		STATE_SPRINTATTACK_BEGIN_WARHAMMER,
		STATE_SPRINTATTACK_WARHAMMER,
		STATE_SPRINT_BEGIN_WARHAMMER,
		STATE_SPRINT_LOOP_WARHAMMER,
		STATE_SPRINT_END_WARHAMMER,


		/* Attack */
		STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L,
		STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R,

		STATE_VERTICALATTACK_WARHAMMER_L,
		STATE_VERTICALATTACK_WARHAMMER_R,

		STATE_ATTACK_STING_WARHAMMER_L,
		STATE_ATTACK_STING_WARHAMMER_R,


		STATE_GROGGYATTACK_WARHAMMER,

		STATE_AIRSPIKE_BEGIN_WARHAMMER,
		STATE_AIRSPIKE_LOOP_WARHAMMER,
		STATE_AIRSPIKE_END_WARHAMMER,

		/* Bounce */
		STATE_BOUNCE_WARHAMMER_L,
		STATE_BOUNCE_WARHAMMER_R,

		/* Guard */
		STATE_GUARD_BEGIN_WARHAMMER,
		STATE_GUARD_LOOP_WARHAMMER,
		STATE_GUARD_END_WARHAMMER,
		STATE_GUARD_CANCEL_WARHAMMER,

		/* Install */
		STATE_INSTALL_BEIGN_WARHAMMER,
		STATE_INSTALL_LOOP_WARHAMMER,
		STATE_INSTALL_END_WARHAMMER,

		STATE_CHARGE_WARHAMMER_R,
		STATE_CHARGE_WARHAMMER_L,

		STATE_CHARGE_FRONT_WARHAMMER_R,
		STATE_CHARGE_FRONT_WARHAMMER_L,

		STATE_CHARGEATTACK_WARHAMMER_R,
		STATE_CHARGEATTACK_WARHAMMER_L,

		STATE_CHARGEATTACK_FRONT_WARHAMMER_R,
		STATE_CHARGEATTACK_FRONT_WARHAMMER_L,


		STATE_HIT_WARHAMMER,
		STATE_GUARDHIT_WARHAMMER,
		STATE_GROGGYHIT_WARHAMMER,
		STATE_STINGHIT_WARHAMMER,
		STATE_FLYHIT_WARHAMMER,

		STATE_GUARDDASH_WARHAMMER,


#pragma endregion

#pragma region Archer
			
		/* Base_R */
		STATE_IDLE_ARCHER_R,
		STATE_WALK_ARCHER_R,
		STATE_RUNBEGIN_ARCHER_R,
		STATE_RUN_ARCHER_R,

		STATE_JUMP_ARCHER_R,
		STATE_JUMPFALL_ARCHER_R,
		STATE_JUMP_LAND_ARCHER_R,

		STATE_STOP_ARCHER_R,

		/* Base_L */
		STATE_IDLE_ARCHER_L,
		STATE_WALK_ARCHER_L,
		STATE_RUNBEGIN_ARCHER_L,
		STATE_RUN_ARCHER_L,

		STATE_JUMP_ARCHER_L,
		STATE_JUMPFALL_ARCHER_L,
		STATE_JUMP_LAND_ARCHER_L,

		STATE_STOP_ARCHER_L,

		STATE_SPRINT_JUMP_ARCHER,
		STATE_SPRINT_JUMPFALL_ARCHER,
		STATE_SPRINT_BEGIN_ARCHER,
		STATE_SPRINT_LOOP_ARCHER,
		STATE_SPRINT_END_ARCHER,

		STATE_ATTACK_SWING_ARCHER,

		STATE_ATTACK_AIMING_ARCHER,
		STATE_ATTACK_BEGIN_ARCHER,
		STATE_ATTACK_SHOOT_ARCHER,

		STATE_ATTACK_AIMING_SNIPING_ARCHER,
		STATE_ATTACK_BEGIN_SNIPING_ARCHER,
		STATE_ATTACK_SHOOT_SNIPING_ARCHER,

		STATE_ATTACK_AIMING_POISION_ARCHER,
		STATE_ATTACK_BEGIN_POISION_ARCHER,
		STATE_ATTACK_SHOOT_POISION_ARCHER,

		STATE_SWAP_ARCHER,

		STATE_BOUNCE_ARCHER,

		STATE_ATTACK_CANCEL_ARCHER,

		STATE_GUARD_ARCHER,
		STATE_GUARD_CANCEL_ARCHER,

		STATE_HIT_ARCHER,
		STATE_GUARDHIT_ARCHER,
		STATE_GROGGYHIT_ARCHER,
		STATE_STINGHIT_ARCHER,
		STATE_FLYHIT_ARCHER,

#pragma endregion


#pragma region Valkyrie
		//VALKYRIE



		/* Base_R */
		STATE_IDLE_VALKYRIE_R,
		STATE_WALK_VALKYRIE_R,
		STATE_RUNBEGIN_VALKYRIE_R,
		STATE_RUN_VALKYRIE_R,

		STATE_JUMP_VALKYRIE_R,
		STATE_JUMPFALL_VALKYRIE_R,
		STATE_JUMP_LAND_VALKYRIE_R,

		STATE_STOP_VALKYRIE_R,

		STATE_BOUNCE_VALKYRIE_R,

		/* Base_L */
		STATE_IDLE_VALKYRIE_L,
		STATE_WALK_VALKYRIE_L,
		STATE_RUNBEGIN_VALKYRIE_L,
		STATE_RUN_VALKYRIE_L,

		STATE_JUMP_VALKYRIE_L,
		STATE_JUMPFALL_VALKYRIE_L,
		STATE_JUMP_LAND_VALKYRIE_L,

		STATE_STOP_VALKYRIE_L,

		STATE_BOUNCE_VALKYRIE_L,

		/*Sprint*/
		STATE_SPRINT_JUMP_VALKYRIE,
		STATE_SPRINT_JUMPFALL_VALKYRIE,
		STATE_SPRINTATTACK_BEGIN_VALKYRIE,
		STATE_SPRINTATTACK_VALKYRIE,
		STATE_SPRINT_BEGIN_VALKYRIE,
		STATE_SPRINT_LOOP_VALKYRIE,
		STATE_SPRINT_END_VALKYRIE,

		STATE_ATTACK_HORIZONTALUP_VALKYRIE_L,
		STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L, // 중상단 공격
		STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L,
		STATE_ATTACK_STING_VALKYRIE_L,
		//
		//		/* R */
		STATE_ATTACK_HORIZONTALUP_VALKYRIE_R,
		STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R, // 중상단 공격
		STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R,
		STATE_ATTACK_STING_VALKYRIE_R,

		STATE_ATTACK_VERTICALCUT_VALKYRIE,


		STATE_SWITCH_R_TO_L_VALKYRIE,
		STATE_SWITCH_L_TO_R_VALKYRIE,

		STATE_COUNTER_VALKYRIE,
		STATE_SHIELDATTACK_VALKYRIE,
		STATE_SPINATTACK_VALKYRIE,

		STATE_GUARD_BEGIN_VALKYRIE,
		STATE_GUARD_LOOP_VALKYRIE,
		STATE_GUARD_END_VALKYRIE,
		STATE_GUARD_CANCEL_VALKYRIE,
		STATE_GUARDDASH_VALKYRIE,

		STATE_HIT_VALKYRIE,
		STATE_GUARDHIT_VALKYRIE,
		STATE_GROGGYHIT_VALKYRIE,
		STATE_STINGHIT_VALKYRIE,
		STATE_FLYHIT_VALKYRIE,

#pragma endregion

#pragma region Paladin
				//VALKYRIE

		STATE_SWITCH_R_TO_L_PALADIN,
		STATE_SWITCH_L_TO_R_PALADIN,

		STATE_IDLE_PALADIN_R,
		STATE_WALK_PALADIN_R,
		STATE_RUNBEGIN_PALADIN_R,
		STATE_RUN_PALADIN_R,

		STATE_JUMP_PALADIN_R,
		STATE_JUMPFALL_PALADIN_R,
		STATE_JUMP_LAND_PALADIN_R,

		STATE_STOP_PALADIN_R,

		STATE_BOUNCE_PALADIN_R,

		/* Base_L */
		STATE_IDLE_PALADIN_L,
		STATE_WALK_PALADIN_L,
		STATE_RUNBEGIN_PALADIN_L,
		STATE_RUN_PALADIN_L,

		STATE_JUMP_PALADIN_L,
		STATE_JUMPFALL_PALADIN_L,
		STATE_JUMP_LAND_PALADIN_L,

		STATE_STOP_PALADIN_L,

		STATE_BOUNCE_PALADIN_L,

		/*Sprint*/
		STATE_SPRINT_JUMP_PALADIN,
		STATE_SPRINT_JUMPFALL_PALADIN,
		STATE_SPRINTATTACK_BEGIN_PALADIN,
		STATE_SPRINTATTACK_PALADIN,
		STATE_SPRINT_BEGIN_PALADIN,
		STATE_SPRINT_LOOP_PALADIN,
		STATE_SPRINT_END_PALADIN,


		STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L, // 중상단 공격
		STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R, // 중상단 공격

		STATE_ATTACK_VERTICALCUT_PALADIN,


		STATE_SHIELDWALL_BEGIN_PALADIN,
		STATE_SHIELDWALL_LOOP_PALADIN,
		STATE_SHIELDWALL_END_PALADIN,
		STATE_SHIELDWALL_HIT_PALADIN,

		STATE_RUSH_BEGIN_PALADIN,
		STATE_RUSH_LOOP_PALADIN,
		STATE_RUSH_END_PALADIN,

		STATE_SHIELDSLAM_PALADIN,


		STATE_GUARD_BEGIN_PALADIN,
		STATE_GUARD_LOOP_PALADIN,
		STATE_GUARD_END_PALADIN,
		STATE_GUARD_CANCEL_PALADIN,

		STATE_HIT_PALADIN,
		STATE_GUARDHIT_PALADIN,
		STATE_GROGGYHIT_PALADIN,
		STATE_STINGHIT_PALADIN,
		STATE_FLYHIT_PALADIN,

#pragma endregion


#pragma region Qanda

			/* Base_R */
			STATE_IDLE_QANDA,
			STATE_WALK_QANDA,
			STATE_RUN_QANDA,

			STATE_JUMP_QANDA,
			STATE_JUMPFALL_QANDA,
			STATE_JUMP_LAND_QANDA,

			STATE_STOP_QANDA,

			STATE_SPRINT_JUMP_QANDA,
			STATE_SPRINT_JUMPFALL_QANDA,
			STATE_SPRINT_BEGIN_QANDA,
			STATE_SPRINT_LOOP_QANDA,
			STATE_SPRINT_END_QANDA,

			STATE_ATTACK_AIMING_QANDA,
			STATE_ATTACK_BEGIN_QANDA,
			STATE_ATTACK_SHOOT_QANDA,

			STATE_ATTACK_AIMING_SNIPING_QANDA,
			STATE_ATTACK_BEGIN_SNIPING_QANDA,
			STATE_ATTACK_SHOOT_SNIPING_QANDA,

			STATE_SHADOWSTEP_QANDA,

			STATE_ATTACK_CANCEL_QANDA,

			STATE_GUARD_QANDA,
			STATE_GUARD_CANCEL_QANDA,

			STATE_HIT_QANDA,
			STATE_GUARDHIT_QANDA,
			STATE_GROGGYHIT_QANDA,
			STATE_STINGHIT_QANDA,
			STATE_FLYHIT_QANDA,

#pragma endregion

#pragma region Priest

			STATE_IDLE_PRIEST,
			STATE_WALK_PRIEST,
			STATE_RUN_PRIEST,

			STATE_JUMP_PRIEST,
			STATE_JUMPFALL_PRIEST,
			STATE_JUMP_LAND_PRIEST,

			STATE_STOP_PRIEST,

			STATE_BOUNCE_PRIEST,


			/*Sprint*/
			STATE_SPRINT_JUMP_PRIEST,
			STATE_SPRINT_JUMPFALL_PRIEST,
			STATE_SPRINTATTACK_BEGIN_PRIEST,
			STATE_SPRINTATTACK_PRIEST,
			STATE_SPRINT_BEGIN_PRIEST,
			STATE_SPRINT_LOOP_PRIEST,
			STATE_SPRINT_END_PRIEST,

			STATE_ATTACK_STING_PRIEST,

			STATE_PROJECTILECATCH_BEGIN_PRIEST,
			STATE_PROJECTILECATCH_LOOP_PRIEST,
			STATE_PROJECTILECATCH_HIT_PRIEST,
			STATE_PROJECTILECATCH_SHOOT_PRIEST,

			STATE_WINDATTACK_PRIEST,
			STATE_AIRDASH_PRIEST,

			STATE_CURE_BEGIN_PRIEST,
			STATE_CURE_LOOP_PRIEST,
			STATE_CURE_END_PRIEST,

			STATE_HIT_PRIEST,
			STATE_GUARDHIT_PRIEST,
			STATE_GROGGYHIT_PRIEST,
			STATE_STINGHIT_PRIEST,
			STATE_FLYHIT_PRIEST,

#pragma endregion

#pragma region Lancer


			STATE_IDLE_LANCER,
			//STATE_WALK_LANCER,
			STATE_RUN_LANCER,

			STATE_JUMP_LANCER,
			STATE_JUMPFALL_LANCER,
			STATE_JUMP_LAND_LANCER,

			STATE_STOP_LANCER,
			STATE_BOUNCE_LANCER,

			//STATE_SPRINT_JUMP_LANCER,
			//STATE_SPRINT_JUMPFALL_LANCER,
			//STATE_SPRINT_BEGIN_LANCER,
			//STATE_SPRINT_LOOP_LANCER,
			//STATE_SPRINT_END_LANCER,

			STATE_ATTACK_DEAFULT_LANCER,
			STATE_ATTACK_STING_LANCER,
			
			STATE_ATTACK_BREEZE_BEGIN_LANCER,
			STATE_ATTACK_BREEZE_LOOP_LANCER,

			//STATE_ATTACK_AIMING_SNIPING_LANCER,
			//STATE_ATTACK_BEGIN_SNIPING_LANCER,
			//STATE_ATTACK_SHOOT_SNIPING_LANCER,

			//STATE_SHADOWSTEP_LANCER,

			//STATE_ATTACK_CANCEL_LANCER,

			//STATE_GUARD_LANCER,
			//STATE_GUARD_CANCEL_LANCER,

			STATE_HIT_LANCER,
			STATE_GUARDHIT_LANCER,
			STATE_GROGGYHIT_LANCER,
			STATE_STINGHIT_LANCER,
			STATE_FLYHIT_LANCER,

#pragma endregion

#pragma region AI_Sandbag
		STATE_IDLE_WARRIOR_R_AI_ENEMY,
		STATE_IDLE_WARRIOR_L_AI_ENEMY,


		/* 체인지. */
		STATE_HIT_TEST_ENEMY,
		STATE_GUARDHIT_ENEMY,
		STATE_GROGGY_ENEMY,
		STATE_STINGHIT_ENEMY,
		STATE_FLYHIT_ENEMY,

		STATE_GUARD_BEGIN_WARRIOR_AI_ENEMY,
		STATE_GUARD_LOOP_WARRIOR_AI_ENEMY,
		STATE_GUARD_CANCEL_WARRIOR_AI_ENEMY,
		STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY,
		STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY,

		STATE_FALL_WARRIOR_L_AI_ENEMY,
		STATE_FALL_WARRIOR_R_AI_ENEMY,

		STATE_LAND_WARRIOR_L_AI_ENEMY,
		STATE_LAND_WARRIOR_R_AI_ENEMY,

		STATE_BOUNCE_WARRIOR_L_AI_ENEMY,
		STATE_BOUNCE_WARRIOR_R_AI_ENEMY,

		SANDBAG_STATE_IDLE_WARHAMMER_R,

#pragma endregion

#pragma region AI_Warrior

		AI_STATE_IDLE_WARRIOR_L,
		AI_STATE_IDLE_WARRIOR_R,

		AI_STATE_WALK_WARRIOR_L,
		AI_STATE_WALK_WARRIOR_R,

		AI_STATE_RUN_WARRIOR_L,
		AI_STATE_RUN_WARRIOR_R,

		AI_STATE_RUNBEGIN_WARRIOR_L,
		AI_STATE_RUNBEGIN_WARRIOR_R,

		AI_STATE_GUARD_BEGIN_WARRIOR,
		AI_STATE_GUARD_LOOP_WARRIOR,
		AI_STATE_GUARD_CANCEL_WARRIOR,
		AI_STATE_GUARD_END_WARRIOR,

		AI_STATE_BOUNE_WARRIOR_L,
		AI_STATE_BOUNE_WARRIOR_R,

		AI_STATE_ATTACK_HORIZONTALMIDDLE_L,
		AI_STATE_ATTACK_HORIZONTALMIDDLE_R, 


		AI_STATE_TG_HIT_WARRIOR,
		AI_STATE_TG_GUARDHIT_WARRIOR,
		AI_STATE_TG_GROGGYHIT_WARRIOR,
		AI_STATE_TG_STINGHIT_WARRIOR,
		AI_STATE_TG_FLYHIT_WARRIOR,

			
#pragma endregion

		AI_STATE_COMMON_CHANGE_HERO,

#pragma region AI_Warrior_Behavior
		AI_STATE_PATROL_DEFAULT_WARRIOR_L,
		AI_STATE_PATROL_DEFAULT_WARRIOR_R,
		AI_STATE_PATROL_WARRIOR_SWITCH_R_TO_L,
		AI_STATE_PATROL_WARRIOR_SWITCH_L_TO_R,
		AI_STATE_PATROL_WALK_WARRIOR_L,
		AI_STATE_PATROL_WALK_WARRIOR_R,


		AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_L,
		AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R,
		AI_STATE_PATHNAVIGATION_WALK_WARRIOR_L,
		AI_STATE_PATHNAVIGATION_WALK_WARRIOR_R,
		AI_STATE_PATHNAVIGATION_SPRINTBEGIN_WARRIOR,
		AI_STATE_PATHNAVIGATION_SPRINTLOOP_WARRIOR,
		AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR,
		AI_STATE_PATHNAVIGATION_SPRINTJUMP_WARRIOR,
		AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_WARRIOR,
		AI_STATE_PATHNAVIGATION_JUMP_WARRIOR_L,
		AI_STATE_PATHNAVIGATION_JUMP_WARRIOR_R,


		AI_STATE_COMBAT_DEFAULT_WARRIOR_L,
		AI_STATE_COMBAT_DEFAULT_WARRIOR_R,

		AI_STATE_COMBAT_GUARDBEGIN_WARRIOR,
		AI_STATE_COMBAT_GUARDLOOP_WARRIOR,
		AI_STATE_COMBAT_GUARDDASH_WARRIOR,
		AI_STATE_COMBAT_GUARDEND_WARRIOR,
		AI_STATE_COMBAT_GUARDCANCEL_WARRIOR,

		AI_STATE_COMBAT_OXEN_BEGIN_WARRIOR,
		AI_STATE_COMBAT_OXEN_LOOP_WARRIOR,
		AI_STATE_COMBAT_OXEN_LOOPATTACK_WARRIOR,
		AI_STATE_COMBAT_OXEN_END_WARRIOR,

		AI_STATE_COMBAT_GUARDBREAK_WARRIOR,

		AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_L,
		AI_STATE_COMBAT_HORIZONTALUP_WARRIOR_R,

		AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L,
		AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R,

		AI_STATE_COMBAT_HORIZONTALDOWN_WARRIOR_L,
		AI_STATE_COMBAT_HORIZONTALDOWN_WARRIOR_R,

		AI_STATE_COMBAT_VERTICALCUT_WARRIOR,


		AI_STATE_COMMON_BOUNCE_WARRIOR_L,
		AI_STATE_COMMON_BOUNCE_WARRIOR_R,
		AI_STATE_COMMON_HIT_WARRIOR,
		AI_STATE_COMMON_GUARDHIT_WARRIOR,
		AI_STATE_COMMON_GROGGYHIT_WARRIOR,
		AI_STATE_COMMON_STINGHIT_WARRIOR,
		AI_STATE_COMMON_FLYHIT_WARRIOR,
		AI_STATE_COMMON_FALL_WARRIOR_L,
		AI_STATE_COMMON_FALL_WARRIOR_R,
		AI_STATE_COMMON_LAND_WARRIOR_L,
		AI_STATE_COMMON_LAND_WARRIOR_R,


#pragma endregion




#pragma region AI_Engineer_Behavior
		AI_STATE_PATROL_DEFAULT_ENGINEER_L,
		AI_STATE_PATROL_DEFAULT_ENGINEER_R,
		AI_STATE_PATROL_ENGINEER_SWITCH_R_TO_L,
		AI_STATE_PATROL_ENGINEER_SWITCH_L_TO_R,
		AI_STATE_PATROL_WALK_ENGINEER_L,
		AI_STATE_PATROL_WALK_ENGINEER_R,


		AI_STATE_COMBAT_DEFAULT_ENGINEER_L,
		AI_STATE_COMBAT_DEFAULT_ENGINEER_R,

		AI_STATE_COMBAT_GUARDBEGIN_ENGINEER,
		AI_STATE_COMBAT_GUARDLOOP_ENGINEER,
		AI_STATE_COMBAT_GUARDDASH_ENGINEER,
		AI_STATE_COMBAT_GUARDEND_ENGINEER,
		AI_STATE_COMBAT_GUARDCANCEL_ENGINEER,

		AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_L,
		AI_STATE_COMBAT_HORIZONTALMIDDLE_ENGINEER_R,

		AI_STATE_COMBAT_VERTICALCUT_ENGINEER_L,
		AI_STATE_COMBAT_VERTICALCUT_ENGINEER_R,

		AI_STATE_COMBAT_AIRSPIKE_BEGIN_ENGINEER,
		AI_STATE_COMBAT_AIRSPIKE_LOOP_ENGINEER,
		AI_STATE_COMBAT_AIRSPIKE_END_ENGINEER,

		AI_STATE_COMBAT_GROGGYATTACK_ENGINEER,


		AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_L,
		AI_STATE_PATHNAVIGATION_DEFAULT_ENGINEER_R,
		AI_STATE_PATHNAVIGATION_WALK_ENGINEER_L,
		AI_STATE_PATHNAVIGATION_WALK_ENGINEER_R,
		AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ENGINEER,
		AI_STATE_PATHNAVIGATION_SPRINTLOOP_ENGINEER,
		AI_STATE_PATHNAVIGATION_SPRINTEND_ENGINEER,
		AI_STATE_PATHNAVIGATION_SPRINTJUMP_ENGINEER,
		AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_ENGINEER,
		AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_L,
		AI_STATE_PATHNAVIGATION_JUMP_ENGINEER_R,

		AI_STATE_COMMON_BOUNCE_ENGINEER_L,
		AI_STATE_COMMON_BOUNCE_ENGINEER_R,
		AI_STATE_COMMON_HIT_ENGINEER,
		AI_STATE_COMMON_GUARDHIT_ENGINEER,
		AI_STATE_COMMON_GROGGYHIT_ENGINEER,
		AI_STATE_COMMON_STINGHIT_ENGINEER,
		AI_STATE_COMMON_FLYHIT_ENGINEER,
		AI_STATE_COMMON_FALL_ENGINEER_L,
		AI_STATE_COMMON_FALL_ENGINEER_R,
		AI_STATE_COMMON_LAND_ENGINEER_L,
		AI_STATE_COMMON_LAND_ENGINEER_R,

#pragma endregion



#pragma region AI_Fiona_Behavior
		AI_STATE_PATROL_DEFAULT_FIONA_L,
		AI_STATE_PATROL_DEFAULT_FIONA_R,
		AI_STATE_PATROL_FIONA_SWITCH_R_TO_L,
		AI_STATE_PATROL_FIONA_SWITCH_L_TO_R,
		AI_STATE_PATROL_WALK_FIONA_L,
		AI_STATE_PATROL_WALK_FIONA_R,


		AI_STATE_COMBAT_DEFAULT_FIONA_L,
		AI_STATE_COMBAT_DEFAULT_FIONA_R,

		AI_STATE_COMBAT_GUARDBEGIN_FIONA,
		AI_STATE_COMBAT_GUARDLOOP_FIONA,
		AI_STATE_COMBAT_GUARDDASH_FIONA,
		AI_STATE_COMBAT_GUARDEND_FIONA,
		AI_STATE_COMBAT_GUARDCANCEL_FIONA,

		AI_STATE_COMBAT_COUNTER_FIONA,
		AI_STATE_COMBAT_SHIELDATTACK_FIONA,
		AI_STATE_COMBAT_SPINATTACK_FIONA,

		AI_STATE_COMBAT_HORIZONTALUP_FIONA_L,
		AI_STATE_COMBAT_HORIZONTALUP_FIONA_R,

		AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_L,
		AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R,

		AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_L,
		AI_STATE_COMBAT_HORIZONTALDOWN_FIONA_R,

		AI_STATE_COMBAT_VERTICALCUT_FIONA,


		AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_L,
		AI_STATE_PATHNAVIGATION_DEFAULT_FIONA_R,
		AI_STATE_PATHNAVIGATION_WALK_FIONA_L,
		AI_STATE_PATHNAVIGATION_WALK_FIONA_R,
		AI_STATE_PATHNAVIGATION_SPRINTBEGIN_FIONA,
		AI_STATE_PATHNAVIGATION_SPRINTLOOP_FIONA,
		AI_STATE_PATHNAVIGATION_SPRINTEND_FIONA,
		AI_STATE_PATHNAVIGATION_SPRINTJUMP_FIONA,
		AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_FIONA,
		AI_STATE_PATHNAVIGATION_JUMP_FIONA_L,
		AI_STATE_PATHNAVIGATION_JUMP_FIONA_R,


		AI_STATE_COMMON_BOUNCE_FIONA_L,
		AI_STATE_COMMON_BOUNCE_FIONA_R,
		AI_STATE_COMMON_HIT_FIONA,
		AI_STATE_COMMON_GUARDHIT_FIONA,
		AI_STATE_COMMON_GROGGYHIT_FIONA,
		AI_STATE_COMMON_STINGHIT_FIONA,
		AI_STATE_COMMON_FLYHIT_FIONA,
		AI_STATE_COMMON_FALL_FIONA_L,
		AI_STATE_COMMON_FALL_FIONA_R,
		AI_STATE_COMMON_LAND_FIONA_L,
		AI_STATE_COMMON_LAND_FIONA_R,
#pragma endregion


#pragma region AI_Paladin_Behavior

			AI_STATE_PATROL_DEFAULT_PALADIN_L,
			AI_STATE_PATROL_DEFAULT_PALADIN_R,
			AI_STATE_PATROL_PALADIN_SWITCH_R_TO_L,
			AI_STATE_PATROL_PALADIN_SWITCH_L_TO_R,
			AI_STATE_PATROL_WALK_PALADIN_L,
			AI_STATE_PATROL_WALK_PALADIN_R,


			AI_STATE_COMBAT_DEFAULT_PALADIN_L,
			AI_STATE_COMBAT_DEFAULT_PALADIN_R,
			AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_L,
			AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R,
			AI_STATE_COMBAT_VERTICALCUT_PALADIN,

			AI_STATE_COMBAT_SHIELDWALL_BEGIN_PALADIN,
			AI_STATE_COMBAT_SHIELDWALL_LOOP_PALADIN,
			AI_STATE_COMBAT_SHIELDWALL_END_PALADIN,
			AI_STATE_COMBAT_SHIELDWALL_HIT_PALADIN,

			AI_STATE_COMBAT_RUSH_BEGIN_PALADIN,
			AI_STATE_COMBAT_RUSH_LOOP_PALADIN,
			AI_STATE_COMBAT_RUSH_END_PALADIN,

			AI_STATE_COMBAT_SHIELDSLAM_PALADIN,


			AI_STATE_COMBAT_GUARDBEGIN_PALADIN,
			AI_STATE_COMBAT_GUARDLOOP_PALADIN,
			AI_STATE_COMBAT_GUARDEND_PALADIN,
			AI_STATE_COMBAT_GUARDCANCEL_PALADIN,


			AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_L,
			AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_R,
			AI_STATE_PATHNAVIGATION_WALK_PALADIN_L,
			AI_STATE_PATHNAVIGATION_WALK_PALADIN_R,
			AI_STATE_PATHNAVIGATION_SPRINTBEGIN_PALADIN,
			AI_STATE_PATHNAVIGATION_SPRINTLOOP_PALADIN,
			AI_STATE_PATHNAVIGATION_SPRINTEND_PALADIN,
			AI_STATE_PATHNAVIGATION_SPRINTJUMP_PALADIN,
			AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_PALADIN,
			AI_STATE_PATHNAVIGATION_JUMP_PALADIN_L,
			AI_STATE_PATHNAVIGATION_JUMP_PALADIN_R,

			AI_STATE_COMMON_BOUNCE_PALADIN_L,
			AI_STATE_COMMON_BOUNCE_PALADIN_R,
			AI_STATE_COMMON_HIT_PALADIN,
			AI_STATE_COMMON_GUARDHIT_PALADIN,
			AI_STATE_COMMON_GROGGYHIT_PALADIN,
			AI_STATE_COMMON_STINGHIT_PALADIN,
			AI_STATE_COMMON_FLYHIT_PALADIN,
			AI_STATE_COMMON_FALL_PALADIN_L,
			AI_STATE_COMMON_FALL_PALADIN_R,
			AI_STATE_COMMON_LAND_PALADIN_L,
			AI_STATE_COMMON_LAND_PALADIN_R,

#pragma endregion


#pragma region AI_Archer_Behavior

			AI_STATE_PATROL_DEFAULT_ARCHER_L,
			AI_STATE_PATROL_DEFAULT_ARCHER_R,
			AI_STATE_PATROL_WALK_ARCHER_L,
			AI_STATE_PATROL_WALK_ARCHER_R,


			AI_STATE_COMBAT_DEFAULT_ARCHER_L,
			AI_STATE_COMBAT_DEFAULT_ARCHER_R,

			AI_STATE_COMBAT_ATTACK_SWING_ARCHER,
					 
			AI_STATE_COMBAT_ATTACK_AIMING_ARCHER,
			AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER,
			AI_STATE_COMBAT_ATTACK_SHOOT_ARCHER,
					 
			AI_STATE_COMBAT_ATTACK_AIMING_SNIPING_ARCHER,
			AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER,
			AI_STATE_COMBAT_ATTACK_SHOOT_SNIPING_ARCHER,
					 
			AI_STATE_COMBAT_ATTACK_AIMING_POISION_ARCHER,
			AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER,
			AI_STATE_COMBAT_ATTACK_SHOOT_POISION_ARCHER,

			AI_STATE_COMBAT_STATE_SWAP_ARCHER,

			AI_STATE_COMBAT_GUARDLOOP_ARCHER,


			//AI_STATE_PATHNAVIGATION_DEFAULT_ARCHER_L,
			//AI_STATE_PATHNAVIGATION_DEFAULT_ARCHER_R,
			//AI_STATE_PATHNAVIGATION_WALK_ARCHER_L,
			//AI_STATE_PATHNAVIGATION_WALK_ARCHER_R,
			//AI_STATE_PATHNAVIGATION_SPRINTBEGIN_ARCHER,
			//AI_STATE_PATHNAVIGATION_SPRINTLOOP_ARCHER,
			//AI_STATE_PATHNAVIGATION_SPRINTEND_ARCHER,
			//AI_STATE_PATHNAVIGATION_SPRINTJUMP_ARCHER,
			//AI_STATE_PATHNAVIGATION_SPRINTJUMPFALL_ARCHER,
			//AI_STATE_PATHNAVIGATION_JUMP_ARCHER_L,
			//AI_STATE_PATHNAVIGATION_JUMP_ARCHER_R,


			AI_STATE_COMMON_BOUNCE_ARCHER,
			AI_STATE_COMMON_HIT_ARCHER,
			AI_STATE_COMMON_GUARDHIT_ARCHER,
			AI_STATE_COMMON_GROGGYHIT_ARCHER,
			AI_STATE_COMMON_STINGHIT_ARCHER,
			AI_STATE_COMMON_FLYHIT_ARCHER,
			AI_STATE_COMMON_FALL_ARCHER_L,
			AI_STATE_COMMON_FALL_ARCHER_R,
			AI_STATE_COMMON_LAND_ARCHER_L,
			AI_STATE_COMMON_LAND_ARCHER_R,

#pragma endregion

			STATE_END,
	};

	//// HR
	enum WEAPON_TYPE
	{
		WEAPON_LONGSWORD,
		WEAPON_SPEAR,
		WEAPON_END,
	};
	enum CURVE_TYPE
	{
		CURVE_LINEAR,
		CURVE_SIN,
		CURVE_SPIRAL,
		CURVE_ROTATION,
		CURVE_CHARGE,
		CURVE_CIRCLE,
		CANNON_BONE,
		SHADOWSTEP,
		CURVE_END,
	};
	enum class eBehaviorType
	{
		ePatrol,
		eFollow,
		eAttack,
		ePathNavigation,
		eResurrect,
		eChange,
		eCNT
	};
}
