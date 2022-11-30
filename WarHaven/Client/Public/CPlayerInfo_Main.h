#pragma once
#include "CPlayerInfo.h"

BEGIN(Client)
class CPlayerInfo_Main final
	: public CPlayerInfo
{
	DECLARE_PLAYERINFO(CPlayerInfo_Main);

private:
	CPlayerInfo_Main();
	virtual ~CPlayerInfo_Main();

public:
	virtual HRESULT	Initialize();
	
};

END