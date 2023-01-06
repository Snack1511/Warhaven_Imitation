#include "stdafx.h"
#include "CAttack_Qanda.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CUnit_Qanda.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"
#include "CAnimWeapon_Crow.h"

CAttack_Qanda::CAttack_Qanda()
{
}

CAttack_Qanda::~CAttack_Qanda()
{
}

HRESULT CAttack_Qanda::Initialize()
{

	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;
	//Vertical은 전부 Land로 맞춤
	/* Setting for Blendable */

	m_iIdle_Index = 1;
	m_iLandRightIndex = 7;
	m_iLandLeftIndex = 99;
	m_iJumpFallRightIndex = 0;
	m_iJumpFallLeftIndex = 99;


	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 8;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 9;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 10;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 11;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 12;


	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 18;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 19;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 20;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 21;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 22;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 23;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 24;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 25;

	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 3;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 4;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 5;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 6;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 2;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_eWalkState = STATE_WALK_QANDA;
	m_eJumpState = STATE_JUMP_QANDA;
	m_eLandState = STATE_WALK_QANDA;
	m_eFallState = STATE_JUMPFALL_QANDA;
	m_eRunState = STATE_WALK_QANDA;
	m_eIdleState = STATE_IDLE_QANDA;
	m_eBounceState = STATE_WALK_QANDA;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	m_bLandMove = true;

#define INDEXCHECK(index) if (index >= 9999) return E_FAIL;

	INDEXCHECK(m_iIdle_Index);
	INDEXCHECK(m_iJumpFallLeftIndex);
	INDEXCHECK(m_iJumpFallRightIndex);

	INDEXCHECK(m_iLandLeftIndex);
	INDEXCHECK(m_iLandRightIndex);

	INDEXCHECK(m_iFinishedFrame);
	INDEXCHECK(m_iAttackEndIndex);

#define STATECHECK(state) if (state >= STATE_END) return E_FAIL;

	STATECHECK(m_eWalkState);
	STATECHECK(m_eRunState);
	STATECHECK(m_eJumpState);
	STATECHECK(m_eFallState);
	STATECHECK(m_eLandState);
	STATECHECK(m_eIdleState);
	STATECHECK(m_eBounceState);

	for (_uint i = 0; i < STATE_DIRECTION_END; ++i)
	{
		if (
			m_iRunRightAnimIndex[i] <= 0 ||
			m_iWalkRightAnimIndex[i] <= 0 ||
			m_iJumpRightAnimIndex[i] <= 0
			)
			return E_FAIL;
	}

	


	/* Blend Stop Event*/
	Add_KeyFrame(m_iStopIndex, 998);
	Add_KeyFrame(m_iAttackEndIndex, 999);
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.8f, 1000); // war 시작, 끝 사이
	Add_KeyFrame((m_iStopIndex + m_iAttackEndIndex) / 1.9f, 1001); // eng


	m_fMyAccel = 20.f;
	m_fMyMaxLerp = 0.1f;

	return S_OK;
}

void	CAttack_Qanda::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
}

void	CAttack_Qanda::OnCollisionStay(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType)
{
}

void CAttack_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

	m_AnimWeaponOffsetMatrix = pAnimCrow->Use_OwnerBoneOffset();

	m_fOffSetLerp = _float3(1.f, 1.5f, 1.f);

	pAnimCrow->Use_OwnerBoneOffset().m[3][0] = 0.f;
	pAnimCrow->Use_OwnerBoneOffset().m[3][1] = 0.5f;
	pAnimCrow->Use_OwnerBoneOffset().m[3][2] = 0.f;

	m_fMaxTime = 3.f;

	m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	if (!m_pCoreBone)
		assert(0);

	//if (!static_cast<CUnit_Qanda*>(pOwner)->Get_Crow())
	//	static_cast<CUnit_Qanda*>(pOwner)->Create_Crow();

	//DISABLE_COMPONENT(GET_COMPONENT_FROM(static_cast<CUnit_Qanda*>(pOwner)->Get_Crow(), CModel));


	static_cast<CUnit_Qanda*>(pOwner)->Enable_Trail(true);
	static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->On_ChangePhase(CAnimWeapon_Crow::eATTACKLOOP);
	GAMEINSTANCE->Start_RadialBlur(0.01f);
	

	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

void CAttack_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();
	static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->On_ChangePhase(CAnimWeapon_Crow::eIDLE);

	pAnimCrow->Use_OwnerBoneOffset() = m_AnimWeaponOffsetMatrix;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	static_cast<CUnit_Qanda*>(pOwner)->Enable_Trail(false);
	GAMEINSTANCE->Stop_RadialBlur();

}

STATE_TYPE CAttack_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

	}


	if (KEY(LBUTTON, AWAY))
		m_bKeyInput = true;

	_uint iDirection = Get_Direction();

	if (pOwner->Is_MainPlayer())
	{
		switch (m_eEnum)
		{
		case Client::CAttack_Qanda::Enum::eWALK:
			Update_Walk(pOwner, pAnimator);

			break;
		case Client::CAttack_Qanda::Enum::eRUN:
			Update_Run(pOwner, pAnimator);

			break;
		case Client::CAttack_Qanda::Enum::eJUMP:
			Update_Jump(pOwner, pAnimator);
			DoMove(iDirection, pOwner);

			break;
		case Client::CAttack_Qanda::Enum::eFALL:
			Update_Fall(pOwner, pAnimator);
			DoMove(iDirection, pOwner);
				

			break;
		case Client::CAttack_Qanda::Enum::eLAND:
			if (m_bLandMove)
			{
				DoMove(iDirection, pOwner);

				if(iDirection == STATE_DIRECTION_END)
					Update_Idle(pOwner, pAnimator);
				else
					Update_Walk(pOwner, pAnimator);
			}
				
			else
				Update_Land(pOwner, pAnimator);

			break;
		case Client::CAttack_Qanda::Enum::eIDLE:
			Update_Idle(pOwner, pAnimator);

			break;
		default:
			break;
		}

		if (m_bCam)
			Follow_MouseLook_Turn(pOwner);
	}

	/* =================================== */
	_float4 vCamLook = pOwner->Get_FollowCamLook();
	_float4x4 matRotY = XMMatrixRotationAxis(_float4(0.f, 1.f, 0.f, 0.f).XMLoad(), ToRadian(3.5f));
	_float4x4 matRotX = XMMatrixRotationAxis(pOwner->Get_FollowCamRight().XMLoad(), ToRadian(8.f));
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

	pOwner->Get_Transform()->Set_NoLerp();
	pOwner->Get_Transform()->Set_Look(vCamLookNoY);

	m_pCoreBone->Set_PrevMatrix(matOffset);

	static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat() = matOffset;

	/* =================================== */


		/* 모든 스태틱 충돌체와 캐릭터에게 ray를 쏴서 충돌체크 */
	_float4 vHitPos;
	if (CAttack_Qanda::Check_CrowRay(&vHitPos, pOwner))
	{
		_float4 vProjPos = CUtility_Transform::Get_ProjPos(vHitPos);
		static_cast<CUnit_Qanda*>(pOwner)->ReMap_Trail(vHitPos);
	}

	if (pAnimator->Get_CurAnimFrame() >= 160)
	{
		pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP3;
		return STATE_ATTACK_SHOOT_QANDA;
	}

	//if (pAnimator->Get_CurAnimFrame() >= 60)
	//{
	//	//_uint iFrame = pAnimator->Get_CurAnimFrame();
	//	////m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;
	//	//pAnimator->Set_CurFrame(0);
	//	pAnimator->Set_CurAnimIndex(ANIM_BASE_R, m_iWalkRightAnimIndex[Get_Direction()], ANIM_DIVIDE::eBODYLOWER);
	//	//pAnimator->Set_CurFrame(iFrame);
	//	//	return m_eStateType;
	//}


		

	return __super::Tick(pOwner, pAnimator);
}

STATE_TYPE CAttack_Qanda::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
			Move_Cycle(pAnimator, m_iWalkRightAnimIndex, ANIM_BASE_R);

		else
			On_EnumChange(Enum::eIDLE, pAnimator);

	}
	else
	{
		//Move(Get_Direction(), pOwner);


	}

	if (pAnimator->Is_ActionFinished())
		return m_eWalkState;

	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(SPACE, TAP))
	{
		On_EnumChange(Enum::eJUMP, pAnimator);
	}


	return eType;
}

STATE_TYPE CAttack_Qanda::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
			Move_Cycle(pAnimator, m_iRunRightAnimIndex, ANIM_BASE_R);

		else
			On_EnumChange(Enum::eIDLE, pAnimator);
	}
	else
	{
		//Move(Get_Direction(), pOwner);


	}

	if (pAnimator->Is_ActionFinished())
		return m_eRunState;

	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(SPACE, TAP))
	{
		On_EnumChange(Enum::eJUMP, pAnimator);
	}


	return eType;
}

STATE_TYPE CAttack_Qanda::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CAttack_Qanda::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CAttack_Qanda::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	//Move(Get_Direction(), pOwner);


	if (m_bLandMove || pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eIDLE, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eIdleState;



	return STATE_END;
}

STATE_TYPE CAttack_Qanda::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
	{
		On_EnumChange(Enum::eFALL, pAnimator);
	}
	else if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
	{
		if (KEY(CTRL, HOLD))
		{
			On_EnumChange(Enum::eWALK, pAnimator);
		}
		else
		{
			On_EnumChange(Enum::eRUN, pAnimator);
		}
	}
	else if (KEY(SPACE, TAP))
		On_EnumChange(Enum::eJUMP, pAnimator);

	else
		if (pAnimator->Is_ActionFinished())
			return m_eIdleState;


	return STATE_END;
}

void CAttack_Qanda::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}



void CAttack_Qanda::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
{
	_uint iDirection = Get_Direction();

	if (iDirection == STATE_DIRECTION_END)
		return;

	//현재실행중인애만 다르면 댐

	if (m_iAnimIndex != arrDirectionAnimIndices[iDirection])//|| pAnimator->Get_CurCycleAnimIndex() != arrDirectionAnimIndices[iDirection])
	{
		m_iAnimIndex = arrDirectionAnimIndices[iDirection];

		pAnimator->Set_CurAnimIndex(eAnimType, m_iAnimIndex, ANIM_DIVIDE::eBODYLOWER);
		pAnimator->Set_AnimSpeed(eAnimType, m_iAnimIndex, m_fDirectionAnimSpeed[iDirection]);
		pAnimator->Set_InterpolationTime(eAnimType, m_iAnimIndex, m_fInterPolationTime);
	}


	DoMove(iDirection, m_pOwner);
}

void CAttack_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
		if (QANDA == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;


	default:
		break;
	}
}

void CAttack_Qanda::Enter_Attack_Begin(CUnit* pOwner)
{
	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	pOwner->Get_Status().fBackStepSpeed = pOwner->Get_Status().fWalkSpeed;

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed * 0.35f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fRunSpeed;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(14, m_fInterPolationTime, m_fAnimSpeed);

	m_bMoveTrigger = false;

	m_iMinCancelAnimIndex = 30;

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_ZOOM);

}

void CAttack_Qanda::Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, _uint eCamLerpType)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	m_bAiming = true;
	if (ePrevType == m_eStateType)
	{

	}


	static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(16, 0.f, m_fAnimSpeed);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE(eCamLerpType));
}

void CAttack_Qanda::Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator)
{
	_float4x4 matOffset;
	matOffset.Identity();
	m_pCoreBone->Set_PrevMatrix(matOffset);
	static_cast<CUnit_Qanda*>(pOwner)->Get_CoreMat() = matOffset;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	//ENABLE_COMPONENT(GET_COMPONENT_FROM(static_cast<CUnit_Qanda*>(pOwner)->Get_Crow(), CModel));

	CUser::Get_Instance()->Set_CrossHairPos(_float4(0.f, 0.f, 0.3f, 1.f));
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}

void CAttack_Qanda::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CAttack_Qanda::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CAttack_Qanda::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CAttack_Qanda::Enum::eJUMP:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		m_pOwner->Get_PhysicsCom()->Set_Jump(m_pOwner->Get_Status().fJumpPower);
		{
			_uint iDirection = Get_Direction_Four();
			if (iDirection == STATE_DIRECTION_END)
				iDirection = STATE_DIRECTION_NW; // 제자리 점프

			pAnimator->Set_CurAnimIndex(ANIM_BASE_R, m_iJumpRightAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
		}


		break;
	case Client::CAttack_Qanda::Enum::eFALL:
		pAnimator->Set_CurAnimIndex(ANIM_BASE_R, m_iJumpFallRightIndex, ANIM_DIVIDE::eBODYLOWER);

		break;
	case Client::CAttack_Qanda::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		pAnimator->Set_CurAnimIndex(ANIM_BASE_R, m_iLandRightIndex, ANIM_DIVIDE::eBODYLOWER);

		Create_SoilEffect();
		break;

	case Client::CAttack_Qanda::Enum::eIDLE:
		pAnimator->Set_CurAnimIndex(ANIM_BASE_R, m_iIdle_Index, ANIM_DIVIDE::eBODYLOWER);
		break;
	default:
		break;
	}
}


_bool CAttack_Qanda::Check_CrowRay(_float4* pOutPos, CUnit* pOwner)
{
	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(m_pOwner)->Get_Crow();

	if (!pAnimCrow && m_pOwner != pOwner)
		return false;

	_float4 vStartPos = pAnimCrow->Get_Transform()->Get_World(WORLD_POS);// pAnimCrow->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir = pAnimCrow->Get_Transform()->Get_World(WORLD_LOOK) * -1.f;
	_float fMaxDistance = pAnimCrow->Get_MaxDistance();

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

		if (pUnit == pOwner)
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
		//if (*pOutPos != vFinalHitPos)
		//{
		//	CUser::Get_Instance()->Set_ArcherPoint(true);
		//}
		//else
		//{
		//	CUser::Get_Instance()->Set_ArcherPoint(false);
		//}

		*pOutPos = vFinalHitPos;
	}

	return true;
}
