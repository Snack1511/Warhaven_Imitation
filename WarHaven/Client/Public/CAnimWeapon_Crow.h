#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CAnimator;
class CHierarchyNode;
class CBoneCollider;
class CPhysics;
END


BEGIN(Client)

class CUnit;
class CCrowBoom;

class CAnimWeapon_Crow
	: public CGameObject
{
	DECLARE_PROTOTYPE(CAnimWeapon_Crow);

protected:
	CAnimWeapon_Crow();
	virtual ~CAnimWeapon_Crow();

public:
	static CAnimWeapon_Crow* Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName,
		_float fRadianX = 270.f, _float fRadianY = 0.f, _float fRadianZ = 270.f);

public:
	enum ePhyxState { eIDLE, eSHOOT, eATTACKLOOP, eHIT, eATTACKLAUNCH, eEND };

public:
	void	Crow_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	void	Crow_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	void	Crow_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	const ePhyxState& Get_Phase() { return m_eCurPhase; }
	const _float& Get_MaxDistance() { return m_fMaxDistance; }



public:
	void Boom_Crow();
	
	void ChangeColor_Charge();
	void ChangeColor_Shoot();
	void ChangeColor_End();

public:
	void On_ChangePhase(ePhyxState eNextPhase);
	void Shoot_Crow(_float4 vShootPos, _float4 vShootDir);

public:
	void	Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);

public:
	_float4x4& Use_OwnerBoneOffset() { return m_OwnerBoneOffsetMatrix; }

	void	Set_PhiysicsSpeed(_float fMaxSpeed);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUnit* m_pOwnerUnit = nullptr;
	CCrowBoom* m_pCrowBoom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CHierarchyNode* m_pOwnerBone = nullptr;
	
	CPhysics* m_pPhysics = nullptr;
	CBoneCollider* m_pBoneColider = nullptr;
	PxConvexMesh* m_pConvexMesh = nullptr;
	PxRigidDynamic* m_pActor = nullptr;

	_uint m_iMyColType = 0;



private:
	ePhyxState	m_eCurPhase = eIDLE;
	_float4		m_vStartPosition = ZERO_VECTOR;
	_float4		m_vChaseLook = ZERO_VECTOR;
	_float4		m_vChaseRight = ZERO_VECTOR;

	_float		m_fMaxSpeed = 1000.f;

	_float		m_fLoopTimeAcc = 0.f;
	_float		m_fMaxLoopTime = 0.5f;
	_float		m_fMaxShootTime = 2.2f;
	_float		m_fMaxDistance = 50.f;


private:
	_float4x4	m_OwnerBoneOffsetMatrix;
	_float		m_fTimeAcc = 0.f;

private:
	HRESULT		SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, 
		_float fRadianX = 270.f, _float fRadianY = 0.f, _float fRadianZ = 270.f);

private:
	virtual void	Late_Tick() override;
};

END