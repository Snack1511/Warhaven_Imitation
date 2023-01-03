#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CAnimator;
class CHierarchyNode;
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
	enum ePhyxState {eIDLE, eSHOOT, eATTACKLOOP, eHIT, eATTACKLAUNCH, eEND};

public:
	void On_ChangePhase(ePhyxState eNextPhase);
	void Shoot_Crow(_float4 vShootPos, _float4 vShootDir);
public:
	void	Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);

public:
	_float4x4& Use_OwnerBoneOffset() { return m_OwnerBoneOffsetMatrix; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	CUnit* m_pOwnerUnit = nullptr;
	CCrowBoom* m_pCrowBoom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CHierarchyNode* m_pOwnerBone = nullptr;
	
	//PxConvexMesh* m_pConvexMesh = nullptr;
	//PxRigidDynamic* m_pActor = nullptr;

private:
	ePhyxState	m_eCurPhase = eIDLE;
	_float4		m_vStartPosition = ZERO_VECTOR;

	_float		m_fMaxSpeed = 1000.f;

	_float		m_fLoopTimeAcc = 0.f;
	_float		m_fMaxLoopTime = 0.5f;
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