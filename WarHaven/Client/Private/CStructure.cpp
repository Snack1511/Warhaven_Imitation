#include "stdafx.h"
#include "CStructure.h"

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
CStructure::CStructure()
{
}

CStructure::~CStructure()
{
}

void CStructure::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

void CStructure::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
}

void CStructure::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}

void CStructure::Set_Passes(VTXMODEL_PASS_TYPE ePassType)
{
}

HRESULT CStructure::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();

	Add_Component<CRenderer>(pRenderer);
	_float4x4 matTrans;
	matTrans.Identity();

	//_matrix Mat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//XMStoreFloat4x4(&matTrans, Mat);
	CModel* pModel = CModel::Create(0, TYPE_NONANIM, TEXT("C:/Users/jusin/Desktop/meshtest/FBX/SM_Module_Gate_CastleGate01a.fbx"), matTrans);
	Add_Component(pModel);



	return S_OK;
}

HRESULT CStructure::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	//__super::Initialize();


	return S_OK;
}

HRESULT CStructure::Start()
{
	__super::Start();

	m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f, 0.f));
	//m_pModelCom->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);
    return S_OK;
}

void CStructure::OnEnable()
{
	__super::OnEnable();
}

void CStructure::OnDisable()
{
	__super::OnDisable();
}


HRESULT CStructure::SetUp_Model(const UNIT_MODEL_DATA& tData)
{
    return S_OK;
}

void CStructure::My_Tick()
{
}

void CStructure::My_LateTick()
{
}
