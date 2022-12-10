#include "CUI_Paden.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include <CUtility_Transform.h>
#include "CShader.h"
#include "Loading_Manager.h"

HRESULT CUI_Paden::Initialize_Prototype()
{
	Create_InGameTimer();

	Create_ScoreNum();
	Create_ScoreGauge();

	Create_PointUI();
	Init_PointUI();

	return S_OK;
}

HRESULT CUI_Paden::Start()
{
	__super::Start();

	Bind_Shader();

	m_pInGameTimer->SetActive(true);
	SetActive_ScoreGauge(true);
	SetActive_ScoreNum(true);
	SetActive_PointUI(true);

	return S_OK;
}

void CUI_Paden::Set_Shader_PointGauge_A(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_A], sizeof(_float));
}

void CUI_Paden::Set_Shader_PointGauge_R(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_R], sizeof(_float));
}

void CUI_Paden::Set_Shader_PointGauge_C(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_C], sizeof(_float));
}

void CUI_Paden::Set_ScoreNum(_uint iTeamType, _uint iScore)
{
	m_eTeamType = (TeamType)iTeamType;

	for (int i = 2; i >= 0; --i)
	{
		m_iPrvScore[m_eTeamType][i] = m_iCurvScore[m_eTeamType][i];

		_uint iDigitDmg = iScore % 10;
		m_iCurvScore[m_eTeamType][i] = iDigitDmg;
		iScore /= 10;

		if (m_iPrvScore[m_eTeamType][i] != m_iCurvScore[m_eTeamType][i])
		{
			m_iChangeNumIdx[i] = i;

			m_bIsChangeNum = true;
			m_bIsDisableNum = true;
		}
	}
}

void CUI_Paden::Set_ConquestTime(_float fConquestTime, _float fMaxConquestTime)
{
	m_fConquestTime = fConquestTime;
	m_fMaxConquestTime = fMaxConquestTime;
}

void CUI_Paden::Set_Proj_StrongHoldUI(_uint iPointIdx, CTransform* pTransform)
{
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform);
	vNewPos.y += 1.f;

	/*for (int i = 0; i < PP_End; ++i)
	{
		m_pArrProj_StrongHoldUI[i][iPointIdx]->Set_Pos(vNewPos);
	}*/
}

void CUI_Paden::SetActive_ScoreGauge(_bool value)
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			m_pArrScoreGauge[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::SetActive_ScoreNum(_bool value)
{
	for (int i = 0; i < Team_End; ++i)
	{
		for (int j = 0; j < Num_End; ++j)
		{
			m_pArrScoreNum[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::SetActive_PointUI(_bool value)
{
	for (int i = 0; i < Point_End; ++i)
	{
		for (int j = 0; j < PU_End; ++j)
		{
			m_pArrPointUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::Interact_PointUI(string strPadenPointKey, _uint iTeamType, _uint iTriggerState)
{
	_float fDuration = 0.3f;

	for (int i = 0; i < PU_End; ++i)
	{
		switch (iTriggerState)
		{
		case TS_Enter:

			if (strPadenPointKey == "Paden_Trigger_A")
			{
				m_pArrPointUI[Point_A][i]->DoScale(10.f, fDuration);
				m_pArrPointUI[Point_A][i]->DoMove(0.f, 200.f, fDuration);

				Set_PointGauge_Color(iTeamType, Point_A);
			}
			else if (strPadenPointKey == "Paden_Trigger_R")
			{
				m_pArrPointUI[Point_R][i]->DoScale(10.f, fDuration);
				m_pArrPointUI[Point_R][i]->DoMove(0.f, 200.f, fDuration);

				Set_PointGauge_Color(iTeamType, Point_R);
			}
			else if (strPadenPointKey == "Paden_Trigger_C")
			{
				m_pArrPointUI[Point_C][i]->DoScale(10.f, fDuration);
				m_pArrPointUI[Point_C][i]->DoMove(0.f, 200.f, fDuration);

				Set_PointGauge_Color(iTeamType, Point_C);
			}

			break;

		case TS_Exit:

			if (strPadenPointKey == "Paden_Trigger_A")
			{
				m_pArrPointUI[Point_A][i]->DoScale(-10.f, fDuration);
				m_pArrPointUI[Point_A][i]->DoMove(m_fMainPointUIPosX, m_fPointUIPosY, fDuration);
			}
			else if (strPadenPointKey == "Paden_Trigger_R")
			{
				m_pArrPointUI[Point_R][i]->DoScale(-10.f, fDuration);
				m_pArrPointUI[Point_R][i]->DoMove(m_fMainPointUIPosX, m_fPointUIPosY, fDuration);
			}
			else if (strPadenPointKey == "Paden_Trigger_C")
			{
				m_pArrPointUI[Point_C][i]->DoScale(-10.f, fDuration);
				m_pArrPointUI[Point_C][i]->DoMove(m_fMainPointUIPosX, m_fPointUIPosY, fDuration);
			}

			break;
		}
	}
}

void CUI_Paden::Set_PointGauge_Color(_uint iTeamType, PointName ePointName)
{
	switch (iTeamType)
	{
	case 3:
		m_pArrPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorBlue);
		break;

	case 13:
		m_pArrPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorRed);
		break;
	}
}

void CUI_Paden::My_Tick()
{
	__super::My_Tick();

	Update_InGameTimer();

	Set_PointTextPosY();

	for (int i = 0; i < Point_End; ++i)
	{
		_float fConquestRatio = m_fConquestTime / m_fMaxConquestTime;
		if (m_fConquestTime >= m_fMaxConquestTime)
			fConquestRatio = 1.f;

		m_fConquestRatio[i] = 1 - fConquestRatio;
	}

	if (KEY(X, TAP))
	{
		m_iCurScore--;
	}

	for (int i = 0; i < Team_End; ++i)
	{
		Set_ScoreNum(i, m_iCurScore);
	}

	if (m_bIsChangeNum)
	{
		if (m_bIsDisableNum)
		{
			m_bIsDisableNum = false;

			//Disable_Fade(m_pArrScoreNum[m_eTeamType][m_iChangeNumIdx], 0.25f);

			m_bIsEnableNum = true;
		}
		else if (m_bIsEnableNum)
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > 0.25f)
			{
				m_fAccTime = 0.f;

				for (int i = 0; i < 3; ++i)
				{
					GET_COMPONENT_FROM(m_pArrScoreNum[m_eTeamType][m_iChangeNumIdx[i]], CTexture)->Set_CurTextureIndex(m_iCurvScore[m_eTeamType][m_iChangeNumIdx[i]]);
					Enable_Fade(m_pArrScoreNum[m_eTeamType][m_iChangeNumIdx[i]], 0.25f);
				}

				m_bIsEnableNum = false;
				m_bIsChangeNum = false;
			}
		}
	}
}

void CUI_Paden::OnEnable()
{
	__super::OnEnable();

	m_pInGameTimer->SetActive(true);
	SetActive_ScoreGauge(true);
	SetActive_ScoreNum(true);
	SetActive_PointUI(true);
}

void CUI_Paden::OnDisable()
{
	__super::OnDisable();

	m_pInGameTimer->SetActive(false);
	SetActive_ScoreGauge(false);
	SetActive_ScoreNum(false);
	SetActive_PointUI(false);
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

void CUI_Paden::Update_InGameTimer()
{
	if (m_fInGameTime < 0.f)
	{
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
}

void CUI_Paden::Create_ScoreNum()
{
	for (int i = 0; i < Num_End; ++i)
	{
		m_pScoreNum[i] = CUI_Object::Create();

		m_pScoreNum[i]->Set_FadeDesc(0.25f, 0.25f, true);

		GET_COMPONENT_FROM(m_pScoreNum[i], CTexture)->Remove_Texture(0);
		Read_Texture(m_pScoreNum[i], "/Number", "Num");

		m_pScoreNum[i]->Set_PosY(323.f);
		m_pScoreNum[i]->Set_Scale(35.f, 41.f);
		m_pScoreNum[i]->Set_Sort(0.5f);

		m_pScoreNum[i]->Set_FadeDesc(m_fGaugeNumFadeSpeed);

		CREATE_GAMEOBJECT(m_pScoreNum[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pScoreNum[i]);

		for (int j = 0; j < Team_End; ++j)
		{
			m_pArrScoreNum[j][i] = m_pScoreNum[i]->Clone();

			_float fPosX = -112.f + (i * 12.f) + (j * 200.f);
			m_pArrScoreNum[j][i]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrScoreNum[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrScoreNum[j][i]);
		}
	}
}

void CUI_Paden::Create_ScoreGauge()
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		m_pScoreGauge[i] = CUI_Object::Create();

		if (i < Gauge_Icon)
		{
			m_pScoreGauge[i]->Set_PosY(300.f);
			m_pScoreGauge[i]->Set_Scale(150.f, 5.f);

			if (i == Gauge_BG)
			{
				m_pScoreGauge[i]->Set_Sort(0.5f);
			}
			else if (i == Gauge_Bar)
			{
				GET_COMPONENT_FROM(m_pScoreGauge[i], CTexture)->Remove_Texture(0);
				Read_Texture(m_pScoreGauge[i], "/Paden/TopGauge", "Bar");

				m_pScoreGauge[i]->Set_Sort(0.49f);
			}
		}
		else
		{
			if (i == Gauge_Icon)
			{
				GET_COMPONENT_FROM(m_pScoreGauge[i], CTexture)->Remove_Texture(0);
				Read_Texture(m_pScoreGauge[i], "/Paden", "Team");

				m_pScoreGauge[i]->Set_Sort(0.5f);

				m_pScoreGauge[i]->Set_PosY(325);
				m_pScoreGauge[i]->Set_Scale(32.f);
			}
		}

		CREATE_GAMEOBJECT(m_pScoreGauge[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pScoreGauge[i]);

		for (int j = 0; j < 2; ++j)
		{
			m_pArrScoreGauge[i][j] = m_pScoreGauge[i]->Clone();

			if (i < Gauge_Icon)
			{
				_float fPosX = -125 + (j * 250.f);
				m_pArrScoreGauge[i][j]->Set_PosX(fPosX);

				if (i == Gauge_Bar)
				{
					GET_COMPONENT_FROM(m_pArrScoreGauge[i][j], CTexture)->Set_CurTextureIndex(j);
				}
			}
			else
			{
				if (i == Gauge_Icon)
				{
					GET_COMPONENT_FROM(m_pArrScoreGauge[i][j], CTexture)->Set_CurTextureIndex(j);

					_float fPosX = -60.f + (j * 120.f);
					m_pArrScoreGauge[i][j]->Set_PosX(fPosX);
				}
			}

			CREATE_GAMEOBJECT(m_pArrScoreGauge[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrScoreGauge[i][j]);
		}
	}
}

void CUI_Paden::Create_PointUI()
{
	for (int i = 0; i < PU_End; ++i)
	{
		m_pPointUI[i] = CUI_Object::Create();

		m_pPointUI[i]->Set_PosY(m_fPointUIPosY);

		if (i == PU_Point)
		{
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Paden", "Point");
			m_pPointUI[i]->Set_Color(m_vColorOutline);

			m_pPointUI[i]->Set_Scale(40.f);
			m_pPointUI[i]->Set_Sort(0.5f);
		}
		else if (i == PU_Gauge)
		{
			GET_COMPONENT_FROM(m_pPointUI[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);

			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Paden", "Gauge");
			m_pPointUI[i]->Set_Color(m_vColorGauge);

			m_pPointUI[i]->Set_Scale(30.f);
			m_pPointUI[i]->Set_Sort(0.49f);
		}
		else if (i == PU_Text)
		{
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Paden/Text", "Text");

			m_pPointUI[i]->Set_Scale(50.f);
			m_pPointUI[i]->Set_Sort(0.48f);
		}

		CREATE_GAMEOBJECT(m_pPointUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPointUI[i]);

		for (int j = 0; j < Point_End; ++j)
		{
			m_pArrPointUI[j][i] = m_pPointUI[i]->Clone();

			_float fPosX = -50.f + (j * 50.f);
			m_pArrPointUI[j][i]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrPointUI[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrPointUI[j][i]);
		}
	}
}

void CUI_Paden::Init_PointUI()
{
	for (int i = 0; i < Point_End; ++i)
	{
		for (int j = 0; j < PU_End; ++j)
		{
			if (i == Point_A)
			{
				GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(i);
			}
			else
			{
				GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(i);

				if (j == PU_Gauge)
				{
					GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
				}
			}
		}
	}
}

void CUI_Paden::Set_PointTextPosY()
{
	for (int i = 0; i < Point_End; ++i)
	{
		_float fPosY = m_pArrPointUI[i][PU_Point]->Get_PosY();
		_float fTextPosY = fPosY - 2.5f;
		m_pArrPointUI[i][PU_Text]->Set_PosY(fTextPosY);
	}
}

void CUI_Paden::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pArrPointUI[PU_Gauge][Point_A], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_A, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrPointUI[PU_Gauge][Point_R], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_R, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrPointUI[PU_Gauge][Point_C], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_C, this, placeholders::_1, "g_fValue");
}