#pragma once
#include "CUI_Wrapper.h"
#include "../../Engine/Public/PhysX/foundation/PxPreprocessor.h"

BEGIN(Client)

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

public:
	void Set_ScoreNum(_uint iTeamType, _uint iScore);
	void Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime);
	void Set_Proj_StrongHoldUI(_uint iPointIdx, CTransform* pTransform);

	void SetActive_ScoreNum(_bool value);
	void SetActive_ScoreGauge(_bool value);
	void SetActive_PointUI(_bool value);

	void Conquest_PointUI(string strPointName, _uint iTeamType);

	void Interact_PointUI(string wstrPadenPointKey, _uint iTeamType, _uint iTriggerState);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pInGameTimer = nullptr;

	_float m_fInGameTime = 1800.f;

private:
	void Create_InGameTimer();
	void Update_InGameTimer();

private:	// 각 팀별 스코어
	enum TeamType { Team_Red, Team_Blue, Team_End };
	TeamType m_eTeamType = Team_End;

	enum ScoreGaugeNum { Num0, Num1, Num2, Num_End };
	CUI_Object* m_pScoreNum[Num_End];
	CUI_Object* m_pArrScoreNum[Team_End][Num_End];

	_float m_fGaugeNumFadeSpeed = 0.3f;

	_uint m_iPrvScore[Team_End][3];
	_uint m_iCurvScore[Team_End][3];

	_uint m_iChangeNumIdx[3] = { 0 };
	_bool m_bIsChangeNum = false;
	_bool m_bIsDisableNum = false;
	_bool m_bIsEnableNum = false;

	_uint m_iMaxScore = 100;
	_uint m_iCurScore = 100;
	_float m_fScoreRatio = 0.f;

private:
	void Create_ScoreNum();

private:
	enum ScoreGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_End };

	CUI_Object* m_pScoreGauge[Gauge_End];
	CUI_Object* m_pArrScoreGauge[Gauge_End][2];

private:
	void Create_ScoreGauge();

private:
	enum PointUI { PU_Point, PU_Gauge, PU_Text, PU_End };
	enum PointName { Point_A, Point_R, Point_C, Point_End };

	PointName m_ePoint = Point_End;

	CUI_Object* m_pPointUI[PU_End];
	CUI_Object* m_pArrPointUI[Point_End][PU_End];
	CUI_Object* m_pArrProjPointUI[Point_End][PU_End];

	_float m_fConquestTime[Point_End];
	_float m_fMaxConquestTime[Point_End];
	_float m_fConquestRatio[Point_End];

private:
	void Create_PointUI();
	void Init_PointUI();

	void Set_PointTextPosY();

	void Set_PointGauge_Color(_uint iTeamType, PointName ePointName);

private:
	_float4 m_vColorBG = _float4(0.3f, 0.3f, 0.3f, 1.f);
	_float4 m_vColorGauge = _float4(0.5f, 0.5f, 0.5f, 0.5f);
	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);

	_float4 m_vColorBlue = _float4(0.1f, 0.6f, 1.f, 0.9f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.1f, 0.9f);

	_float m_fPointUIPosY = 260.f;

private:
	void Bind_Shader();
};

END