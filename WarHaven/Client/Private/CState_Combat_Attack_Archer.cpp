#include "stdafx.h"
#include "CState_Combat_Attack_Archer.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Archer.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"
#include "CCamera_Follow.h"

CState_Combat_Attack_Archer::CState_Combat_Attack_Archer()
{
}

CState_Combat_Attack_Archer::~CState_Combat_Attack_Archer()
{
}

HRESULT CState_Combat_Attack_Archer::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;

	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 0.f;

	m_iStateChangeKeyFrame = 99;

	m_iDirectionRand = STATE_DIRECTION_N;

	return S_OK;
}


void CState_Combat_Attack_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	m_fMaxTime = 3.f;

	m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	if (!m_pCoreBone)
		assert(0);

	//m_fMatRotYRadian = frandom(0.f, 7.f);
	//m_fMatRotXRadian = frandom(4.f, 12.f);

	m_fMatRotYRadian = frandom(5.5f, 7.5f);
	m_fMatRotXRadian = frandom(4.1f, 11.3f);

	//m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	//if (!m_pCoreBone)
	//	assert(0);

	CState_Combat::Enter(pOwner, pAnimator, ePrevStateType);
}

void CState_Combat_Attack_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_Attack_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bAiming)
	{
		if (m_fTimeAcc < m_fMaxTime)
		{
			m_fTimeAcc += fDT(0);

			if (m_fTimeAcc > m_fMaxTime / 3.f)
				pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP1;

			else if (m_fTimeAcc > m_fMaxTime / 1.5f)
				pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP2;
		}
		else
			pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP3;

		if (m_fTimeAcc > m_fAIDelayTime)
			return m_iAINextState;

	}



	_float4 vCamLook = pOwner->Get_FollowCamLook();
	_float4x4 matRotY = XMMatrixRotationAxis(_float4(0.f, 1.f, 0.f, 0.f).XMLoad(), ToRadian(m_fMatRotYRadian));
	_float4x4 matRotX = XMMatrixRotationAxis(pOwner->Get_FollowCamRight().XMLoad(), ToRadian(m_fMatRotXRadian));
	vCamLook = vCamLook.MultiplyNormal(matRotY);
	vCamLook = vCamLook.MultiplyNormal(matRotX);

	/* À§ ¾Æ·¡¸¸ ²ª¾îÁà¾ßÇÔ */
	_float4x4 matOffset;

	_float4 vCamLookNoY = vCamLook;
	vCamLookNoY.y = 0.f;
	vCamLookNoY.Normalize();

	_float fDot = vCamLook.Dot(vCamLookNoY);
	_float fRadian = acosf(fDot);

	if (vCamLook.y < 0.f)
		fRadian *= -1.f;

	matOffset = XMMatrixRotationAxis(_float4(0.f, -1.f, 0.f, 0.f).XMLoad(), fRadian);

	pOwner->Get_Transform()->Set_NoLerp();
	pOwner->Get_Transform()->Set_Look(vCamLookNoY);

	m_pCoreBone->Set_PrevMatrix(matOffset);

	static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat() = matOffset;



	CUnit* pTargetUnit = pOwner->Get_TargetUnit();
	_float4 vLook = _float4(0.f, 0.f, 0.f, 1.f);
	if (!pTargetUnit)
	{
		m_vAIRandLook = _float4(frandom(0.f, 1.f), frandom(0.f, 1.f), frandom(0.f, 1.f));
		vLook = m_vAIRandLook;
		DoMove_AI_NoTarget(pOwner, pAnimator);
	}
	else
	{
		CTransform* pMyTransform = pOwner->Get_Transform();
		CPhysics* m_pPhysics = pOwner->Get_PhysicsCom();

		vLook =  pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
		vLook.y = 0.f;

		pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);
		m_pPhysics->Set_Dir(vLook);
	}




	return __super::Tick(pOwner, pAnimator);
}



void CState_Combat_Attack_Archer::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}





void CState_Combat_Attack_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 1000:
		/*effect*/
		if (ARCHER == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;


	default:
		break;
	}
}

void CState_Combat_Attack_Archer::Enter_Attack_Begin(CUnit* pOwner)
{
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKBEGIN, m_fInterPolationTime, m_fAnimSpeed);

	m_bMoveTrigger = false;

	m_iMinCancelAnimIndex = 30;

	m_iRand = random(0, 5);

	if (m_iRand == 0)
		m_bKeyInputable = true;
	else if (m_iRand == 1)
		m_bKeyInput = true;

}

void CState_Combat_Attack_Archer::Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	m_bAiming = true;

	m_iRand = random(0, 3);

	if (m_iRand == 0)
		m_fAIDelayTime = 0.f;

	else if (m_iRand == 1)
		m_fAIDelayTime = m_fMaxTime / 2.9f;

	else if (m_iRand == 2)
		m_fAIDelayTime = m_fMaxTime / 1.4f;

	else

		m_fAIDelayTime = m_fMaxTime + FLT_MIN;



	if (ePrevType == m_eStateType)
	{

		pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKBEGIN, FLT_MAX, FLT_MAX);
		pOwner->Set_AnimWeaponFrame(102);
	}

	if (ePrevType == STATE_ATTACK_BEGIN_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_POISION_ARCHER ||
		ePrevType == STATE_ATTACK_BEGIN_SNIPING_ARCHER)
	{
		m_fAnimSpeed = FLT_MIN;

		_uint iDirection = Get_Direction();

		if (iDirection != STATE_DIRECTION_END)
		{
			//_uint iWalkAnimIndex = pOwner->Get_PreAnimIndex();
			//_uint iMyMaxAnimIndex = 40;

			//while (iMyMaxAnimIndex < iWalkAnimIndex)
			//	iWalkAnimIndex -= 15;

			//pAnimator->Set_CurFrame(iWalkAnimIndex);
		}


		pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLOOP, FLT_MAX, FLT_MIN);
	}
}


void CState_Combat_Attack_Archer::Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
}

_bool CState_Combat_Attack_Archer::Check_ArrowRay(_float4* pOutPos)
{
	_float4 vStartPos = static_cast<CUnit_Archer*>(m_pOwner)->Get_CurArrow()->Get_ArrowHeadPos();
	_float4 vDir = static_cast<CUnit_Archer*>(m_pOwner)->Get_CurArrow()->Get_Transform()->Get_World(WORLD_RIGHT);
	_float fMaxDistance = static_cast<CUnit_Archer*>(m_pOwner)->Get_CurArrow()->Get_MaxDistance();

	_float fMinDist;
	_float4 vFinalHitPos;

	if (GAMEINSTANCE->Shoot_RaytoStaticActors(&vFinalHitPos, &fMinDist, vStartPos, vDir, fMaxDistance))
		*pOutPos = vFinalHitPos;

	list<CGameObject*>& listPlayers = GAMEINSTANCE->Get_ObjGroup(GROUP_PLAYER);
	list<PxController*> listPxControllers;
	for (auto& elem : listPlayers)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(elem);
		if (!pPlayer)
			continue;

		if (!pPlayer->Is_Valid())
			continue;

		CUnit* pUnit = pPlayer->Get_CurrentUnit();

		if (!pUnit->Is_Valid())
			continue;

		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pUnit->Get_Transform()->Get_World(WORLD_POS).XMLoad(), 1.5f))
			continue;

		CPhysXCharacter* pPhysXCom = GET_COMPONENT_FROM(pUnit, CPhysXCharacter);

		PxController* pController = pPhysXCom->Get_PxController(); 

		if (!pController)
			continue;

		listPxControllers.push_back(pController);
	}

	if (GAMEINSTANCE->Shoot_RaytoControllers(listPxControllers, fMinDist, &vFinalHitPos, vStartPos, vDir, fMaxDistance))
	{
		if (*pOutPos != vFinalHitPos)
		{
			CUser::Get_Instance()->Set_ArcherPoint(true);
		}
		else
		{
			CUser::Get_Instance()->Set_ArcherPoint(false);
		}

		*pOutPos = vFinalHitPos;
	}

	return true;
}
