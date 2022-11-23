#pragma once
#include "CPhysXCharacter.h"

BEGIN(Engine)

class CHierarchyNode;

class ENGINE_DLL CCameraCollider
	: public CPhysXCharacter, public PxUserControllerHitReport
{
public:
	typedef struct BONECOLLIDERDESC
	{
		_float		fRadius = 0.5f;
		_float		fHeight = 1.f;
		_float4 vOffset = ZERO_VECTOR;

	}BONECOLLIDERDESC;


protected:
	CCameraCollider(_uint iGroupID);
	virtual ~CCameraCollider();

public:
	static CCameraCollider* Create(_uint iGroupID, const BONECOLLIDERDESC& tPhysXCCTDESC);

public:
	virtual void onShapeHit(const PxControllerShapeHit& hit);
	virtual void onControllerHit(const PxControllersHit& hit);
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {};

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

private:
	_float4				m_vPrevPos = ZERO_VECTOR;

	BONECOLLIDERDESC	m_tColliderDesc;

	_bool				m_bStart = false;

	PxRigidActor* m_pCurrentHitActor = nullptr;
	PxShape* m_pCurrentShape = nullptr;



};

END