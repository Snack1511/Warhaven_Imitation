#include "stdafx.h"
#include "CAnimWeapon.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CColorController.h"

CAnimWeapon::CAnimWeapon()
{
}

CAnimWeapon::~CAnimWeapon()
{
}

CAnimWeapon* CAnimWeapon::Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName, 
	_float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimWeapon* pInstance = new CAnimWeapon;

	pInstance->m_pOwnerUnit = pOwnerUnit;

	if (FAILED(pInstance->SetUp_Model(wstrModelFilePath, wstrAnimFilePath, strBoneName, fRadianX, fRadianY, fRadianZ)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CAnimWeapon");
		return nullptr;
	}
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CAnimWeapon");
		return nullptr;
	}

	return pInstance;
}

void CAnimWeapon::Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pAnimator->Set_CurAnimIndex(0, iAnimIndex);
	m_pAnimator->Set_InterpolationTime(0, iAnimIndex, fInterpolateTime);
	m_pAnimator->Set_AnimSpeed(0, iAnimIndex, fAnimSpeed);
}


HRESULT CAnimWeapon::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);


    return S_OK;
}

HRESULT CAnimWeapon::Initialize()
{
    return S_OK;
}

HRESULT CAnimWeapon::Start()
{
	__super::Start();

	if (m_pAnimator)
		Set_AnimIndex(0, 0.1f, 1.f);

    return S_OK;
}

HRESULT CAnimWeapon::SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, _float fRadianX, _float fRadianY, _float fRadianZ)
{
	if (wstrAnimFilePath.empty())
	{
		CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrModelFilePath,
			XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(fRadianZ)) * XMMatrixRotationY(XMConvertToRadians(fRadianY)) * XMMatrixRotationX(XMConvertToRadians(fRadianX))
		);
		pModel->Add_Model(wstrModelFilePath, 1);
		pModel->Initialize();
		Add_Component(pModel);
		pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
		pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	}
	else
	{
		CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, wstrAnimFilePath.c_str());
		pAnimator->Initialize();
		if (!pAnimator)
			return E_FAIL;
		Add_Component(pAnimator);
		m_pAnimator = pAnimator;

		_float4x4 matIdentity;
		matIdentity.Identity();

		CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrModelFilePath,
			XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(fRadianZ)) * XMMatrixRotationY(XMConvertToRadians(fRadianY)) * XMMatrixRotationX(XMConvertToRadians(fRadianX))
		);
		pModel->Add_Model(wstrModelFilePath, 1);
		pModel->Initialize();
		Add_Component(pModel);
		pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
		pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	}
	

	m_pOwnerBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(strBoneName.c_str());
	if (!m_pOwnerBone)
		return E_FAIL;


	return S_OK;
}

void CAnimWeapon::Late_Tick()
{
	_float4x4		matBone = m_pOwnerBone->Get_BoneMatrix();

	m_pTransform->Get_Transform().matMyWorld = matBone;

	m_pTransform->Make_WorldMatrix();
}
