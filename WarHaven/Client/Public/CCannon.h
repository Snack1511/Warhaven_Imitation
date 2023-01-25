#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CAnimator;
class CHierarchyNode;
END

BEGIN(Client)

class CPlayer;
class CCamera_Follow;
class CCannonBall;
class CUI_Trail;

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
	void		Exit_Cannon();
	void		Shoot_Cannon();
	_bool		Can_ControlCannon(CPlayer* pPlayer);
	CPlayer* Get_CurOwnerPlayer() { return m_pCurOwnerPlayer; }

private:
	CPlayer* m_pCurOwnerPlayer = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CCamera_Follow* m_pCannonCam = nullptr;

	CHierarchyNode* m_pBonePitch = nullptr;

private:
	_float		m_fCannonMoveSpeed = 0.4f;
	_float		m_fCurPitch = 0.f;
	_float		m_fCurYaw = 0.f;

private:
	_float		m_fCannonCoolAcc = 0.f;
	_float		m_fCannonCoolTime = 10.f;
	CCannonBall* m_pCannonBall = nullptr;

private:
	CUI_Trail* m_pUI_Trail = nullptr;

private:
	_float		Lerp_Position(_float fCurPosition, _float fTargetPosition, _float fRange);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	

};

END