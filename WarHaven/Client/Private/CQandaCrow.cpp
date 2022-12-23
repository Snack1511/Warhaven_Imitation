#include "stdafx.h"
#include "CQandaCrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CCollider_Sphere.h"


CQandaCrow::CQandaCrow()
{
}
CQandaCrow::~CQandaCrow()
{
	//m_Test.clear();
}

CQandaCrow* CQandaCrow::Create()
{
	CQandaCrow* pInstance = new CQandaCrow;

	if (FAILED(pInstance->CQandaCrow::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CQandaCrow");
		return nullptr;
	}

	return pInstance;
}

HRESULT CQandaCrow::Start()
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

HRESULT CQandaCrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/Crow/SKEL_Crow_A00_50.fbx")))
        return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Collider(COL_REDFLYATTACKGUARDBREAK, 2.f)))
		return E_FAIL;

#else



	if (FAILED(SetUp_Collider(COL_BLUEFLYATTACKGUARDBREAK, 2.f)))
		return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE


	m_hcCode = HASHCODE(CQandaCrow);


    return CProjectile::Initialize_Prototype();
}

void CQandaCrow::My_Tick()
{
	__super::My_Tick();

	if (m_eCurPhase == eSTICK && m_eCurPhase == eHIT)
	{
		m_fCurColliderTime += fDT(0);

		if (!m_bShoot)
		{
			m_pOwnerUnit->Enable_FlyAttackCollider(true);
			m_pOwnerUnit->Enable_FlyAttackCollider(true);
			m_bShoot = true;
		}
		else
		{
			if (m_fCurColliderTime > m_fMaxColliderTime)
			{
				m_fCurColliderTime = -9999.f;
				m_pOwnerUnit->Enable_FlyAttackCollider(false);
				m_pOwnerUnit->Enable_FlyAttackCollider(false);
			}
			
		}
	}
		


}

void CQandaCrow::OnEnable()
{
	__super::OnEnable();

	m_fCurColliderTime = 0.f;
	m_bShoot = false;
}
void CQandaCrow::OnDisable()
{
	__super::OnDisable();
}

