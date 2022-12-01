#pragma once
#include "CPlayerInfo.h"

BEGIN(Client)
class CPlayerInfo_Default final
	: public CPlayerInfo
{
private:
	CPlayerInfo_Default();
	virtual ~CPlayerInfo_Default();

public:
	static CPlayerInfo_Default* Create();

public:
	virtual HRESULT	Initialize();

};

END