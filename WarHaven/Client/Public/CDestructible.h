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
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	void	Set_Position(_float4 vPosition);
	void	Set_Look(_float4 vLook);


private:
	/* �μ��� �� ���� ����Ʈ Ű */
	wstring	m_wstrDestoryMultiEffectsKey;

	/* hit �� �� ������ ����Ʈ Ű*/
	wstring	m_wstrHitMultiEffectsKey;

	/* ��� �¾ƾ� �μ��� �� */
	_uint	m_iHitCount = 1;

private:
	list<CPhysXCollider*>	m_PhysXColliders;

private:
	HRESULT	SetUp_Destructible(wstring wstrMeshFilePath);

	

	



};

END