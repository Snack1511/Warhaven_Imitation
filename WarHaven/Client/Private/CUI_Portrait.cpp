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
	m_bAbleRotationPort = true;

	GET_COMPONENT_FROM(m_arrPortraitUI[0][Port], CTexture)->Set_CurTextureIndex(iIndex);
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	if (KEY(T, TAP))
	{
		static int iIndex = 0;
		iIndex++;
		if (iIndex >= 10)
			iIndex = 0;

		Set_Portrait(iIndex);
	}

	_float fEffectSpeed = fDT(0) * 5.f;
	m_fEffectValue -= fEffectSpeed;
	
	Rotation_UserPort();
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

void CUI_Portrait::Rotation_UserPort()
{
	_float fStartBG = 64.f;
	_float fStartPort = 63.f;
	_float fEndPoint = 0.f;
	_float fDuration = 0.3f;

	// 4¹ø È¸Àü
	// ³ª ¿©¿õ ³ª ¿©¿õ
	if (m_bAbleRotationPort)
	{
		_float4 vPos = m_arrPortraitUI[0][BG]->Get_Transform()->Get_Scale();

		if (m_iRotationCount < 5)
		{
			if (vPos.x >= fStartBG)
			{
				m_arrPortraitUI[0][BG]->Lerp_ScaleX(fStartBG, fEndPoint, fDuration);
				m_arrPortraitUI[0][Port]->Lerp_ScaleX(fStartPort, fEndPoint, fDuration);
			}

			if (vPos.x <= fEndPoint)
			{
				m_arrPortraitUI[0][BG]->Lerp_ScaleX(fEndPoint, fStartBG, fDuration);
				m_arrPortraitUI[0][Port]->Lerp_ScaleX(fEndPoint, fStartPort, fDuration);
			}
		}
		else
		{
			m_iRotationCount = 0;
			m_bAbleRotationPort = false;
		}
	}
	
}
