#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_ScoreBoard : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_ScoreBoard);
	DECLARE_GAMEOBJECT(CUI_ScoreBoard);

private:
	CUI_ScoreBoard();
	virtual ~CUI_ScoreBoard();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	void Get_ScoreInfo(CPlayer* pPlayer);
	void Sort_ScoreInfo();

	void Set_PointColor(_bool IsMainTeam, _uint iPoinIdx);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_END;

	list<CUI_Object*> m_pScoreList;

	CUI_Object* m_pScoreBG = nullptr;

	enum Score_MiniMap { Map_Circle0, Map_Circle1, Map_Frame, Map_Map, Map_End };
	CUI_Object* m_pScoreMiniMap[Map_End];

	enum Team { Team_Blue, Team_Red, Team_End };
	enum Score_PlayerList { List_Team, List_Kill, List_Death, List_Line, List_LineGlow, List_BG, List_End };
	CUI_Object* m_pSocrePlayerList[List_End];
	CUI_Object* m_pArrSocrePlayerList[Team_End][List_End];

	enum Squad { Squad_BG, Squad_Port, Squad_Num, Squad_End };
	CUI_Object* m_pSquard[Squad_End];
	CUI_Object* m_pArrSquard[4][Squad_End];

	enum Point { Point_A, Point_R, Point_C, Point_End };
	enum PointUI { PU_Outline, PU_Text, PU_End };
	CUI_Object* m_pPointUI[PU_End];
	CUI_Object* m_pArrPointUI[Point_End][PU_End];

public:
	map<_uint, list<CUI_ScoreInfo*>> Get_ScoreInfoMap();

private:
	map<Team, list<CUI_ScoreInfo*>> m_pScoreInfoMap;

private:
	_float4 m_vColorCircle = _float4(1.f, 1.f, 1.f, 0.1f);
	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);
	_float4 m_vColorBlue = _float4(0.f, 0.8f, 1.f, 0.1f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.f, 0.1f);

private:
	void Create_ScoreMiniMap();
	void Create_ScorePlayerList();
	void Create_Squad();
	void Create_PointUI();

	void Init_ScoreMiniMap();
	void Init_ScorePlayerList();
	void Init_Squad();
	void Init_PointUI();

	void Set_Squad();
};

END