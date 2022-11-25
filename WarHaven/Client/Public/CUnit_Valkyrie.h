#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Valkyrie
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Valkyrie);

private:
	CUnit_Valkyrie();
	virtual ~CUnit_Valkyrie();

public:
	static	CUnit_Valkyrie* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(_bool bPlayer);

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
};

END