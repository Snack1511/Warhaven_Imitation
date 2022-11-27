#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "CUI_Renderer.h"
#include "CShader.h"
#include "Renderer.h"

CUI_Crosshair::CUI_Crosshair()
{
}

CUI_Crosshair::~CUI_Crosshair()
{
}

HRESULT CUI_Crosshair::Initialize_Prototype()
{
	Read_UI("Crosshair");

	Ready_Texture();

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			m_arrCrosshair[i][j] = m_Prototypes[j]->Clone();
		}
	}

	Ready_Crosshair();

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_Crosshair::Set_ShaderResources_Arrow(CShader* pShader, const char* pConstName)
{
	_float4 vColor;
	for (int i = 0; i < 3; ++i)
	{
		vColor = m_arrCrosshair[i][Arrow]->Get_Color();
	}

	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
}

void CUI_Crosshair::Set_ShaderResources_ArrowBG(CShader* pShader, const char* pConstName)
{
	_float4 vColor;
	for (int i = 0; i < 3; ++i)
	{
		vColor = m_arrCrosshair[i][ArrowBG]->Get_Color();
	}

	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
}

void CUI_Crosshair::Set_Crosshair(_uint iIndex)
{
	m_iPrvCrosshair = m_iCurCrosshair;
	m_iCurCrosshair = iIndex;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			DISABLE_GAMEOBJECT(m_arrCrosshair[i][j]);
		}
	}

	switch (m_iCurCrosshair)
	{
	case WARRIOR:
		DefaultCrosshair();
		break;

	case SPEAR:
		DefaultCrosshair(3);
		break;

	case ARCHER:
		ArrowCrosshair();
		break;

	case PALADIN:
		DefaultCrosshair();
		break;

	case PRIEST:
		DefaultCrosshair(2);
		break;

	case ENGINEER:
		DefaultCrosshair(2);
		break;

	case FIONA:
		DefaultCrosshair();
		break;

	case QANDA:
		DefaultCrosshair(3);
		break;

	case HOEDT:
		DefaultCrosshair();
		break;

	case LANCER:
		DefaultCrosshair(4);
		break;
	}
}

void CUI_Crosshair::Set_Pass()
{
	for (int i = 0; i < 3; ++i)
	{
		GET_COMPONENT_FROM(m_arrCrosshair[i][Arrow], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Color);
		GET_COMPONENT_FROM(m_arrCrosshair[i][ArrowBG], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Color);
	}
}

void CUI_Crosshair::Bind_Shader()
{
	for (int i = 0; i < 3; ++i)
	{
		GET_COMPONENT_FROM(m_arrCrosshair[i][Arrow], CShader)
			->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_ShaderResources_Arrow, this, placeholders::_1, "g_vColor");

		GET_COMPONENT_FROM(m_arrCrosshair[i][ArrowBG], CShader)
			->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_ShaderResources_ArrowBG, this, placeholders::_1, "g_vColor");
	}
}

void CUI_Crosshair::DefaultCrosshair(_uint iIndex)
{
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Point]);
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Outline]);

	GET_COMPONENT_FROM(m_arrCrosshair[0][Point], CTexture)->Set_CurTextureIndex(1);
	GET_COMPONENT_FROM(m_arrCrosshair[0][Outline], CTexture)->Set_CurTextureIndex(1);

	// 기본 비활성화
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][GaugeBG]);
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Gauge]);

	_float fRotZ = 180.f;
	m_arrCrosshair[0][GaugeBG]->Set_RotationZ(fRotZ);
	m_arrCrosshair[0][Gauge]->Set_RotationZ(fRotZ);

	for (_uint i = 0; i < iIndex; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrCrosshair[i][ArrowBG]);
		ENABLE_GAMEOBJECT(m_arrCrosshair[i][Arrow]);

		GET_COMPONENT_FROM(m_arrCrosshair[i][ArrowBG], CTexture)->Set_CurTextureIndex(0);
		GET_COMPONENT_FROM(m_arrCrosshair[i][Arrow], CTexture)->Set_CurTextureIndex(0);

		m_arrCrosshair[i][Arrow]->Set_Pos(0.f, 0.f);
		m_arrCrosshair[i][ArrowBG]->Set_Pos(0.f, 0.f);

		m_arrCrosshair[i][Arrow]->Set_Scale(100.f);
		m_arrCrosshair[i][ArrowBG]->Set_Scale(100.f);

		if (iIndex == 2)
		{
			_float fRotZ = -90.f - (i * 180.f);
			m_arrCrosshair[i][Arrow]->Set_RotationZ(fRotZ);
			m_arrCrosshair[i][ArrowBG]->Set_RotationZ(fRotZ);
		}

		if (iIndex == 3)
		{
			_float fRotZ = -120.f * i;
			m_arrCrosshair[i][Arrow]->Set_RotationZ(fRotZ);
			m_arrCrosshair[i][ArrowBG]->Set_RotationZ(fRotZ);
		}

		if (iIndex == 4)
		{
			_float fPosX = -50.f + (i * 30.f);
			_float fPosY = -100.f;

			_float fScaleX = 35.f;
			_float fScaleY = 85.f;

			m_arrCrosshair[i][Arrow]->Set_Pos(fPosX, fPosY);
			m_arrCrosshair[i][ArrowBG]->Set_Pos(fPosX, fPosY);

			m_arrCrosshair[i][Arrow]->Set_Scale(fScaleX, fScaleY);
			m_arrCrosshair[i][ArrowBG]->Set_Scale(fScaleX, fScaleY);

			m_arrCrosshair[i][Arrow]->Set_RotationZ(0.f);
			m_arrCrosshair[i][ArrowBG]->Set_RotationZ(0.f);

			GET_COMPONENT_FROM(m_arrCrosshair[i][ArrowBG], CTexture)->Set_CurTextureIndex(1);
			GET_COMPONENT_FROM(m_arrCrosshair[i][Arrow], CTexture)->Set_CurTextureIndex(2);
		}
	}
}

void CUI_Crosshair::ArrowCrosshair()
{
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Point]);
	GET_COMPONENT_FROM(m_arrCrosshair[0][Point], CTexture)->Set_CurTextureIndex(1);

	// 타겟팅 중이면 포인터 변경
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Point]);
	GET_COMPONENT_FROM(m_arrCrosshair[0][Point], CTexture)->Set_CurTextureIndex(0);

	// 기본 비활성화
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Outline]);
	GET_COMPONENT_FROM(m_arrCrosshair[0][Outline], CTexture)->Set_CurTextureIndex(0);

	ENABLE_GAMEOBJECT(m_arrCrosshair[0][GaugeBG]);
	ENABLE_GAMEOBJECT(m_arrCrosshair[0][Gauge]);

	_float fRotZ = 180.f;
	m_arrCrosshair[0][GaugeBG]->Set_RotationZ(fRotZ);
	m_arrCrosshair[0][Gauge]->Set_RotationZ(fRotZ);

	for (int i = 0; i < 3; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrCrosshair[i][ArrowBG]);
		ENABLE_GAMEOBJECT(m_arrCrosshair[i][Arrow]);

		_float fRotZ = -120.f * i;

		m_arrCrosshair[i][Arrow]->Set_RotationZ(fRotZ);
		m_arrCrosshair[i][ArrowBG]->Set_RotationZ(fRotZ);
	}
}

void CUI_Crosshair::My_Tick()
{
	__super::My_Tick();
}

void CUI_Crosshair::Ready_Texture()
{
	m_Prototypes[Point] = m_pUIMap[TEXT("Crosshair_Point")];
	m_Prototypes[Outline] = m_pUIMap[TEXT("Crosshair_Outline")];
	m_Prototypes[ArrowBG] = m_pUIMap[TEXT("Crosshair_ArrowBG")];
	m_Prototypes[Arrow] = m_pUIMap[TEXT("Crosshair_Arrow")];
	m_Prototypes[GaugeBG] = m_pUIMap[TEXT("Crosshair_GaugeBG")];
	m_Prototypes[Gauge] = m_pUIMap[TEXT("Crosshair_Gauge")];

	for (int i = 0; i < Type_End; ++i)
	{
		GET_COMPONENT_FROM(m_Prototypes[i], CTexture)->Remove_Texture(0);
	}

	Read_Texture(m_Prototypes[Point], "/HUD/Crosshair", "Point");
	Read_Texture(m_Prototypes[Outline], "/HUD/Crosshair", "Outline");
	Read_Texture(m_Prototypes[ArrowBG], "/HUD/Crosshair", "Arrow");
	Read_Texture(m_Prototypes[Arrow], "/HUD/Crosshair", "Arrow");
	Read_Texture(m_Prototypes[GaugeBG], "/HUD/Crosshair", "Gauge");
	Read_Texture(m_Prototypes[Gauge], "/HUD/Crosshair", "Gauge");
}

void CUI_Crosshair::Ready_Crosshair()
{
	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			CREATE_GAMEOBJECT(m_arrCrosshair[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrCrosshair[i][j]);

			m_arrCrosshair[i][j]->Set_Sort(0.3f);
		}
	}
}
