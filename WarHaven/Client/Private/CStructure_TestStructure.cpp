#include "stdafx.h"
#include "CStructure_TestStructure.h"
#include "GameInstance.h"
#include "Model.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Physics.h"
CStructure_TestStructure::CStructure_TestStructure()
{
}

CStructure_TestStructure::~CStructure_TestStructure()
{
}

CStructure_TestStructure* CStructure_TestStructure::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CStructure_TestStructure* pInstance = new CStructure_TestStructure;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CStructure_TestStructure");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure_TestStructure");
		return nullptr;
	}

	return pInstance;
}

HRESULT CStructure_TestStructure::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);


	Add_Component(CPhysics::Create(0));

	//추가적으로 Animator 만들어야댐.


	return S_OK;
}

HRESULT CStructure_TestStructure::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pPhysics = GET_COMPONENT(CPhysics);
	//__super::Initialize();

	return S_OK;
}

HRESULT CStructure_TestStructure::Start()
{
	__super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);
	return S_OK;
}

void CStructure_TestStructure::OnEnable()
{
	__super::OnEnable();
}

void CStructure_TestStructure::OnDisable()
{
	__super::OnDisable();
}
