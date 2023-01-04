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
	// 0 무승부, 1 승리, 2 패배
	void SetActive_Result(_uint iResult, _bool value);
	void SetActive_Result(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum ResultUI
	{
		Result_BG,
		Result_TextBG0, Result_TextBG1,
		Result_Text0, Result_Text1,
		Result_Line,
		Result_End
	};

	CUI_Object* m_pResultUI[Result_End];

	_bool m_bLerpLine = false;
	_bool m_bLerpText0 = false;
	_bool m_bLerpText1 = false;

private:
	// 원 추가해야함
	enum ResultScoreBG { Score_BG, Score_Result, Score_Text, Score_End };
	CUI_Object* m_pResultScoreBG[Score_End];

	enum ResultMVP { MVP_Text, MVP_Player, MVP_End };
	CUI_Object* m_pResultMVP[MVP_End];

	// 리스트 라인 추가해야하ㅣㅁ
	enum Team { Team_Blue, Team_Red, Team_End };
	enum ResultScoreList { List_BG, List_Team, List_Score, List_Kill, List_Dead, List_End };
	CUI_Object* m_pResultScoreList[List_End];
	CUI_Object* m_pArrResultScoreList[Team_End][List_End];

	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);

private:
	void Create_ResultScoreBG();
	void Create_ResultMVP();
	void Create_ResultScoreList();

private:
	void Create_ResultUI();
};

END