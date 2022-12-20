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
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<CGameObject*> m_Test;
	_bool m_bShot = false;
	_float m_fTick = 0.f;

	_float m_fCurPoisonTime = 0.f;
	_float m_fMaxPoisonTime = 0.f;

	_uint m_iTickCnt = 0;
	_uint m_iMaxTickCnt = 5;
};

END