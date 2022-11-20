#include "CStructure_Instance.h"
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
#include "MeshContainer.h"
CStructure_Instance::CStructure_Instance()
{
}

CStructure_Instance::~CStructure_Instance()
{
}

CStructure_Instance* CStructure_Instance::Create(wstring MeshPath, wstring strInstancePath)
{
	CStructure_Instance* pInstance = new CStructure_Instance;

	if (FAILED(pInstance->SetUp_InstanceModel(MeshPath, strInstancePath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_InstanceModel : CStructure_Instance");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure_Instance");
		return nullptr;
	}

	return pInstance;
}

CStructure_Instance* CStructure_Instance::Create(wstring MeshPath, _int iNumInstance, VTXINSTANCE* pInstanceVTX)
{
	CStructure_Instance* pInstance = new CStructure_Instance;

	if (FAILED(pInstance->SetUp_InstanceModel(MeshPath, iNumInstance, pInstanceVTX)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_InstanceModel : CStructure_Instance");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure_Instance");
		return nullptr;
	}

	return pInstance;
}

HRESULT CStructure_Instance::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODELINSTANCE,
		VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();

	Add_Component<CRenderer>(pRenderer);
	return S_OK;
}

HRESULT CStructure_Instance::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	//__super::Initialize();


	return S_OK;
}

HRESULT CStructure_Instance::Start()
{
	__super::Start();

	//m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f, 0.f));
	m_pModelCom->Set_ShaderPassToAll(VTXINSTANCE_PASS_DEFAULT);
	return S_OK;
}


HRESULT CStructure_Instance::SetUp_InstanceModel(wstring strMeshPath, wstring strInstnacePath)
{
	_float4x4 TransformMatrix =
		XMMatrixIdentity();
		//XMMatrixScaling(0.005f, 0.005f, 0.005f)
		//* XMMatrixRotationY(XMConvertToRadians(270.0f));
	m_DebugPath = strMeshPath;
	CModel* pModel = CModel::Create(0, TYPE_NONANIM, strMeshPath, strInstnacePath, DEFAULT_MODEL_MATRIX);

	Add_Component(pModel);
	return S_OK;
}

HRESULT CStructure_Instance::SetUp_InstanceModel(wstring strMeshPath, _int iNumInstance, VTXINSTANCE* pInstanceVTX)
{
	//_float4x4 TransformMatrix =
	//	XMMatrixIdentity();
	m_DebugPath = strMeshPath;
	CModel* pModel = CModel::Create(0, TYPE_NONANIM, strMeshPath, iNumInstance, pInstanceVTX, DEFAULT_MODEL_MATRIX);

	Add_Component(pModel);
	return S_OK;
}

void CStructure_Instance::My_Tick()
{
	__super::My_Tick();
}

void CStructure_Instance::My_LateTick()
{
	__super::My_LateTick();
}
