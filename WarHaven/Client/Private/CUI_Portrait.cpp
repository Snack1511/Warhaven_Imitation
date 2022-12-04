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

	Set_FadeHeroPort();

	__super::Start();

	return S_OK;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	m_fEffectValue -= fDT(0) * 0.1f;

	Change_UserPort();

	/*if (m_bAbleRotationPort)
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
	}*/

	// 영웅 변신 상태일 때
	if (m_bAbleHero)
	{
		_float fDuration = 0.1f;
		if (m_eHeroPortAnimType == Enable)
		{
			if (m_bIsHeroLerp)
			{
				if (m_iHeroEndIdx > User)
				{
					for (int i = 0; i < Type_End; ++i)
					{
						if (i == Key)
						{
							Enable_Fade(m_arrPortraitUI[m_iHeroEndIdx][i], fDuration);
							continue;
						}

						if (i == Effect)
						{
							Enable_Fade(m_arrPortraitUI[m_iHeroEndIdx][i], fDuration);
							continue;
						}

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
	}
}

void CUI_Portrait::Set_UserPort(_uint iClass)
{
	m_iPrvClass = m_iCurClass;
	m_iCurClass = iClass;

	// 유저 포트레이트가 바뀔 때
	if (m_iPrvClass != m_iCurClass)
	{
		if (iClass <= ENGINEER)
		{
			m_pUserPortTexture->Set_CurTextureIndex(iClass);
		}
		else if (iClass > ENGINEER)
		{
			m_bChangeUserPort = true;
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
			m_pUserPortrait[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/T_RoundPortraitBG.dds"));
		}
		else if (i == UP_Port)
		{
			m_pUserPortTexture = GET_COMPONENT_FROM(m_pUserPortrait[i], CTexture);
			m_pUserPortTexture->Remove_Texture(0);
			Read_Texture(m_pUserPortrait[i], "/HUD/Portrait", "Class");
		}
		else if (i == UP_Effect)
		{

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

			if (m_iChangeUserPortCount < 7)
			{
				if (m_iChangeUserPortCount % 2 == 0)
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
				m_iChangeUserPortCount = 0;
				m_bChangeUserPort = false;
			}
		}
		else
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > m_fDoScaleUserPortDuration)
			{
				m_fAccTime = 0.f;
				m_bDoScaleUserPort = false;
			}
		}
	}
}

void CUI_Portrait::SetTexture_UserPort()
{
	if (m_iChangeUserPortCount == 3)
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
	m_fDoScaleUserPortDuration = 0.1f * (m_iChangeUserPortCount * 0.5f);

	for (int i = 0; i < UP_Effect; ++i)
	{
		if (value == true)
		{
			m_pUserPortrait[i]->DoScaleX(64.f, m_fDoScaleUserPortDuration);

			if (i == UP_Port)
			{
				Enable_Fade(m_pUserPortrait[i], m_fDoScaleUserPortDuration);
			}
		}
		else
		{
			m_pUserPortrait[i]->DoScaleX(64.f, m_fDoScaleUserPortDuration);

			if (i == UP_Port)
			{
				Disable_Fade(m_pUserPortrait[i], m_fDoScaleUserPortDuration);
			}
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

	for (int i = 1; i < 5; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			GET_COMPONENT_FROM(m_arrPortraitUI[i][j], CFader)->Get_FadeDesc() = tFadeDesc;
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

void CUI_Portrait::OnEnable()
{
	__super::OnEnable();
}

void CUI_Portrait::OnDisable()
{
	__super::OnDisable();
}

void CUI_Portrait::Set_ShaderEffect(CShader* pShader, const char* constName)
{
	pShader->Set_RawValue("g_fValue", &m_fEffectValue, sizeof(_float));
}

void CUI_Portrait::Set_HeroPort(HeroPortAnimType eState)
{
	m_bAbleHero = true;
	m_eHeroPortAnimType = eState;
	m_bIsHeroLerp = true;
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
	for (int i = 1; i < 5; ++i)
	{
		m_arrPortraitUI[i][Effect]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

		GET_COMPONENT_FROM(m_arrPortraitUI[i][Effect], CShader)->CallBack_SetRawValues += bind(&CUI_Portrait::Set_ShaderEffect, this, placeholders::_1, "g_fValue");
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

	m_Prototypes[BG]->Set_Sort(0.3f);
	m_Prototypes[Port]->Set_Sort(0.29f);
	m_Prototypes[Key]->Set_Sort(0.3f);
	m_Prototypes[Effect]->Set_Sort(0.31f);

	GET_COMPONENT_FROM(m_Prototypes[Port], CTexture)->Remove_Texture(0);

	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_06.dds"));

	GET_COMPONENT_FROM(m_Prototypes[BG], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_RoundPortraitBGSmall.dds"));

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
	m_arrPortraitUI[0][Key] = nullptr;
	DELETE_GAMEOBJECT(m_arrPortraitUI[0][Effect]);
	m_arrPortraitUI[0][Effect] = nullptr;

	for (int i = 1; i < 5; ++i)
	{
		float fPosX = 260.f + (i * 55.f);
		_uint iPortIndex = 5 + i;
		_uint iKeyIndex = 32 + i;

		GET_COMPONENT_FROM(m_arrPortraitUI[i][BG], CTexture)->Set_CurTextureIndex(1);
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Port], CTexture)->Set_CurTextureIndex(iPortIndex);
		GET_COMPONENT_FROM(m_arrPortraitUI[i][Key], CTexture)->Set_CurTextureIndex(iKeyIndex);

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
				m_arrPortraitUI[i][j]->Set_Scale(65.f);
			}

			// 클론 객체 생성 후 비활성화
			CREATE_GAMEOBJECT(m_arrPortraitUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrPortraitUI[i][j]);
		}
	}
}