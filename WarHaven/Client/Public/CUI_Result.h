#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Result : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Result);
	DECLARE_GAMEOBJECT(CUI_Result);

private:
	CUI_Result();
	virtual ~CUI_Result();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_MVP(CShader* pShader, const char* pConstName);

public:
	// 0 ¹«½ÂºÎ, 1 ½Â¸®, 2 ÆÐ¹è
	void SetActive_Result(_uint iResult, _bool value);
	void SetActive_Result(_bool value);

	void Set_Result(_uint iResult) { m_iResult = iResult; }

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pFade = nullptr;

	enum ResultUI { Result_BG, Result_TextBG0, Result_TextBG1, Result_Text0, Result_Text1, Result_Line, Result_End };
	CUI_Object* m_pResultUI[Result_End];

	enum ResultScoreBG { Score_BG, Score_Result, Score_Text, Score_Circle0, Score_Circle1, Score_End };
	CUI_Object* m_pResultScoreBG[Score_End];

	enum ResultMVP { MVP_Text, MVP_Blind, MVP_Player, MVP_End };
	CUI_Object* m_pResultMVP[MVP_End];

	enum Team { Team_Blue, Team_Red, Team_End };
	enum ResultScoreList { List_BG, List_Line, List_Team, List_Kill, List_Dead, List_End };
	CUI_Object* m_pResultScoreList[List_End];
	CUI_Object* m_pArrResultScoreList[Team_End][List_End];

	_uint m_iResultProgressCnt = 0;

	_float m_fRotValue = 0.f;
	_float m_fTextLerpRatio = 0.f;

	_bool m_bLerpLine = false;
	_bool m_bLerpText0 = false;
	_bool m_bLerpText1 = false;

	_bool m_bIsEnd = false;
	_bool m_bResultDisable = false;

	_uint m_iResult = 0;
	_float m_fScoreTime = 0.f;

	_bool m_bIsMVP = false;
	_float m_fDissolveValue = 1.f;

private: // Color
	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);
	_float4 m_vColorBlue = _float4(0.f, 0.8f, 1.f, 0.1f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.f, 0.1f);

private:
	map<_uint, list<CUI_ScoreInfo*>> m_pScoreInfoMap;
	_uint m_iTeamCnt = 0;

private:
	void Create_Fade();
	void Create_ResultUI();
	void Create_ResultScoreBG();
	void Create_ResultMVP();
	void Create_ResultScoreList();

	void Init_ResultScoreBG();
	void Init_ResultMVP();
	void Init_ResultScoreList();

	void Progress_Result();
};

END