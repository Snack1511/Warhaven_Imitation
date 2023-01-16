#include "stdafx.h"
#include "CPurpleArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CColorController.h"
#include "Loading_Manager.h"
#include "CCamera_Follow.h"

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
	tColorDesc.vTargetColor = _float4((160.f / 255.f), (20.f / 255.f), (20.f / 255.f), 0.1f);
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

	m_fMaxSpeed = 50.f;
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
				Death_Cam(pUnit);
				
				CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Poison_End",
					pUnit, pUnit->Get_Transform()->Get_World(WORLD_POS));

				CFunctor::Play_Sound(L"Effect_Poison_Boom", CHANNEL_EFFECTS,
					m_pTransform->Get_World(WORLD_POS), 1.f);

				DISABLE_GAMEOBJECT(this);
			}
				
			else
			{
				pUnit->On_PlusHp(m_fDamage, m_pOwnerUnit, false, 9999);

				AddictionEffect(pUnit);
				
				if (pUnit->Get_Status().fHP <= 0.f)
				{
					Death_Cam(pUnit);
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

void CPurpleArrow::Death_Cam(CUnit* pOtherUnit)
{
	CState::HIT_INFO tHitInfo;
	//ZeroMemory(&tHitInfo, sizeof(CState::HIT_INFO));

	tHitInfo.eHitType = CState::HIT_TYPE::eUP;
	tHitInfo.fKnockBackPower = 3.f;
	tHitInfo.fJumpPower = 0.f;

	pOtherUnit->On_PlusHp(m_fDamage * 5.f, m_pOwnerUnit, false, 3);
	pOtherUnit->Enter_State(pOtherUnit->Get_HitType().eGroggyState, &tHitInfo);

	if (pOtherUnit->Get_Status().fHP <= 0.f)
	{
		CTransform* pMyTransform = m_pOwnerUnit->Get_Transform();
		_float4 vMyPos = pMyTransform->Get_World(WORLD_POS);

		// 데드에 넘겨주기	
		if (CLoading_Manager::Get_Instance()->Get_LoadLevel() >= LEVEL_PADEN)
		{
			m_pOwnerUnit->Get_OwnerPlayer()->On_ScoreKDA_Kill(pOtherUnit->Get_OwnerPlayer());
			pOtherUnit->Get_OwnerPlayer()->On_ScoreKDA_Death();
		}


		CUser::Get_Instance()->Add_KillLog(m_pOwnerUnit->Get_OwnerPlayer(), pOtherUnit->Get_OwnerPlayer());

		if (pOtherUnit->Get_OwnerPlayer()->IsMainPlayer())
		{
			CUser::Get_Instance()->Turn_HeroGaugeFire(false);
			CUser::Get_Instance()->SetActive_SquardInfo(false);
			CUser::Get_Instance()->SetActive_HUD(false);
			CUser::Get_Instance()->Set_TargetInfo(m_pOwnerUnit->Get_OwnerPlayer()->Get_PlayerInfo());
			CUser::Get_Instance()->Toggle_DeadUI(true);

			// Other(죽은) 유닛의 타겟은 죽인 유닛을 바라볼 수 있도록 설정
			pOtherUnit->Get_FollowCam()->Set_FollowTarget(m_pOwnerUnit);
		}
		else
		{
			// 맞은놈이 메인 플레이어 였다면? 처치 로그
			if (pOtherUnit->Get_OwnerPlayer()->IsMainPlayer())
			{
				wstring wstrEnemyName = m_pOwnerUnit->Get_OwnerPlayer()->Get_PlayerName();
				CUser::Get_Instance()->Add_KillName(wstrEnemyName);
			}
		}

		pOtherUnit->On_Die();
	}

}
