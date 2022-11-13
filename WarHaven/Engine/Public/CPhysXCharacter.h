#pragma once
#include "PhysXCollider.h"

BEGIN(Engine)

class CPhysics;
class CHierarchyNode;

class ENGINE_DLL CPhysXCharacter
	: public CPhysXCollider, PxUserControllerHitReport
{
public:

	typedef struct PHYSXCCTDESC
	{
		
		_float		fRadius = 0.5f;
		_float		fHeight = 1.f;
	}PHYSXCCTDESC;

	DECLARE_PROTOTYPE(CPhysXCharacter);


protected:
	CPhysXCharacter(_uint iGroupID);
	virtual ~CPhysXCharacter();

public:
	static CPhysXCharacter* Create(_uint iGroupID, const PHYSXCCTDESC& tPhysXCCTDESC);

public:
	void	Set_Position(_float4 vPos);


public: /* Character Controller CallBack */
	virtual void onShapeHit(const PxControllerShapeHit& hit);
	virtual void onControllerHit(const PxControllersHit& hit) {};
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {};

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;

	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Release() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

protected:
	CPhysics* m_pPhysicsCom = nullptr;

	PHYSXCCTDESC		m_tPhysXCCTDESC;
	PxController*		m_pPxController = nullptr;

	PxControllerFilters		m_tControllerFilters;

	_bool				m_bAirTemp = false;

protected:
	void			Update_Colliders();


	


};

END