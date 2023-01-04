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
	enum Score_PlayerList { List_TeamIcon, List_KillIcon, List_Line, List_LineGlow, List_BG, List_End };
	CUI_Object* m_pSocrePlayerList[List_End];
	CUI_Object* m_pArrSocrePlayerList[Team_End][List_End];

	enum Squad { Squad_BG, Squad_Port, Squad_Num, Squad_End };
	CUI_Object* m_pSquard[Squad_End];
	CUI_Object* m_pArrSquard[4][Squad_End];

	//enum Score_PlayerInfo { Info_BG, Info_Rank, Info_ClassName, Info_Kill, Info_End };
	//CUI_Object* m_pPlayerInfo[Info_End];

	// BG : pos : (200, 20)
	// Rank : scale : 0.2, offset : (3,3), bold, center, pos : (x - 85, 170)
	// ClassName : scale : 0.2 offset : (15, -12), bold, pos : (x - 65), 170), scale : 16
	// Kill Score : scale : 0.2, offset : (5, 3), bold, center, pos : (x + 85, 170)

	_float4 m_vColorCircle = _float4(1.f, 1.f, 1.f, 0.1f);
	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);
	_float4 m_vColorBlue = _float4(0.f, 0.8f, 1.f, 0.1f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.f, 0.1f);

private:
	void Create_ScoreMiniMap();
	void Create_ScorePlayerList();
	void Create_Squad();

	void Init_ScoreMiniMap();
	void Init_ScorePlayerList();
	void Init_Squad();

	void Set_Squad();
};

END