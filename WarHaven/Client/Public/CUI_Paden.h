#pragma once
#include "CUI_Wrapper.h"
#include "../../Engine/Public/PhysX/foundation/PxPreprocessor.h"

BEGIN(Client)

class CUI_Paden : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Paden);
	DECLARE_GAMEOBJECT(CUI_Paden);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_MainPointGauge(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_RespawnPointGauge(CShader* pShader, const char* pConstName);

public:
	void Set_ScoreNum(_uint iTeamType, _uint iScore);

	void Set_Proj_StrongHoldUI(_uint iPointIdx, CTransform* pTransform);

	void SetActive_StrongHoldGauge(_bool value);
	void SetActive_ScoreNum(_bool value);
	void SetActive_TopPointUI(_bool value);
	void SetActive_ProjStrongHoldUI(_bool value);

public:
	enum TriggerState { TS_Enter, TS_Exit, TS_End };
	void Interact_StrongHoldUI(string wstrPadenPointKey, _uint iTeamType, _uint iTriggerState);
	void Set_StrongHoldUI_Color(_uint iTeamType);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pInGameTimer = nullptr;

private:	// 각 팀별 스코어
	enum TeamType { Team_Red, Team_Blue, Team_End };
	TeamType m_eTeamType = Team_End;

	enum ScoreGaugeNum { Num0, Num1, Num2, Num_End };
	CUI_Object* m_pScoreNum[Num_End];
	CUI_Object* m_pArrScoreNum[Team_End][Num_End];

	vector<_uint> m_vecPrvScore[Team_End];
	vector<_uint> m_vecCurScore[Team_End];

	_uint m_iChangeNumIdx = 0;
	_bool m_bIsChangeNum = false;
	_bool m_bIsDisableNum = false;
	_bool m_bIsEnableNum = false;

private:
	void Create_ScoreNum();
	void Init_ScoreVector();

private:
	enum StrongHoldGauge { Gauge_BG, Gauge_Bar, Gauge_Icon, Gauge_End };
	enum TopPointUI { SU_BG, SU_Gauge, SU_Outline, SU_Icon, SU_End };
	enum ProjPointUI { PP_BG, PP_Gauge, PP_Outline, PP_Icon, PP_End };

	CUI_Object* m_pStrongHoldGauge[Gauge_End];
	CUI_Object* m_pArrStrongHoldGauge[Gauge_End][2];


	CUI_Object* m_pStrongHoldUI[SU_End];
	CUI_Object* m_pArrStrongHoldUI[SU_End][3];

	CUI_Object* m_pProj_StrongHoldUI[PP_End];
	CUI_Object* m_pArrProj_StrongHoldUI[PP_End][3];

	_float4 m_vColorBG = _float4(0.3f, 0.3f, 0.3f, 1.f);
	_float4 m_vColorGauge = _float4(1.f, 1.f, 1.f, 0.1f);
	_float4 m_vColorOutline = _float4(0.7f, 0.7f, 0.7f, 1.f);

	_float4 m_vColorBlue = _float4(0.1f, 0.6f, 1.9f, 0.5f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.063f, 0.5f);

	_float m_fInGameTime = 1800.f;

	_float m_fGaugeNumFadeSpeed = 0.3f;

	_float m_fPointUIPosY = 260.f;
	_float m_fMainPointUIPosX = -50.f;

	_float m_fGaugeRatio[3] = { 0.5f };

private:
	void Create_InGameTimer();
	void Create_StrongHoldGauge();
	void Create_StrongHoldUI();
	void Create_Proj_StrongHoldUI();

	void Init_StrongHoldUI();

private:
	void Bind_Shader();
};

END