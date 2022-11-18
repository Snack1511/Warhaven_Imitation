#pragma once
#include "CUnit.h"

BEGIN(Client)
class CUnit_WarHammer;

class CBarricade
	: public CUnit
{


	DECLARE_PROTOTYPE(CBarricade);

	enum BarricadeCnt { BarricadeCnt_END = 2};

private:
	CBarricade();
	virtual ~CBarricade();

public:
	static	CBarricade* Create(const UNIT_MODEL_DATA& tUnitModelData, CUnit_WarHammer* pOwner);


public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype(CUnit_WarHammer* pOwner);
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUnit_WarHammer* m_pOwner = nullptr;


};

END