#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Spearman
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Spearman);

private:
	CUnit_Spearman();
	virtual ~CUnit_Spearman();

public:
	static	CUnit_Spearman* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END