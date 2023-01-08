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

public:
	wstring MainPlayerName();

	_bool Unlock_RabbitHat() { return m_bIsGetRabbitHat; }

private:
	_bool m_bIsGetRabbitHat = false;
};

END