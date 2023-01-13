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
class CCannonBoom;

class CCannonBall 
	: public CGameObject
{
	DECLARE_GAMEOBJECT(CCannonBall);
	DECLARE_PROTOTYPE(CCannonBall);

protected:
	CCannonBall();
	virtual ~CCannonBall();

public:
	void		Shoot_Cannon(CPlayer* pOwnerPlayer, _float4 vShootPos, _float4 vShootDir);
	void		Shoot_CatchedCannon(_float4 vShootDir);

public:
	virtual void	Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	CPlayer* m_pOwnerPlayer = nullptr;

protected:
	_float	m_fForcePower = 10000.f;
	_float	m_fDamage = 0.f;

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	_bool	m_bCloned = false;
	PxConvexMesh* m_pConvexMesh = nullptr;
	PxRigidDynamic* m_pActor = nullptr;

private:
	_bool	m_bCatched = false;
	CCollider_Sphere* m_pCollider = nullptr;
	CCannonBoom* m_pCannonBoom = nullptr;
	CHierarchyNode* m_pTargetBone = nullptr;

private:
	void Create_Light(CGameObject* pOwner, _float4 vOffset, _float fRange, _float fRandomRange,
		_float fFadeInTime, _float fDuration, _float fFadeOutTime, _float4 Diffuse, _bool bLoop);
	void	On_Boom();
};

END