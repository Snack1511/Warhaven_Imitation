#pragma once

namespace Engine
{
	enum RES_TYPE { RES_MATERIAL, RES_BONE, RES_MESH, RES_ANIMATION, RES_END };

	enum MODEL_TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_ONLYANIMATION, TYPE_END };

	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum RENDER_GROUP { RENDER_PRIORITY, RENDER_PLAYERSHADOW, RENDER_SHADOW, RENDER_NONALPHA,
		RENDER_ALPHA, RENDER_TRAIL, RENDER_DISTORTION, RENDER_SSD, RENDER_UI, RENDER_END };

	enum class ANIM_DIVIDE
	{
		eDEFAULT,
		eBODYUPPER,
		eBODYLOWER,
		eCount
	};

	enum OBJ_GROUP
	{
		GR_DEFAULT,
		GR_END = 32
	};

	enum COL_GROUP
	{
		COL_DEFAULT,
		COL_END = 32
	};

	enum COM_GROUP
	{
		COM_GROUP_DEFAULT,
		COM_GROUP_END = 32
	};

#pragma region CHANNEL_ENUM

	enum CHANNEL_GROUP
	{
		CH_BGM,
		CH_GROUP_0,
		CH_GROUP_1,
		CH_GROUP_2,
		CH_GROUP_3,
		CH_GROUP_END = 5
	};


#pragma endregion




#pragma region KEY_MANGER

	enum class KEY
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT,
		LSHIFT,
		SPACE,
		CTRL,
		ENTER,
		ESC,
		TAB,
		
		LBUTTON,
		RBUTTON,
		MOUSEXBUTTON1,
		MOUSEXBUTTON2,

		BACKSPACE,

		INSERTKEY,
		HOMEKEY,
		ENDKEY,
		PAGEUP,
		PAGEDOWN,
		DELETEKEY,
		LAST,
	};

	enum class KEY_STATE {
		TAP, // �� ���� ����
		HOLD,// �� ������ ����
		AWAY,// �� �� ����
		NONE,// �� �ȴ��� ����
	};
#pragma endregion

}