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
#include "Easing_Utillity.h"

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
	Create_StageNameRect();
	Crerate_PlayBtnMouseEnterLine();
	Create_SelectTextRect();

	return S_OK;
}

HRESULT CUI_MainPlay::Start()
{
	__super::Start();

	Bind_Shader();
	Bind_Btn();

	Set_FadeModeWindow();

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

	if (m_pPlayBtnMouseEnterLineArr[0]->Is_Valid())
	{
		_float fCurScaleY = m_pPlayBtnMouseEnterLineArr[0]->Get_Scale().y;
		if (fCurScaleY <= 4.f)
		{
			DISABLE_GAMEOBJECT(m_pPlayBtnMouseEnterLineArr[0]);
		}
	}

	if (m_pPlayBtnMouseEnterLineArr[1]->Is_Valid())
	{
		_float fCurScaleY = m_pPlayBtnMouseEnterLineArr[1]->Get_Scale().y;
		if (fCurScaleY <= 4.f)
		{
			DISABLE_GAMEOBJECT(m_pPlayBtnMouseEnterLineArr[1]);
		}
	}
}

void CUI_MainPlay::Set_Shader_StageHighlight(CShader* pShader, const char* pConstName)
{
	_float4 vColor = _float4(1.f, 1.f, 1.f, 0.3f);
	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
}

void CUI_MainPlay::Set_Shader_StageClickRect(CShader* pShader, const char* pConstName)
{
	if (m_pStageSelectRect->Is_Valid())
	{
		_float4 vColor = _float4(0.773f, 0.714f, 0.596f, 1.f);
		pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
	}
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
	m_pTarget = GET_COMPONENT_FROM(m_pPlayBtnUI[iEventNum], CButton)->Get_TargetUI();

	_float4 vPos = m_pTarget->Get_Pos();
	_float4 vScale = m_pTarget->Get_Scale();

	_float fPosX = vPos.x + vScale.x - 20.f;
	_float fDuration = 0.2f;

	m_pBtnHightlight->Set_Pos(fPosX, vPos.y);
	m_pBtnHightlight->Set_Scale(vScale.x * 3.f, 500.f);
	m_pTarget->Lerp_PosX(vPos.x, vPos.x + 20.f, fDuration);

	m_pPlayBtnMouseEnterLineArr[iEventNum]->Set_PosY(vPos.y - 3.5f);
	m_pPlayBtnMouseEnterLineArr[iEventNum]->Lerp_ScaleY(4.f, vScale.y - 22.f, fDuration);

	ENABLE_GAMEOBJECT(m_pBtnHightlight);
	ENABLE_GAMEOBJECT(m_pPlayBtnMouseEnterLineArr[iEventNum]);
}

void CUI_MainPlay::On_PointExit_Start(const _uint& iEventNum)
{
	_float4 vPos = m_pPlayBtnUI[0]->Get_Pos();

	_float fDuration = 0.2f;

	m_pPlayBtnUI[0]->Lerp_PosX(vPos.x, -500.f, fDuration);

	_float fCurScaleY = m_pPlayBtnMouseEnterLineArr[0]->Get_Scale().y;
	m_pPlayBtnMouseEnterLineArr[0]->Lerp_ScaleY(fCurScaleY, 4.f, fDuration);

	DISABLE_GAMEOBJECT(m_pBtnHightlight);
}

void CUI_MainPlay::On_PointExit_Mode(const _uint& iEventNum)
{
	_float4 vPos = m_pPlayBtnUI[1]->Get_Pos();

	_float fDuration = 0.2f;

	m_pPlayBtnUI[1]->Lerp_PosX(vPos.x, -535.f, fDuration);

	_float fCurScaleY = m_pPlayBtnMouseEnterLineArr[1]->Get_Scale().y;

	m_pPlayBtnMouseEnterLineArr[1]->Lerp_ScaleY(fCurScaleY, 4.f, fDuration);

	DISABLE_GAMEOBJECT(m_pBtnHightlight);
}

void CUI_MainPlay::On_PointUpEvent_Start(const _uint& iEventNum)
{
	if (!m_pBG->Is_Valid())
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
}

void CUI_MainPlay::On_PointUpEvent_Mode(const _uint& iEventNum)
{
	if (!m_pBG->Is_Valid())
	{
		SetActive_ModeWindow();
	}
}

void CUI_MainPlay::On_PointEnter_Stage(const _uint& iEventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[iEventNum], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		Enable_Fade(m_pStageHighlights[iEventNum]);
	}
}

void CUI_MainPlay::On_PointStay_Stage(const _uint& iEventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[iEventNum], CTexture)->Get_CurTextureIndex();
	if (iTextureNum > 3)
	{
		CUser::Get_Instance()->Get_Cursor()->Set_Mouse(CUI_Cursor::Disable);
	}
}

void CUI_MainPlay::On_PointExit_Stage(const _uint& iEventNum)
{
	Disable_Fade(m_pStageHighlights[iEventNum]);
}

void CUI_MainPlay::On_PointDown_Stage(const _uint& iEventNum)
{
	_uint iTextureNum = GET_COMPONENT_FROM(m_pStageSelectBtn[iEventNum], CTexture)->Get_CurTextureIndex();
	if (iTextureNum <= 3)
	{
		_float4 vPos = m_pStageSelectBtn[iEventNum]->Get_Pos();

		if (iTextureNum == 0)
		{
			Enable_StageClickRect(vPos);

			m_eStage = Select_Stage::Test;
			m_pStageNameRect->Set_FontText(TEXT("모드 - 테스트"));
		}
		else if (iTextureNum == 1)
		{
			Enable_StageClickRect(vPos);
		}
		else if (iTextureNum == 2)
		{
			Enable_StageClickRect(vPos);
		}
		else if (iTextureNum == 3)
		{
			Enable_StageClickRect(vPos);

			m_eStage = Select_Stage::Training;
			m_pStageNameRect->Set_FontText(TEXT("모드 - 훈련소"));
		}
	}
}

void CUI_MainPlay::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pStageSelectRect, CShader)->CallBack_SetRawValues += bind(&CUI_MainPlay::Set_Shader_StageClickRect, this, placeholders::_1, "g_vColor");

	GET_COMPONENT_FROM(m_pStageHighlight, CShader)->CallBack_SetRawValues += bind(&CUI_MainPlay::Set_Shader_StageHighlight, this, placeholders::_1, "g_vColor");

	GET_COMPONENT_FROM(m_pBtnHightlight, CShader)->CallBack_SetRawValues += bind(&CUI_MainPlay::Set_Shader_BtnHighlight, this, placeholders::_1, "g_fValue");
}

void CUI_MainPlay::Bind_Btn()
{
	m_pPlayBtnUI[0]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Start, this, placeholders::_1);
	m_pPlayBtnUI[1]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Mode, this, placeholders::_1);

	m_pPlayBtnUI[0]->CallBack_PointExit += bind(&CUI_MainPlay::On_PointExit_Start, this, placeholders::_1);
	m_pPlayBtnUI[1]->CallBack_PointExit += bind(&CUI_MainPlay::On_PointExit_Mode, this, placeholders::_1);

	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnUI[i]->CallBack_PointEnter += bind(&CUI_MainPlay::On_PointEnter_PlayBtn, this, i);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pStageSelectBtn[i]->CallBack_PointEnter += bind(&CUI_MainPlay::On_PointEnter_Stage, this, i);
		m_pStageSelectBtn[i]->CallBack_PointStay += bind(&CUI_MainPlay::On_PointStay_Stage, this, i);
		m_pStageSelectBtn[i]->CallBack_PointExit += bind(&CUI_MainPlay::On_PointExit_Stage, this, i);

		m_pStageSelectBtn[i]->CallBack_PointDown += bind(&CUI_MainPlay::On_PointDown_Stage, this, i);
	}
}

void CUI_MainPlay::Set_FadeModeWindow()
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.3f;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.3f;

	GET_COMPONENT_FROM(m_pBG, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pEscKey, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pLine, CFader)->Get_FadeDesc() = tFadeDesc;

	GET_COMPONENT_FROM(m_pStageSelectRect, CFader)->Get_FadeDesc() = tFadeDesc;

	for (int i = 0; i < 4; ++i)
	{
		GET_COMPONENT_FROM(m_pStageSelectBtn[i], CFader)->Get_FadeDesc() = tFadeDesc;
		GET_COMPONENT_FROM(m_pStageHighlights[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}

	for (int i = 0; i < 2; ++i)
	{
		GET_COMPONENT_FROM(m_pLockBtn[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}
}

void CUI_MainPlay::SetActive_ModeWindow()
{
	if (!m_pBG->Is_Valid())
	{
		Enable_Fade(m_pBG);
		Enable_Fade(m_pLine);

		Enable_Fade(m_pTextModeSelect);
		Enable_Fade(m_pEscKey);

		Enable_Fade(m_pStageSelectRect);
		Enable_Fade(m_pSelectTextRect);

		for (int i = 0; i < 4; ++i)
		{
			Enable_Fade(m_pStageSelectBtn[i]);
		}

		for (int i = 0; i < 2; ++i)
		{
			Enable_Fade(m_pLockBtn[i]);
		}
	}
	else
	{
		Disable_Fade(m_pBG);
		Disable_Fade(m_pLine);

		Disable_Fade(m_pTextModeSelect);
		Disable_Fade(m_pEscKey);

		Disable_Fade(m_pStageSelectRect);
		Disable_Fade(m_pSelectTextRect);

		for (int i = 0; i < 4; ++i)
		{
			Disable_Fade(m_pStageSelectBtn[i]);
			Disable_Fade(m_pStageHighlights[i]);
		}

		for (int i = 0; i < 2; ++i)
		{
			Disable_Fade(m_pLockBtn[i]);
		}
	}
}

void CUI_MainPlay::Enable_StageHighlight(_float4 vPos)
{
	m_pStageHighlight->Set_Pos(vPos.x, vPos.y);

	ENABLE_GAMEOBJECT(m_pStageHighlight);
}

void CUI_MainPlay::Enable_StageClickRect(_float4 vPos)
{
	m_pStageSelectRect->Set_Pos(vPos.x, vPos.y);
	m_pSelectTextRect->Set_Pos(vPos.x - 65.f, vPos.y - 100.f);

	ENABLE_GAMEOBJECT(m_pStageSelectRect);
	ENABLE_GAMEOBJECT(m_pSelectTextRect);
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
	m_pTextModeSelect->Set_FontColor(_float4(0.f, 0.f, 0.f, 0.f));

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

	GET_COMPONENT_FROM(m_pPrototypeLock, CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Lock);

	for (int i = 0; i < 2; ++i)
	{
		m_pLockBtn[i] = m_pPrototypeLock->Clone();
	}

	CREATE_GAMEOBJECT(m_pPrototypeLock, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPrototypeLock);

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pLockBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pLockBtn[i]);

		m_pLockBtn[i]->Set_Sort(0.8f);
	}

	m_pLockBtn[0]->Set_Pos(m_pStageSelectBtn[1]->Get_PosX(), m_pStageSelectBtn[1]->Get_PosY());
	m_pLockBtn[1]->Set_Pos(m_pStageSelectBtn[2]->Get_PosX(), m_pStageSelectBtn[2]->Get_PosY());
}

void CUI_MainPlay::Create_StageHighlight()
{
	m_pStageHighlight = CUI_Object::Create();
	m_pStageHighlight->Set_Scale(216.f, 340.f);
	m_pStageHighlight->Set_Sort(0.8f);
	m_pStageHighlight->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_GradientSmall2.dds"));
	m_pStageHighlight->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));

	for (int i = 0; i < 4; ++i)
	{
		m_pStageHighlights[i] = m_pStageHighlight->Clone();

		_float fPosX = -360.f + (i * 240.f);
		m_pStageHighlights[i]->Set_PosX(fPosX);

		CREATE_GAMEOBJECT(m_pStageHighlights[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pStageHighlights[i]);
	}

	m_pStageSelectRect = CUI_Object::Create();
	m_pStageSelectRect->Set_Scale(230.f, 355.f);
	m_pStageSelectRect->Set_Sort(0.75f);
	m_pStageSelectRect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_2pxStrokeBox.png"));

	m_pStageSelectRect->Set_IsSlice(true);
	m_pStageSelectRect->Set_TextureSzie(_float2(32.f, 32.f));
	m_pStageSelectRect->Set_SliceRatio(_float4(0.1f, 0.1f, 0.8f, 0.8f));

	m_pStageSelectRect->Set_Pos(m_pStageSelectBtn[0]->Get_PosX(), m_pStageSelectBtn[0]->Get_PosY());

	CREATE_GAMEOBJECT(m_pStageHighlight, GROUP_UI);
	DELETE_GAMEOBJECT(m_pStageHighlight);

	CREATE_GAMEOBJECT(m_pStageSelectRect, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pStageSelectRect);
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

void CUI_MainPlay::Create_StageNameRect()
{
	m_pStageNameRect = CUI_Object::Create();
	m_pStageNameRect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_BoxFrame.png"));

	m_pStageNameRect->Set_Scale(90.f, 28.f);
	m_pStageNameRect->Set_Sort(0.91f);
	m_pStageNameRect->Set_Pos(-545.f, 220.f);

	m_pStageNameRect->Set_IsSlice(true);
	m_pStageNameRect->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));
	m_pStageNameRect->Set_TextureSzie(_float2(50.f, 50.f));

	m_pStageNameRect->Set_FontRender(true);
	m_pStageNameRect->Set_FontStyle(true);
	m_pStageNameRect->Set_FontOffset(-39.f, -11.f);
	m_pStageNameRect->Set_FontScale(0.2f);
	m_pStageNameRect->Set_FontColor(m_vFontColor);
	m_pStageNameRect->Set_FontText(TEXT("모드 - 테스트"));

	CREATE_GAMEOBJECT(m_pStageNameRect, GROUP_UI);
}

void CUI_MainPlay::Crerate_PlayBtnMouseEnterLine()
{
	m_pPlayBtnMouseEnterLine = CUI_Object::Create();

	m_pPlayBtnMouseEnterLine->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_3pxBoxWhite.dds"));

	m_pPlayBtnMouseEnterLine->Set_PosX(-590.f);
	m_pPlayBtnMouseEnterLine->Set_Scale(4.f);
	m_pPlayBtnMouseEnterLine->Set_Sort(0.91f);
	m_pPlayBtnMouseEnterLine->Set_Color(_float4(0.773f, 0.714f, 0.596f, 1.f));

	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnMouseEnterLineArr[i] = m_pPlayBtnMouseEnterLine->Clone();

		CREATE_GAMEOBJECT(m_pPlayBtnMouseEnterLineArr[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPlayBtnMouseEnterLineArr[i]);
	}

	CREATE_GAMEOBJECT(m_pPlayBtnMouseEnterLine, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPlayBtnMouseEnterLine);
}

void CUI_MainPlay::Create_SelectTextRect()
{
	m_pSelectTextRect = CUI_Object::Create();
	m_pSelectTextRect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_SaleTag.png"));

	m_pSelectTextRect->Set_Sort(0.8f);
	m_pSelectTextRect->Set_Scale(60.f, 30.f);
	m_pSelectTextRect->Set_Pos(-425.f, -100.f);

	m_pSelectTextRect->Set_IsSlice(true);
	m_pSelectTextRect->Set_TextureSzie(_float2(25.f, 25.f));
	m_pSelectTextRect->Set_SliceRatio(_float4(0.5f, 0.5f, 0.5f, 0.5f));

	m_pSelectTextRect->Set_FontRender(true);
	m_pSelectTextRect->Set_FontStyle(true);

	m_pSelectTextRect->Set_FontText(TEXT("선택 중"));
	m_pSelectTextRect->Set_FontOffset(-22.f, -10.f);
	m_pSelectTextRect->Set_FontScale(0.2f);
	m_pSelectTextRect->Set_FontColor(m_vFontColor);

	CREATE_GAMEOBJECT(m_pSelectTextRect, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pSelectTextRect);
}
