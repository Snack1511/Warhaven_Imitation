#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_WarHammer
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_WarHammer);

	enum Barricade_Cnt { BARRICADE_CNT = 2};

private:
	CUnit_WarHammer();
	virtual ~CUnit_WarHammer();

public:
	static	CUnit_WarHammer* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override; 

private:
	list<CGameObject*> m_Barricade;
};

END