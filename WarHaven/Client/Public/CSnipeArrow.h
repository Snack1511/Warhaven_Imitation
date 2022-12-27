#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CSnipeArrow
	: public CProjectile
{
	DECLARE_PROTOTYPE(CSnipeArrow);

private:
	CSnipeArrow();
	virtual ~CSnipeArrow();

public:
	static CSnipeArrow* Create();

public:
	virtual HRESULT Start() override;
	virtual HRESULT	Initialize_Prototype() override;
	virtual void OnDisable() override;


private:
	_bool m_bShot = false;
	_float m_fTick = 0.f;


};

END