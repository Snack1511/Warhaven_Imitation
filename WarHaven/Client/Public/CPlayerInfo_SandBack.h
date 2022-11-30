#pragma once
#include "CPlayerInfo.h"

BEGIN(Client)
class CPlayerInfo_SandBack final
	: public CPlayerInfo
{
private:
	CPlayerInfo_SandBack();
	virtual ~CPlayerInfo_SandBack();

public:
	static CPlayerInfo_SandBack* Create();

public:
	virtual HRESULT	Initialize();

};

END