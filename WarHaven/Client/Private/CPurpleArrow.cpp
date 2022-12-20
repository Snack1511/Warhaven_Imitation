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

	m_fMaxLoopTime = 0.2f;
	m_fDamage = -10.f;

    return CProjectile::Initialize_Prototype();
}

void CPurpleArrow::My_Tick()
{
	__super::My_Tick();
	
	if (m_eCurPhase == eSTICK)
	{
		m_fLoopTimeAcc += fDT(0);
	
		if (m_fLoopTimeAcc > m_fMaxLoopTime)
		{
			_float4 vOtherDir = m_pHitUnit->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);

			_float4 vCurLook = Get_Transform()->Get_World(WORLD_LOOK).Normalize();

			_bool bFace = false;

			//양수면 앞임.
			if (vCurLook.Dot(vOtherDir) > 0.f)
				bFace = true;
			else
				bFace = false;

			_float fDamage = m_fDamage;

			if (bFace)
				fDamage *= 2.f;

			static_cast<CUnit*>(m_pHitUnit)->On_PlusHp(fDamage, m_pOwnerUnit, bFace);
			++m_iTickCnt;
		}
	}
}
