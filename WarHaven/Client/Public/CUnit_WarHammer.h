#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_WarHammer
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_WarHammer);

	enum BARRICADE_CNT { BARRICADE_CNT_END = 2};

private:
	CUnit_WarHammer();
	virtual ~CUnit_WarHammer();

public:
	static	CUnit_WarHammer* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(_bool bPlayer);

public:
	void	Set_BarricadeMatrix();

public:
	void	Disable_Barricade(CGameObject* pBarricade);	
	void	Create_Barricade();

	_uint	Get_Size() {
		return (_uint)m_Barricade.size();
	}

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override; 

protected:
	//virtual void My_Tick() override;
	//virtual void My_LateTick() override;

private:
	list<CGameObject*> m_Barricade;
	_float4x4		   m_BarricadeMatrix;



};

END