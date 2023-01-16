#pragma once
namespace Client
{

	struct FADEDESC
	{
		enum FADE_STYLE
		{
			FADE_STYLE_DEFAULT,
			FADE_STYLE_MOVE,
			FADE_STYLE_SCALEUP, // 원본보다 크기가 위
			FADE_STYLE_SCALEDOWN, // 원본보다 크기가 작음
			FADE_STYLE_END,
		};

		enum FADEOUT_TYPE
		{
			FADEOUT_DELETE,
			FADEOUT_DISABLE,
			FADEOUT_NEXTTEXTURE,
			FADEOUT_RANDOMTEXTURE,
			FADEOUT_NONE,
			FADEOUT_END,
		};

		FADE_STYLE		eFadeStyle = FADE_STYLE_END;
		FADEOUT_TYPE	eFadeOutType = FADEOUT_END;
		_byte			bFadeInFlag = 0;
		_byte			bFadeOutFlag = 0;
		_float			fFadeInStartTime = 0.f;
		_float			fFadeInTime = 0.f;
		_float			fFadeOutTime = 0.f;
		_float			fFadeOutStartTime = 0.f;
		_float			fAlpha = 0.f;
		KEY				eKeyType = KEY::LAST;
	};



	struct UNIT_MODEL_DATA
	{
		wstring	strModelPaths[MODEL_PART_END];
		string	strRefBoneName[MODEL_PART_END];
	};

	struct BEHAVIOR_DESC
	{

		class CTrigger* pTriggerPtr = nullptr;
		class CPlayer* pAlliesPlayer = nullptr;
		class CPlayer* pEnemyPlayer = nullptr;
		class CCannon* pNearCannon = nullptr;

		_float fDelayTime = 0.f;

		_int iPriorityScore = 0;

		eBehaviorType eCurType;
		eBehaviorType ePrevType;

	};
}

