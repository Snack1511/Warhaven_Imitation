#include "stdafx.h"
#include "stdafx.h"
#include "CUI_Portrait.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "Transform.h"
#include "CShader.h"
#include "Renderer.h"

CUI_Portrait::CUI_Portrait()
{
}

CUI_Portrait::~CUI_Portrait()
{
}

HRESULT CUI_Portrait::Initialize_Prototype()
{
	Read_UI("Port");

	m_Prototypes[BG] = m_pUIMap[TEXT("PortBG")];
	m_Prototypes[Port] = m_pUIMap[TEXT("Port")];
	m_Prototypes[Key] = m_pUIMap[TEXT("PortKey")];
	m_Prototypes[Effect] = m_pUIMap[TEXT("Port_Effect")];

	GET_COMPONENT_FROM(m_Prototypes[Port], CTexture)->Remove_Texture(0);

	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_06.dds"));
	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_13.dds"));

	GET_COMPONENT_FROM(m_Prototypes[BG], CTexture)
		->Add_Texture(TEXT("../Bin/R\esources/Textures/UI/Circle/T_RoundPortraitBGSmall.dds"));

	Read_Texture(m_Prototypes[Port], "/HUD/Portrait", "Class");
	Read_Texture(m_Prototypes[Key], "/KeyIcon/Keyboard", "Key");

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			m_arrPortraitUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	return S_OK;
}

HRESULT CUI_Portrait::Initialize()
{
	return S_OK;
}

HRESULT CUI_Portrait::Start()
{
	Enable_UserPortrait();
	Enable_HeroPortrait();

	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_Portrait::Set_ShaderEffect(CShader* pShader, const char* constName)
{
	pShader->Set_RawValue("g_fValue", &m_fEffectValue, sizeof(_float));
}

void CUI_Portrait::Set_Portrait(_uint iIndex)
{
	m_bIsHeroLerp = true;
	m_iHeroStartIdx = Hero1;
	m_iHeroEndIdx = Hero4;
	m_iHeroActiveCount = 0;

	m_iPrvPort = m_iCurPort;
	m_iCurPort = iIndex;

	m_bAbleRotationPort = true;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	if (!m_bIsHero)
	{
		if (KEY(T, TAP))
		{
			static int iIndex = 0;
			iIndex++;
			if (iIndex >= 6)
				iIndex = 0;

			Set_Portrait(iIndex);
		}

		if (KEY(NUM1, TAP))
		{
			m_bIsHero = true;

			Set_Portrait(6);
		}
		else if (KEY(NUM2, TAP))
		{
			m_bIsHero = true;

			Set_Portrait(7);
		}
		else if (KEY(NUM3, TAP))
		{
			m_bIsHero = true;

			Set_Portrait(8);
		}
		else if (KEY(NUM4, TAP))
		{
			m_bIsHero = true;

			Set_Portrait(9);
		}
	}
	else
	{
		if (KEY(NUM1, TAP))
		{
			m_bIsHero = false;

			Set_Portrait(m_iPrvPort);
		}
	}

	_float fEffectSpeed = fDT(0) * 5.f;
	m_fEffectValue -= fEffectSpeed;

	Change_UserPort();
	Enable_HeroPort();
	Disable_HeroPort();
}

void CUI_Portrait::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Portrait::Enable_UserPortrait()
{
	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);

		CREATE_GAMEOBJECT(m_arrPortraitUI[0][i], GROUP_UI);
	}

	DELETE_GAMEOBJECT(m_arrPortraitUI[0][Key]);
	DELETE_GAMEOBJECT(m_arrPortraitUI[0][Effect]);
}

void CUI_Portrait::Enable_HeroPortrait()
{
	for (int i = 1; i < 5; ++i)
	{
		float fPosX = 260.f + (i * 55.f);
		_uint iPortIndex = 5 + i;
		_uint iKeyIndex = 32 + i;

		GET_COMPONENT_FROM(m_arrPortraitUI[i][BG], CTexture)->Set_CurTextureIndex(1);
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Port], CTexture)->Set_CurTextureIndex(iPortIndex);
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Key], CTexture)->Set_CurTextureIndex(iKeyIndex);

		m_arrPortraitUI[i][BG]->Set_Sort(0.1f);
		m_arrPortraitUI[i][Effect]->Set_Sort(0.2f);

		for (int j = 0; j < Type_End; ++j)
		{
			m_arrPortraitUI[i][j]->Set_Pos(fPosX, -230.f);
			m_arrPortraitUI[i][j]->Set_Scale(43.f);

			if (j == Key)
			{
				m_arrPortraitUI[i][j]->Set_Pos(fPosX, -195.f);
				m_arrPortraitUI[i][j]->Set_Scale(15.f);
			}

			if (j == Effect)
			{
				m_arrPortraitUI[i][j]->Set_Scale(55.f);
			}

			CREATE_GAMEOBJECT(m_arrPortraitUI[i][j], GROUP_UI);
		}
	}
}

void CUI_Portrait::Set_Pass()
{
	for (int i = 0; i < 5; ++i)
	{
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Effect], CRenderer)
			->Set_Pass(VTXTEX_PASS_UI_PortEffect);
	}
}

void CUI_Portrait::Bind_Shader()
{
	GET_COMPONENT_FROM(m_arrPortraitUI[0][Effect], CShader)
		->CallBack_SetRawValues += bind(&CUI_Portrait::Set_ShaderEffect, this, placeholders::_1, "g_fValue");
}

void CUI_Portrait::Change_UserPort()
{
	if (m_bAbleRotationPort)
	{
		_float4 vScale = m_arrPortraitUI[0][BG]->Get_Transform()->Get_Scale();
		CTexture* pTexture = GET_COMPONENT_FROM(m_arrPortraitUI[0][Port], CTexture);

		if (!m_bIsUserLerp)
		{
			m_bIsUserLerp = true;

			if (m_iRotationCount == 1)
			{
				PortSizeDown();
			}
			else if (m_iRotationCount == 2)
			{
				pTexture->Set_CurTextureIndex(m_iCurPort);
				PortSizeUP();
			}
			else if (m_iRotationCount == 3)
			{
				PortSizeDown();
			}
			else if (m_iRotationCount == 4)
			{
				pTexture->Set_CurTextureIndex(m_iPrvPort);
				PortSizeUP();
			}
			else if (m_iRotationCount == 5)
			{
				PortSizeDown();
			}
			else if (m_iRotationCount == 6)
			{
				pTexture->Set_CurTextureIndex(m_iCurPort);
				PortSizeUP();
			}
		}
		else
		{
			if (vScale.x <= m_fMinValue)
			{
				m_bIsUserLerp = false;
				m_iRotationCount++;
			}
			else if (vScale.x >= 64.f)
			{
				m_bIsUserLerp = false;
				m_iRotationCount++;
			}
		}

		if (m_iRotationCount > 6)
		{
			m_iRotationCount = 0;
			m_bAbleRotationPort = false;
		}
	}
}

void CUI_Portrait::Enable_HeroPort()
{
	if (m_iPrvPort >= 6)
	{
		_float fDuration = 0.3f;

		if (m_bIsHeroLerp)
		{
			for (m_iHeroEndIdx; m_iHeroEndIdx >= Hero1;)
			{
				for (int j = 0; j < Type_End; ++j)
				{
					if (j == Key)
					{
						// 페이드아웃만
						continue;
					}

					if (j == Effect)
					{
						// 페이드아웃만
						continue;
					}

					m_arrPortraitUI[m_iHeroEndIdx][j]->Lerp_ScaleX(0.f, 43.f, fDuration);
				}

				m_bIsHeroLerp = false;
				break;
			}
		}
		else
		{
			Enable_HeroLerp(true, fDuration);
		}
	}
}

void CUI_Portrait::Disable_HeroPort()
{
	if (m_bIsHero)
	{
		if (m_iPrvPort < 6)
		{
			_float fDuration = 0.3f;

			if (m_bIsHeroLerp)
			{
				for (m_iHeroStartIdx; m_iHeroStartIdx < PortEnd;)
				{
					for (int j = 0; j < Type_End; ++j)
					{
						if (j == Key)
						{
							// 페이드아웃만
							continue;
						}

						if (j == Effect)
						{
							// 페이드아웃만
							continue;
						}

						m_arrPortraitUI[m_iHeroStartIdx][j]->Lerp_ScaleX(43.f, 0.f, fDuration);
					}

					m_bIsHeroLerp = false;
					break;
				}
			}
			else
			{
				Enable_HeroLerp(false, fDuration);
			}
		}
	}
}

void CUI_Portrait::Enable_HeroLerp(_bool value, _float fDuration)
{
	if (m_iHeroActiveCount < 3)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > fDuration)
		{
			if (value == false)
			{
				m_iHeroStartIdx++;

				m_bIsHeroLerp = true;
				m_fAccTime = 0.f;

				m_iHeroActiveCount++;
			}
			else
			{
				m_iHeroEndIdx--;

				m_bIsHeroLerp = true;
				m_fAccTime = 0.f;

				m_iHeroActiveCount++;
			}
		}
	}
}

void CUI_Portrait::PortSizeUP()
{
	_float fDuration = 0.1f * (m_iRotationCount * 0.5f);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(0.f, 64.f, fDuration);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(0.f, 63.f, fDuration);
}

void CUI_Portrait::PortSizeDown()
{
	_float fDuration = 0.1f * (m_iRotationCount * 0.5f);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(64.f, 0.f, fDuration);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(63.f, 0.f, fDuration);
}
