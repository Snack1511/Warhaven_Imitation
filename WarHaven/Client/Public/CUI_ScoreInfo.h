#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;

class CUI_ScoreInfo : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_ScoreInfo);
	DECLARE_GAMEOBJECT(CUI_ScoreInfo);

private:
	CUI_ScoreInfo();
	virtual ~CUI_ScoreInfo();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	void Set_Player(CPlayer* pPlayer);
	void Set_Rank(_uint iRank);
	void Set_PosX(_float fPosX);

	_uint Get_KillCnt() { return m_iKillCnt; }

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum Info { Info_BG, Info_Rank, Info_ClassName, Info_Kill, Info_Num, Info_End };
	CUI_Object* m_pInfo[Info_End];
	
	_uint m_iSquadIdx = 1;
	_uint m_iRank = 0;
	_uint m_iKillCnt = 0;

private:
	void Create_Info();
	void Init_Info();

	// 스쿼드 멤버일 경우
	// 메인 플레이어면 bg 흰색
	// 아닐 경우 초록색

	// 랭크 옆에 인덱스

	// 위치가 바뀔 때마다 Y 위치 받아와서 업데이트

	// Rank :  pos : (x - 85, 170)
	// Kill Score :  pos : (x + 85, 170)
	// ClassName :  pos : (x - 65), 170), 
};

END