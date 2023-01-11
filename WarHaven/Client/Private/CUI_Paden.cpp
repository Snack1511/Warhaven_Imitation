#include "CUI_Paden.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include <CUtility_Transform.h>
#include "CShader.h"
#include "Loading_Manager.h"
#include "CUser.h"
#include "CPlayer.h"
#include "CUnit.h"
#include "CTeamConnector.h"
#include "Transform.h"
#include "Camera.h"

HRESULT CUI_Paden::Initialize_Prototype()
{
	Create_InGameTimer();

	Create_ScoreNum();
	Create_ScoreGauge();
	Create_HwaraGauge();

	Create_PointUI();
	Init_PointUI();

	Create_TargetPointUI();

	Create_Popup();

	return S_OK;
}

HRESULT CUI_Paden::Start()
{
	__super::Start();

	Init_HwaraGauge();

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

void CUI_Paden::Set_Shader_HwaraArrow_Blue(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fUVPlusY", &m_fUVTexY[Team_Blue], sizeof(_float));

	cout << m_fUVTexY[Team_Blue] << endl;
}

void CUI_Paden::Set_Shader_HwaraArrow_Red(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fUVPlusY", &m_fUVTexY[Team_Red], sizeof(_float));
}

void CUI_Paden::Set_Team(CTeamConnector* pAllyTeam, CTeamConnector* pEnemyTeam)
{
	m_pAllyTeam = pAllyTeam;
	m_pEnemyTeam = pEnemyTeam;
}

void CUI_Paden::Set_TargetPointPos(_uint iTargetIdx)
{
	m_eTargetPoint = (PointName)iTargetIdx;

	m_bSetTargetPoint = true;
}

void CUI_Paden::Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime)
{
	_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);

	if (strPadenPointKey == "Paden_Trigger_A" || strPadenPointKey == "Hwara_Center")
	{
		m_fConquestRatio[Point_A] = fConquestRatio;
	}
	else if (strPadenPointKey == "Paden_Trigger_R" || strPadenPointKey == "Hwara_Respawn")
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
	vNewPos.y += 5.f;

	if (m_bSetTargetPoint)
		m_pArrTargetPoint[1]->SetActive(isInFrustum);

	for (int i = 0; i < PU_End; ++i)
	{
		m_pArrProjPointUI[iPointIdx][i]->SetActive(isInFrustum);

		m_pArrProjPointUI[iPointIdx][i]->Set_Pos(vNewPos);
	}

	return;

	if (isInFrustum)
	{
		_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform);
		vNewPos.y += 5.f;

		for (int i = 0; i < PU_End; ++i)
		{
			if (!m_pArrProjPointUI[iPointIdx][i]->Is_Valid())
				m_pArrProjPointUI[iPointIdx][i]->SetActive(true);

			m_pArrProjPointUI[iPointIdx][i]->Set_Pos(vNewPos);
		}
	}
	else
	{
		for (int i = 0; i < PU_End; ++i)
		{
			if (m_pArrProjPointUI[iPointIdx][i]->Is_Valid())
				m_pArrProjPointUI[iPointIdx][i]->SetActive(false);
		}

		return;

		if (KEY(Z, TAP))
			m_bIsVector = !m_bIsVector;

		if (m_bIsVector)
		{
			cout << "벡터" << endl;
			Update_IndicatorVector(pTransform);
		}
		else
		{
			cout << "각도" << endl;
			Update_IndicatorAngle(pTransform);
		}
	}
}

void CUI_Paden::Set_TargetTransform(CTransform* pTargetTransform)
{
	if (!m_bSetTargetPoint)
		return;

	m_pTargetTransform = pTargetTransform;
}

void CUI_Paden::SetActive_ScoreGauge(_bool value)
{
	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_PADEN)
	{
		for (int i = 0; i < Gauge_End; ++i)
		{
			for (int j = 0; j < Team_End; ++j)
			{
				m_pArrScoreGauge[i][j]->SetActive(value);
			}
		}
	}
	else
	{
		for (int i = 0; i < Team_End; ++i)
		{
			for (int j = 0; j < Hwara_End; ++j)
			{
				if (m_pArrHwaraGauge[i][j])
					m_pArrHwaraGauge[i][j]->SetActive(value);
			}
		}
	}
}

void CUI_Paden::SetActive_ScoreNum(_bool value)
{
	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_PADEN)
	{
		for (int i = 0; i < Team_End; ++i)
		{
			for (int j = 0; j < Num_End; ++j)
			{
				m_pArrScoreNum[i][j]->SetActive(value);
			}
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

			if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_HWARA)
				m_pArrProjPointUI[Point_C][j]->SetActive(false);
		}
	}
}

void CUI_Paden::SetActive_TargetPoint(_bool value)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pArrTargetPoint[i]->SetActive(true);
	}
}

void CUI_Paden::Conquest_PointUI(string strPointName, _bool bIsMainPlayerTeam)
{
	_float4 vColor;

	if (bIsMainPlayerTeam)
	{
		vColor = m_vColorBlue;
	}
	else
	{
		vColor = m_vColorRed;
	}

	for (int i = 0; i < PU_Text; ++i)
	{
		if (strPointName == "Paden_Trigger_A" || strPointName == "Hwara_Center")
		{
			m_pArrPointUI[Point_A][i]->Set_Color(vColor);
			m_pArrProjPointUI[Point_A][i]->Set_Color(vColor);
		}
		else if (strPointName == "Paden_Trigger_R" || strPointName == "Hwara_Respawn")
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

void CUI_Paden::Move_PointUI(string strPadenPointKey, _uint iTriggerState)
{
	_float fDuration = 0.3f;

	for (int i = 0; i < PU_End; ++i)
	{
		switch (iTriggerState)
		{
		case TS_Enter:

			if (strPadenPointKey == "Paden_Trigger_A" || strPadenPointKey == "Hwara_Center")
			{
				m_pArrPointUI[Point_A][i]->DoScale(10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_A][i]->Get_Pos();
				vPos.x = 0.f;
				vPos.y = 200.f;
				m_pArrPointUI[Point_A][i]->DoMove(vPos, fDuration, 0.f);
			}
			else if (strPadenPointKey == "Paden_Trigger_R" || strPadenPointKey == "Hwara_Respawn")
			{
				m_pArrPointUI[Point_R][i]->DoScale(10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_R][i]->Get_Pos();
				vPos.x = 0.f;
				vPos.y = 200.f;
				m_pArrPointUI[Point_R][i]->DoMove(vPos, fDuration, 0.f);
			}
			else if (strPadenPointKey == "Paden_Trigger_C")
			{
				m_pArrPointUI[Point_C][i]->DoScale(10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_C][i]->Get_Pos();
				vPos.x = 0.f;
				vPos.y = 200.f;
				m_pArrPointUI[Point_C][i]->DoMove(vPos, fDuration, 0.f);
			}

			break;

		case TS_Exit:

			if (strPadenPointKey == "Paden_Trigger_A" || strPadenPointKey == "Hwara_Center")
			{
				m_pArrProjPointUI[Point_A][i]->SetActive(true);

				m_pArrPointUI[Point_A][i]->DoScale(-10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_A][i]->Get_Pos();
				vPos.x -= 50.f;
				vPos.y = m_fPointUIPosY;
				m_pArrPointUI[Point_A][i]->DoMove(vPos, fDuration, 0);

			}
			else if (strPadenPointKey == "Paden_Trigger_R" || strPadenPointKey == "Hwara_Respawn")
			{
				m_pArrProjPointUI[Point_R][i]->SetActive(true);

				m_pArrPointUI[Point_R][i]->DoScale(-10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_R][i]->Get_Pos();
				vPos.y = m_fPointUIPosY;
				m_pArrPointUI[Point_R][i]->DoMove(vPos, fDuration, 0);

			}
			else if (strPadenPointKey == "Paden_Trigger_C")
			{
				m_pArrProjPointUI[Point_C][i]->SetActive(true);

				m_pArrPointUI[Point_C][i]->DoScale(-10.f, fDuration);

				_float4 vPos = m_pArrPointUI[Point_C][i]->Get_Pos();
				vPos.x += 50.f;
				vPos.y = m_fPointUIPosY;
				m_pArrPointUI[Point_C][i]->DoMove(vPos, fDuration, 0);

			}

			break;
		}
	}
}

void CUI_Paden::Enable_Popup(_uint iIndex)
{
	GET_COMPONENT_FROM(m_pPopupUI, CTexture)->Set_CurTextureIndex(iIndex);
	Enable_Fade(m_pPopupUI, 0.3f);
}

void CUI_Paden::Interact_PointUI(_bool bIsMainPlayerTeam, string strPadenPointKey)
{
	_float fDuration = 0.3f;

	for (int i = 0; i < PU_End; ++i)
	{
		if (strPadenPointKey == "Paden_Trigger_A" || strPadenPointKey == "Hwara_Center")
		{
			Set_PointGauge_Color(bIsMainPlayerTeam, Point_A);
		}
		else if (strPadenPointKey == "Paden_Trigger_R" || strPadenPointKey == "Hwara_Respawn")
		{
			Set_PointGauge_Color(bIsMainPlayerTeam, Point_R);
		}
		else if (strPadenPointKey == "Paden_Trigger_C")
		{
			Set_PointGauge_Color(bIsMainPlayerTeam, Point_C);
		}
	}
}

void CUI_Paden::Set_PointGauge_Color(_bool bIsMainPlayerTeam, PointName ePointName)
{
	if (bIsMainPlayerTeam)
	{
		m_pArrPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorBlue);
		m_pArrProjPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorBlue);
	}
	else
	{
		m_pArrPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorRed);
		m_pArrProjPointUI[ePointName][PU_Gauge]->Set_Color(m_vColorRed);
	}
}

void CUI_Paden::My_Tick()
{
	__super::My_Tick();

	for (int i = 0; i < Team_End; ++i)
	{
		_float fScaleX = m_pArrHwaraGauge[i][Hwara_Arrow]->Get_Scale().x;
		m_fUVTexY[i] = (fScaleX * 0.01f) * 3.f;
	}

	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_TEST)
		return;

	Update_InGameTimer();
	Update_Score();
}

void CUI_Paden::My_LateTick()
{
	__super::My_LateTick();

	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_TEST)
		return;

	Set_PointTextPosY();
	Update_TargetPointPos();
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

void CUI_Paden::Update_IndicatorVector(CTransform* pTargetTransform)
{
	_float fMaxPosX = 100.f;
	_float fMaxPosY = 100.f;

	_float4 vTargetPos = pTargetTransform->Get_World(WORLD_POS);
	vTargetPos.y = 0.f;

	CTransform* pCamTransform = GAMEINSTANCE->Get_CurCam()->Get_Transform();
	_float4 vCamPos = pCamTransform->Get_World(WORLD_POS);
	vCamPos.y = 0.f;

	_float4 vCamToTargetDir = vTargetPos - vCamPos;

	_float4 vCamLook = pCamTransform->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;

	_float fLookLength = vCamToTargetDir.Dot(vCamLook);

	vCamLook = vCamLook.Normalize() * fLookLength;

	_float4 vLookPos = vCamPos + vCamLook;

	_float4 vDir = vTargetPos - vLookPos;
	vDir.y = 0.f;
	vDir.Normalize();

	_float fIndicatorPosX = vDir.x * fMaxPosX;
	_float fIndicatorPosY = vDir.z * fMaxPosY;

	if (m_bSetTargetPoint)
	{
		m_pArrTargetPoint[1]->Set_Pos(fIndicatorPosX, fIndicatorPosY);

		for (int i = 0; i < PU_End; ++i)
		{
			m_pArrProjPointUI[m_eTargetPoint][i]->Set_Pos(fIndicatorPosX, fIndicatorPosY);
			m_pArrProjPointUI[m_eTargetPoint][i]->SetActive(true);
		}
	}
}

void CUI_Paden::Update_IndicatorAngle(CTransform* pTargetTransform)
{
	_float fMaxPosX = 100.f;
	_float fMaxPosY = 100.f;

	_float4 vTargetPos = pTargetTransform->Get_World(WORLD_POS);
	vTargetPos.y = 0.f;

	CTransform* pCamTransform = GAMEINSTANCE->Get_CurCam()->Get_Transform();
	_float4 vCamPos = pCamTransform->Get_World(WORLD_POS);
	vCamPos.y = 0.f;

	_float4 vCamToTargetDir = vTargetPos - vCamPos;
	vCamToTargetDir.Normalize();
	vCamToTargetDir.y = 0.f;

	_float4 vCamLook = pCamTransform->Get_World(WORLD_LOOK);
	vCamLook.Normalize();
	vCamLook.y = 0.f;

	_float fDot = vCamToTargetDir.Dot(vCamLook);
	_float fRadian = acosf(fDot);
	_float fAngle = XMConvertToDegrees(acosf(fDot));

	// fAngle이 0보다 크면 앞에, 작으면 뒤에
	// 
	// 	(-cos, sin)	ㅣ(cos, sin)
	//	ㅡㅡㅡㅡㅡㅡㅣㅡㅡㅡㅡㅡㅡ
	// 	(-cos, -sin)ㅣ(cos, -sin)

	_float fPosX = cosf(fRadian);
	_float fPosY = fPosX > 0.f ? sinf(fRadian) : -sinf(fRadian);

	_float fIndicatorPosX = fPosX * fMaxPosX;
	_float fIndicatorPosY = fPosY * fMaxPosY;

	if (m_bSetTargetPoint)
	{
		m_pArrTargetPoint[1]->Set_Pos(fIndicatorPosX, fIndicatorPosY);

		for (int i = 0; i < PU_End; ++i)
		{
			m_pArrProjPointUI[m_eTargetPoint][i]->Set_Pos(fIndicatorPosX, fIndicatorPosY);
			m_pArrProjPointUI[m_eTargetPoint][i]->SetActive(true);
		}
	}
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
	m_pInGameTimer->Set_FontScale(0.3f);

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

		if (iMin >= 29)
		{
			if (iSec < 59)
			{
				if (!m_bShowInfoUI)
				{
					m_bShowInfoUI = true;
					CUser::Get_Instance()->SetActive_InfoUI(true);
				}
			}

			if (iSec < 57)
			{
				if (!m_bShowStartPopup)
				{
					m_bShowStartPopup = true;
					Enable_Popup(0);
				}
			}
		}
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
	_uint iAllyCurScore = m_pAllyTeam->Get_Score();
	_uint iAllyMaxScore = m_pAllyTeam->Get_MaxScore();

	_uint iEnemyCurScore = m_pEnemyTeam->Get_Score();
	_uint iEnemyMaxScore = m_pEnemyTeam->Get_MaxScore();

	m_iScore[Team_Red] = iAllyCurScore;
	m_fScoreRatio[Team_Red] = (_float)iAllyCurScore / (_float)iAllyMaxScore;

	m_iScore[Team_Blue] = iEnemyCurScore;
	m_fScoreRatio[Team_Blue] = (_float)iEnemyCurScore / (_float)iEnemyMaxScore;

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
				m_pScoreGauge[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.5f));

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
			m_pPointUI[i]->Set_Sort(0.55f);
		}
		else if (i == PU_Gauge)
		{
			GET_COMPONENT_FROM(m_pPointUI[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_CircleGauge);

			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Paden", "Gauge");
			m_pPointUI[i]->Set_Color(m_vColorGauge);

			m_pPointUI[i]->Set_Scale(30.f);
			m_pPointUI[i]->Set_Sort(0.56f);
		}
		else if (i == PU_Text)
		{
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Paden/Text", "Text");

			m_pPointUI[i]->Set_Scale(50.f);
			m_pPointUI[i]->Set_Sort(0.54f);
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
			if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_HWARA)
			{
				if (j == PU_Text)
				{
					GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(i);
					GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(i);
				}
				else
				{
					if (i == Point_R)
					{
						GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
						GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
					}
					else
					{
						GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(2);
						GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(2);

						if (j == PU_Gauge)
						{
							GET_COMPONENT_FROM(m_pArrPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
							GET_COMPONENT_FROM(m_pArrProjPointUI[i][j], CTexture)->Set_CurTextureIndex(1);
						}
					}
				}
			}
			else
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

void CUI_Paden::Create_TargetPointUI()
{
	m_pTargetPoint = CUI_Object::Create();

	m_pTargetPoint->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_PingGreenLeader.dds"));

	m_pTargetPoint->Set_Scale(32.f);
	m_pTargetPoint->Set_Sort(0.45f);

	CREATE_GAMEOBJECT(m_pTargetPoint, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pTargetPoint);

	for (int i = 0; i < 2; ++i)
	{
		m_pArrTargetPoint[i] = m_pTargetPoint->Clone();

		CREATE_GAMEOBJECT(m_pArrTargetPoint[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrTargetPoint[i]);
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

	GET_COMPONENT_FROM(m_pArrHwaraGauge[Team_Blue][Hwara_Arrow], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_HwaraArrow_Blue, this, placeholders::_1, "g_fUVPlusY");
	GET_COMPONENT_FROM(m_pArrHwaraGauge[Team_Red][Hwara_Arrow], CShader)->CallBack_SetRawValues += bind(&CUI_Paden::Set_Shader_HwaraArrow_Red, this, placeholders::_1, "g_fUVPlusY");
}

void CUI_Paden::Create_Popup()
{
	m_pPopupUI = CUI_Object::Create();

	m_pPopupUI->Set_FadeDesc(0.3f, 0.3f, 3.f, true);

	GET_COMPONENT_FROM(m_pPopupUI, CTexture)->Remove_Texture(0);
	Read_Texture(m_pPopupUI, "/Paden/Popup", "Popup");

	m_pPopupUI->Set_Scale(340.f, 70.f);
	m_pPopupUI->Set_PosY(175.f);

	CREATE_GAMEOBJECT(m_pPopupUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pPopupUI);
}

void CUI_Paden::Create_HwaraGauge()
{
	for (int i = 0; i < Hwara_End; ++i)
	{
		m_pHwaraGauge[i] = CUI_Object::Create();

		m_pHwaraGauge[i]->Set_Sort(0.49f);
		m_pHwaraGauge[i]->Set_PosY(250.f);
		m_pHwaraGauge[i]->Set_Scale(350.f, 30.f);

		switch (i)
		{
		case Hwara_BG:
			GET_COMPONENT_FROM(m_pHwaraGauge[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pHwaraGauge[i], "/Paden/TopGauge", "Bar");
			m_pHwaraGauge[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.5f));
			break;

		case Hwara_Arrow:
			m_pHwaraGauge[i]->Set_Sort(0.5f);
			m_pHwaraGauge[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Paden/T_ArrowStroke.dds"));
			break;

		case Hwara_Glow:
			m_pHwaraGauge[i]->Set_Scale(120.f, 40.f);
			m_pHwaraGauge[i]->Set_RotationZ(90.f);
			m_pHwaraGauge[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Paden/T_AdditiveGlow.dds"));
			break;
		}
	}
}

void CUI_Paden::Init_HwaraGauge()
{
	for (int i = 0; i < Hwara_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pHwaraGauge[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHwaraGauge[i]);

		for (int j = 0; j < Team_End; ++j)
		{
			m_pArrHwaraGauge[j][i] = m_pHwaraGauge[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrHwaraGauge[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrHwaraGauge[j][i]);
		}
	}

	for (int i = 0; i < Team_End; ++i)
	{
		for (int j = 0; j < Hwara_End; ++j)
		{
			_float fPosX = -350.f + (i * 700.f);
			m_pArrHwaraGauge[i][j]->Set_PosX(fPosX);

			switch (j)
			{
			case Hwara_BG:				
				m_pArrHwaraGauge[i][j]->Set_TextureIndex(i);
				break;

			case Hwara_Arrow:
				GET_COMPONENT_FROM(m_pArrHwaraGauge[i][j], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HwaraArrow);
				break;
			}
		}
	}

}

void CUI_Paden::Update_TargetPointPos()
{
	if (m_bSetTargetPoint)
	{
		_float4 vPointPos = m_pArrPointUI[m_eTargetPoint][PU_Point]->Get_Pos();
		vPointPos.y += 20.f;

		_float4 vProjPointPos = m_pArrProjPointUI[m_eTargetPoint][PU_Point]->Get_Pos();
		vProjPointPos.y += 20.f;

		m_pArrTargetPoint[0]->Set_Pos(vPointPos);
		m_pArrTargetPoint[1]->Set_Pos(vProjPointPos);
	}
}
