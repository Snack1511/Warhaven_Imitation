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

	Set_FadeHeroPort();

	__super::Start();

	return S_OK;
}

void CUI_Portrait::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			if (m_arrPortraitUI[i][j])
				ENABLE_GAMEOBJECT(m_arrPortraitUI[i][j]);
		}
	}
}

void CUI_Portrait::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			if (m_arrPortraitUI[i][j])
				DISABLE_GAMEOBJECT(m_arrPortraitUI[i][j]);
		}
	}
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

		Enable_Fade(m_arrPortraitUI[0][i], 0.1f);
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

	_float fEffectSpeed = fDT(0) * 5.f;
	m_fEffectValue -= fEffectSpeed;

	Change_UserPort();

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

	GET_COMPONENT_FROM(m_arrPortraitUI[0][Port], CFader)->Get_FadeDesc() = tFadeDesc;
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

	Set_FadeUserPort(fRotSpeed);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(0.f, 64.f, fRotSpeed);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(0.f, 63.f, fRotSpeed);

	Enable_Fade(m_arrPortraitUI[0][Port], fRotSpeed);
}

void CUI_Portrait::PortSizeDown(_float fDuration)
{
	_float fRotSpeed = 0.1f * (m_iRotationCount * 0.5f);

	Set_FadeUserPort(fRotSpeed);

	m_arrPortraitUI[0][BG]->Lerp_ScaleX(64.f, 0.f, fRotSpeed);
	m_arrPortraitUI[0][Port]->Lerp_ScaleX(63.f, 0.f, fRotSpeed);

	Disable_Fade(m_arrPortraitUI[0][Port], fRotSpeed);
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
	m_Prototypes[Effect]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_13.dds"));

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
				m_arrPortraitUI[i][j]->Set_Scale(55.f);
			}

			// 클론 객체 생성 후 비활성화
			CREATE_GAMEOBJECT(m_arrPortraitUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrPortraitUI[i][j]);
		}
	}
}