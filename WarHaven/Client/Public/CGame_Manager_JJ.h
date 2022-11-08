#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CGame_Manager_JJ
{
	DECLARE_SINGLETON(CGame_Manager_JJ);

private:
	CGame_Manager_JJ();
	~CGame_Manager_JJ();

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
