#include "CUI_MainPlay.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "CUI_Renderer.h"
#include "CUser.h"
#include "CUI_Cursor.h"
#include "CShader.h"

#include "Loading_Manager.h"

CUI_MainPlay::CUI_MainPlay()
{
}

CUI_MainPlay::~CUI_MainPlay()
{
}

HRESULT CUI_MainPlay::Initialize_Prototype()
{
	m_wstrName = TEXT("MainPlay_Btn");

	__super::Initialize_Prototype();

	GET_COMPONENT_FROM(m_pUI, CTexture)
		->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/PlayText/Text_Mode.png"));

	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnUI[i] = m_pUI->Clone();
	}

	Create_ModeBG();
	Create_StageBtn();
	Create_LockImg();

	return S_OK;
}

HRESULT CUI_MainPlay::Start()
{
	__super::Start();

	DELETE_GAMEOBJECT(m_pUI);

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pPlayBtnUI[i], GROUP_UI);
	}

	Set_ModeBtn();

	Bind_Btn();

	return S_OK;
}

void CUI_MainPlay::My_Tick()
{
	__super::My_Tick();

	if (m_pBG->Is_Valid())
	{
		if (KEY(ESC, TAP))
		{
			SetActive_ModeWindow();
		}
	}
}

void CUI_MainPlay::On_PointUpEvent_Start(const _uint& iEventNum)
{
	switch (m_eStage)
	{
	case CUI_MainPlay::Test:
		CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
		break;

	case CUI_MainPlay::Training:
		Call_MsgBox(TEXT("훈련소"));
		// CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
		break;
	}
}

void CUI_MainPlay::On_PointStay_Stage(const _uint& iEventNum)
{
	CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
}

void CUI_MainPlay::On_PointDown_Stage0(const _uint& iEnventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[0], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		m_eStage = Select_Stage::Test;
	}
}

void CUI_MainPlay::On_PointDown_Stage1(const _uint& iEnventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[1], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		Call_MsgBox(TEXT("테스트 레벨1"));
	}
}

void CUI_MainPlay::On_PointDown_Stage2(const _uint& iEnventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[2], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		Call_MsgBox(TEXT("테스트 레벨2"));
	}
}

void CUI_MainPlay::On_PointDown_Stage3(const _uint& iEnventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[3], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		m_eStage = Select_Stage::Training;
	}
}

void CUI_MainPlay::On_PointUpEvent_Mode(const _uint& iEventNum)
{
	SetActive_ModeWindow();
}

void CUI_MainPlay::Bind_Btn()
{
	m_pPlayBtnUI[0]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Start, this, placeholders::_1);
	m_pPlayBtnUI[1]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Mode, this, placeholders::_1);

	// 스테이지 버튼 전부 접근
	for (int i = 0; i < 4; ++i)
	{
		_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Get_CurTextureIndex();
		if (iTextureNum > 3)
		{
			m_pStageSelectBtn[i]->CallBack_PointStay += bind(&CUI_MainPlay::On_PointStay_Stage, this, placeholders::_1);
		}
	}

	m_pStageSelectBtn[0]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage0, this, placeholders::_1);
	m_pStageSelectBtn[1]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage1, this, placeholders::_1);
	m_pStageSelectBtn[2]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage2, this, placeholders::_1);
	m_pStageSelectBtn[3]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage3, this, placeholders::_1);
}

void CUI_MainPlay::SetActive_ModeWindow()
{
	if (!m_pBG->Is_Valid())
	{
		for (int i = 0; i < 2; ++i)
		{
			DISABLE_GAMEOBJECT(m_pPlayBtnUI[i]);
		}

		ENABLE_GAMEOBJECT(m_pBG);
		ENABLE_GAMEOBJECT(m_pTextModeSelect);
		ENABLE_GAMEOBJECT(m_pEscKey);
		ENABLE_GAMEOBJECT(m_pLine);

		for (int i = 0; i < 4; ++i)
		{
			ENABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
		}

		Set_LockImg();
	}
	else
	{
		for (int i = 0; i < 2; ++i)
		{
			ENABLE_GAMEOBJECT(m_pPlayBtnUI[i]);
		}

		DISABLE_GAMEOBJECT(m_pBG);
		DISABLE_GAMEOBJECT(m_pTextModeSelect);
		DISABLE_GAMEOBJECT(m_pEscKey);
		DISABLE_GAMEOBJECT(m_pLine);

		for (int i = 0; i < 4; ++i)
		{
			DISABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
		}

		for (int i = 0; i < 3; ++i)
		{
			DISABLE_GAMEOBJECT(m_pLockBtn[i]);
		}
	}
}

void CUI_MainPlay::Set_ModeBtn()
{
	m_pPlayBtnUI[1]->Set_Scale(120.f, 60.f);
	m_pPlayBtnUI[1]->Set_Pos(-560.f, 78.f);

	GET_COMPONENT_FROM(m_pPlayBtnUI[1], CTexture)->Set_CurTextureIndex(1);
}

void CUI_MainPlay::Set_LockImg()
{
	for (int i = 0; i < 3; ++i)
	{
		_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Get_CurTextureIndex();

		if (iTextureNum > 3)
		{
			_float4 vPos = m_pStageSelectBtn[i]->Get_Pos();

			if (i == 0)
			{
				if (iTextureNum == 4)
				{
					m_pLockBtn[i]->Set_Pos(vPos.x, vPos.y);

					ENABLE_GAMEOBJECT(m_pLockBtn[i]);
				}
			}

			if (i == 1)
			{
				if (iTextureNum == 5)
				{
					m_pLockBtn[i]->Set_Pos(vPos.x, vPos.y);

					ENABLE_GAMEOBJECT(m_pLockBtn[i]);
				}
			}

			if (i == 2)
			{
				if (iTextureNum == 6)
				{
					m_pLockBtn[i]->Set_Pos(vPos.x, vPos.y);

					ENABLE_GAMEOBJECT(m_pLockBtn[i]);
				}
			}
		}
	}
}

void CUI_MainPlay::Create_ModeBG()
{
	m_pBG = CUI_Object::Create();
	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.9f);
	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG2.dds"));

	m_pTextModeSelect = CUI_Object::Create();
	m_pTextModeSelect->Set_Pos(-470.f, 230.f);
	m_pTextModeSelect->Set_Sort(0.85f);
	m_pTextModeSelect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
	m_pTextModeSelect->Set_FontRender(true);
	m_pTextModeSelect->Set_FontStyle(true);
	m_pTextModeSelect->Set_FontScale(0.4f);
	m_pTextModeSelect->Set_FontText(TEXT("게임 모드 선택"));

	m_pEscKey = CUI_Object::Create();
	m_pEscKey->Set_Pos(-600.f, -330.f);
	m_pEscKey->Set_Scale(36.f, 28.f);
	m_pEscKey->Set_Sort(0.85f);
	m_pEscKey->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_BlackEscKeyIcon.png"));
	m_pEscKey->Set_FontRender(true);
	m_pEscKey->Set_FontStyle(true);
	m_pEscKey->Set_FontOffset(31.f, -16.f);
	m_pEscKey->Set_FontScale(0.3f);
	m_pEscKey->Set_FontText(TEXT("뒤로가기"));

	m_pLine = CUI_Object::Create();
	m_pLine->Set_Pos(0.f, -200.f);
	m_pLine->Set_Scale(1000.f, 20.f);
	m_pLine->Set_Sort(0.85f);
	m_pLine->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_LineDeco.png"));

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);

	CREATE_GAMEOBJECT(m_pTextModeSelect, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pTextModeSelect);

	CREATE_GAMEOBJECT(m_pEscKey, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pEscKey);

	CREATE_GAMEOBJECT(m_pLine, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pLine);
}

void CUI_MainPlay::Create_StageBtn()
{
	m_pPrototypeStageBtn = CUI_Object::Create();

	m_pPrototypeStageBtn->Set_Scale(216.f, 340.f);
	m_pPrototypeStageBtn->Set_Sort(0.85f);

	m_pPrototypeStageBtn->Set_MouseTarget(true);

	m_pPrototypeStageBtn->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage1.dds"));

	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage2.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage3.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage4.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage1.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage2.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage3.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage4.dds"));

	for (int i = 0; i < 4; ++i)
	{
		m_pStageSelectBtn[i] = m_pPrototypeStageBtn->Clone();
	}

	CREATE_GAMEOBJECT(m_pPrototypeStageBtn, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPrototypeStageBtn);

	for (int i = 0; i < 4; ++i)
	{
		_float fPosX = -360.f + (i * 240.f);
		m_pStageSelectBtn[i]->Set_Pos(fPosX, 0.f);

		// 훈련장 제외 모든 버튼 비활성화
		GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i + (1 * 4));

		m_pStageSelectBtn[i]->Set_FontRender(true);
		m_pStageSelectBtn[i]->Set_FontStyle(true);

		if (i == 0)
		{
			m_pStageSelectBtn[i]->Set_FontOffset(vFontOffset.x, vFontOffset.y);
			m_pStageSelectBtn[i]->Set_FontScale(m_fFontSize);
			m_pStageSelectBtn[i]->Set_FontText(TEXT("일반전"));

			GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i);
		}

		if (i == 1)
		{
			m_pStageSelectBtn[i]->Set_FontOffset(vFontOffset.x, vFontOffset.y);
			m_pStageSelectBtn[i]->Set_FontScale(m_fFontSize);
			m_pStageSelectBtn[i]->Set_FontText(TEXT("경쟁전"));
		}

		if (i == 2)
		{
			m_pStageSelectBtn[i]->Set_FontOffset(vFontOffset.x, vFontOffset.y);
			m_pStageSelectBtn[i]->Set_FontScale(m_fFontSize);
			m_pStageSelectBtn[i]->Set_FontText(TEXT("사용자 지정 게임"));
		}

		// 훈련장 활성화
		if (i == 3)
		{
			m_pStageSelectBtn[i]->Set_FontOffset(vFontOffset.x, vFontOffset.y);
			m_pStageSelectBtn[i]->Set_FontScale(m_fFontSize);
			m_pStageSelectBtn[i]->Set_FontText(TEXT("훈련소"));

			GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i);
		}

		CREATE_GAMEOBJECT(m_pStageSelectBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
	}
}

void CUI_MainPlay::Create_LockImg()
{
	m_pPrototypeLock = CUI_Object::Create();
	m_pPrototypeLock->Set_Scale(64.f);

	m_pPrototypeLock->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_Lock.dds"));

	for (int i = 0; i < 3; ++i)
	{
		m_pLockBtn[i] = m_pPrototypeLock->Clone();
	}

	CREATE_GAMEOBJECT(m_pPrototypeLock, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPrototypeLock);

	for (int i = 0; i < 3; ++i)
	{
		CREATE_GAMEOBJECT(m_pLockBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pLockBtn[i]);

		m_pLockBtn[i]->Set_Sort(0.8f);
	}
}
