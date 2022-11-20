#include "CUI_MainPlay.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "CUI_Renderer.h"
#include "CUser.h"
#include "CUI_Cursor.h"
#include "CShader.h"
#include "CFader.h"
#include "CButton.h"

#include "Loading_Manager.h"

CUI_MainPlay::CUI_MainPlay()
{
}

CUI_MainPlay::~CUI_MainPlay()
{
}

HRESULT CUI_MainPlay::Initialize_Prototype()
{
	Create_PlayBtn();
	Create_ModeBG();
	Create_StageBtn();
	Create_LockImg();
	Create_StageHighlight();
	Create_BtnHighlight();

	return S_OK;
}

HRESULT CUI_MainPlay::Start()
{
	__super::Start();

	Bind_Shader();
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

void CUI_MainPlay::Set_Shader_StageHighlight(CShader* pShader, const char* pConstName)
{
	_float4 vColor = _float4(1.f, 1.f, 1.f, 0.05f);
	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
}

void CUI_MainPlay::Set_Shader_BtnHighlight(CShader* pShader, const char* pConstName)
{
	if (m_pBtnHightlight->Is_Valid())
	{
		m_fAccTime += fDT(0) * 0.25f;
		pShader->Set_RawValue("g_fValue", &m_fAccTime, sizeof(_float));
	}
}

void CUI_MainPlay::On_PointEnter_PlayBtn(const _uint& iEventNum)
{
	for (int i = 0; i < 2; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pPlayBtnUI[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			_float4 vPos = pTarget->Get_Pos();
			_float4 vScale = pTarget->Get_Scale();
			_float fPosX = vPos.x + vScale.x;
			_float fScaleX = vScale.x * 3.f;

			m_pBtnHightlight->Set_Pos(fPosX - 20.f, vPos.y);
			m_pBtnHightlight->Set_Scale(fScaleX, 500.f);

			ENABLE_GAMEOBJECT(m_pBtnHightlight);
		}
	}
}

void CUI_MainPlay::On_PointExit_PlayBtn(const _uint& iEventNum)
{
	DISABLE_GAMEOBJECT(m_pBtnHightlight);
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
		break;
	}
}

void CUI_MainPlay::On_PointEnter_Stage(const _uint& iEventNum)
{
	for (int i = 0; i < 4; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pStageSelectBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			_uint iTextureNum = GET_COMPONENT_FROM(pTarget, CTexture)->Get_CurTextureIndex();
			if (iTextureNum <= 3)
			{
				_float4 vPos = pTarget->Get_Pos();
				m_pStageHighlight->Set_Pos(vPos.x, vPos.y);

				ENABLE_GAMEOBJECT(m_pStageHighlight);
			}
		}
	}
}

void CUI_MainPlay::On_PointStay_Stage(const _uint& iEventNum)
{
	for (int i = 0; i < 4; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pStageSelectBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			_uint iTextureNum = GET_COMPONENT_FROM(pTarget, CTexture)->Get_CurTextureIndex();
			if (iTextureNum > 3)
			{
				CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
			}
		}
	}
}

void CUI_MainPlay::On_PointExit_Stage(const _uint& iEventNum)
{
	DISABLE_GAMEOBJECT(m_pStageHighlight);
}

void CUI_MainPlay::On_PointDown_Stage(const _uint& iEventNum)
{
	for (int i = 0; i < 4; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pStageSelectBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			_uint iTextureNum = GET_COMPONENT_FROM(pTarget, CTexture)->Get_CurTextureIndex();
			if (iTextureNum <= 3)
			{
				if (iTextureNum == 0)
				{
					m_eStage = Select_Stage::Test;
				}
				else if (iTextureNum == 1)
				{

				}
				else if (iTextureNum == 2)
				{

				}
				else if (iTextureNum == 3)
				{
					m_eStage = Select_Stage::Training;
				}
			}
		}
	}
}

void CUI_MainPlay::On_PointUpEvent_Mode(const _uint& iEventNum)
{
	SetActive_ModeWindow();
}

void CUI_MainPlay::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pStageHighlight, CShader)
		->CallBack_SetRawValues += bind(&CUI_MainPlay::Set_Shader_StageHighlight, this, placeholders::_1, "g_vColor");

	GET_COMPONENT_FROM(m_pBtnHightlight, CShader)
		->CallBack_SetRawValues += bind(&CUI_MainPlay::Set_Shader_BtnHighlight, this, placeholders::_1, "g_fValue");
}

void CUI_MainPlay::Bind_Btn()
{
	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnUI[i]->CallBack_PointEnter += bind(&CUI_MainPlay::On_PointEnter_PlayBtn, this, placeholders::_1);
		m_pPlayBtnUI[i]->CallBack_PointExit += bind(&CUI_MainPlay::On_PointExit_PlayBtn, this, placeholders::_1);
	}

	m_pPlayBtnUI[0]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Start, this, placeholders::_1);
	m_pPlayBtnUI[1]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Mode, this, placeholders::_1);

	for (int i = 0; i < 4; ++i)
	{
		m_pStageSelectBtn[i]->CallBack_PointEnter += bind(&CUI_MainPlay::On_PointEnter_Stage, this, placeholders::_1);
		m_pStageSelectBtn[i]->CallBack_PointStay += bind(&CUI_MainPlay::On_PointStay_Stage, this, placeholders::_1);
		m_pStageSelectBtn[i]->CallBack_PointExit += bind(&CUI_MainPlay::On_PointExit_Stage, this, placeholders::_1);
		m_pStageSelectBtn[i]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage, this, placeholders::_1);
	}
}

void CUI_MainPlay::SetActive_ModeWindow()
{
	if (!m_pBG->Is_Valid())
	{
		for (int i = 0; i < 2; ++i)
		{
			// 왼쪽으로 이동
			// 위로 살짝 이동
			// 페이드 아웃
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
		DISABLE_GAMEOBJECT(m_pStageHighlight);

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

void CUI_MainPlay::Enable_StageHighlight(_float4 vPos)
{
	m_pStageHighlight->Set_Pos(vPos.x, vPos.y);

	ENABLE_GAMEOBJECT(m_pStageHighlight);
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

void CUI_MainPlay::Create_PlayBtn()
{
	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnUI[i] = CUI_Object::Create();

		m_pPlayBtnUI[i]->Set_Sort(0.91f);
		m_pPlayBtnUI[i]->Set_MouseTarget(true);

		m_pPlayBtnUI[i]->Set_FontRender(true);
		m_pPlayBtnUI[i]->Set_FontStyle(true);

		GET_COMPONENT_FROM(m_pPlayBtnUI[i], CTexture)->Remove_Texture(0);

		CREATE_GAMEOBJECT(m_pPlayBtnUI[i], GROUP_UI);
	}

	m_pPlayBtnUI[0]->Set_Pos(-500.f, 180.f);
	m_pPlayBtnUI[0]->Set_Scale(200.f, 64.f);
	m_pPlayBtnUI[0]->Set_FontText(TEXT("게임 시작"));
	m_pPlayBtnUI[0]->Set_FontScale(0.7f);
	m_pPlayBtnUI[0]->Set_FontOffset(-100.f, -35.f);

	m_pPlayBtnUI[1]->Set_Pos(-535.f, 100.f);
	m_pPlayBtnUI[1]->Set_Scale(128.f, 40.f);
	m_pPlayBtnUI[1]->Set_FontText(TEXT("모드 변경"));
	m_pPlayBtnUI[1]->Set_FontScale(0.4f);
	m_pPlayBtnUI[1]->Set_FontOffset(-60.f, -17.f);
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

void CUI_MainPlay::Create_StageHighlight()
{
	m_pStageHighlight = CUI_Object::Create();
	m_pStageHighlight->Set_Scale(216.f, 340.f);
	m_pStageHighlight->Set_Sort(0.8f);
	m_pStageHighlight->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_GradientSmall2.dds"));

	CREATE_GAMEOBJECT(m_pStageHighlight, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pStageHighlight);
}

void CUI_MainPlay::Create_BtnHighlight()
{
	m_pBtnHightlight = CUI_Object::Create();
	m_pBtnHightlight->Set_Scale(600.f, 400.f);
	m_pBtnHightlight->Set_Sort(0.92f);

	m_pBtnHightlight->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/LobbyEffect/T_AdditiveGlow3.dds"));
	m_pBtnHightlight->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/LobbyEffect/T_Trail_01.dds"));

	GET_COMPONENT_FROM(m_pBtnHightlight, CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_MainEffect);

	CREATE_GAMEOBJECT(m_pBtnHightlight, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBtnHightlight);
}
