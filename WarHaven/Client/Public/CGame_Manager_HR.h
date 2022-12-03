#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CGame_Manager_HR
{
	DECLARE_SINGLETON(CGame_Manager_HR);

private:
	CGame_Manager_HR();
	~CGame_Manager_HR();

public:
	LEVEL_TYPE_CLIENT Get_CurrentLevel() { return m_eCurrentLV; }
	void Set_CurrentLevel(LEVEL_TYPE_CLIENT eLevel) { m_eCurrentLV = eLevel; }

public:
	HRESULT	Initialize();
	void	Tick();

private:
	LEVEL_TYPE_CLIENT m_eCurrentLV = LEVEL_END;
};
END
