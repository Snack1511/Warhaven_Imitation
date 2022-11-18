#include "stdafx.h"
#include "CUnit.h"

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

#include "MeshContainer.h"

#include "CState.h"
#include "CState_Manager.h"

#include "CPhysXCharacter.h"

#include "CBoneCollider.h"

#include "CCamera_Follow.h"


#define PHYSX_ON


CUnit::CUnit()
{
}

CUnit::~CUnit()
{
	SAFE_DELETE(m_pCurState);
}

void CUnit::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType)
{
	int i = 0;
}

void CUnit::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
}

void CUnit::Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType)
{
	int i = 0;
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

void CUnit::On_PlusHp(_float fHp)
{
	m_tUnitStatus.fHP += fHp;

	if (m_tUnitStatus.fHP <= 0.f)
	{
		m_tUnitStatus.fHP = 0.f;
		//On_Die();
	}
	else if (m_tUnitStatus.fHP >= m_tUnitStatus.fMaxHP)
	{
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}
}

_bool CUnit::Is_Weapon_R_Collision()
{
	if (!m_pWeaponCollider_R)
		return false;

	return m_pWeaponCollider_R->Is_Collision();
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
	tDesc.fHeight = 1.0f;
	CPhysXCharacter* pPhysXCharacter = CPhysXCharacter::Create(CP_BEFORE_TRANSFORM, tDesc);
	Add_Component(pPhysXCharacter);
#endif // PHYSX_OFF

	
	


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

	if(m_pWeaponCollider_L)
		DISABLE_COMPONENT(m_pWeaponCollider_L);

	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	//m_pAnimator->Set_CurAnimIndex(ANIM_BASE, 22);
	//DISABLE_COMPONENT(m_pPhysics);

#ifdef PHYSX_ON
	m_pPhysics->Set_NaviOn();
#endif

	CallBack_CollisionEnter += bind(&CUnit::Unit_CollisionEnter, this, placeholders::_1, placeholders::_2);
	CallBack_CollisionStay += bind(&CUnit::Unit_CollisionStay, this, placeholders::_1, placeholders::_2);
	CallBack_CollisionExit += bind(&CUnit::Unit_CollisionExit, this, placeholders::_1, placeholders::_2);

	
	SetUp_TrailEffect(m_tUnitStatus.eWeapon);

	m_pPhysics->Set_Jump(0.f);
	
	if (!m_pCurState)
	{
		Call_MsgBox(L"상태 세팅 안댔음");
		return E_FAIL;
	}

	m_pCurState->Enter(this, m_pAnimator, m_eCurState);

	if (m_pUnitCollider[BODY])
		ENABLE_COMPONENT(m_pUnitCollider[BODY]);
	if (m_pUnitCollider[HEAD])
		ENABLE_COMPONENT(m_pUnitCollider[HEAD]);

	return S_OK;
}

void CUnit::OnEnable()
{
	__super::OnEnable();
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	DISABLE_COMPONENT(m_pPhysics);
	On_PlusHp(m_tUnitStatus.fMaxHP - m_tUnitStatus.fHP);


}

void CUnit::OnDisable()
{
	__super::OnDisable();

}

//void CUnit::Set_Enable_WeaponCol(_bool bEnable)
//{
//	for (int i = 0; i < WEAPONCOLLIDER_END; ++i)
//	{
//		if (bEnable)
//			ENABLE_COMPONENT(m_pUnitCollider[i]);
//		else
//			DISABLE_COMPONENT(m_pUnitCollider[i]);
//	}
//}

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
			ENABLE_COMPONENT(m_pWeaponCollider_R);

			if(m_pWeaponCollider_L)
				ENABLE_COMPONENT(m_pWeaponCollider_L);
			
		}
		else
		{
			DISABLE_COMPONENT(m_pWeaponCollider_R);
			if (m_pWeaponCollider_L)
				DISABLE_COMPONENT(m_pWeaponCollider_L);
		}


	}
}

void CUnit::SetUp_UnitCollider(UNITCOLLIDER ePartType, UNIT_COLLIDERDESC* arrColliderDesc, _uint iNumCollider, _float4x4 matTransformation, _bool bEnable, CHierarchyNode* pRefBone)
{
	if (m_pUnitCollider[ePartType])
		return;

	m_pUnitCollider[ePartType] = m_pUnitCollider[ePartType] = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, arrColliderDesc[0].fRadius, arrColliderDesc[0].eColType, arrColliderDesc[0].vOffsetPos,
		matTransformation, pRefBone);

	Add_Component(m_pUnitCollider[ePartType]);

	Enable_UnitCollider(ePartType, bEnable);

	for (int i = 1; i < iNumCollider; ++i)
	{
		m_pUnitCollider[ePartType]->Add_Collider(arrColliderDesc[i].fRadius, arrColliderDesc[i].vOffsetPos);
	}
}

_float4 CUnit::Get_FollowCamLook()
{
	return m_pFollowCam->Get_Transform()->Get_World(WORLD_LOOK);
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
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) *XMMatrixRotationX(XMConvertToRadians(90.0f))* XMMatrixRotationZ(XMConvertToRadians(180.0f));

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
}

void CUnit::My_LateTick()
{
}

