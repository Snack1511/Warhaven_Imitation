#include "stdafx.h"
#include "CQandaMeteor.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CCollider_Sphere.h"

#include "Transform.h"

#include "CUnit_Qanda.h"
#include "CUI_UnitHUD.h"

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

	pInstance->m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f));

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
		_float4(0.f, -5.f, 0.f, 1.f),	//Weapon Low
		_float4(0.f, 5.f, 0.f, 1.f),	//Weapon High
		_float4(-5.f, 0.f, 0.f, 1.f), //Left
		_float4(5.f, 0.f, 0.f, 1.f), //Right
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		_float4(1.f, 0.3f, 0.3f, 1.f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		120
	);

	m_EffectTest.clear();

	return S_OK;
}

HRESULT CQandaMeteor::Initialize_Prototype()
{
	//if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/Crow/SKEL_Crow_A00_15.fbx")))
	//    return E_FAIL;

	if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/ball/CannonBall.fbx")))
		return E_FAIL;

	if (FAILED(SetUp_Collider(COL_BLUEGROGGYATTACK, 1.f)))
		return E_FAIL;

	m_hcCode = HASHCODE(CQandaMeteor);
	m_fMaxSpeed = 30.f;


	return CProjectile::Initialize_Prototype();
}

void CQandaMeteor::My_Tick()
{
	__super::My_Tick();

	if (m_bShoot)
	{
		Turn_Trail(true);

		if (m_EffectTest.empty())
		{
			m_EffectTest = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Qanda_Sniping", this, ZERO_VECTOR);
		}
		m_bShoot = false;
	}



	if (m_eCurPhase == eSTICK)
	{
		if (m_pOwnerUnit->Is_MainPlayer())
		{
			CUnit::UNIT_STATUS eStatus = m_pOwnerUnit->Get_Status();
			m_pOwnerUnit->Shake_Camera(eStatus.fCamPower * 1.2f, eStatus.fCamTime * 1.2f);
		}

		DISABLE_GAMEOBJECT(this);
	}


}

void CQandaMeteor::OnEnable()
{
	__super::OnEnable();

	m_bShoot = true;

}

void CQandaMeteor::OnDisable()
{
	Turn_Trail(false);

	if (m_bCollect)
		static_cast<CUnit_Qanda*>(m_pOwnerUnit)->Collect_QandaProjectile(m_hcCode, this);

	if (!m_EffectTest.empty())
	{
		for (auto& elem : m_EffectTest)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut(0.5f);
		}
		m_EffectTest.clear();
	}

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Meteo_Boom", m_pTransform->Get_World(WORLD_POS),
		GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE));

	CFunctor::Play_Sound(L"Effect_Meteor_Collision", CHANNEL_EFFECTS, Get_Transform()->Get_World(WORLD_POS));

	if (m_pOwnerUnit->Is_MainPlayer())
	{
		if (m_pTargetUnit)
			m_pTargetUnit->Get_OwnerHUD()->SetActive_TargetUI(0, false);
	}
	__super::OnDisable();
}
