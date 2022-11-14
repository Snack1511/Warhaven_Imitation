#pragma once
#include "CPhysXCharacter.h"

BEGIN(Engine)

class CHierarchyNode;

class ENGINE_DLL CBoneCollider
	: public CPhysXCharacter, public PxUserControllerHitReport
{
public:
	typedef struct BONECOLLIDERDESC
	{
		_float		fRadius = 0.5f;
		_float		fHeight = 1.f;
		CHierarchyNode* pRefBone = nullptr;
		_float4 vOffset = ZERO_VECTOR;

	}BONECOLLIDERDESC;


protected:
	CBoneCollider(_uint iGroupID);
	virtual ~CBoneCollider();

public:
	static CBoneCollider* Create(_uint iGroupID, const BONECOLLIDERDESC& tPhysXCCTDESC);

public:
	virtual void onShapeHit(const PxControllerShapeHit& hit);
	virtual void onControllerHit(const PxControllersHit& hit) {};
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {};

public:
	_bool		Is_Collision() { return m_bCollision; };
	_float4		Get_HitPos() {
		return m_vHitPos;
	};

	_float4x4		Get_HitMatrix() { return m_HitMatrix; }


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;

private:
	BONECOLLIDERDESC	m_tColliderDesc;
	_float4				m_vPrevUp = _float4(0.f, 1.f, 0.f, 0.f);
	_float4				m_vPrevPos = ZERO_VECTOR;

	_float4				m_vHitPos = ZERO_VECTOR;
	_float4x4			m_HitMatrix;

	//실제 현재 충돌중 여부
	_bool				m_bCollision = false;

	//충돌 체크용도
	_bool				m_bCollisionTemp = false;



};

END