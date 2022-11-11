#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "Renderer.h"

CUI_Crosshair::CUI_Crosshair()
{
}

CUI_Crosshair::CUI_Crosshair(const CUI_Crosshair& Prototype)
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
			m_arrSkillUI[i][j] = m_Prototypes[j]->Clone();
		}
	}

	Set_Pass();

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	Prototype_Disable();

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			CREATE_GAMEOBJECT(m_arrSkillUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}

	DefaultCrosshair();

	__super::Start();

	return S_OK;
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

void CUI_Crosshair::Prototype_Disable()
{
	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_Prototypes[i]);
	}
}

void CUI_Crosshair::Set_Crosshair(_uint iIndex)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < Type_End; ++j)
		{
			DISABLE_GAMEOBJECT(m_arrSkillUI[i][j]);
		}
	}

	switch (iIndex)
	{
	case CUnit::CLASS_WARRIOR:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_SPEAR:
		DefaultCrosshair(3);
		break;

	case CUnit::CLASS_ARCHER:
		ArrowCrosshair();
		break;

	case CUnit::CLASS_PALADIN:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_PRIEST:
		DefaultCrosshair(2);
		break;

	case CUnit::CLASS_ENGINEER:
		DefaultCrosshair(2);
		break;

	case CUnit::CLASS_FIONA:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_QANDA:
		DefaultCrosshair(3);
		break;

	case CUnit::CLASS_HOEDT:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_LANCER:
		DefaultCrosshair(4);
		break;
	}
}

void CUI_Crosshair::DefaultCrosshair(_uint iIndex)
{
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Point]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Outline]);

	GET_COMPONENT_FROM(m_arrSkillUI[0][Point], CTexture)->Set_CurTextureIndex(1);
	GET_COMPONENT_FROM(m_arrSkillUI[0][Outline], CTexture)->Set_CurTextureIndex(1);

	// 기본 비활성화
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][GaugeBG]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Gauge]);

	_float fRotZ = 180.f;

	m_arrSkillUI[0][GaugeBG]->Set_RotationZ(fRotZ);
	m_arrSkillUI[0][Gauge]->Set_RotationZ(fRotZ);

	for (int i = 0; i < iIndex; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][ArrowBG]);
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][Arrow]);

		if (iIndex == 2)
		{
			_float fRotZ = -90.f;
			m_arrSkillUI[i][Arrow]->Set_RotationZ(fRotZ);
			m_arrSkillUI[i][ArrowBG]->Set_RotationZ(-fRotZ);
		}

		if (iIndex == 3)
		{
			_float fRotZ = -120.f * i;
			m_arrSkillUI[i][Arrow]->Set_RotationZ(fRotZ);
			m_arrSkillUI[i][ArrowBG]->Set_RotationZ(fRotZ);
		}

		if (iIndex == 4)
		{
			_float fPosX = -50.f + (i * 30.f);
			_float fPosY = -70.f;

			_float fScaleX = 35.f;
			_float fScaleY = 85.f;

			m_arrSkillUI[i][Arrow]->Set_Pos(fPosX, fPosY);
			m_arrSkillUI[i][ArrowBG]->Set_Pos(fPosX, fPosY);

			m_arrSkillUI[i][Arrow]->Set_Scale(fScaleX, fScaleY);
			m_arrSkillUI[i][ArrowBG]->Set_Scale(fScaleX, fScaleY);

			m_arrSkillUI[i][Arrow]->Set_RotationZ(0.f);
			m_arrSkillUI[i][ArrowBG]->Set_RotationZ(0.f);

			GET_COMPONENT_FROM(m_arrSkillUI[i][ArrowBG], CTexture)->Set_CurTextureIndex(1);
			GET_COMPONENT_FROM(m_arrSkillUI[i][Arrow], CTexture)->Set_CurTextureIndex(2);
		}
	}
}

void CUI_Crosshair::ArrowCrosshair()
{
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Point]);
	GET_COMPONENT_FROM(m_arrSkillUI[0][Point], CTexture)->Set_CurTextureIndex(1);

	// 타겟팅 중이면 포인터 변경
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Point]);
	GET_COMPONENT_FROM(m_arrSkillUI[0][Point], CTexture)->Set_CurTextureIndex(0);

	// 기본 비활성화
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Outline]);
	GET_COMPONENT_FROM(m_arrSkillUI[0][Outline], CTexture)->Set_CurTextureIndex(0);

	ENABLE_GAMEOBJECT(m_arrSkillUI[0][GaugeBG]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Gauge]);
	for (int i = 0; i < 3; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][ArrowBG]);
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][Arrow]);

		_float fRotZ = -120.f * i;

		m_arrSkillUI[i][Arrow]->Set_RotationZ(fRotZ);
		m_arrSkillUI[i][ArrowBG]->Set_RotationZ(fRotZ);
	}
}

void CUI_Crosshair::Set_Pass()
{
	for (int i = 0; i < 3; ++i)
	{

	}
}

void CUI_Crosshair::My_Tick()
{
	__super::My_Tick();

	if (KEY(T, TAP))
	{
		static int iIndex = 0;
		iIndex++;
		if (iIndex >= 10)
			iIndex = 0;

		Set_Crosshair(iIndex);
	}
}

void CUI_Crosshair::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Crosshair::OnEnable()
{
	__super::OnEnable();
}

void CUI_Crosshair::OnDisable()
{
	__super::OnDisable();
}
