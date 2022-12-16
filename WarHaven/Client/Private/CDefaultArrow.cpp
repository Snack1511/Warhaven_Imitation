#include "stdafx.h"
#include "CDefaultArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"

CDefaultArrow::CDefaultArrow()
{
}
CDefaultArrow::~CDefaultArrow()
{
}

CDefaultArrow* CDefaultArrow::Create()
{
	CDefaultArrow* pInstance = new CDefaultArrow;

	if (FAILED(pInstance->CDefaultArrow::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CDefaultArrow");
		return nullptr;
	}

	return pInstance;
}

HRESULT CDefaultArrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt.fbx")))
        return E_FAIL;

    if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
        return E_FAIL;
    

    return CProjectile::Initialize_Prototype();
}

void CDefaultArrow::OnDisable()
{
	__super::OnDisable();

	static_cast<CUnit_Archer*>(m_pOwnerUnit)->Collect_Arrow(HASHCODE(CDefaultArrow), this);

}
