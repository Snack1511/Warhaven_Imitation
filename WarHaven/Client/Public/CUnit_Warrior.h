#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Warrior
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Warrior);

private:
	CUnit_Warrior();
	virtual ~CUnit_Warrior();

public:
	static	CUnit_Warrior* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(_bool bPlayer);


	STATE_TYPE		m_eHitState = STATE_END;
	STATE_TYPE		m_eDeathState = STATE_END;
	/* ���� ������ ���� */
	STATE_TYPE		m_eGuardState = STATE_END;

	/* ���� ���� ����*/
	STATE_TYPE		m_eGuardBreakState = STATE_END;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	virtual void My_LateTick();
};

END