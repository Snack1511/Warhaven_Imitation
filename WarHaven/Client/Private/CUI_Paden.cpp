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

	for (int i = 0; i < Team_End; ++i)
	{
		GET_COMPONENT_FROM(m_pArrScoreNum[i][0], CTexture)->Set_CurTextureIndex(1);
		GET_COMPONENT_FROM(m_pArrScoreNum[i][1], CTexture)->Set_CurTextureIndex(0);
		GET_COMPONENT_FROM(m_pArrScoreNum[i][2], CTexture)->Set_CurTextureIndex(0);

		for (int j = 0; j < Num_End; ++j)
		{
			Enable_Fade(m_pArrScoreNum[i][j], m_fScoreFadeSpeed);
		}
	}

	OnEnable();

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

void CUI_Paden::Set_Shader_SocreGauge_Red(CShader* pShader, const char* pConstName)
{
	m_fScoreRatio[Team_Red] = 1 - m_fScoreRatio[Team_Red];
	pShader->Set_RawValue("g_fValue", &m_fScoreRatio[Team_Red], sizeof(_float));

	_bool bFlip = true;
	pShader->Set_RawValue("bFlip", &bFlip, sizeof(_bool));
}

void CUI_Paden::Set_Shader_SocreGauge_Blue(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fScoreRatio[Team_Blue], sizeof(_float));

	_bool bFlip = false;
	pShader->Set_RawValue("bFlip", &bFlip, sizeof(_bool));
}

void CUI_Paden::Set_Score(_uint iTeamType, _uint iScore, _uint iMaxScore)
{
	m_iScore[iTeamType] = iScore;
	m_fScoreRatio[iTeamType] = (_float)iScore / (_float)iMaxScore;
}

void CUI_Paden::Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime)
{
	_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);

	if (strPadenPointKey == "Paden_Trigger_A")
	{
		m_fConquestRatio[Point_A] = fConquestRatio;
	}
	else if (strPadenPointKey == "Paden_Trigger_R")
	{
		m_fConquestRatio[Point_R] = fConquestRatio;
	}
	else if (strPadenPointKey == "Paden_Trigger_C")
	{
		m_fConquestRatio[Point_C] = fConquestRatio;
	}
}

void CUI_Paden::Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum)
{
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform);
	vNewPos.y += 2.f;

	for (int i = 0; i < PU_End; ++i)
	{
		m_pArrProjPointUI[iPointIdx][i]->SetActive(isInFrustum);
		if (!m_pArrProjPointUI[iPointIdx][i]->Is_Valid())
			return;

		if (i == PU_Text)
		{
			m_pArrProjPointUI[iPointIdx][i]->Set_PosX(vNewPos.x);
			continue;
		}

		m_pArrProjPointUI[iPointIdx][i]->Set_Pos(vNewPos);
	}
}

void CUI_Paden::SetActive_ScoreGauge(_bool value)
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		for (int j = 0; j < Team_End; ++j)
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
			m_pArrProjPointUI[i][j]->SetActive(value);
		}
	}
}

void CUI_Paden::Conquest_PointUI(string strPointName, _uint iTeamType)
{
	_float4 vColor;

	switch (iTeamType)
	{
	case 13:
		vColor = m_vColorRed;
		break;

	case 3:
		vColor = m_vColorBlue;
		break;
	}

	for (int i = 0; i < PU_Text; ++i)
	{
		if (strPointName == "Paden_Trigger_A")
		{
			m_pArrPointUI[Point_A][i]->Set_Color(vColor);
			m_pArrProjPointUI[Point_A][i]->Set_Color(vColor);
		}
		else if (strPointName == "Paden_Trigger_R")
		{
			m_pArrPointUI[Point_R][i]->Set_Color(vColor);
			m_pArrProjPointUI[Point_R][i]->Set_Color(vColor);
		}
		else if (strPointName == "Paden_Trigger_C")
		{
			m_pArrPointUI[Point_C][i]->Set_Color(vColor);
			m_pArrProjPointUI[Point_C][i]->Set_Color(vColor);
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
				m_pArrPointUI[Point_A][i]->DoMove(-50.f, m_fPointUIPosY, fDuration);
			}
			else if (strPadenPointKey == "Paden_Trigger_R")
			{
				m_pArrPointUI[Point_R][i]->DoScale(-10.f, fDuration);
				m_pArrPointUI[Point_R][i]->DoMove(0.f, m_fPointUIPosY, fDuration);
			}
			else if (strPadenPointKey == "Paden_Trigger_C")
			{
				m_pArrPointUI[Point_C][i]->DoScale(-10.f, fDuration);
				m_pArrPointUI[Point_C][i]->DoMove(50.f, m_fPointUIPosY, fDuration);
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
		m_pArrProjPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorBlue);
		break;

	case 13:
		m_pArrPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorRed);
		m_pArrProjPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorRed);
		break;
	}
}

void CUI_Paden::My_Tick()
{
	__super::My_Tick();

	Update_InGameTimer();
	Update_Score();
}

void CUI_Paden::My_LateTick()
{
	__super::My_LateTick();

	Set_PointTextPosY();
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
	m_pScoreNum = CUI_Object::Create();

	GET_COMPONENT_FROM(m_pScoreNum, CTexture)->Remove_Texture(0);
	Read_Texture(m_pScoreNum, "/Number", "Num");

	m_pScoreNum->Set_FadeDesc(m_fScoreFadeSpeed);

	m_pScoreNum->Set_PosY(323.f);
	m_pScoreNum->Set_Scale(35.f, 41.f);
	m_pScoreNum->Set_Sort(0.5f);

	CREATE_GAMEOBJECT(m_pScoreNum, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pScoreNum);

	for (int i = 0; i < Team_End; ++i)
	{
		for (int j = 0; j < Num_End; ++j)
		{
			m_pArrScoreNum[i][j] = m_pScoreNum->Clone();

			_float fPosX = -112.f + (j * 12.f) + (i * 200.f);
			m_pArrScoreNum[i][j]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrScoreNum[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrScoreNum[i][j]);
		}
	}
}

void CUI_Paden::Update_Score()
{
	// 100 10 �ڸ� ���ڰ� 0�� �Ǹ� disable

	// 100 ���� �ް�
	// �ش� ������ 1 0 0 ����

	// �ش� �ϴ� ���ڸ� idx�� ���� �ؽ�ó �ҷ�����

	// 100 �� �ҷ�����

	for (int i = 0; i < Team_End; ++i)
	{
		m_vecPrvScore[i].clear();

		if (!m_vecCurScore[i].empty())
		{
			m_vecPrvScore[i] = m_vecCurScore[i];
		}

		m_vecCurScore[i].clear();
		while (m_iRemainderCnt[i] < 3)
		{
			_uint iDigitNum = m_iScore[i] % 10;
			m_vecCurScore[i].push_back(iDigitNum);
			m_iScore[i] /= 10;

			m_iRemainderCnt[i]++;
		}

		m_iRemainderCnt[i] = 0;
		reverse(m_vecCurScore[i].begin(), m_vecCurScore[i].end());

		for (int j = 0; j < Num_End; ++j)
		{
			if (m_vecPrvScore[i].empty())
				return;

			if (m_vecPrvScore[i][j] != m_vecCurScore[i][j])
			{
				GET_COMPONENT_FROM(m_pArrScoreNum[i][j], CTexture)->Set_CurTextureIndex(m_vecCurScore[i][j]);

				if (j < Num2)
				{
					_uint iTextureNum = GET_COMPONENT_FROM(m_pArrScoreNum[i][j], CTexture)->Get_CurTextureIndex();
					if (iTextureNum == 0)
					{
						Disable_Fade(m_pArrScoreNum[i][j], m_fScoreFadeSpeed);
					}
				}

				Enable_Fade(m_pArrScoreNum[i][j], m_fScoreFadeSpeed);
			}
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
				GET_COMPONENT_FROM(m_pScoreGauge[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HorizontalGauge);

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
		DISABLE_GAMEOBJECT(m_pScoreGauge[i]);

		for (int j = 0; j < Team_End; ++j)
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
			m_pPointUI[i]->Set_Sort(0.51f);
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
			m_pArrProjPointUI[j][i] = m_pPointUI[i]->Clone();

			_float fPosX = -50.f + (j * 50.f);
			m_pArrPointUI[j][i]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrPointUI[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrPointUI[j][i]);

			CREATE_GAMEOBJECT(m_pArrProjPointUI[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrProjPointUI[j][i]);
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
				GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(i);
			}
			else
			{
				GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(i);
				GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(i);

				if (j == PU_Gauge)
				{
					GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
					GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
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

		_float fProjPos = m_pArrProjPointUI[i][PU_Point]->Get_PosY();
		_float fTextProjPosY = fProjPos - 2.5f;
		m_pArrProjPointUI[i][PU_Text]->Set_PosY(fTextProjPosY);
	}
}

void CUI_Paden::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pArrScoreGauge[Gauge_Bar][Team_Red], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_SocreGauge_Red, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrScoreGauge[Gauge_Bar][Team_Blue], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_SocreGauge_Blue, this, placeholders::_1, "g_fValue");

	GET_COMPONENT_FROM(m_pArrPointUI[Point_A][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_A, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrProjPointUI[Point_A][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_A, this, placeholders::_1, "g_fValue");

	GET_COMPONENT_FROM(m_pArrPointUI[Point_R][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_R, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrProjPointUI[Point_R][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_R, this, placeholders::_1, "g_fValue");

	GET_COMPONENT_FROM(m_pArrPointUI[Point_C][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_C, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrProjPointUI[Point_C][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_PointGauge_C, this, placeholders::_1, "g_fValue");

}
