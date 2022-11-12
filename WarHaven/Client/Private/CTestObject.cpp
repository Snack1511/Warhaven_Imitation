#include "stdafx.h"
#include "CTestObject.h"

#include "UsefulHeaders.h"

#include "CMesh_Cube.h"

CTestObject::CTestObject()
{
}

CTestObject::~CTestObject()
{
}

HRESULT CTestObject::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXCUBETEX,
		VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);

	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXCUBETEX_PASS_DEBUG
		, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);
	Add_Component<CMesh>(CMesh_Cube::Create(0));


	CPhysXCollider::PHYSXCOLLIDERDESC		tPhysXColliderDesc;
	ZeroMemory(&tPhysXColliderDesc, sizeof(CPhysXCollider::PHYSXCOLLIDERDESC));

	tPhysXColliderDesc.eShape = CPhysXCollider::COLLIDERSHAPE::BOX;
	tPhysXColliderDesc.eType = CPhysXCollider::COLLIDERTYPE::DYNAMIC;
	tPhysXColliderDesc.fDensity = 1.f;
	tPhysXColliderDesc.vAngles = _float4(frandom(0, 360.f), frandom(0, 360.f), frandom(0, 360.f));
	tPhysXColliderDesc.vPosition = _float4(frandom(-10.f, 10.f), 10.f, frandom(-10.f, 10.f));
	tPhysXColliderDesc.vScale = _float4(1.f, 1.f, 1.f, 1.f);
	CPhysXCollider* pPhysXCollider = CPhysXCollider::Create(CP_BEFORE_TRANSFORM, tPhysXColliderDesc);




	Add_Component(pPhysXCollider);

	return S_OK;
}

HRESULT CTestObject::Initialize()
{
	return S_OK;
}

HRESULT CTestObject::Start()
{
	__super::Start();

	return S_OK;
}
