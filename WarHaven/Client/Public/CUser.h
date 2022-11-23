#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;
class CBloodOverlay;
class CUI_Cursor;
class CPlayer;
class CUI_HUD;

class CUser
{
	DECLARE_SINGLETON(CUser);

private:
	CUser();
	~CUser();

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	On_ExitLevel();


public:
	void	Set_Player(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* Get_PlayerObejects() { return m_pPlayer; }
	CUnit* Get_Player();
	void	Set_FixCursor(_bool bEnable) { m_bFixCursor = bEnable;  ::ShowCursor(!bEnable); }

	CUI_Cursor* Get_Cursor() { return m_pCursor; }
	void Set_Cursor(CUI_Cursor* pCursor) { m_pCursor = pCursor; }

public:
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();

	KEY		Get_LastKey() { 
		if (m_KeyCommands.empty())
			return KEY::LAST;
		return m_KeyCommands.back();
	}
	void	Clear_KeyCommands() { m_KeyCommands.clear(); }
	void	Update_KeyCommands();

public:
	void	Time_Slow();

private:
	CPlayer* m_pPlayer = nullptr;

private:
	_bool	m_bFixCursor = false;
	POINT	m_ptCenter;
	list<KEY>		m_KeyCommands;

public:
	void SetUp_BloodOverlay();
	void Turn_BloodOverLay(_float fHpRatio);

public:
	void On_EnterLevel();

	void SetActive_OxenJumpText(_bool value);
	void SetActive_DamageTex(_float fDmg, _bool bIsHead);

private:
	CBloodOverlay* m_pBloodOverlay = nullptr;
	CUI_Cursor* m_pCursor = nullptr;
	CUI_HUD* m_pUI_HUD = nullptr;
};
END
