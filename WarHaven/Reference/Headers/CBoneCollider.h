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


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;

private:
	BONECOLLIDERDESC	m_tColliderDesc;
	_float4				m_vPrevPos = ZERO_VECTOR;

	//���� ���� �浹�� ����
	_bool				m_bCollision = false;

	//�浹 üũ�뵵
	_bool				m_bCollisionTemp = false;



};

END