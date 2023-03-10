#include "stdafx.h"
#include "CUnit.h"

#include "CEffects_Factory.h"

#include "CPlayer.h"
#include "GameInstance.h"

#include "CCamera_Default.h"
#include "CCollider_Sphere.h"
#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CNavigation.h"
#include "CCell.h"
#include "CCannon.h"
#include "CTrailEffect.h"
#include "CTrailBuffer.h"
#include "Loading_Manager.h"
#include "CMesh_Particle.h"
#include "CGameSystem.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"

#include "MeshContainer.h"

#include "CState.h"
#include "CState_Manager.h"
#include "CUser.h"

#include "CPhysXCharacter.h"

#include "CBoneCollider.h"

#include "CCamera_Follow.h"
#include "CState_Hit.h"

#include "CUtility_Transform.h"

#include "CAnimWeapon.h"

#include "CUser.h"
#include "CUI_Wrapper.h"
#include "CUI_UnitHUD.h"
#include "CUI_Revive.h"

#include "CColorController.h"
#include "CTeamConnector.h"

#include "CUnit_Priest.h"
#include "CUI_UnitHUD.h"

#include "CGlider.h"
#include "CUI_UnitHP.h"	

#include "CRectEffects.h"

#define PHYSX_ON

CUnit::CUnit()
{
}

CUnit::~CUnit()
{
	SAFE_DELETE(m_pCurState);
}

void CUnit::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (eOtherColType == COL_BLUETEAM || eOtherColType == COL_REDTEAM)
		return;

	if (eOtherColType == COL_REVIVE)
	{
		if (pOtherObj == m_pOwnerPlayer)
			return;

		m_pAdjRevivalPlayer = static_cast<CPlayer*>(pOtherObj);

		if (m_pAdjRevivalPlayer->Get_Team() != m_pOwnerPlayer->Get_Team())
			m_pAdjRevivalPlayer = nullptr;

		if (m_pAdjRevivalPlayer)
		{
			if (m_bIsMainPlayer)
			{
				if (!m_pAdjRevivalPlayer->Get_UnitHUD())
					return;

				m_pAdjRevivalPlayer->Get_UnitHUD()->Get_ReviveUI()->Set_ReviveIcon(1);
			}
		}

		return;
	}
	else if (eOtherColType == COL_CANNON)
	{
		m_pAdjCannon = static_cast<CCannon*>(pOtherObj);

		if (m_bIsMainPlayer)
		{
			CUser::Get_Instance()->Set_InteractKey(32);
			CUser::Get_Instance()->Set_InteractText(TEXT("????"));
			CUser::Get_Instance()->Set_InteractTarget(m_pAdjCannon);

			CUser::Get_Instance()->SetActive_InteractUI(true);
		}
	}
	//else if (eMyColType == COL_BLUECURE || eMyColType == COL_REDCURE)
	//{
	//	if (pOtherObj)
	//	{
	//		m_CureObjects.push_back(pOtherObj);

	//		//_float4 vCurLook = Get_Transform()->Get_World(WORLD_LOOK).Normalize();


	//		////?????? ????.
	//		//if (vCurLook.Dot(vOtherDir) > 0.f)
	//		//	tOtherHitInfo.bFace = true;
	//		//else
	//		//	tOtherHitInfo.bFace = false;

	//	}
	//	return;
	//}

	//else if (eOtherColType == COL_BLUECURE || eOtherColType == COL_REDCURE)
	//	return;

	if (!pOtherObj)
	{
		//????
		On_FallDamage(vHitPos.w);
		On_Sliding(vHitPos);
	}

	/* ?????? ?????? Unit?? ?????? Return */
	/* ================================================= */
	CUnit* pOtherUnit = nullptr;
	pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);



	if (!pOtherUnit)
		return;

	/* ================================================= */

	/* ???? hit ?????? delay ???? */
	if (m_fHitDelayAcc > 0.f)
		return;

	m_fHitDelayAcc = m_fHitDelayTime;

	/* SHAKE */
	Shake_Camera(m_tUnitStatus.fCamPower, m_tUnitStatus.fCamTime);

	STATE_TYPE	eFinalHitState = STATE_END;


	UNIT_STATUS& tOtherStatus = pOtherUnit->Get_Status();

	if (!pOtherUnit->Get_CurStateP()) //???? ???? ???? null ????
		return;

	CState::HIT_INFO tOtherHitInfo = pOtherUnit->Get_CurStateP()->Get_HitInfo();

	tOtherHitInfo.vDir = (m_pTransform->Get_World(WORLD_POS) - vHitPos);
	tOtherHitInfo.vDir.y = 0.f;
	tOtherHitInfo.vDir.Normalize();

	tOtherHitInfo.pOtherUnit = pOtherUnit;

	//???? ???? ????
	_float4 vOtherDir = pOtherUnit->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);

	_float4 vCurLook = Get_Transform()->Get_World(WORLD_LOOK).Normalize();


	//?????? ????.
	if (vCurLook.Dot(vOtherDir) > 0.f)
		tOtherHitInfo.bFace = true;
	else
		tOtherHitInfo.bFace = false;

	/* ?????? ???? ?????? ???????? */
	switch (eMyColType)
	{
		/* ???? ???? ???? */
	case COL_BLUEGUARD:
	case COL_REDGUARD:
		On_GuardHit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

		/* ???? ???? ???? */
	case COL_BLUEHITBOX_BODY:
	case COL_REDHITBOX_BODY:
		tOtherHitInfo.bHeadShot = false;
		On_Hit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

	case COL_BLUEHITBOX_HEAD:
	case COL_REDHITBOX_HEAD:
		if (!tOtherHitInfo.bNoneHeadAttack)
			tOtherHitInfo.bHeadShot = true;
		else
			tOtherHitInfo.bHeadShot = false;

		On_Hit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

	default:
		break;

	}


}

void CUnit::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	/*if (eOtherColType == COL_BLUETEAM || eOtherColType == COL_REDTEAM)
	{
		if (static_cast<CUnit*>(pOtherObj)->Is_MainPlayer())
			return;

		_float4 vCurPos = pOtherObj->Get_Transform()->Get_World(WORLD_POS);

		_float4 vDir = CUtility_Transform::Get_Dir_2D(m_pTransform, pOtherObj->Get_Transform());

		vCurPos += vDir * fDT(0);
		static_cast<CUnit*>(pOtherObj)->Teleport_Unit(vCurPos);

	}*/
}

void CUnit::Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	if (eOtherColType == COL_REVIVE)
	{
		if (pOtherObj == m_pOwnerPlayer)
			return;

		// UI Off
		if (m_bIsMainPlayer)
		{
			if (!m_pAdjRevivalPlayer)
				return;

			m_pAdjRevivalPlayer->Get_UnitHUD()->Get_ReviveUI()->Set_ReviveIcon(0);
		}

		if (static_cast<CPlayer*>(pOtherObj)->Get_Team() == m_pOwnerPlayer->Get_Team())
		{
			m_pAdjRevivalPlayer = nullptr;
		}

	}
	else if (eOtherColType == COL_CANNON)
	{
		if (!m_pAdjCannon)
			return;

		if (m_bIsMainPlayer)
			CUser::Get_Instance()->SetActive_InteractUI(false);

		m_pAdjCannon = nullptr;
	}
}

void CUnit::Play_Sound(wstring wstrFileName, _uint iGroupIndex, _float fVolume)
{
	CFunctor::Play_Sound(wstrFileName, iGroupIndex, Get_Transform()->Get_World(WORLD_POS), fVolume);
}

_bool CUnit::Is_Air()
{
	return m_pPhysics->Get_Physics().bAir;
}

void CUnit::Set_DirAsLook()
{
	_float4 vLook = Get_FollowCamLook();
	vLook.y = 0.f;

	m_pTransform->Set_LerpLook(vLook, 0.3f);
	m_pPhysics->Set_Dir(vLook);
}

void CUnit::Set_LookToTarget()
{
	if (!m_pOwnerPlayer->Get_TargetPlayer())
		return;

	CUtility_Transform::LookAt(m_pTransform, m_pOwnerPlayer->Get_TargetPlayer()->Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS), true);

}

void CUnit::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{

}

void CUnit::Set_Passes(VTXANIM_PASS_TYPE ePassType)
{
	vector<pair<_uint, class CMeshContainer*>>& vecMeshCont = m_pModelCom->Get_MeshContainers();
	for (auto& pMC : vecMeshCont)
	{
		pMC.second->Set_CurPass(ePassType);
	}
}

void CUnit::Synchronize_CamPos()
{
	if (!m_pFollowCam)
		return;

	m_pFollowCam->Synchronize_Position();
}

void CUnit::Shake_Camera(_float fPower, _float fTime)
{
	m_pFollowCam->Start_ShakingCamera(fPower, fTime);
}

void CUnit::Lerp_Camera(const _uint& iCameraLerpType)
{
	GET_COMPONENT_FROM(m_pFollowCam, CScript_FollowCam)->Start_LerpType((CScript_FollowCam::CAMERA_LERP_TYPE)iCameraLerpType);
}

void CUnit::On_Respawn()
{
	m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	m_bRespawn = true;


	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.3f;
	tColorDesc.fFadeOutStartTime = 1.f;
	tColorDesc.fFadeOutTime = 3.f;
	tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
	tColorDesc.iMeshPartType = MODEL_PART_BODY;
	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);
	tColorDesc.iMeshPartType = MODEL_PART_HEAD;
	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);

	if (m_bIsMainPlayer)
		Play_Voice(L"Voice_Respawn", 1.f, false);

}

void CUnit::On_Reborn()
{
	ENABLE_GAMEOBJECT(this);
	m_pOwnerPlayer->On_Reborn();
	On_Respawn();
}

void CUnit::On_Die()
{
	m_DeathStones.clear();

	m_pOwnerPlayer->On_Die();

	m_bDie = false;
	m_fDeadTimeAcc = 0.f;

	if (m_pOwnerPlayer->Get_PlayerName() == TEXT("EnemyFinal"))
		CUser::Get_Instance()->Enable_SkinPopup(0);

	_float4 vPos = Get_Transform()->Get_World(WORLD_POS);
	vPos.y += 1.f;
	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MATRIX_IDENTITY);

	m_DeathStones = CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle_Death(L"DeathStoneParticle", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
	vPos.y -= 0.7f;

	_float4x4 vCamMatrix = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KillSmoke_Right", vPos, vCamMatrix);

	DISABLE_GAMEOBJECT(this);


	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
		//SAFE_DELETE(m_pCurState);
	}

	if (m_bIsMainPlayer)
	{
		GAMEINSTANCE->Start_GrayScale(1.f);
	}

}

_float CUnit::Calculate_Damage(_bool bHeadShot, _bool bGuard)
{
#define HEADSHOTRATIO 1.5f
#define GUARDSUCCESS 0.1f

	//?????????? 1.5??

	_float fDamage = m_tUnitStatus.fAttackDamage;
	fDamage *= m_pCurState->Get_DamagePumping();
	fDamage *= (bHeadShot) ? HEADSHOTRATIO : 1.f;
	fDamage *= (bGuard) ? GUARDSUCCESS : 1.f;
	fDamage *= -1.f;

	_float fRandom = frandom(0.8f, 1.2f);

	fDamage *= fRandom;

	return fDamage;
}

_bool CUnit::On_PlusHp(_float fHp, CUnit* pOtherUnit, _bool bHeadShot, _uint iDmgType)
{
	if (bHeadShot)
		m_pOwnerPlayer->Set_DeadByHeadshot(bHeadShot);

	m_tUnitStatus.fHP += fHp;

	/*?????? ????*/
	if (pOtherUnit->m_bIsMainPlayer)
	{
		if (bHeadShot)
		{
			Play_Sound(L"Effect_HeadShot");
			CUser::Get_Instance()->Enable_DamageFont(0, fHp);
		}
		else
		{
			CUser::Get_Instance()->Enable_DamageFont(iDmgType, fHp);
		}
	}

	if (pOtherUnit)
		pOtherUnit->m_pOwnerPlayer->On_PlusGauge(3.f);

	if (m_tUnitStatus.fHP <= 0.f)
	{
		m_tUnitStatus.fHP = 0.f;
		if (pOtherUnit)
			pOtherUnit->m_pOwnerPlayer->On_PlusGauge(3.f);

		return false;
	}
	else if (m_tUnitStatus.fHP >= m_tUnitStatus.fMaxHP)
	{
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}

	return true;
}

void CUnit::On_FallDamage(_float fFallPower)
{
	/*???? ??????*/
	Effect_Fall(fFallPower);

	if (CUser::Get_Instance()->Get_CurLevel() == LEVEL_TYPE_CLIENT::LEVEL_HWARA)
		CUser::Get_Instance()->SetActive_InteractUI(false);


	//-0.6~ -3?? fFallPower

	//???????? ?????????? -> fDT?? ?????? -> fFallPower?? ??????
	_float fFinalFallPower = (fFallPower) / fDT(0);

	if (fFinalFallPower > -13.f)
		return;

	m_tUnitStatus.fHP += fFinalFallPower * 15.f;

	Shake_Camera(m_tUnitStatus.fCamPower, m_tUnitStatus.fCamTime);

	if (m_tUnitStatus.fHP <= 0.f)
	{
		Get_CurStateP()->Play_Voice(this, L"Voice_Dead", 1.f);
		m_tUnitStatus.fHP = 0.f;
		On_Die();
	}
	else if (m_tUnitStatus.fHP >= m_tUnitStatus.fMaxHP)
	{
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}
	else
	{
		Get_CurStateP()->Play_Voice(this, L"Voice_Hit", 1.f);
	}

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Turn_BloodOverLay(m_tUnitStatus.fHP / m_tUnitStatus.fMaxHP);
	}
}

void CUnit::On_Sliding(_float4 vHitNormal)
{
	//if (!m_bIsMainPlayer)
	return;

	vHitNormal.w = 0.f;

	/* 1. up?? ???? */

	_float4 vUp = _float4(0.f, 1.f, 0.f, 0.f);
	_float fDot = vUp.Dot(vHitNormal);

	//1???? ???? ?? 0.5?? 45?? 0???? ??

	if (fDot > 0.4f && fDot < 0.6f)
	{
		Enter_State(STATE_SLIDE_BEGIN_PLAYER);
	}
	else if (fDot > 0.9f)
		Enter_State(STATE_SLIDE_END_PLAYER);

}


void CUnit::On_Attack(CState* pState)
{
	_int iKeyFrame = pState->Get_StateChangeKeyFrame();

	if (iKeyFrame > 0)
		m_fAttackDelay = (_float)iKeyFrame / 60.f;
}

_bool CUnit::Is_Weapon_R_Collision()
{
	if (!m_pWeaponCollider_R)
		return false;

	return m_pWeaponCollider_R->Is_Collision();
}

_bool CUnit::Is_Weapon_R_CCT_Collision()
{
	if (!m_pWeaponCollider_R)
		return false;

	return m_pWeaponCollider_R->Is_CCT_Collision();
}

CPath* CUnit::Get_CurPath()
{
	return m_pOwnerPlayer->Get_CurPath();
}

CPath* CUnit::Get_StartMainPath()
{
	return m_pOwnerPlayer->Get_CurMainPath();
}

CUnit* CUnit::Get_TargetUnit()
{
	if (!m_pOwnerPlayer->Get_TargetPlayer())
		return nullptr;

	return m_pOwnerPlayer->Get_TargetPlayer()->Get_CurrentUnit();
}

CGameObject* CUnit::Get_TargetObject()
{
	return m_pOwnerPlayer->Get_TargetObject();
}


void CUnit::Enter_State(STATE_TYPE eType, void* pData)
{
	if (!m_bControlable)
		return;

	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
	}

	SAFE_DELETE(m_pCurState);

	STATE_TYPE ePrevType = m_eCurState;
	m_eCurState = eType;


	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();

	m_pCurState->Enter(this, m_pAnimator, ePrevType, pData);

}

void CUnit::Reserve_State(STATE_TYPE eType)
{
	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
		SAFE_DELETE(m_pCurState);
	}

	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();
	m_eCurState = eType;

}

void CUnit::Teleport_Unit(_float4 vPosition)
{
#ifdef PHYSX_ON
	m_pTransform->Set_World(WORLD_POS, vPosition);
	m_pTransform->Make_WorldMatrix();
	GET_COMPONENT(CPhysXCharacter)->Set_Position(vPosition);

#endif
}

_float4 CUnit::Get_HitPos()
{
	return  m_pWeaponCollider_R->Get_HitPos();
}

_float4x4 CUnit::Get_HitMatrix()
{
	return  m_pWeaponCollider_R->Get_HitMatrix();
}

CUI_UnitHUD* CUnit::Get_OwnerHUD()
{
	return m_pOwnerPlayer->Get_UnitHUD();
}


HRESULT CUnit::MakeUp_Unit(const UNIT_DESC& tUnitDesc)
{


	return S_OK;
}

void CUnit::On_ChangeClass()
{
	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
		//SAFE_DELETE(m_pCurState);
	}
}

HRESULT CUnit::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);


	Add_Component(CPhysics::Create(0));




#ifdef PHYSX_ON

	//PhysX?????? : ?????? ????
	CPhysXCharacter::PHYSXCCTDESC tDesc;

	tDesc.fRadius = 0.3f;
	tDesc.fHeight = 1.2f;

	CPhysXCharacter* pPhysXCharacter = CPhysXCharacter::Create(CP_BEFORE_TRANSFORM, tDesc);
	Add_Component(pPhysXCharacter);
#endif // PHYSX_OFF


	return S_OK;
}

HRESULT CUnit::Initialize()
{
	if (!m_pOwnerPlayer->IsMainPlayer())
	{
		m_pNavigation = CNavigation::Create(CP_AFTER_TRANSFORM, nullptr, nullptr);
		Add_Component<CNavigation>(m_pNavigation);
		m_pNavigation->Set_Layers(&CGameSystem::Get_Instance()->Get_CellLayer());
	}

	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);
	m_pPhysics = GET_COMPONENT(CPhysics);

#ifdef PHYSX_ON
	m_pPhysXCharacter = GET_COMPONENT(CPhysXCharacter);
	if (!m_pPhysXCharacter)
		return E_FAIL;
#endif // PHYSX_OFF


	if (!m_pModelCom)
		return E_FAIL;

	if (!m_pAnimator)
		return E_FAIL;

	if (!m_pPhysics)
		return E_FAIL;


	if (FAILED(m_pModelCom->SetUp_AnimModel_LOD()))
		return E_FAIL;

	m_pGlider = CGlider::Create(L"../bin/resources/meshes/Riding/Glider_10.fbx",
		L"../bin/resources/Animations/Common/A_Gliding_01.fbx", this, "0B_Spine1", 270.f, 180.f, 90.f);

	CREATE_GAMEOBJECT(m_pGlider, GROUP_PLAYER);
	DISABLE_GAMEOBJECT(m_pGlider);


	return S_OK;
}

HRESULT CUnit::Start()
{
	__super::Start();

	if (m_pWeaponCollider_R)
		DISABLE_COMPONENT(m_pWeaponCollider_R);

	if (m_pWeaponCollider_L)
		DISABLE_COMPONENT(m_pWeaponCollider_L);

	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	//m_pAnimator->Set_CurAnimIndex(ANIM_BASE, 22);
	//DISABLE_COMPONENT(m_pPhysics);

#ifdef PHYSX_ON
	m_pPhysics->Set_NaviOn();
#endif

	CallBack_CollisionEnter += bind(&CUnit::Unit_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CUnit::Unit_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CUnit::Unit_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	m_pPhysics->Set_Jump(0.f);

	//if (!m_pCurState)
	//{
	//	//Call_MsgBox(L"???? ???? ??????");
	//	//return E_FAIL;
	//}
	//else
	//	m_pCurState->Enter(this, m_pAnimator, m_eCurState);

	if (m_pUnitCollider[BODY])
		ENABLE_COMPONENT(m_pUnitCollider[BODY]);
	if (m_pUnitCollider[HEAD])
		ENABLE_COMPONENT(m_pUnitCollider[HEAD]);

	/* PASS */
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	//m_pModelCom->Set_ShaderPass(MODEL_PART_FACE, VTXANIM_PASS_FACE);

	if (m_pGlider)
		m_pGlider->SetUp_GliderTrail();

	m_EyeFlare.clear();

	return S_OK;
}

void CUnit::OnEnable()
{
	__super::OnEnable();
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	if (m_pCurState)
		m_pCurState->Enter(this, m_pAnimator, m_eCurState);

	On_InitSetting();

	for (_int i = 0; i < COOL_END; ++i)
		m_fCoolAcc[i] = 0.f;

	if (m_bRespawn)
	{
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ReSpawnLight_0"), this, m_pTransform->Get_World(WORLD_POS));
		Create_Light(this, _float4(0.f, 0.5f, 0.f), 5.f, 0.f, 1.f, 1.f, 1.f, RGB(255, 160, 50), false);
		//m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}

	m_bRespawn = false;


	if (m_pAnimWeapon)
		ENABLE_GAMEOBJECT(m_pAnimWeapon);
}

void CUnit::OnDisable()
{
	__super::OnDisable();
	if (m_pAnimWeapon)
		DISABLE_GAMEOBJECT(m_pAnimWeapon);


}


void CUnit::Enable_HitBoxColliders(_bool bEnable)
{
	if (bEnable)
	{
		ENABLE_COMPONENT(m_pUnitCollider[UNITCOLLIDER::BODY]);
		ENABLE_COMPONENT(m_pUnitCollider[UNITCOLLIDER::HEAD]);

	}
	else
	{
		DISABLE_COMPONENT(m_pUnitCollider[UNITCOLLIDER::BODY]);
		DISABLE_COMPONENT(m_pUnitCollider[UNITCOLLIDER::HEAD]);
	}
}

void CUnit::Enable_UnitCollider(UNITCOLLIDER ePartType, _bool bEnable)
{
	if (!m_pUnitCollider[ePartType])
		return;

	if (bEnable)
		ENABLE_COMPONENT(m_pUnitCollider[ePartType]);
	else
		DISABLE_COMPONENT(m_pUnitCollider[ePartType]);

	if (ePartType == WEAPON_R)
	{
		if (bEnable)
		{
			if (m_pWeaponCollider_R)
				ENABLE_COMPONENT(m_pWeaponCollider_R);
		}
		else
		{
			if (m_pWeaponCollider_R)
				DISABLE_COMPONENT(m_pWeaponCollider_R);
		}
	}
}

void CUnit::Enable_GuardBreakCollider(UNITCOLLIDER ePartType, _bool bEnable)
{
	if (!m_pUnitCollider[ePartType])
		return;


	if (ePartType == GUARDBREAK_R)
	{
		if (bEnable)
		{
			ENABLE_COMPONENT(m_pUnitCollider[ePartType]);

			if (m_pWeaponCollider_R)
				ENABLE_COMPONENT(m_pWeaponCollider_R);
		}
		else
		{
			DISABLE_COMPONENT(m_pUnitCollider[ePartType]);

			if (m_pWeaponCollider_R)
				DISABLE_COMPONENT(m_pWeaponCollider_R);
		}
	}
	else if (ePartType == GUARDBREAK_L)
	{
		if (bEnable)
		{
			ENABLE_COMPONENT(m_pUnitCollider[ePartType]);

			if (m_pWeaponCollider_L)
				ENABLE_COMPONENT(m_pWeaponCollider_L);
		}
		else
		{
			DISABLE_COMPONENT(m_pUnitCollider[ePartType]);

			if (m_pWeaponCollider_L)
				DISABLE_COMPONENT(m_pWeaponCollider_L);
		}
	}



}

void CUnit::Enable_FlyAttackCollider(_bool bEnable)
{
	if (!m_pUnitCollider[FLYATTACK])
		return;

	if (bEnable)
	{
		ENABLE_COMPONENT(m_pUnitCollider[FLYATTACK]);

		if (m_pWeaponCollider_R)
			ENABLE_COMPONENT(m_pWeaponCollider_R);
	}
	else
	{
		DISABLE_COMPONENT(m_pUnitCollider[FLYATTACK]);

		if (m_pWeaponCollider_R)
			DISABLE_COMPONENT(m_pWeaponCollider_R);
	}
}

void CUnit::Enable_GuardCollider(_bool bEnable)
{
	if (!m_pUnitCollider[GUARD])
		return;

	if (bEnable)
	{
		/* ?????? ??????*/
		ENABLE_COMPONENT(m_pUnitCollider[GUARD]);
		DISABLE_COMPONENT(m_pUnitCollider[HEAD]);
		DISABLE_COMPONENT(m_pUnitCollider[BODY]);
	}
	else
	{
		DISABLE_COMPONENT(m_pUnitCollider[GUARD]);
		ENABLE_COMPONENT(m_pUnitCollider[HEAD]);
		ENABLE_COMPONENT(m_pUnitCollider[BODY]);
	}

}

void CUnit::Enable_GroggyCollider(_bool bEnable)
{
	if (!m_pUnitCollider[GROGGY])
		return;

	if (bEnable)
	{
		/* ?????? ??????*/
		ENABLE_COMPONENT(m_pUnitCollider[GROGGY]);
		ENABLE_COMPONENT(m_pWeaponCollider_R);
	}
	else
	{
		DISABLE_COMPONENT(m_pUnitCollider[GROGGY]);
		DISABLE_COMPONENT(m_pWeaponCollider_R);
	}
}

void CUnit::Enable_Glider(_bool bEnable)
{
	if (bEnable)
		ENABLE_GAMEOBJECT(m_pGlider);
	else
		DISABLE_GAMEOBJECT(m_pGlider);
}

void CUnit::Set_GliderAnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pGlider->Set_AnimIndex(iAnimIndex, fInterpolateTime, fAnimSpeed);
}


void CUnit::SetUp_Colliders(_bool bBlueTeam)
{
	COL_GROUP_CLIENT	eTeam = (bBlueTeam) ? COL_BLUETEAM : COL_REDTEAM;

	UNIT_COLLIDERDESC tDesc;
	tDesc.eColType = eTeam;
	tDesc.fRadius = 1.f;
	tDesc.vOffsetPos = _float4(0.f, 1.f, 0.f);

	SetUp_UnitCollider(CUnit::TEAM, &tDesc, 1);

}

void CUnit::SetUp_UnitCollider(UNITCOLLIDER ePartType, UNIT_COLLIDERDESC* arrColliderDesc, _uint iNumCollider, _float4x4 matTransformation, _bool bEnable, CHierarchyNode* pRefBone)
{
	if (m_pUnitCollider[ePartType])
		return;

	m_pUnitCollider[ePartType] = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, arrColliderDesc[0].fRadius, arrColliderDesc[0].eColType, arrColliderDesc[0].vOffsetPos,
		matTransformation, pRefBone);

	if (!m_pUnitCollider[ePartType])
		return;

	Add_Component(m_pUnitCollider[ePartType]);

	Enable_UnitCollider(ePartType, bEnable);

	for (_uint i = 1; i < iNumCollider; ++i)
	{
		m_pUnitCollider[ePartType]->Add_Collider(arrColliderDesc[i].fRadius, arrColliderDesc[i].vOffsetPos);
	}
}

void CUnit::SetUp_HitStates(UNIT_TYPE eUnitType)
{
	if (eUnitType > UNIT_TYPE::eUNIT_TYPE_END)
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
}

void CUnit::Check_NearObject_IsInFrustum(CGameObject** pNearObject)
{
	_float fPreLength = m_fMaxDistance + 1.f;

	// ???? ???????? ????????.
	list<CGameObject*>& listPlayers = GAMEINSTANCE->Get_ObjGroup(GROUP_PLAYER);

	CGameObject* pPreObject = *pNearObject;

	*pNearObject = nullptr;

	for (auto& elem : listPlayers)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(elem);
		if (!pPlayer)
			continue;

		if (!pPlayer->Is_Valid())
			continue;

		// ?????? ?????? ????????
		if (CUser::Get_Instance()->Get_CurLevel() != LEVEL_TEST)
		{
			// ???? ???? ?????? ??????
			if (m_bForUseTeam)
			{
				// ???? ?????? ?????????? ???? ????????
				if (m_pOwnerPlayer->Get_Team()->Get_TeamType() != pPlayer->Get_Team()->Get_TeamType())
					continue;

			}
			else
			{
				// ???? ?????? ?????????? ???? ???????? 
				if (m_pOwnerPlayer->Get_Team()->Get_TeamType() == pPlayer->Get_Team()->Get_TeamType())
					continue;

			}

		}

		CUnit* pUnit = pPlayer->Get_CurrentUnit();

		if (!pUnit->Is_Valid())
			continue;

		if (pUnit == this)
			continue;

		if (pUnit->Get_Status().fHP <= 0.f)
			continue;

		// ???????? ????????????
		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pUnit->Get_Transform()->Get_World(WORLD_POS).XMLoad(), m_fMaxDistance / 5.f))
			continue;

		_float fMyLength = (elem->Get_Transform()->Get_World(WORLD_POS) - Get_Transform()->Get_World(WORLD_POS)).Length();

		// ?????? ???? ???????????? ?????? ????
		if (fPreLength < fMyLength)
			continue;

		*pNearObject = pUnit;
		fPreLength = fMyLength;

	}


	if (pPreObject == *pNearObject)
		m_bSameNearObject = true;
	else
		m_bSameNearObject = false;

}

void CUnit::Check_MultipleObject_IsInFrustum()
{

	// ???? ???????? ????????.
	list<CGameObject*>& listPlayers = GAMEINSTANCE->Get_ObjGroup(GROUP_PLAYER);

	for (auto& elem : listPlayers)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(elem);
		if (!pPlayer)
			continue;

		if (!pPlayer->Is_Valid())
			continue;

		// ?????? ?????? ????????
		if (CUser::Get_Instance()->Get_CurLevel() != LEVEL_TEST)
		{
			// ???? ???? ?????? ??????
			if (m_bForUseTeam)
			{
				// ???? ?????? ?????????? ???? ????????
				if (m_pOwnerPlayer->Get_Team()->Get_TeamType() != pPlayer->Get_Team()->Get_TeamType())
					continue;

			}
			else
			{
				// ???? ?????? ?????????? ???? ???????? 
				if (m_pOwnerPlayer->Get_Team()->Get_TeamType() == pPlayer->Get_Team()->Get_TeamType())
					continue;

			}

		}

		CUnit* pUnit = pPlayer->Get_CurrentUnit();

		if (!pUnit->Is_Valid())
			continue;

		if (pUnit == this)
			continue;

		_float fMyLength = (pUnit->Get_Transform()->Get_World(WORLD_POS) - Get_Transform()->Get_World(WORLD_POS)).Length();

		_float fMaxDistance = 20.f;
		if (fMyLength > fMaxDistance)
			continue;

		if (m_bForUseTeam == false)
			int a = 0;

		// ???????? ????????????
		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(pUnit->Get_Transform()->Get_World(WORLD_POS).XMLoad(), 2.f))
			continue;

		// ray???? ?????? ????
		_float4 vOutPos;
		_float fOutDist;
		_float4 vCamPos = Get_FollowCam()->Get_Transform()->Get_World(WORLD_POS);//GAMEINSTANCE->Get_ViewPos();
		_float4 vOtherPos = pUnit->Get_Transform()->Get_World(WORLD_POS);
		vOtherPos.y += 0.8f;
		_float4 vRayDir = vOtherPos - vCamPos;
		_float fLength = vRayDir.Length();

		if (GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vCamPos, vRayDir.Normalize(), fLength))
			continue;

		for (auto& FrustumObj : m_MultipleFrustumObject)
		{
			if (FrustumObj == pUnit)
				return;
		}

		m_MultipleFrustumObject.push_back(pUnit);
	}


}

void CUnit::On_ChangeToHero(_uint iIndex)
{
	m_pOwnerPlayer->Change_UnitClass((CLASS_TYPE)iIndex);
	m_pOwnerPlayer->AbleHero() = false;

	if (m_pCurState)
		m_pCurState->Exit(this, m_pAnimator);
}

_float4 CUnit::Get_FollowCamLook()
{
	_float4 vCamLook = m_pFollowCam->Get_Transform()->Get_World(WORLD_LOOK);
	return vCamLook;
}

_float4 CUnit::Get_FollowCamLook_Turn()
{
	_float4 vCamLook = m_pFollowCam->Get_Transform()->Get_World(WORLD_LOOK);
	_float4x4 matRotY = XMMatrixRotationAxis(_float4(0.f, 1.f, 0.f, 0.f).XMLoad(), ToRadian(20.f));

	return vCamLook.MultiplyNormal(matRotY);
}

_float4 CUnit::Get_FollowCamRight()
{
	return m_pFollowCam->Get_Transform()->Get_World(WORLD_RIGHT);
}


list<_float4>& CUnit::Get_CurRoute()
{
	return m_pOwnerPlayer->Get_CurRoute();
}

void CUnit::TurnOn_TrailEffect(_bool bOn)
{
	if (!m_pTrailEffect)
		return;

	m_pTrailEffect->TurnOn_TrailEffect(bOn);
	m_pTrailEffect2->TurnOn_TrailEffect(bOn);

	if (m_pDistortionTrail)
		m_pDistortionTrail->TurnOn_TrailEffect(bOn);

}



void CUnit::SetUp_EyeTrail(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pEyeTrail = CTrailEffect::Create(1, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pEyeTrail2 = CTrailEffect::Create(1, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pEyeTrail)
		return;

	CREATE_GAMEOBJECT(m_pEyeTrail, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pEyeTrail2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pEyeTrail2, CMesh))->Set_NoCurve();

	m_pEyeTrail->Set_EffectFlag(SH_EFFECT_NONE);
	m_pEyeTrail2->Set_EffectFlag(SH_EFFECT_NONE);

	m_pEyeTrail->TurnOn_TrailEffect(false);
	m_pEyeTrail2->TurnOn_TrailEffect(false);
}

void CUnit::SetUp_TrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pTrailEffect = CTrailEffect::Create(0, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pTrailEffect2 = CTrailEffect::Create(0, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pTrailEffect)
		return;

	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pTrailEffect2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect2, CMesh))->Set_NoCurve();

	m_pTrailEffect->TurnOn_TrailEffect(false);
	m_pTrailEffect2->TurnOn_TrailEffect(false);

}

void CUnit::SetUp_DistortionTrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName)
{
	m_pDistortionTrail = CTrailEffect::Create(0, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(strBoneName.c_str()), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pDistortionTrail)
		return;

	CREATE_GAMEOBJECT(m_pDistortionTrail, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pDistortionTrail, CMesh))->Set_NoCurve();

	m_pDistortionTrail->TurnOn_TrailEffect(false);

	m_pDistortionTrail->Set_Distortion();
}

HRESULT CUnit::SetUp_Model(const UNIT_MODEL_DATA& tData)
{
	m_tModelData = tData;
	_float4x4			TransformMatrix;
	TransformMatrix = DEFAULT_TRANS_MATRIX;

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, tData.strModelPaths[MODEL_PART_SKEL], TransformMatrix);

	if (!pModel)
		return E_FAIL;

	for (_uint i = 1; i < MODEL_PART_END; ++i)
	{
		if (tData.strModelPaths[i].empty())
			continue;

		if (!tData.strRefBoneName[i].empty())
		{
			if (FAILED(pModel->Add_Model(tData.strModelPaths[i], i, tData.strRefBoneName[i])))
				return E_FAIL;
		}
		else
		{
			if (FAILED(pModel->Add_Model(tData.strModelPaths[i], i)))
				return E_FAIL;
		}

	}

	pModel->Enable_ModelParts(0, false);


	pModel->Initialize();
	Add_Component(pModel);

	return S_OK;
}

HRESULT CUnit::SetUp_Navigation(CCell* pStartCell)
{
	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, pStartCell, m_pPhysics);

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);
	pNaviCom->Set_Epsilon(1.f);

	_float4 vStartPos = pStartCell->Get_Point(CCell::POINT_A);
	m_pTransform->Set_World(WORLD_POS, vStartPos);

	return S_OK;
}

void CUnit::My_Tick()
{
	if (Get_OwnerHUD())
		Get_OwnerHUD()->Set_UnitHP(m_tUnitStatus.fHP, m_tUnitStatus.fMaxHP);

	for (_int i = 0; i < COOL_END; ++i)
	{
		if (m_fCoolAcc[i] > 0.f)
		{
			m_fCoolAcc[i] -= fDT(0);
		}
		else
		{
			m_fCoolAcc[i] = 0.f;
		}

		if (m_bIsMainPlayer)
		{
			CUser::Get_Instance()->Set_SkillCoolTime(i, m_fCoolAcc[i], m_fCoolTime[i]);
		}
	}

	if (m_fAttackDelay > 0.f)
		m_fAttackDelay -= fDT(0);
	else
		m_fAttackDelay = 0.f;

	if (m_fGlidingTime > 0.f)
		m_fGlidingTime -= fDT(0);
	else
		m_fGlidingTime = 0.f;


	if (!m_pCurState)
	{
		Call_MsgBox(L"State is Nullptr : CUnit");
		return;
	}

	if (m_eReserveState != STATE_END)
	{
		if (m_pCurState->m_iStateChangeKeyFrame <= m_pAnimator->Get_CurAnimFrame() || m_pAnimator->Is_CurAnimFinished())
		{
			if (m_eReserveState != m_eDefaultState)
			{
				Enter_State(m_eReserveState);
				m_eDefaultState = m_eReserveState;
				m_eReserveState = STATE_END;
			}

			/* BehaviorDesc?? ???????? */
			m_pOwnerPlayer->On_RealChangeBehavior();

		}
		else if (m_bForceChangeBehavior)
		{
			if (m_eReserveState != m_eDefaultState)
			{
				Enter_State(m_eReserveState);
				m_eDefaultState = m_eReserveState;
				m_eReserveState = STATE_END;
			}

			m_pOwnerPlayer->On_RealChangeBehavior();
			m_bForceChangeBehavior = false;
		}

	}

	STATE_TYPE eNewState = STATE_END;
	eNewState = m_pCurState->Tick(this, m_pAnimator);



	if (STATE_END != eNewState)
	{
		Enter_State(eNewState);
	}

	if (m_fHitDelayAcc > 0.f)
		m_fHitDelayAcc -= fDT(0);
	else
		m_fHitDelayAcc = 0.f;
}

void CUnit::My_LateTick()
{
	if (m_bDie)
	{
		m_fDeadTimeAcc += fDT(0);
		if (m_fDeadTimeAcc >= m_fDeadTime)
		{
			On_Die();
		}
	}

	m_CureObjects.clear();
}

void CUnit::Create_Light(_float4 vPos, _float fRange, _float fRandomRange, _float fFadeInTime, _float fDuration, _float fFadeOutTime, _float4 Diffuse,
	LIGHTDESC::EASING_TYPE eInType, LIGHTDESC::EASING_TYPE eOutType)
{
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.eFadeType = tagLightDesc::FADEIN;

	LightDesc.vPosition = vPos;
	LightDesc.fRange = fRange;
	LightDesc.fRandomRange = fRandomRange;

	LightDesc.fLightFadeInTime = fFadeInTime;
	LightDesc.fLightTime = fDuration;
	LightDesc.fLightFadeOutTime = fFadeOutTime;

	LightDesc.vTargetDiffuse = Diffuse;
	LightDesc.vTargetAmbient = _float4(0.2f, 0.2f, 0.2f);
	LightDesc.vTargetSpecular = _float4(1.f, 1.f, 1.f);

	LightDesc.eInEasingType = eInType;
	LightDesc.eOutEasingType = eOutType;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CUnit::Create_Light(CGameObject* pOwner, _float4 vOffset, _float fRange, _float fRandomRange,
	_float fFadeInTime, _float fDuration, _float fFadeOutTime, _float4 Diffuse, _bool bLoop)
{
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.eFadeType = tagLightDesc::FADEIN;

	LightDesc.pOwner = pOwner;
	LightDesc.vOffset = vOffset;
	LightDesc.fRange = fRange;
	LightDesc.fRandomRange = fRandomRange;

	LightDesc.fLightFadeInTime = fFadeInTime;
	LightDesc.fLightTime = fDuration;
	LightDesc.fLightFadeOutTime = fFadeOutTime;

	LightDesc.vTargetDiffuse = Diffuse;
	LightDesc.vTargetAmbient = _float4(0.2f, 0.2f, 0.2f);
	LightDesc.vTargetSpecular = _float4(1.f, 1.f, 1.f);

	LightDesc.eInEasingType = tagLightDesc::EAS_Linear;
	LightDesc.eOutEasingType = tagLightDesc::EAS_Linear;
	LightDesc.bLoop = bLoop;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CUnit::Effect_Parring(_float4 vHitPos)
{
	Create_Light(vHitPos, 2.5f, 0.f, 0.f, 0.f, 1.f, RGB(255, 140, 40),
		LIGHTDESC::EASING_TYPE::EAS_ElasticEaseIn,
		LIGHTDESC::EASING_TYPE::EAS_ElasticEaseOut);
	/*CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HitSmokeParticle_0"), vHitPos);*/
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Parring_Particle", vHitPos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));

	CFunctor::Play_Sound(L"Effect_Guard", CHANNEL_EFFECTS, vHitPos);
}

void CUnit::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	//???????? ?????????? ??????
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Hit_Particle", vHitPos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
}

void CUnit::Effect_Fall(_float fFallPower)
{
	/* ???? ?????? */
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Fall_Particle", m_pTransform->Get_World(WORLD_POS));
	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", this, m_pTransform->Get_World(WORLD_POS));

}

void CUnit::Turn_EyeTrail(_bool bOnOff)
{
	if (!m_pEyeTrail)
		return;

	m_pEyeTrail->TurnOn_TrailEffect(bOnOff);
	m_pEyeTrail2->TurnOn_TrailEffect(bOnOff);
}

void CUnit::Turn_EyeFlare(_bool bOnOff, wstring wstrKey)
{
	if (bOnOff)
	{
		if (m_EyeFlare.empty())
			m_EyeFlare = CEffects_Factory::Get_Instance()->Create_MultiEffects(wstrKey, this, ZERO_VECTOR);
	}
	else
	{
		if (!m_EyeFlare.empty())
		{
			for (auto& elem : m_EyeFlare)
			{
				DISABLE_GAMEOBJECT(elem);
			}
			m_EyeFlare.clear();
		}

	}
}

void CUnit::Effect_HeroToDefaultUnit(CUnit* pOwner)
{

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"UnHenshin", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
}

void CUnit::Set_ShaderNoSpec(const _tchar* pModelPath)
{
	for (_uint i = 0; i < MODEL_PART_END; ++i)
	{
		_int iTemp = 0;
		iTemp = (_int)m_tModelData.strModelPaths[i].find(pModelPath);

		if (iTemp > 0)
			m_pModelCom->Set_ShaderFlag(i, SH_LIGHT_NOSPEC);
	}

}


void CUnit::On_InitSetting()
{
	for (_uint i = 0; i < UNITCOLLIDER_END; ++i)
	{
		switch (i)
		{
		case UNITCOLLIDER::TEAM:
		case UNITCOLLIDER::BODY:
		case UNITCOLLIDER::HEAD:

			break;
		case UNITCOLLIDER::WEAPON_L:
		case UNITCOLLIDER::WEAPON_R:
		case UNITCOLLIDER::GUARD:
		case UNITCOLLIDER::GUARDBREAK_L:
		case UNITCOLLIDER::GUARDBREAK_R:
		case UNITCOLLIDER::GROGGY:
		case UNITCOLLIDER::FLYATTACK:
			if (m_pUnitCollider[i])
				DISABLE_COMPONENT(m_pUnitCollider[i]);

			if (m_pWeaponCollider_L)
				DISABLE_COMPONENT(m_pWeaponCollider_L);

			if (m_pWeaponCollider_R)
				DISABLE_COMPONENT(m_pWeaponCollider_R);

			break;


		default:
			break;
		}
	}

}


void CUnit::Add_DeathStones(const list<CGameObject*>& StoneParticleList)
{
	for (auto& elem : StoneParticleList)
		m_DeathStones.push_back(elem);
}

void CUnit::Start_Reborn()
{
	if (!m_pOwnerPlayer->Is_AbleRevival())
		return;

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Toggle_DeadUI(false);
		CUser::Get_Instance()->SetActive_HUD_RevivalUI(true);
	}

	m_pOwnerPlayer->Start_Reborn();

	static_cast<CMesh_Particle*>(m_DeathStones.front())->Start_Reverse(this);

	for (auto& elem : m_DeathStones)
	{
		if (elem == m_DeathStones.front())
			continue;

		static_cast<CMesh_Particle*>(elem)->Start_Reverse(nullptr);
	}
}

void CUnit::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	//????...??????..
	m_pOwnerPlayer->Reserve_BehaviorDesc(pBehaviorDesc);




}

void CUnit::On_FinishGame(_bool bWin)
{
	Enter_State((bWin) ? STATE_VICTORY : STATE_DEFEAT);
	m_eReserveState = STATE_END;
}

void CUnit::Play_Voice(wstring strName, _float fVol, _bool bDist)
{


	CLASS_TYPE eClass = Get_OwnerPlayer()->Get_CurClass();
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	wstring strKey = strName;


	switch (eClass)
	{
	case Client::WARRIOR:
		strKey += L"_Warrior";
		break;
	case Client::ARCHER:
		strKey += L"_Archer";
		break;
	case Client::PALADIN:
		strKey += L"_Paladin";
		break;
	case Client::PRIEST:
		strKey += L"_Priest";
		break;
	case Client::ENGINEER:
		strKey += L"_Warhammer";
		break;
	case Client::FIONA:
		strKey += L"_Fiona";
		break;
	case Client::QANDA:
		strKey += L"_Qanda";
		break;
	case Client::LANCER:
		strKey += L"_Lancer";
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}

	if (bDist)
		CFunctor::Play_Sound(strKey, CHANNEL_VOICE, vPos, fVol);
	else
		CFunctor::Play_Sound(strKey, CHANNEL_VOICE, fVol);
}

void CUnit::Set_AnimWeaponIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	if (!m_pAnimWeapon)
		return;

	m_pAnimWeapon->Set_AnimIndex(iAnimIndex, fInterpolateTime, fAnimSpeed);
}


void CUnit::Set_AnimWeaponFrame(_uint iChangeFrame)
{
	if (!m_pAnimWeapon)
		return;

	GET_COMPONENT_FROM(m_pAnimWeapon, CAnimator)->Set_CurFrame(iChangeFrame);
}

//_float4x4& CUnit::Use_OwnerBoneOffset()
//{
//	_float4x4 vMatrix = XMMatrixIdentity();
//
//	if (!m_pAnimWeapon)
//		return vMatrix;
//
//	return m_pAnimWeapon->Use_OwnerBoneOffset();
//}



void CUnit::On_Hit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo)
{
	CState::HIT_INFO tInfo = *(CState::HIT_INFO*)(pHitInfo);
	_float fDamage = pOtherUnit->Calculate_Damage(tInfo.bHeadShot, false);

	_bool bDie = On_PlusHp(fDamage, pOtherUnit, tInfo.bHeadShot, 3);

	CFunctor::Play_Sound(L"Effect_Blood", CHANNEL_EFFECTS, Get_Transform()->Get_World(WORLD_POS));

	_float fVolume = 1.f;

	if (!m_bIsMainPlayer)
	{
		if (!Get_OwnerHUD())
			return;

		Get_OwnerHUD()->SetActive_UnitHP(true);
		fVolume = 0.2f;
	}

	Get_CurStateP()->Play_Voice(this, L"Voice_Hit", fVolume);

	/*?????? ????????*/
	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Turn_BloodOverLay(m_tUnitStatus.fHP / m_tUnitStatus.fMaxHP);
	}

	m_pOwnerPlayer->Set_IsBattle(true);

	if (!bDie)
	{
		// ???????????? ??????
		if (m_bIsMainPlayer && CUser::Get_Instance()->Get_CurLevel() == LEVEL_BOOTCAMP)
		{
			m_tUnitStatus.fHP = 1.f;
		}
		else
		{
			if (tInfo.bSting && m_tHitType.eHitState != m_tHitType.eStingHitState)
			{
				Enter_State(m_tHitType.eStingHitState, pHitInfo);
			}
			else
			{
				On_DieBegin(pOtherUnit, vHitPos);
				Enter_State(m_tHitType.eHitState, pHitInfo);
			}

			//On_DieBegin(pOtherUnit, vHitPos);
			//Enter_State(m_tHitType.eHitState, pHitInfo);

			return;

		}

	}

	pOtherUnit->Effect_Hit(this, vHitPos);

	switch (iOtherColType)
	{
	case COL_REDATTACK:
	case COL_BLUEATTACK:
		Enter_State(m_tHitType.eHitState, pHitInfo);
		break;
		//?????? GuardBreak?? ?????? ????
	case COL_BLUEGUARDBREAK:
	case COL_REDGUARDBREAK:
		//1. ??????
		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eHitState, pHitInfo);
		break;

	case COL_REDFLYATTACK:
	case COL_BLUEFLYATTACK:
	case COL_BLUEFLYATTACKGUARDBREAK:
	case COL_REDFLYATTACKGUARDBREAK:
		//1. ??????

		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eFlyState, pHitInfo);
		break;

	case COL_BLUEGROGGYATTACK:
	case COL_REDGROGGYATTACK:
		//1. ??????

		Play_Sound(L"Effect_Grogy_Warhammer");

		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eGroggyState, pHitInfo);
		break;



	default:
		break;
	}
}


void CUnit::On_GuardHit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo)
{
	//?? Guard???????? ?????? hit?? ????

	CState::HIT_INFO tInfo = *(CState::HIT_INFO*)(pHitInfo);

	//?????????????? ???? ???? ????
	if (!tInfo.bFace ||
		iOtherColType == COL_BLUEGROGGYATTACK ||
		iOtherColType == COL_REDGROGGYATTACK)
	{
		On_Hit(pOtherUnit, iOtherColType, vHitPos, pHitInfo);
		return;
	}

	//?????? ???? ???? ????
	tInfo.bHeadShot = false;
	_float fDamage = pOtherUnit->Calculate_Damage(tInfo.bHeadShot, true);

	_bool bDie = On_PlusHp(fDamage, pOtherUnit, tInfo.bHeadShot, 1);
	if (!bDie)
		m_tUnitStatus.fHP = 1.f;


	switch (iOtherColType)
	{
		//?????? ?????? ?????? ?????? ????
	case COL_REDATTACK:
	case COL_BLUEATTACK:
	case COL_REDFLYATTACK:
	case COL_BLUEFLYATTACK:
		//1. ??????
		Effect_Parring(vHitPos);

		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eGuardState, pHitInfo);
		pOtherUnit->On_Bounce(pHitInfo);

		break;

		//?????? GuardBreak?? ?????? ????
	case COL_BLUEGUARDBREAK:
	case COL_REDGUARDBREAK:
		//1. ??????
		Effect_Parring(vHitPos);

		Play_Sound(L"Effect_GuardBreak");

		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eGuardBreakState, pHitInfo);
		break;

	case COL_BLUEFLYATTACKGUARDBREAK:
	case COL_REDFLYATTACKGUARDBREAK:
		//1. ??????

		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eFlyState, pHitInfo);
		break;

	case COL_BLUEGROGGYATTACK:
	case COL_REDGROGGYATTACK:
		//1. ??????
		Effect_Parring(vHitPos);
		//2. ???? ?? ???? ????
		Enter_State(m_tHitType.eGroggyState, pHitInfo);
		break;

	default:
		break;
	}
}

void CUnit::On_DieBegin(CUnit* pOtherUnit, _float4 vHitPos)
{
	m_bDie = true;
	CFunctor::Play_Sound(L"Effect_Die", CHANNEL_EFFECTS, Get_Transform()->Get_World(WORLD_POS));


	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StoneSpark", vHitPos);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Hit_Particle", vHitPos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));

	// ?????? ????????	
	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() >= LEVEL_PADEN)
	{
		pOtherUnit->Get_OwnerPlayer()->On_ScoreKDA_Kill(m_pOwnerPlayer);
		Get_OwnerPlayer()->On_ScoreKDA_Death();
	}

	CUser::Get_Instance()->Add_KillLog(pOtherUnit->Get_OwnerPlayer(), m_pOwnerPlayer);

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Turn_HeroGaugeFire(false);
		CUser::Get_Instance()->SetActive_SquardInfo(false);
		CUser::Get_Instance()->SetActive_HUD(false);
		CUser::Get_Instance()->Set_TargetInfo(pOtherUnit->Get_OwnerPlayer()->Get_PlayerInfo());
		CUser::Get_Instance()->Toggle_DeadUI(true);

		m_pFollowCam->Set_FollowTarget(pOtherUnit);
	}
	else
	{
		if (pOtherUnit->m_bIsMainPlayer)
		{
			wstring wstrEnemyName = m_pOwnerPlayer->Get_PlayerName();
			CUser::Get_Instance()->Add_KillName(wstrEnemyName);
		}
	}


	Get_CurStateP()->Play_Voice(this, L"Voice_Dead", 1.f);


}

void CUnit::On_Bounce(void* pHitInfo)
{
	//Left???? Right???? ????

	if (m_tHitType.eBounce != STATE_END)
		Enter_State(m_tHitType.eBounce, pHitInfo);
}

