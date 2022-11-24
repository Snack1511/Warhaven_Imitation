#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)
class CTable abstract
{
protected:
	CTable();
	virtual ~CTable();

public:
	virtual HRESULT	Initialize();
	virtual void Release();
};

END