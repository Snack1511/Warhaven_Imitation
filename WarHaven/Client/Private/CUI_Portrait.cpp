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

	Set_FadeHeroPort();

	SetActive_HeroPort(true);

	return S_OK;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	Change_UserPort();

	if (KEY(Z, TAP))
	{
		m_bAbleHero = true;
		m_iLastHeroPort = 4;
	}

	if (m_bAbleHero)
	{
		cout << m_iLastHeroPort << endl;

		_float fDuration = 0.1f;
		m_fAccTime += fDT(0);

		if (m_eAcitveType == Enable)
		{
			if (!m_bLerpHeroPort)
			{
				m_bLerpHeroPort = true;

				for (int i = 0; i < HP_End; ++i)
				{
					if (i == HP_Effect)
					{
						Enable_Fade(m_pArrHeroPortrait[i][m_iLastHeroPort], fDuration);
						continue;
					}
					else if (i == HP_Key)
					{
						Enable_Fade(m_pArrHeroPortrait[i][m_iLastHeroPort], fDuration);
						continue;
					}

					m_pArrHeroPortrait[i][m_iLastHeroPort]->DoScaleX(-43.f, fDuration);
					Enable_Fade(m_pArrHeroPortrait[i][m_iLastHeroPort], fDuration);
				}
			}
		}

		if (m_fAccTime > fDuration)
		{
			m_fAccTime = 0.f;
			m_bLerpHeroPort = false;

			m_iLastHeroPort--;
			if (m_iLastHeroPort == 0)
			{
				m_iLastHeroPort = 4;
				m_bAbleHero = false;
			}
		}
	}

	/*if (m_bAbleHero)
	{
		if (m_eHeroPortAnimType == Enable)
		{
			if (m_bIsHeroLerp)
			{
				if (m_iHeroEndIdx > User)
				{
					for (int i = 0; i < Type_End; ++i)
					{
						m_arrPortraitUI[m_iHeroEndIdx][i]->Lerp_ScaleX(0.f, 43.f, fDuration);
						Enable_Fade(m_arrPortraitUI[m_iHeroEndIdx][i], fDuration);
					}

					m_bIsHeroLerp = false;
				}
			}
			else
			{
				_float4 vScale = m_arrPortraitUI[m_iHeroEndIdx][BG]->Get_Transform()->Get_Scale();
				if (vScale.x >= 43.f)
				{
					m_iHeroEndIdx--;
					m_bIsHeroLerp = true;
				}
			}
		}
		else if (m_eHeroPortAnimType == Disable)
		{
			if (m_bIsHeroLerp)
			{
				if (m_iHeroStartIdx < PortEnd)
				{
					for (int i = 0; i < Type_End; ++i)
					{
						if (i == Key)
						{
							Disable_Fade(m_arrPortraitUI[m_iHeroStartIdx][i], fDuration);
							continue;
						}

						if (i == Effect)
						{
							Disable_Fade(m_arrPortraitUI[m_iHeroStartIdx][i], fDuration);
							continue;
						}

						m_arrPortraitUI[m_iHeroStartIdx][i]->Lerp_ScaleX(43.f, 0.f, fDuration);
						Disable_Fade(m_arrPortraitUI[m_iHeroStartIdx][i], fDuration);
					}

					m_bIsHeroLerp = false;
				}
			}
			else
			{
				_float4 vScale = m_arrPortraitUI[m_iHeroStartIdx][BG]->Get_Transform()->Get_Scale();
				if (vScale.x <= m_fMinValue)
				{
					m_iHeroStartIdx++;
					m_bIsHeroLerp = true;
				}
			}
		}
	}
	else
	{
		m_eHeroPortAnimType = AnimEnd;
	}*/
}

void CUI_Portrait::Set_UserPort(_uint iClass)
{
	m_iPrvClass = m_iCurClass;
	m_iCurClass = iClass;

	cout << m_iPrvClass << endl;
	cout << m_iCurClass << endl;

	if (m_iPrvClass != m_iCurClass)
	{
		m_bChangeUserPort = true;
	}
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
			// m_pHeroPortrait[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Glow_08.dds"));

			m_pHeroPortrait[i]->Set_Scale(65.f);
			m_pHeroPortrait[i]->Set_Sort(0.51f);
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

			float fPosX = 260.f + (j * 55.f);
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

void CUI_Portrait::Set_FadeHeroPort()
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

	for (int i = 0; i < HP_End; ++i)
	{
		GET_COMPONENT_FROM(m_pHeroPortrait[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}
}

void CUI_Portrait::Set_ShaderEffect(CShader* pShader, const char* constName)
{
	//pShader->Set_RawValue("g_fValue", &m_fEffectValue, sizeof(_float));
}

//void CUI_Portrait::Set_HeroPort()
//{
//	//m_bAbleHero = true;
//	//m_eHeroPortAnimType = eState;
//	//m_bIsHeroLerp = true;
//}

void CUI_Portrait::Set_Pass()
{
	/*for (int i = 1; i < 5; ++i)
	{
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Effect], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_PortEffect);
	}*/
}

void CUI_Portrait::Bind_Shader()
{
	/*for (int i = 1; i < 5; ++i)
	{
		m_arrPortraitUI[i][Effect]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

		GET_COMPONENT_FROM(m_arrPortraitUI[i][Effect], CShader)->CallBack_SetRawValues += bind(&CUI_Portrait::Set_ShaderEffect, this, placeholders::_1, "g_fValue");
	}*/
}

void CUI_Portrait::Active_HeroPort(HeroPortActive eType)
{
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