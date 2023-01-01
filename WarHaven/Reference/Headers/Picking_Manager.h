#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CMesh;

class CPicking_Manager
{
	DECLARE_SINGLETON(CPicking_Manager)

private:
	CPicking_Manager();
	~CPicking_Manager();

public:
	HRESULT	Initialize(const GRAPHICDESC&	tGraphicDesc);

public:
	void	Regist_Mesh(CMesh* pMesh, _float fDistanceToPlayer);
	void	Execute_Picking();
	void	Compute_WorldRay();

public:
	_bool	Is_Picked(list<CGameObject*>& GameObjectList, _float4* pOut, _float4* pOutNormal = nullptr);
	_bool	Is_Picked_Cubes(vector<CGameObject*>& GameObjectList, _float4* pOut, _uint* pOutIndex, _float4* pOutNormal = nullptr);
	_bool	Is_Picked(CGameObject* pGameObject, _float4*	pOut, _float4* pOutNormal = nullptr);
	_bool	Is_Picked_Mesh(CMesh* pRenderer, _uint3* pOutPickedIndex, _float4* pOut, _float4* pOutNormal = nullptr);
	_bool	Is_Picked_FixedHeight(_float fHeight, _float4* pOut);

	_bool	Is_Picked(CMesh* pRenderer, _float4*	pOut, _float4* pOutNormal = nullptr);
	_bool	Is_Picked(CMesh* pRenderer, _uint3* pOutPickedIndex, _float4*	pOut,  _float4* pOutNormal = nullptr);


	_bool	Is_Picked(CMesh* pRenderer, _float4 vStartPos, _float4 vRayDir, _float4* pOut, _float4* pOutNormal = nullptr);

private:
	list<pair<_float, CMesh*>>	m_PickingMeshes;
	GRAPHICDESC		m_GraphicDesc;

	_float3			m_vRayDir;
	_float3			m_vRayPos;

private:
	void	Sort_Lists();
	void	Picking();
};

END