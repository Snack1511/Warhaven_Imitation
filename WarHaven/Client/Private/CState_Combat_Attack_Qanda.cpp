#include "stdafx.h"
#include "CState_Combat_Attack_Qanda.h"

#include "UsefulHeaders.h"

#include "CEffects_Factory.h"
#include "CUnit_Qanda.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"
#include "CAnimWeapon_Crow.h"

CState_Combat_Attack_Qanda::CState_Combat_Attack_Qanda()
{
}

CState_Combat_Attack_Qanda::~CState_Combat_Attack_Qanda()
{
}

HRESULT CState_Combat_Attack_Qanda::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;
	m_tHitInfo.bNoneHeadAttack = true;

	m_iStopIndex = 0;
	
	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.4f;

	return S_OK;
}

void CState_Combat_Attack_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

	m_AnimWeaponOffsetMatrix = pAnimCrow->Use_OwnerBoneOffset();

	m_fOffSetLerp = _float3(1.f, 1.5f, 1.f);

	m_fMatRotYRadian = frandom(5.5f, 7.5f);
	m_fMatRotXRadian = frandom(4.1f, 11.3f);

	pAnimCrow->Use_OwnerBoneOffset().m[3][0] = 0.f;
	pAnimCrow->Use_OwnerBoneOffset().m[3][1] = 0.5f;
	pAnimCrow->Use_OwnerBoneOffset().m[3][2] = 0.f;

	m_fMaxTime = 3.f;

	m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	if (!m_pCoreBone)
		assert(0);

	static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->On_ChangePhase(CAnimWeapon_Crow::eATTACKLOOP);

	m_bCharge = true;

	m_iRand = random(0, 2);

	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

void CState_Combat_Attack_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();
	static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->On_ChangePhase(CAnimWeapon_Crow::eIDLE);

	pAnimCrow->Use_OwnerBoneOffset() = m_AnimWeaponOffsetMatrix;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	static_cast<CUnit_Qanda*>(pOwner)->Enable_Trail(false);
}

STATE_TYPE CState_Combat_Attack_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pUnit = pOwner->Get_TargetUnit();

	if (pUnit)
	{
		CTransform* pMyTransform = pOwner->Get_Transform();

		_float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
		vLook.y = 0.f;
		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
	}

	if (m_bAiming)
	{
		if (m_fTimeAcc < m_fMaxTime)
		{
			m_fTimeAcc += fDT(0);

			// 첫번째
			if (m_fTimeAcc > m_fMaxTime / 3.f)
			{
				if (m_bCharge)
				{
					CUnit_Qanda* pQanda = static_cast<CUnit_Qanda*>(m_pOwner);

					CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Charge_End",
						pQanda->Get_Crow(), ZERO_VECTOR);

					pQanda->Turn_ChargeEffect(false);

					m_bCharge = false;
				}
				pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP1;
			}
			// 두번째
			else if (m_fTimeAcc > m_fMaxTime / 1.5f)
				pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP2;
		}
		// 세번째
		else
			pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP3;
	}


	/* =================================== */
	_float4 vCamLook = pOwner->Get_FollowCamLook();
	_float4x4 matRotY = XMMatrixRotationAxis(_float4(0.f, 1.f, 0.f, 0.f).XMLoad(), ToRadian(m_fMatRotYRadian));
	_float4x4 matRotX = XMMatrixRotationAxis(pOwner->Get_FollowCamRight().XMLoad(), ToRadian(m_fMatRotXRadian));
	vCamLook = vCamLook.MultiplyNormal(matRotY);
	vCamLook = vCamLook.MultiplyNormal(matRotX);

	/* 위 아래만 꺾어줘야함 */
	_float4x4 matOffset;

	_float4 vCamLookNoY = vCamLook;
	vCamLookNoY.y = 0.f;
	vCamLookNoY.Normalize();

	_float fDot = vCamLook.Dot(vCamLookNoY);
	_float fRadian = acosf(fDot);

	if (vCamLook.y < 0.f)
		fRadian *= -1.f;

	matOffset = XMMatrixRotationAxis(_float4(0.f, -1.f, 0.f, 0.f).XMLoad(), fRadian);

	m_pCoreBone->Set_PrevMatrix(matOffset);

	static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat() = matOffset;

	if (pAnimator->Get_CurAnimFrame() >= 160)
	{
		pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP3;
		return AI_STATE_COMBAT_SHOOT_QANDA;
	}



	return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
		//case 998:

		//	m_bAfterEffect = true;
		//	m_bHitEffect = true;
		//	pOwner->TurnOn_TrailEffect(true);

		//	/* dash Front */
		//	if (!pOwner->Is_Air())
		//	{
		//		pOwner->Set_DirAsLook();
		//		pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
		//		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
		//		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.7f;
		//		
		//	}

		//	

		//	m_bBlendable = false;
		//	if (m_eEnum == Enum::eWALK || m_eEnum == Enum::eRUN)
		//	pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);

		//	break;

		//case 999:

		//	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
		//	m_bAfterEffect = false;
		//	m_bBlood = false;
		//	pOwner->TurnOn_TrailEffect(false);

		//	//m_bBlendable = true;
		//	if (m_eAnimLeftorRight == ANIM_BASE_L)
		//		m_eAnimLeftorRight = ANIM_BASE_R;
		//	else
		//		m_eAnimLeftorRight = ANIM_BASE_L;


		//	break;

	case 1000:
		/*effect*/
		//if (QANDA == pOwner->Get_Status().eClass)
		//	if (!pOwner->Is_Air())
		//		Create_SoilEffect();

		break;


	default:
		break;
	}
}

void CState_Combat_Attack_Qanda::Enter_Attack_Begin(CUnit* pOwner)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(14, m_fInterPolationTime, m_fAnimSpeed);
	static_cast<CUnit_Qanda*>(pOwner)->Turn_ChargeEffect(true);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_QANDA);
}

void CState_Combat_Attack_Qanda::Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, _uint eCamLerpType)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	m_bAiming = true;

	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(16, 0.f, m_fAnimSpeed);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE(eCamLerpType));
}

void CState_Combat_Attack_Qanda::Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator)
{
	_float4x4 matOffset;
	matOffset.Identity();
	m_pCoreBone->Set_PrevMatrix(matOffset);
	static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat() = matOffset;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}
