#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;


END

BEGIN(Client)

class CPlayer;
class CUnit;

class CUnit_Lancer_Head
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUnit_Lancer_Head);

private:
	CUnit_Lancer_Head();
	virtual ~CUnit_Lancer_Head();

public:
	static	CUnit_Lancer_Head* Create(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit);


public:
	HRESULT	 SetUp_Model(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	CHierarchyNode* m_pOwnerBone = nullptr;
	CUnit*			m_pOwnerUnit = nullptr;

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	_float4 m_vPos = _float4(-163.f, 6.f, -1.2f);
};

END