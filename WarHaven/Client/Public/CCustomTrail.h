#pragma once
#include "CMesh.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CHierarchyNode;
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CCustomTrail
	: public CMesh
{
	DECLARE_PROTOTYPE(CCustomTrail);

private:
	CCustomTrail(_uint iGroupIdx);
	virtual ~CCustomTrail();

public:
	static CCustomTrail* Create(_uint iGroupIdx, _uint iTriCnt);

public:
	virtual _uint3 Get_Indices(_uint iIndex) const override;

public:
	void	ReMap_TrailBuffers(list<_float4>& NodesList, _float fWide);

public:
	// CMesh을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual	void	Tick() override;
	virtual	void	Late_Tick() override;

private:
	_uint	m_iTriCnt = 0;



};

END