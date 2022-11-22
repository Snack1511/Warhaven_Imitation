#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CShader;
class CPhysXCollider;
END

BEGIN(Client)

class CMesh_Particle
	: public CEffect
{
	DECLARE_PROTOTYPE(CMesh_Particle);

private:
	CMesh_Particle();
	CMesh_Particle(const CMesh_Particle& _origin);
	virtual ~CMesh_Particle();

public:
	static CMesh_Particle* Create(wstring wstrModelFilePath , _uint iNumInstance, wstring strName,
		_float fDensity = 1.f, _float fLifeTime = 3.f, wstring wstrTextureFilePath = wstring(), wstring wstrNormalTexturePath = wstring());

public:
	void			Start_Particle(_float4 vPos, _float4 vDir, _float fPower = 1.f);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

private:
	//부서질 수 있는 객체
	// 1. 인스턴싱으로 객체 만들고
	_uint		m_iNumInstance = 0;

	//미리 쿠킹해놓을 메쉬
	PxConvexMesh* m_pConvexMesh = nullptr;

	// 2. 인스턴싱당 피직스 하나 붙여
	vector<PxRigidDynamic*>	m_vecRigidDynamics;
	
	// 3. 인스턴싱 매트릭스에 피직스로 연동
	_float4x4* m_pInstanceMatrices = nullptr;

	_float	m_fTimeAcc = 0.f;
	_float	m_fLifeTime = 0.f;

	_bool		m_bCloned = false;

	_float	m_fDensity = 1.f;

private:
	HRESULT		SetUp_MeshParticle(wstring wstrModelFilePath);
	
private:
	virtual void My_Tick();
	virtual void My_LateTick();
};
END
