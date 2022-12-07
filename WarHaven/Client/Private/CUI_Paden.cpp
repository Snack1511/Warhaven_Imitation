#include "CUI_Paden.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"

HRESULT CUI_Paden::Initialize_Prototype()
{
	Create_InGameTimer();
	Create_StrongHoldGauge();
	Create_TopStrongPointUI();

	return S_OK;
}

HRESULT CUI_Paden::Start()
{
	__super::Start();

	m_pInGameTimer->SetActive(true);
	SetActive_StrongHoldGauge(true);
	SetActive_TopPointUI(true);

	return S_OK;
}

void CUI_Paden::SetActive_StrongHoldGauge(_bool value)
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			m_pArrStrongHoldGauge[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::SetActive_TopPointUI(_bool value)
{
	for (int i = 0; i < TP_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrTopPointUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::My_Tick()
{
	__super::My_Tick();

	if (m_fInGameTime < 0.f)
	{
		cout << "게임 종료" << endl;
		m_fInGameTime = 0.f;
	}
	else
	{
		m_fInGameTime -= fDT(0);

		_uint iMin = m_fInGameTime / 60.f;
		_uint iSec = (_uint)m_fInGameTime % 60;

		_tchar  szTemp[MAX_STR] = {};
		swprintf_s(szTemp, TEXT("%d:%02d"), iMin, iSec);
		m_pInGameTimer->Set_FontText(szTemp);
	}
}

void CUI_Paden::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Paden::OnEnable()
{
	__super::OnEnable();

	m_pInGameTimer->SetActive(true);
	SetActive_StrongHoldGauge(true);
	SetActive_TopPointUI(true);
}

void CUI_Paden::OnDisable()
{
	__super::OnDisable();

	m_pInGameTimer->SetActive(false);
	SetActive_StrongHoldGauge(false);
	SetActive_TopPointUI(false);
}

void CUI_Paden::Create_InGameTimer()
{
	m_pInGameTimer = CUI_Object::Create();

	m_pInGameTimer->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

	m_pInGameTimer->Set_PosY(315.f);

	m_pInGameTimer->Set_FontRender(true);
	m_pInGameTimer->Set_FontStyle(true);
	m_pInGameTimer->Set_FontCenter(true);
	m_pInGameTimer->Set_FontOffset(4.5f, 0.5f);
	m_pInGameTimer->Set_FontScale(0.4f);

	CREATE_GAMEOBJECT(m_pInGameTimer, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInGameTimer);
}

void CUI_Paden::Create_StrongHoldGauge()
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		m_pStrongHoldGauge[i] = CUI_Object::Create();

		if (i < Gauge_Icon)
		{
			m_pStrongHoldGauge[i]->Set_PosY(300.f);
			m_pStrongHoldGauge[i]->Set_Scale(215.f, 8.f);

			if (i == Gauge_BG)
			{
				m_pStrongHoldGauge[i]->Set_Sort(0.5f);
			}
			else if (i == Gauge_Bar)
			{
				GET_COMPONENT_FROM(m_pStrongHoldGauge[i], CTexture)->Remove_Texture(0);
				Read_Texture(m_pStrongHoldGauge[i], "/Paden/TopGauge", "Bar");

				m_pStrongHoldGauge[i]->Set_Sort(0.49f);
			}
		}
		else
		{
			GET_COMPONENT_FROM(m_pStrongHoldGauge[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldGauge[i], "/Paden", "Team");

			m_pStrongHoldGauge[i]->Set_Sort(0.5f);

			m_pStrongHoldGauge[i]->Set_PosY(325);
			m_pStrongHoldGauge[i]->Set_Scale(38.f);
		}

		CREATE_GAMEOBJECT(m_pStrongHoldGauge[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pStrongHoldGauge[i]);

		for (int j = 0; j < 2; ++j)
		{
			m_pArrStrongHoldGauge[i][j] = m_pStrongHoldGauge[i]->Clone();

			if (i < Gauge_Icon)
			{
				_float fPosX = -200.f + (j * 400.f);
				m_pArrStrongHoldGauge[i][j]->Set_PosX(fPosX);

				if (i == Gauge_Bar)
				{
					GET_COMPONENT_FROM(m_pArrStrongHoldGauge[i][j], CTexture)->Set_CurTextureIndex(j);
				}
			}
			else
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldGauge[i][j], CTexture)->Set_CurTextureIndex(j);

				_float fPosX = -105.f + (j * 210.f);
				m_pArrStrongHoldGauge[i][j]->Set_PosX(fPosX);
			}

			CREATE_GAMEOBJECT(m_pArrStrongHoldGauge[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrStrongHoldGauge[i][j]);
		}
	}
}

void CUI_Paden::Create_TopStrongPointUI()
{
	for (int i = 0; i < TP_End; ++i)
	{
		m_pTopPointUI[i] = CUI_Object::Create();

		m_pTopPointUI[i]->Set_PosY(275.f);
		m_pTopPointUI[i]->Set_Scale(35.f);

		if (i == TP_BG)
		{
			GET_COMPONENT_FROM(m_pTopPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTopPointUI[i], "/Paden", "Gauge");
		}
		else if (i == TP_Gauge)
		{
			GET_COMPONENT_FROM(m_pTopPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTopPointUI[i], "/Paden", "Gauge");
		}
		else if (TP_Outline)
		{
			GET_COMPONENT_FROM(m_pTopPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTopPointUI[i], "/Paden", "Outline");

			m_pTopPointUI[i]->Set_Scale(40.f);
		}
		else if (TP_Icon)
		{
			m_pTopPointUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			GET_COMPONENT_FROM(m_pTopPointUI[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Paden/PointIcon_Join.dds"));
		}

		CREATE_GAMEOBJECT(m_pTopPointUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pTopPointUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrTopPointUI[i][j] = m_pTopPointUI[i]->Clone();

			if (j == 0)
			{
				GET_COMPONENT_FROM(m_pArrTopPointUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}
			else
			{
				GET_COMPONENT_FROM(m_pArrTopPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
			}

			CREATE_GAMEOBJECT(m_pArrTopPointUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrTopPointUI[i][j]);
		}
	}
}
