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
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

public:
	virtual void My_Tick() override;

private:
	list<CGameObject*> m_Test;


};

END