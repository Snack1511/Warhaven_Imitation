#pragma once
#include "CUnit.h"

BEGIN(Client)

class CMainMenuUnit
	: public CUnit
{
	DECLARE_PROTOTYPE(CMainMenuUnit);

private:
	CMainMenuUnit();
	virtual ~CMainMenuUnit();

public:
	static	CMainMenuUnit* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void On_Die() override;

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick()override;
};

END