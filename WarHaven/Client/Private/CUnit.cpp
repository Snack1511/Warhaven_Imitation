#include "stdafx.h"
#include "CUnit.h"

#include "GameInstance.h"

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


CUnit::CUnit()
{
}

CUnit::~CUnit()
{
	SAFE_DELETE(m_pCurState);
}

void CUnit::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

void CUnit::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
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

void CUnit::Enter_State(STATE_TYPE eType, ANIM_TYPE eAnimType)
{
	if (m_pCurState)
	{
		m_pCurState->Exit(this, m_pAnimator);
		m_eAnimType = m_pCurState->m_eAnimType;
	}

	SAFE_DELETE(m_pCurState);
	m_eCurState = eType;
	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();

	if(m_eAnimType == ANIM_BASE_L)
		m_pCurState->Set_AnimType(ANIM_BASE_L);

	if (m_eAnimType == ANIM_BASE_R)
		m_pCurState->Set_AnimType(ANIM_BASE_R);

	m_pCurState->Enter(this, m_pAnimator);
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


	return S_OK;
}

HRESULT CUnit::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);
	m_pPhysics = GET_COMPONENT(CPhysics);

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
	
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	//m_pAnimator->Set_CurAnimIndex(ANIM_BASE, 22);
	DISABLE_COMPONENT(m_pPhysics);
	m_pPhysics->Set_NaviOn();

	CallBack_CollisionEnter += bind(&CUnit::Unit_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionStay += bind(&CUnit::Unit_CollisionStay, this, placeholders::_1, placeholders::_2);
	CallBack_CollisionExit += bind(&CUnit::Unit_CollisionExit, this, placeholders::_1, placeholders::_2);

	
	SetUp_TrailEffect(m_tUnitStatus.eWeapon);

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

void CUnit::SetUp_TrailEffect(WEAPON_TYPE eWeapon)
{
	const char* pBoneName = "0B_R_WP1";
	_float4 vWeaponLow;
	_float4 vWeaponHigh;
	_float4 vGlowFlag;
	_float4 vColor;
	wstring wstrMaskMapPath;
	wstring wstrColorMapPath;


	switch (eWeapon)
	{
	case Client::WEAPON_LONGSWORD:
		pBoneName = "0B_R_WP1";
		vWeaponLow = _float4(0.f, 0.f, -168.f, 1.f);
		vWeaponHigh = _float4(0.f, 0.f, -171.f, 1.f);
		vGlowFlag = _float4(1.f, 0.f, 0.f, 0.05f);
		vColor = _float4(1.f, 0.1f, 0.1f, 0.25f);
		wstrMaskMapPath = L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds";
		wstrColorMapPath = L"../bin/resources/Texture/Effects/WarHaven/T_EFF_Blur_05_M.dds";
		break;
	default:
		break;
	}

	m_pTrailEffect = CTrailEffect::Create(0, 10, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode(pBoneName), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pTrailEffect)
		return;

	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();

	m_pTrailEffect->TurnOn_TrailEffect(false);
}

void CUnit::TurnOn_TrailEffect(_bool bOn)
{
	if (!m_pTrailEffect)
		return;

	m_pTrailEffect->TurnOn_TrailEffect(bOn);

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
		Enter_State(eNewState, m_pCurState->m_eAnimType);
	}
}

void CUnit::My_LateTick()
{

}

