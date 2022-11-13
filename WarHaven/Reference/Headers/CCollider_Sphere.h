#pragma once
#include "Collider.h"

BEGIN(Engine)

struct COL_INFO_SPHERE
{
	_float fRadius = 0.f;
	_float4 vOffsetPos = { 0.f, 0.f, 0.f, 1.f };
	_float4 vFinalPos = { 0.f, 0.f, 0.f, 1.f };
};

class ENGINE_DLL CCollider_Sphere final
	: public CCollider
{
	DECLARE_PROTOTYPE(CCollider_Sphere);

private:
	CCollider_Sphere(_uint iGroupID);
	CCollider_Sphere(const CCollider_Sphere& origin);
	virtual ~CCollider_Sphere();

public:
	static	CCollider_Sphere* Create(_uint iGroupID, _float fRadius, const _uint& iColIndex, _float4 vOffsetPos, _float4x4 matTransformation, CHierarchyNode* pRefBone = nullptr);
	
public:
	list<COL_INFO_SPHERE>& Get_ColInfo() { return m_ColInfoList; }

public:
	void	Add_Collider(_float fRadius, _float4 vOffsetPos);

public:
	virtual HRESULT Initialize() override;
	virtual void Late_Tick() override;

#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

private:
	vector<BoundingSphere*> m_pSphere;
	vector<BoundingSphere*> m_pSphere_Original;

	list<COL_INFO_SPHERE>	m_ColInfoList;


};

END