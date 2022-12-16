#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CDefaultArrow
	: public CProjectile
{
	DECLARE_PROTOTYPE(CDefaultArrow);
	DECLARE_GAMEOBJECT(CDefaultArrow);

private:
	CDefaultArrow();
	virtual ~CDefaultArrow();

public:
	virtual HRESULT	Initialize_Protoype() override;


};

END