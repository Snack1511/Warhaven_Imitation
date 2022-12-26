#include "stdafx.h"
#include "CQandaMeteor.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CCollider_Sphere.h"

#include "Transform.h"

#include "CUnit_Qanda.h"

CQandaMeteor::CQandaMeteor()
{
}
CQandaMeteor::~CQandaMeteor()
{
	//m_Test.clear();
}

CQandaMeteor* CQandaMeteor::Create()
{
	CQandaMeteor* pInstance = new CQandaMeteor;

	if (FAILED(pInstance->CQandaMeteor::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CQandaMeteor");
		return nullptr;
	}

	return pInstance;
}

HRESULT CQandaMeteor::Start()
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

HRESULT CQandaMeteor::Initialize_Prototype()
{
    //if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/Crow/SKEL_Crow_A00_15.fbx")))
    //    return E_FAIL;

	if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt.fbx")))
		return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Collider(COL_REDGROGGYATTACK, 1.f)))
		return E_FAIL;

#else



	if (FAILED(SetUp_Collider(COL_BLUEGROGGYATTACK, 1.f)))
		return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE


	m_hcCode = HASHCODE(CQandaMeteor);


    return CProjectile::Initialize_Prototype();
}

void CQandaMeteor::My_Tick()
{
	__super::My_Tick();

	if (!m_Shoot)
	{
		_float4 vLook = m_pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - Get_Transform()->Get_World(WORLD_POS);

		Get_Transform()->Set_World(WORLD_LOOK, vLook);
		m_Shoot = true;
	}


}

void CQandaMeteor::OnEnable()
{
	__super::OnEnable();

	m_Shoot = false;
}
void CQandaMeteor::OnDisable()
{
	static_cast<CUnit_Qanda*>(m_pOwnerUnit)->Collect_QandaProjectile(m_hcCode, this);
	__super::OnDisable();
}
