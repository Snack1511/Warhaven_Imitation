#pragma once
#include "CPlayerInfo.h"

BEGIN(Client)
class CPlayerInfo_Sub final
	: public CPlayerInfo
{
private:
	CPlayerInfo_Sub();
	virtual ~CPlayerInfo_Sub();

public:
	static CPlayerInfo_Sub* Create();

public:
	virtual HRESULT	Initialize();
	virtual HRESULT SetUp_AIPersonality();
};

END