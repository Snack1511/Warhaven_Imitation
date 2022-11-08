#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;
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
	void	Set_Player(CUnit* pPlayer) { m_pPlayer = pPlayer; }
	CUnit* Get_Player() { return m_pPlayer; }
	void	Set_FixCursor(_bool bEnable) { m_bFixCursor = bEnable;  ::ShowCursor(!bEnable); }

public:
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();

	void	Clear_KeyCommands() { m_KeyCommands.clear(); }
	void	Update_KeyCommands();

private:
	CUnit* m_pPlayer = nullptr;

private:
	_bool	m_bFixCursor = false;
	POINT	m_ptCenter;
	list<KEY>		m_KeyCommands;
};
END
