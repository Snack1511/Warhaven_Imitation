#include "CUI_Main.h"
#include "GameInstance.h"
#include "CUI_Renderer.h"
#include "Texture.h"
#include "CShader.h"

#include "CUI_MainPlay.h"
#include "CUI_MainMode.h"

#include "CUI_Object.h"
#include "CButton.h"

CUI_Main::CUI_Main()
{
}

CUI_Main::~CUI_Main()
{
}

HRESULT CUI_Main::Initialize_Prototype()
{
	// 윈도우 준비
	m_Prototypes[Play] = CUI_MainPlay::Create();

	Ready_MainUI();

	Create_BtnHighlight();

	return S_OK;
}

HRESULT CUI_Main::Initialize()
{
	return S_OK;
}

HRESULT CUI_Main::Start()
{
	Enable_MainUI();
	Enable_MainWindow();

	GET_COMPONENT_FROM(m_pBtnHighlight, CShader)
		->CallBack_SetRawValues += bind(&CUI_Main::Set_Shader_BtnHighlight, this, placeholders::_1, "g_fValue");

	for (int i = 0; i < 3; ++i)
	{
		m_pTopBtn[i]->CallBack_PointEnter += bind(&CUI_Main::On_PointEnter_TopBtn, this, placeholders::_1);
		m_pTopBtn[i]->CallBack_PointExit += bind(&CUI_Main::On_PointExit_TopBtn, this, placeholders::_1);

		m_pTopBtn[i]->CallBack_PointDown += bind(&CUI_Main::On_PointDown_TopBtn, this, placeholders::_1);
	}

	SetActive_Window(m_eWindow);

	__super::Start();

	return S_OK;
}

void CUI_Main::My_Tick()
{
}

void CUI_Main::Set_Shader_BtnHighlight(CShader* pShader, const char* pConstName)
{
	if (m_pBtnHighlight->Is_Valid())
	{
		m_fAccTime += fDT(0) * 0.25f;
		pShader->Set_RawValue("g_fValue", &m_fAccTime, sizeof(_float));
	}
}

void CUI_Main::On_PointEnter_TopBtn(const _uint& iEventNum)
{
	for (int i = 0; i < 3; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pTopBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			pTarget->Set_FontColor(_float4(1.f, 1.f, 1.f, 1.f));
		}
	}
}

void CUI_Main::On_PointExit_TopBtn(const _uint& iEventNum)
{
	for (int i = 0; i < 3; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pTopBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			if (!pTarget->Get_IsClick())
			{
				pTarget->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));
			}
		}
	}
}

void CUI_Main::On_PointDown_TopBtn(const _uint& iEventNum)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pTopBtn[i]->Set_IsClick(false);
		m_pTopBtn[i]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pTopBtn[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			pTarget->Set_IsClick(true);
			pTarget->Set_FontColor(_float4(1.f, 1.f, 1.f, 1.f));

			_float4 vPos = pTarget->Get_Pos();
			m_pBtnHighlight->Set_Pos(vPos.x, vPos.y);

			ENABLE_GAMEOBJECT(m_pBtnHighlight);
		}
	}
}

void CUI_Main::SetActive_Window(WindowType eWindow)
{
	m_eWindow = eWindow;

	for (int i = 0; i < WindowType::TypeEnd; ++i)
	{
		//DISABLE_GAMEOBJECT(m_Prototypes[i]);
	}

	ENABLE_GAMEOBJECT(m_Prototypes[eWindow]);
}

void CUI_Main::Ready_MainUI()
{
	Read_UI("Lobby");

	m_pPrototypeUI[Btn] = m_pUIMap[TEXT("Lobby_Btn")];
	m_pPrototypeUI[Key] = m_pUIMap[TEXT("Lobby_Key")];
	m_pPrototypeUI[Goods] = m_pUIMap[TEXT("Lobby_Goods")];

	for (int i = 0; i < MainEnd; ++i)
	{
		m_pPrototypeUI[i]->Set_Sort(0.9f);
	}

	GET_COMPONENT_FROM(m_pPrototypeUI[Btn], CTexture)->Remove_Texture(0);

	GET_COMPONENT_FROM(m_pPrototypeUI[Key], CTexture)
		->Add_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_BlackEKeyIcon.dds"));

	for (int i = 0; i < 3; ++i)
	{
		m_pTopBtn[i] = m_pPrototypeUI[Btn]->Clone();
		m_pGoodsUI[i] = m_pPrototypeUI[Goods]->Clone();
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pKeyUI[i] = m_pPrototypeUI[Key]->Clone();
	}
}

void CUI_Main::Create_BtnHighlight()
{
	m_pBtnHighlight = CUI_Object::Create();

	m_pBtnHighlight->Set_Scale(90.f, 120.f);
	m_pBtnHighlight->Set_Sort(0.95f);

	m_pBtnHighlight->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_BGMainTab.dds"));
	m_pBtnHighlight->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_SelectPT3.png"));
	m_pBtnHighlight->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_soft_smoke.dds"));

	GET_COMPONENT_FROM(m_pBtnHighlight, CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_LobbyEffect);

	CREATE_GAMEOBJECT(m_pBtnHighlight, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBtnHighlight);
}

void CUI_Main::Enable_MainUI()
{
	for (int i = 0; i < 3; ++i)
	{
		CREATE_GAMEOBJECT(m_pPrototypeUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pPrototypeUI[i]);
		m_pPrototypeUI[i] = nullptr;

		CREATE_GAMEOBJECT(m_pTopBtn[i], GROUP_UI);
		CREATE_GAMEOBJECT(m_pGoodsUI[i], GROUP_UI);

		_float fPosX = -530.f + (i * 95.f);
		m_pTopBtn[i]->Set_PosX(fPosX);
		m_pTopBtn[i]->Set_Sort(0.9f);

		m_pTopBtn[i]->Set_FontRender(true);
		m_pTopBtn[i]->Set_FontStyle(true);
		m_pTopBtn[i]->Set_FontScale(0.4f);
		m_pTopBtn[i]->Set_FontOffset(-40.f, -22.f);
		m_pTopBtn[i]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

		m_pGoodsUI[i]->Set_Sort(0.8f);
	}

	m_pTopBtn[1]->Set_FontOffset(-28.5f, -22.f);

	m_pTopBtn[0]->Set_FontText(TEXT("플레이"));
	m_pTopBtn[1]->Set_FontText(TEXT("병영"));
	m_pTopBtn[2]->Set_FontText(TEXT("프로필"));

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pKeyUI[i], GROUP_UI);

		GET_COMPONENT_FROM(m_pKeyUI[i], CTexture)->Set_CurTextureIndex(i);

		_float fPosX = -600.f + (i * 340.f);
		m_pKeyUI[i]->Set_PosX(fPosX);

		m_pKeyUI[i]->Set_Sort(0.95f);
	}
}

void CUI_Main::Enable_MainWindow()
{
	CREATE_GAMEOBJECT(m_Prototypes[Play], GROUP_UI);
}
