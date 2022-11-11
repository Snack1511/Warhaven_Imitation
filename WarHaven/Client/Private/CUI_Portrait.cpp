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

CUI_Portrait::CUI_Portrait(const CUI_Portrait& Prototype)
	: CUI_Wrapper(Prototype)
{
}

CUI_Portrait::~CUI_Portrait()
{
}

HRESULT CUI_Portrait::Initialize_Prototype()
{
	Read_UI("Port");

	m_tPort.m_pUIInstance[Portrait::BG] = m_pUIMap[TEXT("PortBG")];
	m_tPort.m_pUIInstance[Portrait::Port] = m_pUIMap[TEXT("Port")];
	m_tPort.m_pUIInstance[Portrait::Key] = m_pUIMap[TEXT("PortKey")];
	m_tPort.m_pUIInstance[Portrait::Effect] = m_pUIMap[TEXT("Port_Effect")];

	GET_COMPONENT_FROM(m_tPort.m_pUIInstance[Portrait::Port], CTexture)->Remove_Texture(0);

	m_tPort.m_pUIInstance[Portrait::Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_06.dds"));
	m_tPort.m_pUIInstance[Portrait::Effect]->SetTexture(TEXT("../Bin/R\esources/Textures/UI/Effect/T_Pattern_13.dds"));

	GET_COMPONENT_FROM(m_tPort.m_pUIInstance[Portrait::BG], CTexture)
		->Add_Texture(TEXT("../Bin/R\esources/Textures/UI/Circle/T_RoundPortraitBGSmall.dds"));

	Read_Texture(m_tPort.m_pUIInstance[Portrait::Port], "/HUD/Portrait", "Class");
	Read_Texture(m_tPort.m_pUIInstance[Portrait::Key], "/KeyIcon/Keyboard", "Key");

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < Portrait::NAME_END; ++j)
		{
			m_arrtPort[i].m_pUIInstance[j] = m_tPort.m_pUIInstance[j]->Clone();
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
	Enable_Portrait();

	Set_Pass();
	Bind_Sahder();

	__super::Start();

	return S_OK;
}

void CUI_Portrait::Set_ShaderEffect(CShader* pShader, const char* constName)
{
	pShader->Set_RawValue("g_fValue", &m_fEffectValue, sizeof(_float));
}

void CUI_Portrait::Set_Portrait(_uint iIndex)
{
	CTexture* pTexture = GET_COMPONENT_FROM(m_arrtPort[0].m_pUIInstance[Portrait::Port], CTexture);

	switch (iIndex)
	{
	case CUnit::CLASS_WARRIOR:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_SPEAR:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_ARCHER:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_PALADIN:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_PRIEST:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_ENGINEER:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_FIONA:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_QANDA:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_HOEDT:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_LANCER:
		pTexture->Set_CurTextureIndex(iIndex);
		break;
	}
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


	m_fEffectValue -= fDT(0);
}

void CUI_Portrait::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Portrait::Enable_Portrait()
{
	for (_uint i = 0; i < Portrait::NAME_END; ++i)
	{
		CREATE_GAMEOBJECT(m_tPort.m_pUIInstance[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_tPort.m_pUIInstance[i]);

		CREATE_GAMEOBJECT(m_arrtPort[0].m_pUIInstance[i], GROUP_UI);
	}

	DELETE_GAMEOBJECT(m_arrtPort[0].m_pUIInstance[Portrait::Key]);
	DELETE_GAMEOBJECT(m_arrtPort[0].m_pUIInstance[Portrait::Effect]);

	for (int i = 1; i < 5; ++i)
	{
		float fPosX = 260.f + (i * 55.f);
		_uint iPortIndex = 5 + i;
		_uint iKeyIndex = 32 + i;

		GET_COMPONENT_FROM(m_arrtPort[i].m_pUIInstance[Portrait::BG], CTexture)->Set_CurTextureIndex(1);
		GET_COMPONENT_FROM(m_arrtPort[i].m_pUIInstance[Portrait::Port], CTexture)->Set_CurTextureIndex(iPortIndex);
		GET_COMPONENT_FROM(m_arrtPort[i].m_pUIInstance[Portrait::Key], CTexture)->Set_CurTextureIndex(iKeyIndex);

		m_arrtPort[i].m_pUIInstance[Portrait::BG]->Set_Sort(0.1f);
		m_arrtPort[i].m_pUIInstance[Portrait::Effect]->Set_Sort(0.2f);

		for (int j = 0; j < Portrait::NAME_END; ++j)
		{
			m_arrtPort[i].m_pUIInstance[j]->Set_Pos(fPosX, -230.f);
			m_arrtPort[i].m_pUIInstance[j]->Set_Scale(43.f);

			if (j == Portrait::Key)
			{
				m_arrtPort[i].m_pUIInstance[j]->Set_Pos(fPosX, -195.f);
				m_arrtPort[i].m_pUIInstance[j]->Set_Scale(15.f);
			}

			if (j == Portrait::Effect)
			{
				m_arrtPort[i].m_pUIInstance[j]->Set_Scale(55.f);
			}

			CREATE_GAMEOBJECT(m_arrtPort[i].m_pUIInstance[j], GROUP_UI);
		}
	}
}

void CUI_Portrait::Set_Pass()
{
	for (int i = 0; i < 5; ++i)
	{
		GET_COMPONENT_FROM(m_arrtPort[i].m_pUIInstance[Portrait::Effect], CRenderer)
			->Set_Pass(VTXTEX_PASS_UI_PortEffect);
	}
}

void CUI_Portrait::Bind_Sahder()
{
	GET_COMPONENT_FROM(m_arrtPort[0].m_pUIInstance[Portrait::Effect], CShader)
		->CallBack_SetRawValues += bind(&CUI_Portrait::Set_ShaderEffect, this, placeholders::_1, "g_fValue");
}
