#include "stdafx.h"
#include "CTestObject.h"

#include "UsefulHeaders.h"



CTestObject::CTestObject()
{
}

CTestObject::~CTestObject()
{
}

HRESULT CTestObject::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);


	_float4x4 TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, L"../bin/resources/meshes/map/structure/building/SM_Architecture_Building_ShipBase01a.fbx", TransformMatrix);
	Add_Component(pModel);


	CPhysXCollider::PHYSXCOLLIDERDESC		tPhysXColliderDesc;
	ZeroMemory(&tPhysXColliderDesc, sizeof(CPhysXCollider::PHYSXCOLLIDERDESC));

	tPhysXColliderDesc.eShape = CPhysXCollider::COLLIDERSHAPE::BOX;
	tPhysXColliderDesc.eType = CPhysXCollider::COLLIDERTYPE::DYNAMIC;
	tPhysXColliderDesc.fDensity = 1.f;
	tPhysXColliderDesc.vAngles = ZERO_VECTOR;
	tPhysXColliderDesc.vPosition = ZERO_VECTOR;
	tPhysXColliderDesc.vScale = _float4(1.f, 1.f, 1.f, 1.f);

	Add_Component(CPhysXCollider::Create(CP_BEFORE_TRANSFORM, tPhysXColliderDesc));

	return S_OK;
}

HRESULT CTestObject::Initialize()
{
	return S_OK;
}

HRESULT CTestObject::Start()
{
	return S_OK;
}
