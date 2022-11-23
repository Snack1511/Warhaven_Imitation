#pragma once
#include "CStructure.h"

BEGIN(Engine)
class CCollider;
class CCollider_Sphere;
class CPhysXCharacter;
class CPhysics;
END

BEGIN(Client)
class CUnit_WarHammer;

class CBarricade
	: public CStructure
{
	DECLARE_PROTOTYPE(CBarricade);


public:
	enum BarricadeCnt { BarricadeCnt_END = 2};



private:
	CBarricade();
	virtual ~CBarricade();

public:
	static	CBarricade* Create(CUnit_WarHammer* pOwner, wstring strModelPath);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype(CUnit_WarHammer* pOwner);
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CUnit_WarHammer* m_pOwner = nullptr;
	CPhysXCharacter* m_pPhysXCharacter = nullptr;
	CCollider_Sphere* m_pStructCollider = nullptr;
	CPhysics* m_pPhysics = nullptr;

};

END