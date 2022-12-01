#pragma once
#include "CPlayerInfo.h"

BEGIN(Client)
class CPlayerInfo_Leader final
	: public CPlayerInfo
{
private:
	CPlayerInfo_Leader();
	virtual ~CPlayerInfo_Leader();

public:
	static CPlayerInfo_Leader* Create();

public:
	virtual HRESULT	Initialize();

};

END