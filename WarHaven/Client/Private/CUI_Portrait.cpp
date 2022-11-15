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

CUI_Portrait::CUI_Portrait()
{
}

CUI_Portrait::~CUI_Portrait()
{
}

HRESULT CUI_Portrait::Initialize_Prototype()
{
	Read_UI("Port");

	Ready_Portrait();

	return S_OK;
}

HRESULT CUI_Portrait::Initialize()
{
	return S_OK;
}

HRESULT CUI_Portrait::Start()
{
	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_Portrait::Set_ShaderEffect(CShader* pShader, const char* constName)
{
	pShader->Set_RawValue("g_fValue", &m_fEffectValue, sizeof(_float));
}

void CUI_Portrait::Start_Portrait(_uint iIndex)
{
	for (int i = 0; i < 2; ++i)
	{
		GET_COMPONENT_FROM(m_arrPortraitUI[0][Port], CTexture)->Set_CurTextureIndex(iIndex);

		ENABLE_GAMEOBJECT(m_arrPortraitUI[0][i]);
	}
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

void CUI_Portrait::Set_HeroPort(HeroPortAnimType eState)
{
	m_bAbleHero = true;
	m_eHeroPortAnimType = eState;
	m_bIsHeroLerp = true;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	/*if (!m_bIsHero)
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
	}*/

	_float fEffectSpeed = fDT(0) * 5.f;
	m_fEffectValue -= fEffectSpeed;

	Change_UserPort();

	if (m_bAbleHero)
	{
		if (m_eHeroPortAnimType == Enable)
		{
			if (m_bIsHeroLerp)
			{
				_float fDuration = 0.3f;

				if (m_iHeroEndIdx > User)
				{
					for (int i = 0; i < Type_End; ++i)
					{
						ENABLE_GAMEOBJECT(m_arrPortraitUI[m_iHeroEndIdx][i]);

						if (i == Key)
						{
							continue;
						}

						if (i == Effect)
						{
							continue;
						}

						m_arrPortraitUI[m_iHeroEndIdx][i]->Lerp_ScaleX(0.f, 43.f, fDuration);
					}

					m_bIsHeroLerp = false;
				}
				else
				{
					m_eHeroPortAnimType = AnimEnd;
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
				_float fDuration = 0.3f;

				if (m_iHeroStartIdx < PortEnd)
				{
					for (int i = 0; i < Type_End; ++i)
					{
						if (i == Key)
						{
							continue;
						}

						if (i == Effect)
						{
							continue;
						}

						m_arrPortraitUI[m_iHeroStartIdx][i]->Lerp_ScaleX(43.f, 0.f, fDuration);
					}

					m_bIsHeroLerp = false;
				}
				else
				{
					m_eHeroPortAnimType = AnimEnd;
				}
			}
			else
			{
				_float4 vScale = m_arrPortraitUI[m_iHeroStartIdx][BG]->Get_Transform()->Get_Scale();
				if (vScale.x <= m_fMinValue)
				{
					DISABLE_GAMEOBJECT(m_arrPortraitUI[m_iHeroStartIdx][Key]);
					DISABLE_GAMEOBJECT(m_arrPortraitUI[m_iHeroStartIdx][Effect]);

					m_iHeroStartIdx++;
					m_bIsHeroLerp = true;
				}
			}
		}
	}
	else
	{
		m_eHeroPortAnimType = AnimEnd;
	}
}

void CUI_Portrait::Set_Pass()
{
	for (int i = 1; i < 5; ++i)
	{
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Effect], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_PortEffect);
	}
}

void CUI_Portrait::Bind_Shader()
{
	//GET_COMPONENT_FROM(m_arrPortraitUI[0][Effect], CShader)->CallBack_SetRawValues += bind(&CUI_Portrait::Set_ShaderEffect, this, placeholders::_1, "g_fValue");
}

void CUI_Portrait::Change_UserPort()
{
	if (m_bAbleRotationPort)
	{
		_float fDuration = 0.1f;

		_float4 vScale = m_arrPortraitUI[0][BG]->Get_Transform()->Get_Scale();
		CTexture* pTexture = GET_COMPONENT_FROM(m_arrPortraitUI[0][Port], CTexture);

		if (!m_bIsUserLerp)
		{
			m_bIsUserLerp = true;

			if (m_iRotationCount == 1)
			{
				PortSizeDown(fDuration);
			}
			else if (m_iRotationCount == 2)
			{
				pTexture->Set_CurTextureIndex(m_iCurPort);
				PortSizeUP(fDuration);
			}
			else if (m_iRotationCount == 3)
			{
				PortSizeDown(fDuration);
			}
			else if (m_iRotationCount == 4)
			{
				pTexture->Set_CurTextureIndex(m_iPrvPort);
				PortSizeUP(fDuration);
			}
			else if (m_iRotationCount == 5)
			{
				PortSizeDown(fDuration);
			}
			else if (m_iRotationCount == 6)
			{
				pTexture->Set_CurTextureIndex(m_iCurPort);
				PortSizeUP(fDuration);
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

void CUI_Portrait::PortSizeUP(_float fDuration)
{
	_float fRotSpeed = fDuration * (m_iRotationCount * 0.5f);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(0.f, 64.f, fRotSpeed);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(0.f, 63.f, fRotSpeed);
}

void CUI_Portrait::PortSizeDown(_float fDuration)
{
	_float fRotSpeed = 0.1f * (m_iRotationCount * 0.5f);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(64.f, 0.f, fRotSpeed);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(63.f, 0.f, fRotSpeed);
}

void CUI_Portrait::Enable_HeroPort()
{
	if (m_iPrvPort > 5)
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

					ENABLE_GAMEOBJECT(m_arrPortraitUI[m_iHeroEndIdx][j]);
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
	if (bIsHero)
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
	if (m_bAbleHero)
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
}

void CUI_Portrait::Ready_Portrait()
{
	// 원본 객체 배치
	m_Prototypes[BG] = m_pUIMap[TEXT("PortBG")];
	m_Prototypes[Port] = m_pUIMap[TEXT("Port")];
	m_Prototypes[Key] = m_pUIMap[TEXT("PortKey")];
	m_Prototypes[Effect] = m_pUIMap[TEXT("Port_Effect")];

	GET_COMPONENT_FROM(m_Prototypes[Port], CTexture)->Remove_Texture(0);

	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_06.dds"));
	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_13.dds"));

	GET_COMPONENT_FROM(m_Prototypes[BG], CTexture)->Add_Texture(TEXT("../Bin/R\esources/Textures/UI/Circle/T_RoundPortraitBGSmall.dds"));

	Read_Texture(m_Prototypes[Port], "/HUD/Portrait", "Class");
	Read_Texture(m_Prototypes[Key], "/KeyIcon/Keyboard", "Key");

	// 원본 객체 복사
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			m_arrPortraitUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	for (_uint i = 0; i < Type_End; ++i)
	{
		// 원본 객체 생성 후 삭제
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);

		// 클론 생성 후 비활성화
		CREATE_GAMEOBJECT(m_arrPortraitUI[0][i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_arrPortraitUI[0][i]);
	}

	// 유저 포트레이트의 필요없는 객체 삭제
	DELETE_GAMEOBJECT(m_arrPortraitUI[0][Key]);
	DELETE_GAMEOBJECT(m_arrPortraitUI[0][Effect]);

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

			// 클론 객체 생성 후 비활성화
			CREATE_GAMEOBJECT(m_arrPortraitUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrPortraitUI[i][j]);
		}
	}
}