#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Terrain final : public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Terrain);

protected:
	CMesh_Terrain(_uint iGroupIdx);
	virtual ~CMesh_Terrain();

public:
	static CMesh_Terrain* Create(_uint iGroupIdx, const _tchar* pHeightMapFilePath);
	static CMesh_Terrain* Create(_uint iGroupIdx, _uint iNumVerticesX, _uint iNumVerticesZ);

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

	_float4* Get_VerticesColor() { return m_pVerticesColor; }
	 _uint		Get_PhysXNumVertices() { return m_iPhysXNumVertices; }
	 _float3* Get_PhysXVerticesPos() { return m_pPhysXVerticesPos; }
public:
	void	Map_Vertex(_uint  iIndex, _float4 vPosition, _float4 vColor);
	void	ReMap_Vertices();
	void	Update_VertsNormal();

	

public:
	void	ReadyforPhysX(); // PhysX 메쉬 충돌체 생성 전에 호출해야함

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;
	_float4*				m_pVerticesColor = nullptr;

	_float3* m_pPhysXVerticesPos = nullptr;
	_uint m_iPhysXNumVertices = 0;

private:
	HRESULT		SetUp_HeightMap(const _tchar* pHeightMapFilePath);
	HRESULT		SetUp_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ);



};

END