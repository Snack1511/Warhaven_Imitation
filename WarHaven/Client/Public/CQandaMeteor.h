#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CQandaMeteor
	: public CProjectile
{
	DECLARE_PROTOTYPE(CQandaMeteor);

private:
	CQandaMeteor();
	virtual ~CQandaMeteor();

public:
	static CQandaMeteor* Create();

public:
	virtual HRESULT Start() override;
	virtual HRESULT	Initialize_Prototype() override;
	virtual void	My_Tick() override;

public:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<CGameObject*> m_EffectTest;
	_bool				m_bShoot = true;

	//_float m_fMaxColliderTime = 1.f;
	//_float m_fCurColliderTime = 0.f;

};

END