#pragma once
#include "MeshContainer.h"

BEGIN(Engine)
class ENGINE_DLL CInstanceMesh
	: public CMeshContainer
{
	DECLARE_PROTOTYPE(CInstanceMesh);

private:
	CInstanceMesh(_uint iGroupIndex);
	CInstanceMesh(const CInstanceMesh& _origin);
	virtual ~CInstanceMesh();

public:
	static CInstanceMesh* Create(wstring wstrInstanceDataPath, CResource_Mesh* pMeshResource, _float4x4 TransformMatrix);
	static CInstanceMesh* Create(_uint iNumInstance, CResource_Mesh* pMeshResource, _float4x4 TransformMatrix);
	static CInstanceMesh* Create(_uint iNumInstance, VTXINSTANCE* pInstanceData, CResource_Mesh* pMeshResource, _float4x4 TransformMatrix);

public:
	void	ReMap_Instances(_float4x4* pInstancesMatrices);
	void	ReMap_Instances(_uint iNumInstance, _float4x4* pInstancesMatrices);
	void	Set_SaveMemory() { m_bSaveMemory = true; }

public:
	virtual void	Release() override;
	virtual HRESULT Render() override;
	virtual void Start() override;

private:
	ComPtr<ID3D11Buffer>				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;
	_uint						m_iOriginNumInstance = 0;
	_bool						m_bSaveMemory = false;


	//_float* m_pInstanceSpeeds = nullptr;

private:
	HRESULT	SetUp_InstanceMesh(wstring wstrInstanceDataPath, CResource_Mesh* pResource, _float4x4 TransformMatrix);
	HRESULT	SetUp_InstanceMesh(_uint iNumInstance);
	HRESULT	SetUp_InstanceMesh(_uint iNumInstance, VTXINSTANCE* pInstanceData);


};
END
