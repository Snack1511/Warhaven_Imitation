#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
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
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();

	void	Clear_KeyCommands() { m_KeyCommands.clear(); }
	void	Update_KeyCommands();

private:
	_bool	m_bFixCursor = false;
	POINT	m_ptCenter;
	list<KEY>		m_KeyCommands;
};
END
