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

	void Set_ScaleX(_float fScaleX);

	_uint Get_KillCnt() { return m_iKillCnt; }
	_uint Get_DeathCnt() { return m_iDeathCnt; }
	CPlayer* Get_OwnerPlayer() { return pOwnerPlayer; }

	void Update_KillCnt(_uint iKillCnt) { m_iKillCnt = iKillCnt; }
	void Update_DeathCont(_uint iDeathCnt) { m_iDeathCnt = iDeathCnt; }

	// 0 탭키, 1 결과
	void Set_Type(_uint iType = 0);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum Info { Info_BG, Info_Rank, Info_ClassName, Info_Kill, Info_Death, Info_Num, Info_End };
	CUI_Object* m_pInfo[Info_End];

	CPlayer* pOwnerPlayer = nullptr;

	static _uint m_iSquadIdx;
	_uint m_iRank = 0;
	_uint m_iKillCnt = 0;
	_uint m_iDeathCnt = 0;

	_uint m_iType = 0;

private:
	void Create_Info();
	void Init_Info();

	// 랭크 옆에 인덱스
};

END

// Info_BG 생성될 떄 리스트 백그라운드 크기 X 받아와서 설정
// 위치도 백그라운드 X위치 받아와서 설정