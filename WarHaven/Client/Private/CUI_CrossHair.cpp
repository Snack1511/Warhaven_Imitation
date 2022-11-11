#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"

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
		DefaultCrosshair();
		break;

	case CUnit::CLASS_ARCHER:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_PALADIN:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_PRIEST:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_ENGINEER:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_FIONA:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_QANDA:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_HOEDT:
		DefaultCrosshair();
		break;

	case CUnit::CLASS_LANCER:
		DefaultCrosshair();
		break;
	}
}

void CUI_Crosshair::DefaultCrosshair()
{
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Point]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Outline]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][GaugeBG]);
	ENABLE_GAMEOBJECT(m_arrSkillUI[0][Gauge]);

	for (int i = 0; i < 3; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][ArrowBG]);
		ENABLE_GAMEOBJECT(m_arrSkillUI[i][Arrow]);
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
