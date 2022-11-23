#include "stdafx.h"
#include "CUnit.h"

#include "CEffects_Factory.h"

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
	//내 충돌체에 대해 바디샷 인지 헤드샷 인지 판정

	//칼 충돌체가 충돌했는지 판단
	/*if (!Is_Weapon_R_Collision())
		return;*/


	CUnit* pOtherUnit = nullptr;

#ifdef _DEBUG
	pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);
#else
	pOtherUnit = static_cast<CUnit*>(pOtherObj);

#endif // _DEBUG

	if (!pOtherUnit)
		return;

	/* 이미 hit 상태면 delay 걸기 */
	if (m_fHitDelayAcc > 0.f)
		return;

	m_fHitDelayAcc = m_fHitDelayTime;

	STATE_TYPE	eFinalHitState = STATE_END;
	_bool		bGuardSuccess = false;

	UNIT_STATUS& tOtherStatus = pOtherUnit->Get_Status();

	CState::HIT_INFO tOtherHitInfo = pOtherUnit->Get_CurStateP()->Get_HitInfo();

	tOtherHitInfo.vDir = (m_pTransform->Get_World(WORLD_POS) - vHitPos);
	tOtherHitInfo.vDir.y = 0.f;
	tOtherHitInfo.vDir.Normalize();


	//상대 위치 계산
	_float4 vOtherLook = pOtherObj->Get_Transform()->Get_World(WORLD_LOOK).Normalize();
	_float4 vCurLook = Get_Transform()->Get_World(WORLD_LOOK).Normalize();


	//양수면 앞임.
	if (vCurLook.Dot(vOtherLook) < 0.f)
		tOtherHitInfo.bFace = true;
	else
		tOtherHitInfo.bFace = false;

	switch (eMyColType)
	{

	case COL_PLAYERATTACK:
	case COL_ENEMYATTACK:

		break;

	case COL_PLAYERGUARD:
	case COL_ENEMYGUARD:
	{
		if (tOtherHitInfo.bFace)
		{
			//일단 가드는 성공
			if (eOtherColType == COL_PLAYERGUARDBREAK || eOtherColType == COL_ENEMYGUARDBREAK)
			{
				//가드 브레이크공격이 들어오면 
				eFinalHitState = m_tHitType.m_eGuardBreakState;
				Effect_Hit(vHitPos);
			}
			/* 가드 성공 */
			else
			{
				eFinalHitState = m_tHitType.m_eGuardState;
				bGuardSuccess = true;

			}
		}
		else
		{
			// 공격 공격을 당했다면
			if (eOtherColType == COL_PLAYERFLYATTACK || eOtherColType == COL_ENEMYFLYATTACK)
				eFinalHitState = m_tHitType.m_eFlyState;

			else if (eOtherColType == COL_PLAYERGROGGYATTACK || eOtherColType == COL_ENEMYGROGGYATTACK)
				eFinalHitState = m_tHitType.m_eGroggyState;

			else
				eFinalHitState = m_tHitType.m_eHitState;
		}

	}

	break;

	case COL_PLAYERHITBOX_BODY:
	case COL_ENEMYHITBOX_BODY:

		// 공격 공격을 당했다면
		if (eOtherColType == COL_PLAYERFLYATTACK || eOtherColType == COL_ENEMYFLYATTACK)
			eFinalHitState = m_tHitType.m_eFlyState;

		// 그로기 공격을 당했다면
		else if (eOtherColType == COL_PLAYERGROGGYATTACK || eOtherColType == COL_ENEMYGROGGYATTACK)
			eFinalHitState = m_tHitType.m_eGroggyState;

		else
			eFinalHitState = m_tHitType.m_eHitState;



#ifdef _DEBUG
		cout << " 바디샷 " << endl;
#endif // 


		break;


	case COL_PLAYERHITBOX_HEAD:
	case COL_ENEMYHITBOX_HEAD:


		// 공격 공격을 당했다면
		if (eOtherColType == COL_PLAYERFLYATTACK || eOtherColType == COL_ENEMYFLYATTACK)
			eFinalHitState = m_tHitType.m_eFlyState;

		// 그로기 공격을 당했다면
		else if (eOtherColType == COL_PLAYERGROGGYATTACK || eOtherColType == COL_ENEMYGROGGYATTACK)
			eFinalHitState = m_tHitType.m_eGroggyState;

		else
		{
			eFinalHitState = m_tHitType.m_eHitState;
			tOtherHitInfo.bHeadShot = true;
		}

#ifdef _DEBUG
		cout << " 헤드샷 " << endl;
#endif // 


	default:
		break;
	}


	if (eFinalHitState == STATE_END)
		return;

	_float fDamage = pOtherUnit->Calculate_Damage(tOtherHitInfo.bHeadShot, bGuardSuccess);

	if (pOtherUnit)
	{
		if (pOtherUnit->m_bIsMainPlayer)
		{
			CUser::Get_Instance()->SetActive_DamageTex(fDamage, tOtherHitInfo.bHeadShot);
		}
	}

	if (On_PlusHp(fDamage, pOtherUnit))
	{
		Enter_State(eFinalHitState, &tOtherHitInfo);

		Switch_ClassEffect(pOtherUnit, vHitPos); // 체력 줄을시에만 이펙트 발생 사망시 연기이펙트만 나옴
	}
	else
	{

		//m_eStingHitState (COL 찌르기 상태를 추가하자.)
		//if()

		/* 체력 0 이하로 내려간 경우 */
		m_bDie = true;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StoneSpark", vHitPos);
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
	CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(L"DeathStoneParticle", vPos, _float4(0.f, 1.f, 0.f, 0.f), 0.5f);
	vPos.y -= 0.5f;

	_float4x4 vCamMatrix = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KillSmoke_Right", vPos, vCamMatrix);
	vPos.y += 1.f;

	DISABLE_GAMEOBJECT(this);

	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
		//SAFE_DELETE(m_pCurState);
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

	return fDamage;
}

_bool CUnit::On_PlusHp(_float fHp, CUnit* pOtherUnit)
{
	m_tUnitStatus.fHP += fHp;

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
	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();
	m_eCurState = eType;

}

void CUnit::Teleport_Unit(_float4 vPosition)
{
#ifdef PHYSX_ON
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


	SetUp_TrailEffect(m_tUnitStatus.eWeapon);

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

	for (int i = 1; i < iNumCollider; ++i)
	{
		m_pUnitCollider[ePartType]->Add_Collider(arrColliderDesc[i].fRadius, arrColliderDesc[i].vOffsetPos);
	}
}

void CUnit::SetUp_HitStates(_bool bPlayer)
{
	if (!bPlayer)
		m_tHitType.m_eHitState = STATE_HIT_TEST_ENEMY;
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

void CUnit::SetUp_TrailEffect(WEAPON_TYPE eWeapon)
{
	const char* pBoneName = "empty";
	_float4 vWeaponLow;
	_float4 vWeaponHigh;
	_float4 vWeaponLeft;
	_float4 vWeaponRight;
	_float4 vGlowFlag;
	_float4 vColor;

	_float fWeaponCenter;
	wstring wstrMaskMapPath;
	wstring wstrColorMapPath;
	_uint	m_iTrailCount;

	switch (eWeapon)
	{
	case Client::WEAPON_LONGSWORD:
		/* Trail 길이 */
		m_iTrailCount = 10;
		pBoneName = "0B_R_WP1";
		vWeaponLow = _float4(0.f, 0.f, -168.f, 1.f);
		vWeaponHigh = _float4(0.f, 0.f, -171.f, 1.f);

		fWeaponCenter = (vWeaponLow.z + vWeaponHigh.z) * 0.5f;

		vWeaponLeft = _float4(0.f, -1.5f, fWeaponCenter, 1.f);
		vWeaponRight = _float4(0.f, 1.5f, fWeaponCenter, 1.f);



		vGlowFlag = _float4(1.f, 0.f, 0.f, 0.05f);
		vColor = _float4(1.f, 0.1f, 0.1f, 0.25f);
		wstrMaskMapPath = L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds";
		wstrColorMapPath = L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds";
		break;
	default:
		break;
	}

	/* 3D느낌 주려고 Trail 십자가 모양으로 2개 쓰는 중*/

	m_pTrailEffect = CTrailEffect::Create(0, m_iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(pBoneName), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pTrailEffect2 = CTrailEffect::Create(0, m_iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode(pBoneName), m_pTransform, vGlowFlag, vColor,
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

void CUnit::TurnOn_TrailEffect(_bool bOn)
{
	if (!m_pTrailEffect)
		return;

	m_pTrailEffect->TurnOn_TrailEffect(bOn);
	m_pTrailEffect2->TurnOn_TrailEffect(bOn);

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
			if (i == 0)
			{
				CUser::Get_Instance()->Set_SkillCoolTime(0, m_fCoolAcc[0], m_fCoolTime[0]);
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

void CUnit::Effect_Hit(_float4 vHitPos)
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), vHitPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), vHitPos);
	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"GroundHitParticle", vHitPos);
}

void CUnit::Switch_ClassEffect(CUnit* pUnit, _float4 vHitPos)
{
	CLASS_TYPE eClass = pUnit->Get_Status().eClass;

	if ((STATE_GUARDHIT_WARRIOR != m_eCurState) && (STATE_GUARDHIT_ENEMY != m_eCurState))
	{
		switch (eClass)
		{
		case Client::CUnit::WARRIOR:
			Warrior_Effect(pUnit, vHitPos);
			break;
		case Client::CUnit::SPEAR:
			break;
		case Client::CUnit::ARCHER:
			break;
		case Client::CUnit::PALADIN:
			break;
		case Client::CUnit::PRIEST:
			break;
		case Client::CUnit::ENGINEER:
			break;
		case Client::CUnit::FIONA:
			break;
		case Client::CUnit::QANDA:
			break;
		case Client::CUnit::HOEDT:
			break;
		case Client::CUnit::LANCER:
			break;
		case Client::CUnit::CLASS_END:
			break;
		default:
			break;
		}
	}

}

void CUnit::Warrior_Effect(CUnit* pUnit, _float4 vHitPos)
{
	_float4x4 vCamMatrix = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);

	switch (pUnit->Get_CurState())
	{
	case STATE_ATTACK_HORIZONTALUP_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LU", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_L:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_R_AI_ENEMY:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_HORIZONTALDOWN_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_HORIZONTALUP_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RU", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_HORIZONTALMIDDLE_R:
	case STATE_HORIZONTALMIDDLEATTACK_WARRIOR_L_AI_ENEMY:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_HORIZONTALDOWN_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_VERTICALCUT:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_D", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_STING_PLAYER_L:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, vCamMatrix);
		break;
	case STATE_ATTACK_STING_PLAYER_R:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"StingBlood", vHitPos, vCamMatrix);
		break;

	}

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SmallSparkParticle", pUnit, vHitPos);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSpark", pUnit, vHitPos);
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

			break;


		default:
			break;
		}
	}

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
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.f))
	{
		if (!m_pUnitHUD->Is_Valid())
		{
			if (!m_bIsMainPlayer)
			{
				// 처음에는 동그라미만 뜨다가 가까이 접근하면 적군으로 바뀌고
				// 카메라의 위치를 뷰위치를 받아오고 유닛들의 위치를 받아와서 일정 거리 이하
				// 유틸리티트랜스폼에 함수 구현
				// 데미지를 입을 시 체력바 활성화
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