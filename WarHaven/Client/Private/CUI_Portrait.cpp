#include "stdafx.h"
#include "stdafx.h"
#include "CUI_Portrait.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "Transform.h"
#include "CShader.h"
#include "CUI_Renderer.h"
#include "CFader.h"

CUI_Portrait::CUI_Portrait()
{
}

CUI_Portrait::~CUI_Portrait()
{
}

HRESULT CUI_Portrait::Initialize_Prototype()
{
	Create_UserPort();
	Create_HeroPort();

	return S_OK;
}

HRESULT CUI_Portrait::Initialize()
{
	return S_OK;
}

HRESULT CUI_Portrait::Start()
{
	__super::Start();

	Set_Pass();

	return S_OK;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	Change_UserPort();
	Active_HeroPort();
}

void CUI_Portrait::Set_UserPort(_uint iClass)
{
	m_iPrvClass = m_iCurClass;
	m_iCurClass = iClass;

	if (m_iPrvClass != m_iCurClass)
	{
		m_bChangeUserPort = true;
	}
}

void CUI_Portrait::Set_HeroPort(_uint iType)
{
	m_bAbleHero = true;
	m_eAcitveType = (HeroPortActive)iType;
}

void CUI_Portrait::SetActive_UserPort(_bool value)
{
	for (int i = 0; i < UP_Effect; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pUserPortrait[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pUserPortrait[i]);
		}
	}
}

void CUI_Portrait::SetActive_HeroPort(_bool value)
{
	for (int i = 0; i < HP_End; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (value == true)
			{
				if (!m_bAbleHero)
					return;

				ENABLE_GAMEOBJECT(m_pArrHeroPortrait[i][j]);
			}
			else
			{
				DISABLE_GAMEOBJECT(m_pArrHeroPortrait[i][j]);
			}
		}
	}
}

void CUI_Portrait::Create_UserPort()
{
	for (int i = 0; i < UP_End; ++i)
	{
		m_pUserPortrait[i] = CUI_Object::Create();

		m_pUserPortrait[i]->Set_Pos(-580.f, -280.f);
		m_pUserPortrait[i]->Set_Scale(64.f);

		if (i == UP_BG)
		{
			m_pUserPortrait[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBG.dds"));

			m_pUserPortrait[i]->Set_Sort(0.5f);
		}
		else if (i == UP_Port)
		{
			m_pUserPortTexture = GET_COMPONENT_FROM(m_pUserPortrait[i], CTexture);
			m_pUserPortTexture->Remove_Texture(0);
			Read_Texture(m_pUserPortrait[i], "/HUD/Portrait", "Class");

			m_pUserPortrait[i]->Set_Sort(0.49f);
		}
		else if (i == UP_Effect)
		{
			m_pUserPortrait[i]->Set_Scale(8.f);
			m_pUserPortrait[i]->Set_Sort(0.48f);
		}

		CREATE_GAMEOBJECT(m_pUserPortrait[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pUserPortrait[i]);
	}
}

void CUI_Portrait::Change_UserPort()
{
	if (m_bChangeUserPort)
	{
		if (!m_bDoScaleUserPort)
		{
			m_bDoScaleUserPort = true;

			if (m_iChangeUserPortCount % 2 == 1)
			{
				DoScale_UserPort(false);
			}
			else
			{
				SetTexture_UserPort();
				DoScale_UserPort(true);
			}
		}
		else
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > m_fLerpUserPortDuration)
			{
				m_fAccTime = 0.f;
				m_bDoScaleUserPort = false;

				m_iChangeUserPortCount++;
				if (m_iChangeUserPortCount > 6)
				{
					m_iChangeUserPortCount = 1;
					m_bChangeUserPort = false;
				}
			}
		}
	}
}

void CUI_Portrait::SetTexture_UserPort()
{
	if (m_iChangeUserPortCount == 4)
	{
		m_pUserPortTexture->Set_CurTextureIndex(m_iPrvClass);
	}
	else
	{
		m_pUserPortTexture->Set_CurTextureIndex(m_iCurClass);
	}
}

void CUI_Portrait::DoScale_UserPort(_bool value)
{
	m_fLerpUserPortDuration = 0.1f * (m_iChangeUserPortCount * 0.5f);

	Set_FadeUserPort(m_fLerpUserPortDuration);

	for (int i = 0; i < UP_Effect; ++i)
	{
		if (value == true)
		{
			m_pUserPortrait[i]->Lerp_ScaleX(0.f, 64.f, m_fLerpUserPortDuration);

			if (i == UP_Port)
			{
				Enable_Fade(m_pUserPortrait[i], m_fLerpUserPortDuration);
			}
		}
		else
		{
			m_pUserPortrait[i]->Lerp_ScaleX(64.f, 0.f, m_fLerpUserPortDuration);

			if (i == UP_Port)
			{
				Disable_Fade(m_pUserPortrait[i], m_fLerpUserPortDuration);
			}
		}
	}
}

void CUI_Portrait::Set_FadeUserPort(_float fSpeed)
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = fSpeed;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = fSpeed;

	GET_COMPONENT_FROM(m_pUserPortrait[UP_Port], CFader)->Get_FadeDesc() = tFadeDesc;
}

void CUI_Portrait::Create_HeroPort()
{
	for (int i = 0; i < HP_End; ++i)
	{
		m_pHeroPortrait[i] = CUI_Object::Create();

		m_pHeroPortrait[i]->Set_PosY(-230.f);

		Set_FadeHeroPort(m_pHeroPortrait[i]);

		if (i == HP_BG)
		{
			m_pHeroPortrait[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBGSmall.dds"));

			m_pHeroPortrait[i]->Set_Scale(43.f);
			m_pHeroPortrait[i]->Set_Sort(0.5f);
		}
		else if (i == HP_Port)
		{
			GET_COMPONENT_FROM(m_pHeroPortrait[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pHeroPortrait[i], "/HUD/Portrait", "Hero");

			m_pHeroPortrait[i]->Set_Scale(43.f);
			m_pHeroPortrait[i]->Set_Sort(0.49f);
		}
		else if (i == HP_Effect)
		{
			m_pHeroPortrait[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Glow_08.dds"));
			m_pHeroPortrait[i]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_06.dds"));

			m_pHeroPortrait[i]->Set_Scale(65.f);
			m_pHeroPortrait[i]->Set_Sort(0.51f);

			m_pHeroPortrait[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);
		}
		else if (i == HP_Key)
		{
			GET_COMPONENT_FROM(m_pHeroPortrait[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pHeroPortrait[i], "/KeyIcon/Keyboard/Black", "Num");

			m_pHeroPortrait[i]->Set_PosY(-195.f);
			m_pHeroPortrait[i]->Set_Scale(15.f);
			m_pHeroPortrait[i]->Set_Sort(0.5f);
		}

		CREATE_GAMEOBJECT(m_pHeroPortrait[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pHeroPortrait[i]);

		for (int j = 0; j < 4; ++j)
		{
			m_pArrHeroPortrait[i][j] = m_pHeroPortrait[i]->Clone();

			float fPosX = 305.f + (j * 55.f);
			m_pArrHeroPortrait[i][j]->Set_PosX(fPosX);

			if (i == HP_Port)
			{
				GET_COMPONENT_FROM(m_pArrHeroPortrait[i][j], CTexture)->Set_CurTextureIndex(j);
			}
			else if (i == HP_Key)
			{
				GET_COMPONENT_FROM(m_pArrHeroPortrait[i][j], CTexture)->Set_CurTextureIndex(j + 1);
			}

			CREATE_GAMEOBJECT(m_pArrHeroPortrait[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrHeroPortrait[i][j]);
		}
	}
}

void CUI_Portrait::Set_FadeHeroPort(CUI_Object* pUI)
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

	GET_COMPONENT_FROM(pUI, CFader)->Get_FadeDesc() = tFadeDesc;
}

void CUI_Portrait::Set_Pass()
{
	for (int i = 0; i < 4; ++i)
	{
		GET_COMPONENT_FROM(m_pArrHeroPortrait[HP_Effect][i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_PortEffect);
	}
}

void CUI_Portrait::Active_HeroPort()
{
	if (m_bAbleHero)
	{
		_float fDuration = 0.1f;
		m_fAccTime += fDT(0);

		if (m_eAcitveType == Enable)
		{
			if (!m_bLerpHeroPort)
			{
				m_bLerpHeroPort = true;

				for (int i = 0; i < HP_End; ++i)
				{
					if (i < HP_Effect)
					{
						Enable_Fade(m_pArrHeroPortrait[i][m_iLastHeroPort], fDuration);
						m_pArrHeroPortrait[i][m_iLastHeroPort]->Lerp_ScaleX(0.f, 43.f, fDuration);
					}
					else
					{
						Enable_Fade(m_pArrHeroPortrait[i][m_iLastHeroPort], fDuration);
					}
				}
			}

			if (m_fAccTime > fDuration)
			{
				m_fAccTime = 0.f;
				m_bLerpHeroPort = false;

				m_iLastHeroPort--;
				if (m_iLastHeroPort > 3)
				{
					m_iLastHeroPort = 3;
					m_bAbleHero = false;
				}
			}
		}
		else
		{
			if (!m_bLerpHeroPort)
			{
				m_bLerpHeroPort = true;

				for (int i = 0; i < HP_End; ++i)
				{
					if (i < HP_Effect)
					{
						Disable_Fade(m_pArrHeroPortrait[i][m_iFirstHeroPort], fDuration);
						m_pArrHeroPortrait[i][m_iFirstHeroPort]->Lerp_ScaleX(43.f, 0.f, fDuration);
					}
					else
					{
						Disable_Fade(m_pArrHeroPortrait[i][m_iFirstHeroPort], fDuration);
					}
				}
			}

			if (m_fAccTime > fDuration)
			{
				m_fAccTime = 0.f;
				m_bLerpHeroPort = false;

				m_iFirstHeroPort++;
				if (m_iFirstHeroPort > 3)
				{
					m_iFirstHeroPort = 0;
					m_bAbleHero = false;
				}
			}
		}
	}
}

void CUI_Portrait::OnEnable()
{
	__super::OnEnable();

	SetActive_UserPort(true);
	SetActive_HeroPort(true);
}

void CUI_Portrait::OnDisable()
{
	__super::OnDisable();

	SetActive_UserPort(false);
	SetActive_HeroPort(false);
}