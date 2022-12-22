#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
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
	CCannonBoom* m_pCannonBoom = nullptr;

private:
	void	On_Boom();
};

END