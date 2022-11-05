#pragma once
namespace Client
{
	enum ANIM_TYPE
	{
		ANIM_BASE,
		ANIM_END,
	};

	enum UNIT_EYE_TYPE
	{
		EYE_DEFAULT,
		EYE_END

	};

	enum MODEL_PART_TYPE
	{
		MODEL_PART_HEAD,
		MODEL_PART_BODY,
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
		COL_PLAYERHITBOX,
		COL_PLAYERATTACK,
		COL_PLAYERTHROW,

		COL_ENEMYHITBOX,
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
		LEVEL_LOADING,
		LEVEL_TEST,
		LEVEL_END
	};

	enum COMPONENT_PIPELINE
	{
		CP_BEFORE_NAVIGATION,
		CP_NAVIGATION,
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
		VTXEFFECT_PASS_LINEX,
		VTXEFFECT_PASS_LINEY,
		VTXEFFECT_PASS_TEXT,
		VTXEFFECT_PASS_CARTOON,
		VTXEFFECT_PASS_DISTORTION,
		VTXEFFECT_PASS_NOISE,
		VTXEFFECT_PASS_DECAL,
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
		VTXTEX_PASS_END,
	};

	enum VTXCUBETEX_PASS_TYPE
	{
		VTXCUBETEX_PASS_SKYBOX,
		VTXCUBETEX_PASS_WIRE,
		VTXCUBETEX_PASS_END,
	};

	enum VTXNOR_PASS_TYPE
	{
		VTXNOR_PASS_TERRAIN,
		VTXNOR_PASS_NAVIGATION,
		VTXNOR_PASS_END,
	};

	enum MESH_TYPE
	{
		MESH_RECT,
		MESH_END,
	};

	enum STATE_TYPE
	{
		STATE_IDLE_PLAYER,


		STATE_END,
	};
}

