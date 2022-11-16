#pragma once
namespace Client
{
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


		COL_BODY,
		COL_PLAYERHITBOX_HEAD,
		COL_PLAYERHITBOX_BODY,

		COL_PLAYERATTACK,
		COL_PLAYERTHROW,

		COL_ENEMYHITBOX_HEAD,
		COL_ENEMYHITBOX_BODY,
		COL_ENEMYATTACK,
		COL_ENEMYTHROW,

		COL_WALL,

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

		GROUP_END = GR_END
	};

	enum LEVEL_TYPE_CLIENT
	{
		LEVEL_LOGO,
		LEVEL_LOADING,
		LEVEL_MAINMENU,
		LEVEL_TEST,
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
		VTXANIM_PASS_DRAGON,

		VTXANIM_PASS_END,
	};

	enum VTXINSTANCE_PASS_TYPE
	{
		VTXINSTANCE_PASS_DEFAULT,
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
		VTXTEX_PASS_UI_HpBar,
		VTXTEX_PASS_UI_HeroGauge,
		VTXTEX_PASS_UI_RELIC,
		VTXTEX_PASS_UI_PortEffect,
		VTXTEX_PASS_UI_HeroKeySkill,

		VTXTEX_PASS_ALPHA,
		VTXTEX_PASS_LOADINGBAR,
		VTXTEX_PASS_NOBLACK,
		VTXTEX_PASS_CROSSHAIR,
		VTXTEX_PASS_COLORNOBLACK,
		VTXTEX_PASS_SKILLICON,
		VTXTEX_PASS_HPBAR,
		VTXTEX_PASS_HPBAREFFECT,
		VTXTEX_PASS_ENEMYHPBAR,
		VTXTEX_PASS_ENEMYHPBARLINE,
		VTXTEX_PASS_DIALOG,
		VTXTEX_PASS_BOSSHP,
		VTXTEX_PASS_TRAIL,
		VTXTEX_PASS_KOTO,
		VTXTEX_PASS_FOOTTRAIL,
		VTXTEX_PASS_BLOODOVERLAY,
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

#pragma region Guard
		STATE_GUARD_BEGIN_PLAYER,
		STATE_GUARD_END_PLAYER,
		STATE_GUARD_LOOP_PLAYER,
		STATE_GUARD_CANCEL_PLAYER,


#pragma endregion

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



#pragma region AI
			STATE_IDLE_WARRIOR_R_AI_ENEMY,
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
		CURVE_END,
	};
}

