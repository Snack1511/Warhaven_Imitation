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
	//�μ��� �� �ִ� ��ü
	// 1. �ν��Ͻ����� ��ü �����
	_uint		m_iNumInstance = 0;

	//�̸� ��ŷ�س��� �޽�
	PxConvexMesh* m_pConvexMesh = nullptr;

	// 2. �ν��Ͻ̴� ������ �ϳ� �ٿ�
	vector<PxRigidDynamic*>	m_vecRigidDynamics;
	
	// 3. �ν��Ͻ� ��Ʈ������ �������� ����
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
