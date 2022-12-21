#include "stdafx.h"
#include "CAttack_Archer.h"

#include "UsefulHeaders.h"


#include "CSword_Effect.h"
#include "CColorController.h"
#include "CUnit_Archer.h"

#include "HIerarchyNode.h"
#include "CAnimWeapon.h"

#include "CPhysXCharacter.h"

#include "CProjectile.h"

CAttack_Archer::CAttack_Archer()
{
}

CAttack_Archer::~CAttack_Archer()
{
}

HRESULT CAttack_Archer::Initialize()
{

	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;
	m_tHitInfo.bFace = false;

	m_eAnimDivide = ANIM_DIVIDE::eBODYUPPER;

	m_iStopIndex = 0;
	m_iAttackEndIndex = 0;


	m_eAnimLeftorRight = ANIM_BASE_R;

	m_iIdle_Index = 11;
	m_iLandRightIndex = 17;
	m_iLandLeftIndex = 8;
	m_iJumpFallRightIndex = 10;
	m_iJumpFallLeftIndex = 0;

	m_iRunLeftAnimIndex[STATE_DIRECTION_E] = 28;
	m_iRunLeftAnimIndex[STATE_DIRECTION_N] = 29;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NE] = 30;
	m_iRunLeftAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iRunLeftAnimIndex[STATE_DIRECTION_S] = 32;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SE] = 33;
	m_iRunLeftAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iRunLeftAnimIndex[STATE_DIRECTION_W] = 35;

	m_iRunRightAnimIndex[STATE_DIRECTION_E] = 38;
	m_iRunRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iRunRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iRunRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iRunRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iRunRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iRunRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iRunRightAnimIndex[STATE_DIRECTION_W] = 45;

	m_iWalkRightAnimIndex[STATE_DIRECTION_E] = 38;
	m_iWalkRightAnimIndex[STATE_DIRECTION_N] = 39;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NE] = 40;
	m_iWalkRightAnimIndex[STATE_DIRECTION_NW] = 41;
	m_iWalkRightAnimIndex[STATE_DIRECTION_S] = 42;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SE] = 43;
	m_iWalkRightAnimIndex[STATE_DIRECTION_SW] = 44;
	m_iWalkRightAnimIndex[STATE_DIRECTION_W] = 45;

	m_iWalkLeftAnimIndex[STATE_DIRECTION_E] = 28;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_N] = 29;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NE] = 30;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_S] = 32;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SE] = 33;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iWalkLeftAnimIndex[STATE_DIRECTION_W] = 35;


	m_iJumpRightAnimIndex[STATE_DIRECTION_E] = 13;
	m_iJumpRightAnimIndex[STATE_DIRECTION_N] = 14;
	m_iJumpRightAnimIndex[STATE_DIRECTION_S] = 15;
	m_iJumpRightAnimIndex[STATE_DIRECTION_W] = 16;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NW] = 12;
	m_iJumpRightAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpRightAnimIndex[STATE_DIRECTION_SW] = 99;

	m_iJumpLeftAnimIndex[STATE_DIRECTION_E] = 4;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_N] = 5;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_S] = 6;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_W] = 7;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NW] = 3;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_NE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SE] = 99;
	m_iJumpLeftAnimIndex[STATE_DIRECTION_SW] = 99;

	m_eWalkState = STATE_WALK_ARCHER_R;
	m_eJumpState = STATE_JUMP_ARCHER_R;
	m_eLandState = STATE_WALK_ARCHER_R;
	m_eFallState = STATE_JUMPFALL_ARCHER_R;
	m_eRunState = STATE_WALK_ARCHER_R;
	m_eIdleState = STATE_IDLE_ARCHER_R;
	m_eBounceState = STATE_WALK_ARCHER_R;


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 1.25f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 1.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 0.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 0.8f;

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
		if (m_iRunLeftAnimIndex[i] <= 0 ||
			m_iRunRightAnimIndex[i] <= 0 ||
			m_iWalkLeftAnimIndex[i] <= 0 ||
			m_iWalkRightAnimIndex[i] <= 0 ||
			m_iJumpLeftAnimIndex[i] <= 0 ||
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

void	CAttack_Archer::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
}

void	CAttack_Archer::OnCollisionStay(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType)
{
}

void CAttack_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
	m_pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");

	if (!m_pCoreBone)
		assert(0);

	}

	static_cast<CUnit_Archer*>(pOwner)->Enable_Trail(true);

	__super::Enter(pOwner, pAnimator, ePrevStateType);
}

void CAttack_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	pAnimator->Stop_ActionAnim();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	Exit_Aiming(pOwner);
	static_cast<CUnit_Archer*>(pOwner)->Enable_Trail(false);

}

STATE_TYPE CAttack_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(LBUTTON, AWAY))
		m_bKeyInput = true;


	if (pOwner->Is_MainPlayer())
	{
		switch (m_eEnum)
		{
		case Client::CAttack_Archer::Enum::eWALK:
			Update_Walk(pOwner, pAnimator);

			break;
		case Client::CAttack_Archer::Enum::eRUN:
			Update_Run(pOwner, pAnimator);

			break;
		case Client::CAttack_Archer::Enum::eJUMP:
			Update_Jump(pOwner, pAnimator);

			break;
		case Client::CAttack_Archer::Enum::eFALL:
			Update_Fall(pOwner, pAnimator);

			break;
		case Client::CAttack_Archer::Enum::eLAND:
			if (m_bLandMove)
				DoMove(Get_Direction(), pOwner);

			Update_Land(pOwner, pAnimator);

			break;
		case Client::CAttack_Archer::Enum::eIDLE:
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

	static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat() = matOffset;

	/* =================================== */


	/* 모든 스태틱 충돌체와 캐릭터에게 ray를 쏴서 충돌체크 */
	_float4 vHitPos;
	if (Check_ArrowRay(&vHitPos))
	{
		_float4 vProjPos = CUtility_Transform::Get_ProjPos(vHitPos);
		CUser::Get_Instance()->Set_CrossHairPos(vProjPos);
	}

	static_cast<CUnit_Archer*>(pOwner)->ReMap_Trail(vHitPos);



	return __super::Tick(pOwner, pAnimator);
}

STATE_TYPE CAttack_Archer::Update_Walk(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			if (m_eAnimLeftorRight == ANIM_BASE_L)
			{
				Move_Cycle(pAnimator, m_iWalkLeftAnimIndex, m_eAnimLeftorRight);
			}
			else
			{
				Move_Cycle(pAnimator, m_iWalkRightAnimIndex, m_eAnimLeftorRight);
			}

		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);

		}
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

STATE_TYPE CAttack_Archer::Update_Run(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE	eType = STATE_END;

	if (m_bBlendable)
	{
		if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		{
			if (m_eAnimLeftorRight == ANIM_BASE_L)
			{
				Move_Cycle(pAnimator, m_iRunLeftAnimIndex, m_eAnimLeftorRight);
			}
			else
			{
				Move_Cycle(pAnimator, m_iRunRightAnimIndex, m_eAnimLeftorRight);
			}


		}
		else
		{
			On_EnumChange(Enum::eIDLE, pAnimator);
		}
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

STATE_TYPE CAttack_Archer::Update_Jump(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);
	else if (pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eFALL, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CAttack_Archer::Update_Fall(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		On_EnumChange(Enum::eLAND, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eFallState;

	return STATE_END;
}

STATE_TYPE CAttack_Archer::Update_Land(CUnit* pOwner, CAnimator* pAnimator)
{
	//Move(Get_Direction(), pOwner);


	if (m_bLandMove || pAnimator->Is_CurAnimFinished())
		On_EnumChange(Enum::eIDLE, pAnimator);

	if (pAnimator->Is_ActionFinished())
		return m_eIdleState;



	return STATE_END;
}

STATE_TYPE CAttack_Archer::Update_Idle(CUnit* pOwner, CAnimator* pAnimator)
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

void CAttack_Archer::Create_SoilEffect()
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_R_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", m_pOwner, m_pOwner->Get_Transform()->Get_World(WORLD_POS));
}



void CAttack_Archer::Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType)
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

void CAttack_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
		if (ARCHER == pOwner->Get_Status().eClass)
			if (!pOwner->Is_Air())
				Create_SoilEffect();

		break;


	default:
		break;
	}
}

void CAttack_Archer::Enter_Attack_Begin(CUnit* pOwner)
{
	pOwner->Get_Status().fStoreSpeed = pOwner->Get_Status().fRunSpeed;
	pOwner->Get_Status().fBackStepSpeed = pOwner->Get_Status().fWalkSpeed;

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fWalkSpeed * 0.35f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fRunSpeed;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKBEGIN, m_fInterPolationTime, m_fAnimSpeed);

	m_bMoveTrigger = false;

	m_iMinCancelAnimIndex = 30;

	//pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_CANNON);
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_ZOOM);

}

void CAttack_Archer::Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, _uint eCamLerpType)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;



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

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE(eCamLerpType));
}

void CAttack_Archer::Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator)
{
	_float4x4 matOffset;
	matOffset.Identity();
	m_pCoreBone->Set_PrevMatrix(matOffset);
	static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat() = matOffset;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();

	CUser::Get_Instance()->Set_CrossHairPos(_float4(0.f, 0.f, 0.3f, 1.f));
	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

void CAttack_Archer::On_EnumChange(Enum eEnum, CAnimator* pAnimator)
{
	m_eEnum = eEnum;
	switch (eEnum)
	{
	case Client::CAttack_Archer::Enum::eWALK:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;
		break;
	case Client::CAttack_Archer::Enum::eRUN:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		break;
	case Client::CAttack_Archer::Enum::eJUMP:
		m_fMaxSpeed = m_pOwner->Get_Status().fRunSpeed;
		m_pOwner->Get_PhysicsCom()->Set_Jump(m_pOwner->Get_Status().fJumpPower);
		{
			_uint iDirection = Get_Direction_Four();
			if (iDirection == STATE_DIRECTION_END)
				iDirection = STATE_DIRECTION_NW; // 제자리 점프

			if (m_eAnimLeftorRight == ANIM_BASE_L)
				pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpLeftAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
			else
				pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpRightAnimIndex[iDirection], ANIM_DIVIDE::eBODYLOWER);
		}


		break;
	case Client::CAttack_Archer::Enum::eFALL:
		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallLeftIndex, ANIM_DIVIDE::eBODYLOWER);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iJumpFallRightIndex, ANIM_DIVIDE::eBODYLOWER);

		break;
	case Client::CAttack_Archer::Enum::eLAND:
		m_fMaxSpeed = m_pOwner->Get_Status().fWalkSpeed;

		if (m_eAnimLeftorRight == ANIM_BASE_L)
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandLeftIndex, ANIM_DIVIDE::eDEFAULT);
		else
			pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iLandRightIndex, ANIM_DIVIDE::eDEFAULT);

		Create_SoilEffect();
		break;

	case Client::CAttack_Archer::Enum::eIDLE:
		pAnimator->Set_CurAnimIndex(m_eAnimLeftorRight, m_iIdle_Index);
		break;
	default:
		break;
	}
}

_bool CAttack_Archer::Check_ArrowRay(_float4* pOutPos)
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
