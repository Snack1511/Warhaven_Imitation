#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CPlayer;

class CCannon
	: public CGameObject
{
	DECLARE_PROTOTYPE(CCannon);
	DECLARE_GAMEOBJECT(CCannon);

private:
	CCannon();
	virtual ~CCannon();



public:
	virtual void	Cannon_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Cannon_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Cannon_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	_float4		Get_ControlPos();
	void		Control_Cannon(CPlayer* pPlayer);
	void		Shoot_Cannon();

private:
	CPlayer* m_pCurOwnerPlayer = nullptr;
	CAnimator* m_pAnimator = nullptr;

private:
	virtual void My_Tick() override;
	

};

END