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
	
	CPlayer* pOwnerPlayer = nullptr;

	static _uint m_iSquadIdx;
	_uint m_iRank = 0;
	_uint m_iKillCnt = 0;

private:
	void Create_Info();
	void Init_Info();

	// ∑©≈© ø∑ø° ¿Œµ¶Ω∫
};

END