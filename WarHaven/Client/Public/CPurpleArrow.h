#pragma once
#include "CProjectile.h"

BEGIN(Client)

class CUnit;


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
	void Clear_Addiction();

public:
	virtual HRESULT Start() override;
	virtual HRESULT	Initialize_Prototype() override;
	virtual void	My_Tick() override;

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	HRESULT	SetUp_Colliders(COL_GROUP_CLIENT eColType); // 필요하다면 가상함수.


private:
	list<CGameObject*> m_Test;
	list<CGameObject*> m_Addiction;
	_bool m_bPoison = true;
	_bool m_bAddiction = true;
	_float m_fTick = 0.f;

	_float m_fCurPoisonTime = 0.f;
	_float m_fMaxPoisonTime = 0.f;

	_uint m_iTickCnt = 0;
	_uint m_iMaxTickCnt = 5;

private:
	virtual void Set_ColliderType(eTEAM_TYPE eTeamType);
	void AddictionEffect(CUnit* pUnit);

	void Death_Cam(CUnit* pOtherUnit);


};

END