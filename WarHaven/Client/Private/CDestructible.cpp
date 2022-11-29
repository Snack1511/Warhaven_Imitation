#include "stdafx.h"
#include "CDestructible.h"

#include "UsefulHeaders.h"

#include "PhysXCollider.h"

CDestructible::CDestructible()
{
}

CDestructible::~CDestructible()
{
}

CDestructible* CDestructible::Create(wstring wstrMeshFilePath, wstring wstrDestoryMultiEffectsKey, wstring wstrHitMultiEffectsKey, _uint iHitCount)
{
	CDestructible* pInstance = new CDestructible();

	pInstance->m_wstrDestoryMultiEffectsKey = wstrDestoryMultiEffectsKey;
	pInstance->m_wstrHitMultiEffectsKey = wstrHitMultiEffectsKey;
	pInstance->m_iHitCount= iHitCount;

	if (FAILED(pInstance->SetUp_Destructible(wstrMeshFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_Destructible : CDestructible");
		SAFE_DELETE(pInstance);
		return pInstance;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CDestructible");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CDestructible::On_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (m_fHitDelayAcc > 0.f)
		return;

	m_fHitDelayAcc = m_fHitDelayTime;

	m_iHitCount--;

	_float4x4 matIdentity;
	matIdentity.Identity();



	if (!m_wstrHitMultiEffectsKey.empty())
		CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(m_wstrHitMultiEffectsKey, vHitPos, _float4(0.f, 1.f, 0.f, 0.f), 0.1f, matIdentity);

	if (m_iHitCount == 0)
	{
		DISABLE_GAMEOBJECT(this);
		_float4 vPos = m_pTransform->Get_World(WORLD_POS);
		vPos.y += 0.5f;

		if (!m_wstrDestoryMultiEffectsKey.empty())
			CEffects_Factory::Get_Instance()->Create_Multi_MeshParticle(m_wstrDestoryMultiEffectsKey, vPos, _float4(0.f, 1.f, 0.f, 0.f), 4.f, matIdentity);

		static_cast<CUnit*>(pOtherObj)->Shake_Camera(5.f, 0.6f);

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", vHitPos);
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), vHitPos);
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), vHitPos);
	}
}

HRESULT CDestructible::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	return S_OK;
}

HRESULT CDestructible::Initialize()
{
	return S_OK;
}

HRESULT CDestructible::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CDestructible::On_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

	return S_OK;
}

void CDestructible::Set_Position(_float4 vPosition)
{
	m_pTransform->Set_World(WORLD_POS, vPosition);
	m_pTransform->Make_WorldMatrix();
	for (auto& elem : m_PhysXColliders)
	{
		elem->Set_Position(vPosition.XMLoad());
	}
}

void CDestructible::Set_Look(_float4 vLook)
{
	m_pTransform->Set_Look(vLook);
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	m_pTransform->Make_WorldMatrix();
	for (auto& elem : m_PhysXColliders)
	{
		elem->Set_Position(vPos.XMLoad(), m_pTransform->Get_Quaternion().XMLoad());
	}
}

HRESULT CDestructible::SetUp_Destructible(wstring wstrMeshFilePath)
{
	CModel* pModelCom = CModel::Create(0, TYPE_NONANIM, wstrMeshFilePath, DEFAULT_MODEL_MATRIX);
	if (!pModelCom)
		return E_FAIL;
	Add_Component(pModelCom);

	for (auto& elem : pModelCom->Get_MeshContainers())
	{
		if (elem.first != 0)
			continue;

		CPhysXCollider* pCol = CPhysXCollider::Create_Convex(0, elem.second, m_pTransform);
		if (!pCol)
			continue;

		
		m_PhysXColliders.push_back(pCol);
		Add_Component(pCol);
		pCol->Regist_CollisionCallBack();

	}

	return S_OK;
}

void CDestructible::My_Tick()
{
	if (m_fHitDelayAcc > 0.f)
		m_fHitDelayAcc -= fDT(0.f);
	else
		m_fHitDelayAcc = 0.f;
}
