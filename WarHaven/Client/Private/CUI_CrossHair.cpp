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
	Create_Crosshair();
	Create_ArrowUI();
	Create_LancerUI();

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	__super::Start();

	Init_DefaultCrosshair();
	SetActive_DefaultCrosshair(true);
	SetActive_ArrowUI(true);

	return S_OK;
}

void CUI_Crosshair::OnEnable()
{
	__super::OnEnable();

	// 현재 클라스를 받아와서 해당하는 클래스의 UI 활성화
}

void CUI_Crosshair::OnDisable()
{
	__super::OnDisable();

	SetActive_DefaultCrosshair(false);
	SetActive_ArrowUI(false);
	SetActive_LancerUI(false);
}

void CUI_Crosshair::Set_Crosshair(_uint iClass)
{
	// 워리어 엔지니어
	Init_DefaultCrosshair();

	Init_ArrowUI(iClass);
}

void CUI_Crosshair::SetActive_DefaultCrosshair(_bool value)
{
	if (value == true)
	{
		for (int i = 0; i < CU_End; ++i)
		{
			ENABLE_GAMEOBJECT(m_pCrosshair[i]);
		}
	}
	else
	{
		for (int i = 0; i < CU_End; ++i)
		{
			DISABLE_GAMEOBJECT(m_pCrosshair[i]);
		}
	}
}

void CUI_Crosshair::SetActive_ArrowUI(_bool value)
{
	if (value == true)
	{
		for (int i = 0; i < AU_End; ++i)
		{
			for (int j = 0; j < m_iArrowIndex; ++j)
			{
				ENABLE_GAMEOBJECT(m_pArrArrowUI[i][j]);
			}
		}
	}
	else
	{
		for (int i = 0; i < AU_End; ++i)
		{
			for (int j = 0; j < m_iArrowIndex; ++j)
			{
				DISABLE_GAMEOBJECT(m_pArrArrowUI[i][j]);
			}
		}
	}
}

void CUI_Crosshair::SetActive_LancerUI(_bool value)
{
	if (value == true)
	{
		for (int i = 0; i < LU_End; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				ENABLE_GAMEOBJECT(m_pArrLancerUI[i][j]);
			}
		}
	}
	else
	{
		for (int i = 0; i < LU_End; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				DISABLE_GAMEOBJECT(m_pArrLancerUI[i][j]);
			}
		}
	}
}

void CUI_Crosshair::Create_Crosshair()
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCrosshair[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pCrosshair[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCrosshair[i]);
	}

	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Point], "/HUD/Crosshair", "Point");

	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Outline], "/HUD/Crosshair", "Outline");
}

void CUI_Crosshair::Init_DefaultCrosshair()
{
	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Set_CurTextureIndex(0);

	m_pCrosshair[CU_Point]->Set_Scale(6.f);

	m_pCrosshair[CU_Outline]->Set_Scale(50.f);
	m_pCrosshair[CU_Outline]->Set_Color(_float4(0.2f, 0.2f, 0.2f, 0.4f));
}

void CUI_Crosshair::Create_ArrowUI()
{
	for (int i = 0; i < AU_End; ++i)
	{
		m_pArrowUI[i] = CUI_Object::Create();

		m_pArrowUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Arrow.png"));
		m_pArrowUI[i]->Set_Scale(100.f);

		if (i == AU_BG)
		{
			m_pArrowUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.6f));
			m_pArrowUI[i]->Set_Sort(0.5f);
		}
		else if (i == AU_Arrow)
		{
			m_pArrowUI[i]->Set_Color(m_vArrowColor);
			m_pArrowUI[i]->Set_Sort(0.49f);
		}

		CREATE_GAMEOBJECT(m_pArrowUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pArrowUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrArrowUI[i][j] = m_pArrowUI[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrArrowUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrArrowUI[i][j]);
		}
	}
}

void CUI_Crosshair::Init_ArrowUI(_uint iClass)
{
	if (iClass == CLASS_TYPE::SPEAR || iClass == CLASS_TYPE::ARCHER)
	{
		m_iArrowIndex = 3;

	}
	else if (iClass == CLASS_TYPE::PRIEST || iClass == CLASS_TYPE::ENGINEER)
	{
		m_iArrowIndex = 2;
	}
	else
	{
		return;
	}

	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			_float fRotZ = 0.f;

			if (m_iArrowIndex == 2)
			{
				fRotZ = -90.f - (j * 180.f);
			}
			else if (m_iArrowIndex == 3)
			{
				fRotZ = -120.f * j;
			}

			m_pArrArrowUI[i][j]->Set_RotationZ(fRotZ);
		}
	}
}

void CUI_Crosshair::Create_LancerUI()
{
	for (int i = 0; i < LU_End; ++i)
	{
		m_pLancerUI[i] = CUI_Object::Create();

		m_pLancerUI[i]->Set_PosY(-100.f);
		m_pLancerUI[i]->Set_Scale(35.f, 85.f);

		if (i == LU_BG)
		{
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowBG.png"));
		}
		else if (i == LU_Gauge)
		{
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowGauge.png"));
		}
		else if (i == LU_Full)
		{
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowFull.png"));
		}

		CREATE_GAMEOBJECT(m_pLancerUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pLancerUI[i]);

		for (int j = 0; j < 4; ++j)
		{
			m_pArrLancerUI[i][j] = m_pLancerUI[i]->Clone();

			_float fPosX = -45.f + (j * 30.f);
			m_pArrLancerUI[i][j]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrLancerUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrLancerUI[i][j]);
		}
	}
}
