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
	virtual HRESULT Start() override;
	virtual HRESULT	Initialize_Prototype() override;
	
	virtual void OnDisable() override;

private:
	list<CGameObject*> m_Test;
	_bool m_bShot = false;
	_float m_fTick = 0.f;


};

END