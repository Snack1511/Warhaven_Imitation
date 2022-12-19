#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CDefaultArrow
	: public CProjectile
{
	DECLARE_PROTOTYPE(CDefaultArrow);

private:
	CDefaultArrow();
	virtual ~CDefaultArrow();

public:
	static CDefaultArrow* Create();

public:
	virtual HRESULT	Initialize_Prototype() override;


};

END