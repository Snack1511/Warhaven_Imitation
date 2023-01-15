#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
class CCollider_Sphere;
END

BEGIN(Client)

class CPlayer;
class CCannon;

class CCannonBoom
	: public CGameObject
{
	DECLARE_GAMEOBJECT(CCannonBoom);
	DECLARE_PROTOTYPE(CCannonBoom);

protected:
	CCannonBoom();
	virtual ~CCannonBoom();

public:
	void		Boom(CPlayer* pOwnerPlayer, _float4 vPos);

public:
	virtual void	Boom_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Boom_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Boom_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();
protected:
	CPlayer* m_pOwnerPlayer = nullptr;
	CCollider_Sphere* m_pColliderCom = nullptr;

protected:
	_float	m_fEnableAcc = 0.f;
	_float	m_fEnableTime = 0.1f;

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END