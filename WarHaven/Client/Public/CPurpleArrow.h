#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CPurpleArrow
	: public CProjectile
{
	DECLARE_PROTOTYPE(CPurpleArrow);

private:
	CPurpleArrow();
	virtual ~CPurpleArrow();

public:
	static CPurpleArrow* Create();

public:
	virtual HRESULT Start() override;
	virtual HRESULT	Initialize_Prototype() override;
	virtual void	My_Tick() override;
private:
	list<CGameObject*> m_Test;
	_bool m_bShot = false;
	_float m_fTick = 0.f;

	_uint m_iTickCnt = 5;
};

END