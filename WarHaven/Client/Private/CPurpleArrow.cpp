#include "stdafx.h"
#include "CPurpleArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CColorController.h"

#include "CCollider_Sphere.h"

#include "CState.h"

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

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((200.f / 255.f), (20.f / 255.f), (20.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_SKEL;

	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);

	return S_OK;
}

HRESULT CPurpleArrow::Initialize_Prototype()
{
	m_bFrontPhysX = true;

    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt_Poison.fbx")))
        return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Colliders(COL_REDGUARDBREAK)))
		return E_FAIL;

#else

	if (FAILED(SetUp_Colliders(COL_BLUEGUARDBREAK)))
		return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE

	m_hcCode = HASHCODE(CPurpleArrow);
	m_vArrowHeadPos = _float4(1.2f, 0.f, 0.f);

	m_fMaxPoisonTime = 0.3f;
	m_fDamage = -5.f;

	m_fMaxSpeed = 40.f;
	m_fMaxDistance = 70.f;


    return CProjectile::Initialize_Prototype();
}

void CPurpleArrow::OnEnable()
{
	__super::OnEnable();

	m_bPoison = true;
	m_bAddiction = true;
	m_Addiction.clear();
}


void CPurpleArrow::OnDisable()
{
	if (m_bCollect)
		static_cast<CUnit_Archer*>(m_pOwnerUnit)->Collect_Arrow(m_hcCode, this);

	__super::OnDisable();

	Clear_Addiction();

	m_iTickCnt = 0;
}

HRESULT CPurpleArrow::SetUp_Colliders(COL_GROUP_CLIENT eColType)
{
	_float fRadius = 0.25f;
	_float4 vOffsetPos = ZERO_VECTOR;

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, eColType, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius * 2.f, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius * 2.f, vOffsetPos);

	Add_Component(pCollider);

	m_pCollider = pCollider;

	if (!m_pCollider)
		return E_FAIL;

	return S_OK;
}

void CPurpleArrow::My_Tick()
{
	__super::My_Tick();

	if (m_bPoison)
	{
		m_Test.clear();
		m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest",
			this, m_pTransform->Get_World(WORLD_POS)); //OnEnable 에서 생성시 마지막 위치에 이펙트가 보임
		m_bPoison = false;
	}
	
	if (m_eCurPhase == eSTICK)
	{
		CUnit* pUnit = static_cast<CUnit*>(m_pHitUnit);

		m_fCurPoisonTime += fDT(0);
	
		if (m_fCurPoisonTime > m_fMaxPoisonTime)
		{

			if (m_iTickCnt == m_iMaxTickCnt)
			{
				CState::HIT_INFO tHitInfo;
				//ZeroMemory(&tHitInfo, sizeof(CState::HIT_INFO));

				tHitInfo.eHitType = CState::HIT_TYPE::eUP;
				tHitInfo.fKnockBackPower = 3.f;
				tHitInfo.fJumpPower = 0.f;

				pUnit->On_PlusHp(m_fDamage * 5.f, m_pOwnerUnit, false, 3);
				pUnit->Enter_State(pUnit->Get_HitType().eGroggyState, &tHitInfo);
				
				if (pUnit->Get_Status().fHP <= 0.f)
					pUnit->On_Die();
				
				CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Poison_End",
					pUnit, pUnit->Get_Transform()->Get_World(WORLD_POS));

				DISABLE_GAMEOBJECT(this);
			}
				
			else
			{
				pUnit->On_PlusHp(m_fDamage, m_pOwnerUnit, false, 9999);

				AddictionEffect(pUnit);
				
				if (pUnit->Get_Status().fHP <= 0.f)
				{
					pUnit->On_Die();
					DISABLE_GAMEOBJECT(this);
				}

				++m_iTickCnt;
				m_fCurPoisonTime = 0.f;
			}



			
		}
	}
}

void CPurpleArrow::Set_ColliderType(eTEAM_TYPE eTeamType)
{
	if (eTeamType == eTEAM_TYPE::eBLUE)
		m_pCollider->Set_ColIndex(COL_BLUEGUARDBREAK);
	else
		m_pCollider->Set_ColIndex(COL_REDGUARDBREAK);
}

void CPurpleArrow::AddictionEffect(CUnit* pUnit)
{

	if (m_bAddiction)
	{
		if (!m_Test.empty())
		{
			for (auto& elem : m_Test)
			{
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut(); //기존 독이펙트 제거 후
			}
			m_Test.clear();
		}
	

		m_Addiction = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Addiction",
			pUnit, pUnit->Get_Transform()->Get_World(WORLD_POS)); //중독이펙트 

		m_bAddiction = false;
	}
}

void CPurpleArrow::Clear_Addiction()
{
	if (!m_Addiction.empty())
	{
		for (auto& elem : m_Addiction)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_Addiction.clear();
	}

	if (!m_Test.empty())
	{
		for (auto& elem : m_Test)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut(); //기존 독이펙트 제거 후
		}
		m_Test.clear();
	}
}
