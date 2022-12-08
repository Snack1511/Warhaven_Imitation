#include "CUI_Paden.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include <CUtility_Transform.h>

HRESULT CUI_Paden::Initialize_Prototype()
{
	Create_InGameTimer();
	Create_StrongHoldGauge();
	Create_GaugeNum();
	Create_StrongHoldUI();

	return S_OK;
}

HRESULT CUI_Paden::Start()
{
	__super::Start();

	Init_StrongHoldUI();

	m_pInGameTimer->SetActive(true);
	SetActive_StrongHoldGauge(true);
	SetActive_GaugeNum(true);
	SetActive_TopPointUI(true);

	return S_OK;
}

void CUI_Paden::Set_Proj_StrongHoldUI(_uint iPointIdx, CTransform* pTransform)
{
	if (m_pProj_StrongHoldUI[iPointIdx])
	{
		_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform);

		m_pProj_StrongHoldUI[iPointIdx]->Set_Pos(vNewPos);
	}
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

void CUI_Paden::SetActive_GaugeNum(_bool value)
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < Num_End; ++j)
		{
			m_pGauge_Num[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::SetActive_TopPointUI(_bool value)
{
	for (int i = 0; i < SU_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrStrongHoldUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::Interact_StrongHoldUI(string strPadenPointKey, _uint iTeamType, _uint iTriggerState)
{
	_float fDuration = 0.5f;

	for (int i = 0; i < SU_End; ++i)
	{
		switch (iTriggerState)
		{
		case TS_Enter:

			if (strPadenPointKey == "Paden_Trigger_A")
			{
				m_pArrStrongHoldUI[i][0]->DoScale(10.f, fDuration);
				m_pArrStrongHoldUI[i][0]->DoMove(0.f, 200.f, fDuration);

				Set_StrongHoldUI_Color(iTeamType);
			}

			break;

		case TS_Exit:

			if (strPadenPointKey == "Paden_Trigger_A")
			{
				m_pArrStrongHoldUI[i][0]->DoScale(-10.f, fDuration);
				m_pArrStrongHoldUI[i][0]->DoMove(m_fMainPointUIPosX, m_fPointUIPosY, fDuration);
			}

			break;
		}
	}
}

void CUI_Paden::Set_StrongHoldUI_Color(_uint iTeamType)
{
	switch (iTeamType)
	{
	case 3:
		m_pArrStrongHoldUI[SU_Gauge][0]->Set_Color(m_vColorBlue);
		break;

	case 13:
		m_pArrStrongHoldUI[SU_Gauge][0]->Set_Color(m_vColorRed);
		break;
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
		swprintf_s(szTemp, TEXT("%02d:%02d"), iMin, iSec);
		m_pInGameTimer->Set_FontText(szTemp);
	}

	if (KEY(Z, TAP))
	{
		Interact_StrongHoldUI("Paden_Trigger_A", 3, 0);
	}
	else if (KEY(C, TAP))
	{
		Interact_StrongHoldUI("Paden_Trigger_A", 3, 1);
	}
}

void CUI_Paden::OnEnable()
{
	__super::OnEnable();

	m_pInGameTimer->SetActive(true);
	SetActive_StrongHoldGauge(true);
	SetActive_GaugeNum(true);
	SetActive_TopPointUI(true);
}

void CUI_Paden::OnDisable()
{
	__super::OnDisable();

	m_pInGameTimer->SetActive(false);
	SetActive_StrongHoldGauge(false);
	SetActive_GaugeNum(false);
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
	m_pInGameTimer->Set_FontOffset(5.5f, 0.f);
	m_pInGameTimer->Set_FontScale(0.3);

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
			m_pStrongHoldGauge[i]->Set_Scale(150.f, 5.f);

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
			if (i == Gauge_Icon)
			{
				GET_COMPONENT_FROM(m_pStrongHoldGauge[i], CTexture)->Remove_Texture(0);
				Read_Texture(m_pStrongHoldGauge[i], "/Paden", "Team");

				m_pStrongHoldGauge[i]->Set_Sort(0.5f);

				m_pStrongHoldGauge[i]->Set_PosY(325);
				m_pStrongHoldGauge[i]->Set_Scale(32.f);
			}
		}

		CREATE_GAMEOBJECT(m_pStrongHoldGauge[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pStrongHoldGauge[i]);

		for (int j = 0; j < 2; ++j)
		{
			m_pArrStrongHoldGauge[i][j] = m_pStrongHoldGauge[i]->Clone();

			if (i < Gauge_Icon)
			{
				_float fPosX = -125 + (j * 250.f);
				m_pArrStrongHoldGauge[i][j]->Set_PosX(fPosX);

				if (i == Gauge_Bar)
				{
					GET_COMPONENT_FROM(m_pArrStrongHoldGauge[i][j], CTexture)->Set_CurTextureIndex(j);
				}
			}
			else
			{
				if (i == Gauge_Icon)
				{
					GET_COMPONENT_FROM(m_pArrStrongHoldGauge[i][j], CTexture)->Set_CurTextureIndex(j);

					_float fPosX = -60.f + (j * 120.f);
					m_pArrStrongHoldGauge[i][j]->Set_PosX(fPosX);
				}
			}

			CREATE_GAMEOBJECT(m_pArrStrongHoldGauge[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrStrongHoldGauge[i][j]);
		}
	}
}

void CUI_Paden::Create_GaugeNum()
{
	for (int i = 0; i < Num_End; ++i)
	{
		m_pGaugeNum[i] = CUI_Object::Create();

		GET_COMPONENT_FROM(m_pGaugeNum[i], CTexture)->Remove_Texture(0);
		Read_Texture(m_pGaugeNum[i], "/Number", "Num");

		m_pGaugeNum[i]->Set_PosY(323.f);
		m_pGaugeNum[i]->Set_Scale(35.f, 41.f);
		m_pGaugeNum[i]->Set_Sort(0.5f);

		m_pGaugeNum[i]->Set_FadeDesc(m_fGaugeNumFadeSpeed);

		CREATE_GAMEOBJECT(m_pGaugeNum[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pGaugeNum[i]);

		for (int j = 0; j < 2; ++j)
		{
			m_pGauge_Num[j][i] = m_pGaugeNum[i]->Clone();

			_float fPosX = -112.f + (i * 12.f) + (j * 200.f);
			m_pGauge_Num[j][i]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pGauge_Num[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pGauge_Num[j][i]);
		}
	}
}

void CUI_Paden::Create_StrongHoldUI()
{
	for (int i = 0; i < SU_End; ++i)
	{
		m_pStrongHoldUI[i] = CUI_Object::Create();

		m_pStrongHoldUI[i]->Set_PosY(m_fPointUIPosY);

		if (i == SU_BG)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Paden", "Gauge");
			m_pStrongHoldUI[i]->Set_Color(m_vColorBG);

			m_pStrongHoldUI[i]->Set_Scale(28.f);
			m_pStrongHoldUI[i]->Set_Sort(0.5f);
		}
		else if (i == SU_Gauge)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Paden", "Gauge");
			m_pStrongHoldUI[i]->Set_Color(m_vColorGauge);

			m_pStrongHoldUI[i]->Set_Scale(25.f);
			m_pStrongHoldUI[i]->Set_Sort(0.49f);

			m_pStrongHoldUI[i]->Set_FontRender(true);
			m_pStrongHoldUI[i]->Set_FontStyle(true);
			m_pStrongHoldUI[i]->Set_FontCenter(true);
			m_pStrongHoldUI[i]->Set_FontScale(0.25f);
			m_pStrongHoldUI[i]->Set_FontOffset(5.5f, 4.5f);
		}
		else if (SU_Outline)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Paden", "Outline");
			m_pStrongHoldUI[i]->Set_Color(m_vColorOutline);

			m_pStrongHoldUI[i]->Set_Scale(33.f);
			m_pStrongHoldUI[i]->Set_Sort(0.48f);
		}
		else if (SU_Icon)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Paden/PointIcon_Join.dds"));
			m_pStrongHoldUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 1.f));

			m_pStrongHoldUI[i]->Set_Scale(22.f);
			m_pStrongHoldUI[i]->Set_Sort(0.f);
		}

		CREATE_GAMEOBJECT(m_pStrongHoldUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pStrongHoldUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrStrongHoldUI[i][j] = m_pStrongHoldUI[i]->Clone();

			_float fPosX = -50.f + (j * 50.f);
			m_pArrStrongHoldUI[i][j]->Set_PosX(fPosX);

			if (j == 0)
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}
			else
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldUI[i][j], CTexture)->Set_CurTextureIndex(1);
			}

			CREATE_GAMEOBJECT(m_pArrStrongHoldUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[i][j]);
		}
	}
}

void CUI_Paden::Init_StrongHoldUI()
{
	m_pArrStrongHoldUI[SU_Gauge][0]->Set_FontText(TEXT("A"));
	m_pArrStrongHoldUI[SU_Gauge][1]->Set_FontText(TEXT("R"));
	m_pArrStrongHoldUI[SU_Gauge][2]->Set_FontText(TEXT("C"));
}

void CUI_Paden::Create_Proj_StrongHoldUI()
{
	for (int i = 0; i < 3; ++i)
	{
		m_pProj_StrongHoldUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pProj_StrongHoldUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pProj_StrongHoldUI[i]);
	}
}
