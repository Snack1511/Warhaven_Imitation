#include "stdafx.h"
#include "CDefaultArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"



CDefaultArrow::CDefaultArrow()
{
}
CDefaultArrow::~CDefaultArrow()
{
	//m_Test.clear();
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

HRESULT CDefaultArrow::Start()
{
	__super::Start();


	//m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest", this, m_pTransform->Get_World(WORLD_POS));

	return S_OK;
}

HRESULT CDefaultArrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt.fbx")))
        return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Colliders(COL_REDATTACK)))
		return E_FAIL;

#else

	if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
		return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE


	m_hcCode = HASHCODE(CDefaultArrow);
	m_vArrowHeadPos = _float4(1.2f, 0.f, 0.f);

    return CProjectile::Initialize_Prototype();
}
