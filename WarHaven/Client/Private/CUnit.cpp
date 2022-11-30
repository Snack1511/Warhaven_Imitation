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
#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"

#include "MeshContainer.h"

#include "CState.h"
#include "CState_Manager.h"

#include "CPhysXCharacter.h"

#include "CBoneCollider.h"

#include "CCamera_Follow.h"
#include "CState_Hit.h"

#include "CUtility_Transform.h"

#include "CUser.h"
#include "CUI_Wrapper.h"
#include "CUI_UnitHUD.h"


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
	/* 충돌한 대상이 Unit이 아니면 Return */
	/* ================================================= */
	CUnit* pOtherUnit = nullptr;
	pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);



	if (!pOtherUnit)
		return;

	/* ================================================= */

	/* 이미 hit 상태면 delay 걸기 */
	if (m_fHitDelayAcc > 0.f)
		return;

	m_fHitDelayAcc = m_fHitDelayTime;

	/* SHAKE */
	Shake_Camera(m_tUnitStatus.fCamPower, m_tUnitStatus.fCamTime);

	STATE_TYPE	eFinalHitState = STATE_END;


	UNIT_STATUS& tOtherStatus = pOtherUnit->Get_Status();

	CState::HIT_INFO tOtherHitInfo = pOtherUnit->Get_CurStateP()->Get_HitInfo();

	tOtherHitInfo.vDir = (m_pTransform->Get_World(WORLD_POS) - vHitPos);
	tOtherHitInfo.vDir.y = 0.f;
	tOtherHitInfo.vDir.Normalize();

	//상대 위치 계산
	_float4 vOtherDir = pOtherUnit->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);

	_float4 vCurLook = Get_Transform()->Get_World(WORLD_LOOK).Normalize();

	//양수면 앞임.
	if (vCurLook.Dot(vOtherDir) > 0.f)
		tOtherHitInfo.bFace = true;
	else
		tOtherHitInfo.bFace = false;

	/* 무조건 맞은 쪽에서 처리하기 */
	switch (eMyColType)
	{
		/* 내가 막은 상황 */
	case COL_PLAYERGUARD:
	case COL_ENEMYGUARD:
		On_GuardHit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

		/* 내가 맞은 상황 */
	case COL_PLAYERHITBOX_BODY:
	case COL_ENEMYHITBOX_BODY:
		tOtherHitInfo.bHeadShot = false;
		On_Hit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

	case COL_PLAYERHITBOX_HEAD:
	case COL_ENEMYHITBOX_HEAD:
		tOtherHitInfo.bHeadShot = true;
		On_Hit(pOtherUnit, eOtherColType, vHitPos, &tOtherHitInfo);
		break;

	default:
		break;

	}


}

void CUnit::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CUnit::Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
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

	m_pFollowCam->Get_Transform()->Set_World(WORLD_POS, m_pTransform->Get_World(WORLD_POS));
	m_pFollowCam->Get_Transform()->Make_WorldMatrix();
}

void CUnit::Shake_Camera(_float fPower, _float fTime)
{
	m_pFollowCam->Start_ShakingCamera(fPower, fTime);
}

void CUnit::Lerp_Camera(const _uint& iCameraLerpType)
{
	GET_COMPONENT_FROM(m_pFollowCam, CScript_FollowCam)->Start_LerpType((CScript_FollowCam::CAMERA_LERP_TYPE)iCameraLerpType);
}

void CUnit::On_Die()
{
	m_bDie = false;
	m_fDeadTimeAcc = 0.f;

	_float4 vPos = Get_Transform()->Get_World(WORLD_POS);
	vPos.y += 1.f;

	_float4x4 matWorld = m_pTransform->Get_WorldMatrix(MATRIX_IDENTITY);

	CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeathStoneParticle", vPos, _float4(0.f, 1.f, 0.f, 0.f), 1.f, matWorld);
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
	_float fDmg = m_tUnitStatus.fAttackDamage * m_pCurState->Get_DamagePumping()
		* ((bHeadShot) ? HEADSHOTRATIO : 1.f)
		* ((bGuard) ? GUARDSUCCESS : 1.f);

	//헤드샷이면 1.5배

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
	m_tUnitStatus.fHP += fHp;

	/*데미지 표시*/
	if (pOtherUnit->m_bIsMainPlayer)
	{
		if (bHeadShot)
		{
			CUser::Get_Instance()->Enable_DamageFont(0, fHp);
		}
		else
		{
			CUser::Get_Instance()->Enable_DamageFont(iDmgType, fHp);
		}
	}

	if (m_tUnitStatus.fHP <= 0.f)
	{
		m_tUnitStatus.fHP = 0.f;
		return false;
	}
	else if (m_tUnitStatus.fHP >= m_tUnitStatus.fMaxHP)
	{
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}

	return true;
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

CUnit* CUnit::Get_TargetUnit()
{
	if (!m_pOwnerPlayer->Get_TargetPlayer())
		return nullptr;

	return m_pOwnerPlayer->Get_TargetPlayer()->Get_CurrentUnit();
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

	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();

	m_pCurState->Enter(this, m_pAnimator, m_eCurState, pData);
	m_eCurState = eType;

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


HRESULT CUnit::MakeUp_Unit(const UNIT_DESC& tUnitDesc)
{


	return S_OK;
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

	//PhysX캐릭터 : 캐릭터 본체
	CPhysXCharacter::PHYSXCCTDESC tDesc;

	tDesc.fRadius = 0.25f;
	tDesc.fHeight = 1.5f;

	CPhysXCharacter* pPhysXCharacter = CPhysXCharacter::Create(CP_BEFORE_TRANSFORM, tDesc);
	Add_Component(pPhysXCharacter);
#endif // PHYSX_OFF

	Create_UnitHUD();

	return S_OK;
}

HRESULT CUnit::Initialize()
{
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
	//	//Call_MsgBox(L"상태 세팅 안댔음");
	//	//return E_FAIL;
	//}
	//else
	//	m_pCurState->Enter(this, m_pAnimator, m_eCurState);

	if (m_pUnitCollider[BODY])
		ENABLE_COMPONENT(m_pUnitCollider[BODY]);
	if (m_pUnitCollider[HEAD])
		ENABLE_COMPONENT(m_pUnitCollider[HEAD]);

	Enable_UnitHUD();

	/* PASS */
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);


	return S_OK;
}

void CUnit::OnEnable()
{
	__super::OnEnable();
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	if (m_pCurState)
		m_pCurState->Enter(this, m_pAnimator, m_eCurState);

	On_InitSetting();
}

void CUnit::OnDisable()
{
	__super::OnDisable();

	DISABLE_GAMEOBJECT(m_pUnitHUD);
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
			if (m_pWeaponCollider_L)
				ENABLE_COMPONENT(m_pWeaponCollider_L);
		}
		else
		{
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
		/* 다른건 꺼놓기*/
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
		/* 다른건 꺼놓기*/
		ENABLE_COMPONENT(m_pUnitCollider[GROGGY]);
		ENABLE_COMPONENT(m_pWeaponCollider_R);
	}
	else
	{
		DISABLE_COMPONENT(m_pUnitCollider[GROGGY]);
		DISABLE_COMPONENT(m_pWeaponCollider_R);
	}
}

void CUnit::SetUp_Colliders(_bool bPlayer)
{
	COL_GROUP_CLIENT	eTeam = (bPlayer) ? COL_PLAYERTEAM : COL_ENEMYTEAM;

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

	m_pUnitCollider[ePartType] = m_pUnitCollider[ePartType] = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, arrColliderDesc[0].fRadius, arrColliderDesc[0].eColType, arrColliderDesc[0].vOffsetPos,
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

void CUnit::SetUp_HitStates(_bool bPlayer)
{
	if (!bPlayer)
		m_tHitType.eHitState = STATE_HIT_TEST_ENEMY;
}

void CUnit::On_ChangeToHero(_uint iIndex)
{
	m_pOwnerPlayer->Change_HeroUnit((CPlayer::CLASS_HREO)iIndex);
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


void CUnit::TurnOn_TrailEffect(_bool bOn)
{
	if (!m_pTrailEffect)
		return;

	m_pTrailEffect->TurnOn_TrailEffect(bOn);
	m_pTrailEffect2->TurnOn_TrailEffect(bOn);

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

HRESULT CUnit::SetUp_Model(const UNIT_MODEL_DATA& tData)
{
	m_tModelData = tData;
	_float4x4			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));

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

	for (_int i = 0; i < COOL_END; ++i)
	{
		if (m_fCoolAcc[i] >= 0.01f)
		{
			for (int i = 0; i < COOL_END; ++i)
			{
				CUser::Get_Instance()->Set_SkillCoolTime(i, m_fCoolAcc[i], m_fCoolTime[i]);
			}

			m_fCoolAcc[i] -= fDT(0);
		}
		else
			m_fCoolAcc[i] = 0.f;
	}


	if (m_fAttackDelay > 0.f)
		m_fAttackDelay -= fDT(0);
	else
		m_fAttackDelay = 0.f;

	if (!m_pCurState)
	{
		Call_MsgBox(L"State is Nullptr : CUnit");
		return;
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

	dynamic_cast<CUI_UnitHUD*>(m_pUnitHUD)->Set_UnitStatus(m_tUnitStatus);
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

	Frustum_UnitHUD();
	TransformProjection();
}

void CUnit::Effect_Parring(_float4 vHitPos)
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), vHitPos);
}

void CUnit::Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos)
{
	//공통으로 나와야하는 이펙트
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SmallSparkParticle", pOtherUnit, vHitPos);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSpark", pOtherUnit, vHitPos);
}



void CUnit::TransformProjection()
{
	dynamic_cast<CUI_UnitHUD*>(m_pUnitHUD)->Set_ProjPos(m_pTransform);
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

void CUnit::On_Hit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo)
{
	CState::HIT_INFO tInfo = *(CState::HIT_INFO*)(pHitInfo);
	_float fDamage = pOtherUnit->Calculate_Damage(tInfo.bHeadShot, false);

	_bool bDie = On_PlusHp(fDamage, pOtherUnit, tInfo.bHeadShot, 2);

	/*블러드 오버레이*/
	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Turn_BloodOverLay(m_tUnitStatus.fHP / m_tUnitStatus.fMaxHP);
	}

	if (!bDie)
	{
		On_DieBegin(pOtherUnit, vHitPos);
		Enter_State(m_tHitType.eHitState, pHitInfo);

		return;
	}

	pOtherUnit->Effect_Hit(this, vHitPos);

	switch (iOtherColType)
	{
	case COL_ENEMYATTACK:
	case COL_PLAYERATTACK:
		Enter_State(m_tHitType.eHitState, pHitInfo);
		break;
		//상대방 GuardBreak가 들어온 경우
	case COL_PLAYERGUARDBREAK:
		break;
	case COL_ENEMYGUARDBREAK:
		//1. 이펙트
		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eHitState, pHitInfo);
		break;

	case COL_ENEMYFLYATTACK:
	case COL_PLAYERFLYATTACK:
	case COL_PLAYERFLYATTACKGUARDBREAK:
	case COL_ENEMYFLYATTACKGUARDBREAK:
		//1. 이펙트

		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eFlyState, pHitInfo);
		break;

	case COL_PLAYERGROGGYATTACK:
	case COL_ENEMYGROGGYATTACK:
		//1. 이펙트


		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eGroggyState, pHitInfo);
		break;



	default:
		break;
	}
}


void CUnit::On_GuardHit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo)
{
	//내 Guard충돌체에 무언가 hit한 상황

	CState::HIT_INFO tInfo = *(CState::HIT_INFO*)(pHitInfo);

	//마주보지않았을 경우 가드 실패
	if (!tInfo.bFace)
	{
		On_Hit(pOtherUnit, iOtherColType, vHitPos, pHitInfo);
		return;
	}

	//마주본 경우 가드 성공
	tInfo.bHeadShot = false;
	_float fDamage = pOtherUnit->Calculate_Damage(tInfo.bHeadShot, true);

	_bool bDie = On_PlusHp(fDamage, pOtherUnit, tInfo.bHeadShot, 1);

	if (!bDie)
	{
		On_DieBegin(pOtherUnit, vHitPos);
		Enter_State(m_tHitType.eHitState, pHitInfo);

		return;
	}

	switch (iOtherColType)
	{
		//평범한 상대방 평타가 들어온 경우
	case COL_ENEMYATTACK:
	case COL_PLAYERATTACK:
	case COL_ENEMYFLYATTACK:
	case COL_PLAYERFLYATTACK:
		//1. 이펙트
		Effect_Parring(vHitPos);

		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eGuardState, pHitInfo);
		pOtherUnit->On_Bounce(pHitInfo);

		break;

		//상대방 GuardBreak가 들어온 경우
	case COL_PLAYERGUARDBREAK:
	case COL_ENEMYGUARDBREAK:
		//1. 이펙트
		Effect_Parring(vHitPos);

		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eGuardBreakState, pHitInfo);
		break;

	case COL_PLAYERFLYATTACKGUARDBREAK:
	case COL_ENEMYFLYATTACKGUARDBREAK:
		//1. 이펙트

		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eFlyState, pHitInfo);
		break;

	case COL_PLAYERGROGGYATTACK:
	case COL_ENEMYGROGGYATTACK:
		//1. 이펙트
		Effect_Parring(vHitPos);
		//2. 나와 적 상태 변경
		Enter_State(m_tHitType.eGroggyState, pHitInfo);
		break;

	default:
		break;
	}
}

void CUnit::On_DieBegin(CUnit* pOtherUnit, _float4 vHitPos)
{
	m_bDie = true;
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StoneSpark", vHitPos);
}

void CUnit::On_Bounce(void* pHitInfo)
{
	//Left인지 Right인지 판단
	Enter_State(m_tHitType.eBounce, pHitInfo);
}



void CUnit::Create_UnitHUD()
{
	m_pUnitHUD = CUI_UnitHUD::Create();
}

void CUnit::Enable_UnitHUD()
{
	CREATE_GAMEOBJECT(m_pUnitHUD, GROUP_UI);
}

void CUnit::Frustum_UnitHUD()
{
	_float fDis = CUtility_Transform::Get_FromCameraDistance(this);

	if (fDis < 30.f)
	{
		dynamic_cast<CUI_UnitHUD*>(m_pUnitHUD)->Set_UnitDis(fDis);

		_float4 vPos = m_pTransform->Get_World(WORLD_POS);
		vPos.y += 2.f;

		if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f))
		{
			if (!m_pUnitHUD->Is_Valid())
			{
				if (!m_bIsMainPlayer)
				{
					ENABLE_GAMEOBJECT(m_pUnitHUD);
				}
			}
		}
		else
		{
			if (m_pUnitHUD->Is_Valid())
			{
				DISABLE_GAMEOBJECT(m_pUnitHUD);
			}
		}
	}
	else
	{
		if (m_pUnitHUD->Is_Valid())
		{
			DISABLE_GAMEOBJECT(m_pUnitHUD);
		}
	}
}