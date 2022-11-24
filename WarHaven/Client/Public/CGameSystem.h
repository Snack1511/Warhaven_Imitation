#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPositionTable;

class CGameSystem
{
	DECLARE_SINGLETON(CGameSystem)

private:
	CGameSystem();
	~CGameSystem();

public:
	HRESULT					Initialize();
	HRESULT					Tick();
	void					Release();

public:
	HRESULT					On_EnterBootCamp();

public: /* Position Table */
	HRESULT					Load_Position(string strFileKey);

	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);




private:
	CPositionTable* m_pPositionTable = nullptr;
};

END