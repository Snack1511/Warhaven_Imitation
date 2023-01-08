#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "CUI_Renderer.h"
#include "CShader.h"
#include "Renderer.h"
#include "Easing_Utillity.h"

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

	return S_OK;
}

void CUI_Crosshair::Set_Crosshair(_uint iClass)
{
	m_iClassIndex = iClass;

	switch (m_iClassIndex)
	{
	case WARRIOR:
		Set_DefaultCrosshair();
		break;

	case ARCHER:
		Set_ArcherCrosshair();
		break;

	case ENGINEER:
		Set_DefaultCrosshair();
		break;
	}

	Set_ArrowUI();
}

void CUI_Crosshair::SetActive_Crosshair(_bool value)
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
	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			if (m_pArrArrowUI[i][j]->Is_Valid() == !value)
			{
				m_pArrArrowUI[i][j]->SetActive(value);
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

void CUI_Crosshair::Set_Position(_float4 vPos)
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCrosshair[i]->Set_Pos(vPos);
	}

	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrArrowUI[i][j]->Set_Pos(vPos);
		}
	}
}

void CUI_Crosshair::Set_ArcherPoint(_bool value)
{
	if (m_iClassIndex != ARCHER)
		return;

	if (value == true)
	{
		m_pCrosshair[CU_Point]->Set_Color(_float4(0.8f, 0.2f, 0.2f, 1.f));
		GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(1);
	}
	else
	{
		m_pCrosshair[CU_Point]->Set_Color(_float4(1.f, 1.f, 1.f, 1.f));
		GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	}
}

void CUI_Crosshair::Create_Crosshair()
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCrosshair[i] = CUI_Object::Create();

		m_pCrosshair[i]->Set_Sort(0.5f);

		if (i == CU_Point)
		{
			m_pCrosshair[i]->Set_Scale(6.f);
		}
		else if (i == CU_Outline)
		{
			m_pCrosshair[i]->Set_Scale(50.f);
			m_pCrosshair[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.4f));
		}

		CREATE_GAMEOBJECT(m_pCrosshair[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCrosshair[i]);
	}

	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Point], "/HUD/Crosshair", "Point");

	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Outline], "/HUD/Crosshair", "Outline");
}

void CUI_Crosshair::Set_DefaultCrosshair()
{
	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Set_CurTextureIndex(0);
}

void CUI_Crosshair::Set_ArcherCrosshair()
{
	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Set_CurTextureIndex(1);
}

void CUI_Crosshair::Create_ArrowUI()
{
	for (int i = 0; i < AU_End; ++i)
	{
		m_pArrowUI[i] = CUI_Object::Create();

		m_pArrowUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Arrow.png"));

		if (i == AU_BG)
		{
			m_pArrowUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.6f));
			m_pArrowUI[i]->Set_Sort(0.5f);
			m_pArrowUI[i]->Set_Scale(100.f);
		}
		else if (i == AU_Arrow)
		{
			m_pArrowUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.f));
			m_pArrowUI[i]->Set_Sort(0.49f);
			m_pArrowUI[i]->Set_Scale(130.f);
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

void CUI_Crosshair::Set_ArrowUI()
{
	if (m_iClassIndex == SPEAR || m_iClassIndex == ARCHER)
	{
		m_iArrowIndex = 3;

	}
	else if (m_iClassIndex == PRIEST || m_iClassIndex == ENGINEER)
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

void CUI_Crosshair::Charge_Arrow()
{
	if (m_iClassIndex != ARCHER )
		return;

	_float fChargeWaitTime = 0.3f;
	_float fRotateTime = fChargeWaitTime / (_float)m_iArrowIndex;
	_float fScaleTime = 0.1f;
	_float fAngle = 360.f / (_float)m_iArrowIndex;

	if (KEY(LBUTTON, HOLD))
	{
		if (!m_bIsCharge)
		{
			if (m_bIsChargeWait)
				return;

			m_bIsCharge = true;

			SetActive_ArrowUI(true);
		}
		else
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > fChargeWaitTime)
			{
				m_fAccTime = 0.f;

				if (m_bIsChargeWait)
				{
					if (m_iChargeCount > 2)
					{
						for (int i = 0; i < 3; ++i)
						{
							m_pArrArrowUI[AU_Arrow][i]->Set_Color(_float4(1.f, 0.f, 0.f, 1.f));
						}

						return;
					}

					Rotate_Arrow(fAngle, fRotateTime);

					m_bIsChargeWait = false;
					m_bIsCharge = false;
				}
				else
				{
					m_bIsChargeWait = true;

					if (m_iChargeCount > 2)
						return;

					m_pArrArrowUI[AU_Arrow][m_iChargeCount]->DoScale(-30.f, fScaleTime);
					m_pArrArrowUI[AU_Arrow][m_iChargeCount++]->Set_Color(m_vArrowColor);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_pArrArrowUI[AU_Arrow][i]->Set_Scale(130.f);
			m_pArrArrowUI[AU_Arrow][i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
		}

		m_bIsCharge = false;
		m_bIsChargeWait = false;
		m_iChargeCount = 0;

		m_fAccTime = 0.f;

		Set_ArrowUI();
		SetActive_ArrowUI(false);
	}
}

void CUI_Crosshair::Rotate_Arrow(_float fAngle, _float fDuration)
{
	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			m_pArrArrowUI[i][j]->DoRotate(fAngle, fDuration);
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

void CUI_Crosshair::My_Tick()
{
	__super::My_Tick();

	Charge_Arrow();
}

void CUI_Crosshair::OnEnable()
{
	__super::OnEnable();

	SetActive_Crosshair(true);
}

void CUI_Crosshair::OnDisable()
{
	__super::OnDisable();

	SetActive_Crosshair(false);
	SetActive_ArrowUI(false);
	SetActive_LancerUI(false);
}