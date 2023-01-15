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
	virtual void	My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Turn_Effect(_bool bOnOff);

private:
	list<CGameObject*> m_Test;
	_bool m_bShot = false;
	_float m_fTick = 0.f;
	_bool m_bEffect = true;


};

END