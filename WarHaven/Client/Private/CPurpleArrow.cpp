#include "stdafx.h"
#include "CPurpleArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

CPurpleArrow::CPurpleArrow()
{
}
CPurpleArrow::~CPurpleArrow()
{
	//m_Test.clear();
}

CPurpleArrow* CPurpleArrow::Create()
{
	CPurpleArrow* pInstance = new CPurpleArrow;

	if (FAILED(pInstance->CPurpleArrow::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CPurpleArrow");
		return nullptr;
	}

	return pInstance;
}

HRESULT CPurpleArrow::Start()
{
	__super::Start();

	m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest", this, m_pTransform->Get_World(WORLD_POS));

	return S_OK;
}

HRESULT CPurpleArrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt_Poison.fbx")))
        return E_FAIL;

    if (FAILED(SetUp_Colliders(COL_BLUEFLYATTACKGUARDBREAK)))
        return E_FAIL;

	m_hcCode = HASHCODE(CPurpleArrow);
	m_vArrowHeadPos = _float4(2.f, 0.f, 0.f);

	m_fMaxPoisonTime = 0.2f;
	m_fDamage = 10.f;

    return CProjectile::Initialize_Prototype();
}


void CPurpleArrow::OnEnable()
{
	__super::OnEnable();

	if(m_Test.empty())
		m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest", this, m_pTransform->Get_World(WORLD_POS));

}


void CPurpleArrow::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_Test)
	{
		static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
	}

	m_Test.clear();
}

void CPurpleArrow::My_Tick()
{
	__super::My_Tick();
	
	if (m_eCurPhase == eSTICK)
	{
		if (m_iTickCnt == m_iMaxTickCnt)
			return;

		m_fCurPoisonTime += fDT(0);
	
		if (m_fCurPoisonTime > m_fMaxPoisonTime)
		{
			static_cast<CUnit*>(m_pHitUnit)->On_PlusHp(m_fDamage, m_pOwnerUnit, false);
			++m_iTickCnt;
			m_fCurPoisonTime = 0.f;
		}
	}
}
