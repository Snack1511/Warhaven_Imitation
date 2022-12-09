#pragma once
namespace Client
{
	struct FADEDESC
	{
		enum FADE_STYLE
		{
			FADE_STYLE_DEFAULT,
			FADE_STYLE_MOVE,
			FADE_STYLE_SCALEUP, // �������� ũ�Ⱑ ��
			FADE_STYLE_SCALEDOWN, // �������� ũ�Ⱑ ����
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



	struct UNIT_STATUS
	{
		_float fHP = 100.f;
		_float fMaxHP = 100.f;
		_float fRunSpeed = 7.5f;
		_float fRunBeginSpeed = 11.f;
		_float fWalkSpeed = 2.f;
		_float fDashSpeed = 16.f;
		_float fWallJumpSpeed = 15.f;
		_float fWireSpeed = 25.f;
		_float fJumpPower = 6.5f;
	};

	struct UNIT_MODEL_DATA
	{
		wstring	strModelPaths[MODEL_PART_END];
		string	strRefBoneName[MODEL_PART_END];
	};

	struct BEHAVIOR_DESC
	{
		_uint iStateType = 0;

		void* pTriggerPtr = nullptr;
		void* pAlliesPlayer = nullptr;
		void* pEnemyPlayer = nullptr;

		_float fDelayTime = 0.f;

		_float fPriorityWeight = 0.f;

		eBehaviorType eCurType;
		eBehaviorType ePrevType;

	};
}

