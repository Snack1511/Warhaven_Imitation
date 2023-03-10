#pragma once
#include "CUI_Wrapper.h"
#include "../../Engine/Public/PhysX/foundation/PxPreprocessor.h"

BEGIN(Client)

class CTeamConnector;

class CUI_Paden : public CUI_Wrapper
{
private:
	enum TriggerState { TS_Enter, TS_Exit, TS_End };

	DECLARE_PROTOTYPE(CUI_Paden);
	DECLARE_GAMEOBJECT(CUI_Paden);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_PointGauge_A(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_PointGauge_R(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_PointGauge_C(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_PointGauge_E(CShader* pShader, const char* pConstName);

	virtual void Set_Shader_SocreGauge_Red(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_SocreGauge_Blue(CShader* pShader, const char* pConstName);

	virtual void Set_Shader_HwaraArrow_Blue(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_HwaraArrow_Red(CShader* pShader, const char* pConstName);

public:
	void Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime);
	void Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum);
	void Set_PointUI_ProjectionTransform(string strPadenPointKey, CTransform* pTransform, _bool isInFrustum);

	void Set_TargetTransform(CTransform* pTargetTransform);

	void Set_Team(CTeamConnector* pAllyTeam, CTeamConnector* pEnemyTeam);

	void Set_TargetPointPos(_uint iTargetIdx);

	void SetActive_ScoreNum(_bool value);
	void SetActive_ScoreGauge(_bool value);
	void SetActive_PointUI(_bool value);
	void SetActive_TargetPoint(_bool value);

	void Conquest_PointUI(string strPointName, _bool bIsMainPlayerTeam);

	void Interact_PointUI(_bool bIsMainPlayerTeam, string wstrPadenPointKey);
	void Move_PointUI(string wstrPadenPointKey, _uint iTriggerState);

	void Enable_Popup(_uint iIndex);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	_bool m_bIsVector = true;

	void Update_IndicatorVector(CTransform* pTargetTransform);
	void Update_IndicatorAngle(CTransform* pTargetTransform);

private:
	CTransform* m_pTargetTransform = nullptr;

private:
	enum TeamType { Team_Red, Team_Blue, Team_End };
	TeamType m_eTeamType = Team_End;

	CTeamConnector* m_pAllyTeam = nullptr;
	CTeamConnector* m_pEnemyTeam = nullptr;

private:
	CUI_Object* m_pInGameTimer = nullptr;

	_float m_fInGameTime = 1800.f;

private:
	void Create_InGameTimer();
	void Update_InGameTimer();

private:
	enum ScoreGaugeNum { Num0, Num1, Num2, Num_End };

	CUI_Object* m_pScoreNum = nullptr;
	CUI_Object* m_pArrScoreNum[Team_End][Num_End];

	_float m_fScoreFadeSpeed = 0.25f;
	_float m_fScoreRatio[Team_End];

	_uint m_iScore[Team_End];

	_uint m_iRemainderCnt[Team_End];

	vector<_uint> m_vecPrvScore[Team_End];
	vector<_uint> m_vecCurScore[Team_End];

private:
	void Create_ScoreNum();
	void Update_Score();

private:
	enum ScoreGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_End };

	CUI_Object* m_pScoreGauge[Gauge_End];
	CUI_Object* m_pArrScoreGauge[Gauge_End][Team_End];

private:
	void Create_ScoreGauge();

private:
	enum PointUI { PU_Point, PU_Gauge, PU_Text, PU_End };
	enum PointName { Point_A, Point_R, Point_C, Point_E, Point_End };

	PointName m_ePoint = Point_End;

	CUI_Object* m_pPointUI[PU_End];
	CUI_Object* m_pArrPointUI[Point_End][PU_End];
	CUI_Object* m_pArrProjPointUI[Point_End][PU_End];

	_float m_fConquestTime[Point_End];
	_float m_fMaxConquestTime[Point_End];
	_float m_fConquestRatio[Point_End];

	_bool m_bSetTargetPoint = false;

private:
	CUI_Object* m_pTargetPoint = nullptr;
	CUI_Object* m_pArrTargetPoint[2];

	PointName m_eTargetPoint = PointName::Point_End;

private:
	void Create_TargetPointUI();

private:
	CUI_Object* m_pPopupUI = nullptr;

	_bool m_bShowStartPopup = false;
	_bool m_bShowInfoUI = false;

private:
	void Create_Popup();

private:
	void Create_PointUI();
	void Init_PointUI();

	void Set_PointTextPosY();
	void Update_TargetPointPos();

	void Set_PointGauge_Color(_bool bIsMainPlayerTeam, PointName ePointName);

private:
	_float4 m_vColorBG = _float4(0.3f, 0.3f, 0.3f, 1.f);
	_float4 m_vColorGauge = _float4(0.5f, 0.5f, 0.5f, 0.5f);
	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);

	_float4 m_vColorBlue = _float4(0.1f, 0.6f, 1.f, 0.9f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.1f, 0.9f);

	_float4 m_vGaugeColorBlue = _float4(0.1f, 0.6f, 1.f, 0.5f);
	_float4 m_vGaugeColorRed = _float4(1.f, 0.2f, 0.1f, 0.5f);

	_float m_fPointUIPosY = 260.f;

	eTEAM_TYPE m_eTeam = eTEAM_TYPE::eCOUNT;

private:	// Hwara
	enum HwaraGauge { Hwara_BG, Hwara_Arrow, Hwara_Glow, Hwara_End };
	CUI_Object* m_pHwaraGauge[Hwara_End];
	CUI_Object* m_pArrHwaraGauge[Team_End][Hwara_End];

	_float m_fUVTexY[Team_End];
	_float m_fUVMoveX[Team_End];


private:
	void Create_HwaraGauge();
	void Init_HwaraGauge();

private:
	void Bind_Shader();
};

END