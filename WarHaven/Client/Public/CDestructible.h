#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CPhysXCollider;
END

BEGIN(Client)
class CDestructible
	: public CGameObject
{
	DECLARE_PROTOTYPE(CDestructible)

private:
	CDestructible();
	virtual ~CDestructible();

public:
	static CDestructible* Create(wstring wstrMeshFilePath, wstring wstrDestoryMultiEffectsKey, wstring wstrHitMultiEffectsKey, _uint iHitCount =1);


public:
	virtual void	On_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	void	Set_Position(_float4 vPosition);
	void	Set_Look(_float4 vLook);


private:
	/* 부서질 때 나올 이펙트 키 */
	wstring	m_wstrDestoryMultiEffectsKey;

	/* hit 될 때 나오는 이펙트 키*/
	wstring	m_wstrHitMultiEffectsKey;

	/* 몇대 맞아야 부서질 지 */
	_uint	m_iHitCount = 1;

private:
	list<CPhysXCollider*>	m_PhysXColliders;

private:
	HRESULT	SetUp_Destructible(wstring wstrMeshFilePath);

	

	



};

END