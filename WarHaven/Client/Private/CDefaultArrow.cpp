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

	SetUp_TrailEffect(
		_float4(0.f, -0.15f, 0.f, 1.f),	//Weapon Low
		_float4(0.f, 0.15f, 0.f, 1.f),	//Weapon High
		_float4(-0.15f, 0.f, 0.f, 1.f), //Left
		_float4(0.15f, 0.f, 0.f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.05f), // GlowFlow
		_float4(1.f, 0.1f, 0.1f, 0.4f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_04.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		40
	);


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

void CDefaultArrow::OnDisable()
{
	static_cast<CUnit_Archer*>(m_pOwnerUnit)->Collect_Arrow(m_hcCode, this);
	__super::OnDisable();
}



