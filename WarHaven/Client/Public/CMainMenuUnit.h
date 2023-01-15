#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Lancer_Head;

class CMainMenuUnit
	: public CUnit
{
	DECLARE_PROTOTYPE(CMainMenuUnit);

private:
	CMainMenuUnit();
	virtual ~CMainMenuUnit();

public:
	static	CMainMenuUnit* Create(const UNIT_MODEL_DATA& tUnitModelData, CLASS_TYPE eClassType);

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


public:
	void ReFresh_Animation();

private:
	void Set_EyeEffect();
	void Turn_EyeEffect(_bool bValue);

private:
	ANIM_TYPE	m_eBaseType = ANIM_BASE_R;
	_uint		m_iAnimIndex = 0;
	_float		m_fAnimSpeed = 1.f;

	CLASS_TYPE	m_eClassType = CLASS_END;

	CUnit_Lancer_Head* m_pMyLancerHead = nullptr;
	CAnimWeapon* m_pAnimWeapon = nullptr;
};

END