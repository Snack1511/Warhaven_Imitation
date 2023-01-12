#include "stdafx.h"
#include "CQandaCrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Qanda.h"
#include "CRectEffects.h"

#include "CCollider_Sphere.h"
#include "Model.h"

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
    //if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/Crow/SKEL_Crow_A00_50.fbx")))
    //    return E_FAIL;

	if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt.fbx")))
		return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Collider(COL_REDFLYATTACKGUARDBREAK, 0.8f)))
		return E_FAIL;

#else

	if (FAILED(SetUp_Collider(COL_BLUEFLYATTACKGUARDBREAK, 0.8f)))
		return E_FAIL;


	//if (FAILED(SetUp_Colliders(COL_BLUEFLYATTACKGUARDBREAK)))
	//	return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE

	// m_pUnitCollider[ePartType]->Add_Collider(arrColliderDesc[i].fRadius, arrColliderDesc[i].vOffsetPos);

	m_hcCode = HASHCODE(CQandaCrow);
	m_vArrowHeadPos = _float4(1.2f, 0.f, 0.f);

	m_fMaxSpeed = 30.f;
	m_fMaxDistance = 200.f;

	//m_szMainBoneName = "0B_L_Hat_02";
	//m_szSubBoneName = "0B_L_Hat_02";

	m_szMainBoneName = "0B_R_Hand";
	m_szSubBoneName = "0B_R_Hand";

	//m_szMainBoneName = "0B_COM";
	//m_szSubBoneName = "0B_COM";

    return CProjectile::Initialize_Prototype();
}

void CQandaCrow::My_Tick()
{
	__super::My_Tick();

	if (m_eCurPhase == eSTICK || m_eCurPhase == eHIT)
	{
		m_fCurColliderTime += fDT(0);

		if (m_fCurColliderTime > m_fMaxColliderTime)
		{
			m_fCurColliderTime = -9999.f;
			//static_cast<CUnit_Qanda*>(m_pOwnerUnit)->Collect_QandaProjectile(m_hcCode, this);
			DISABLE_GAMEOBJECT(this);
		}

		if (!m_bShoot)
		{
			ENABLE_COMPONENT(m_pCollider);
			m_bShoot = true;
		}
		else
		{
			if (!m_bCollider)
			{
				DISABLE_COMPONENT(m_pCollider);
				m_bCollider = true;
			}
			
		}
	}
		


}

void CQandaCrow::OnEnable()
{
	
	__super::OnEnable();
	//DISABLE_COMPONENT(GET_COMPONENT(CModel));

	//On_ShootProjectile();


	m_fCurColliderTime = 0.f;
	m_bShoot = false;
	m_bCollider = false;
}
void CQandaCrow::OnDisable()
{
	if (m_bCollect)
		static_cast<CUnit_Qanda*>(m_pOwnerUnit)->Collect_QandaProjectile(m_hcCode, this);
	
	__super::OnDisable();
}

