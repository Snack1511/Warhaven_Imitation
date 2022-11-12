#include "stdafx.h"
#include "CDebugObject.h"

#include "UsefulHeaders.h"

#include "CMesh_Cube.h"


CDebugObject::CDebugObject()
{
}

CDebugObject::~CDebugObject()
{
}

CDebugObject* CDebugObject::Create(_float4 vPos, _float4 vScale, _float4 vAngle)
{
	
	CDebugObject* pInstance = new CDebugObject;

	if (FAILED(pInstance->SetUp_PhysXCollider(vPos, vScale, vAngle)))
	{
		Call_MsgBox(L"Failed to SetUp_PhysXCollider : CDebugObject");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDebugObject");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CDebugObject::SetUp_ShaderResources(CShader* pShaderCom, const char* pConstantName)
{
	pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
}

HRESULT CDebugObject::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXCUBETEX,
		VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);

	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXCUBETEX_PASS_DEBUG
		, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);
	Add_Component<CMesh>(CMesh_Cube::Create(0));

	

	return S_OK;
}

HRESULT CDebugObject::Initialize()
{
	return S_OK;
}

HRESULT CDebugObject::Start()
{
	__super::Start();

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CDebugObject::SetUp_ShaderResources, this, placeholders::_1, placeholders::_2);

	return S_OK;
}

HRESULT CDebugObject::SetUp_PhysXCollider(_float4 vPos, _float4 vScale, _float4 vAngle)
{
	CPhysXCollider::PHYSXCOLLIDERDESC		tPhysXColliderDesc;
	ZeroMemory(&tPhysXColliderDesc, sizeof(CPhysXCollider::PHYSXCOLLIDERDESC));

	tPhysXColliderDesc.eShape = CPhysXCollider::COLLIDERSHAPE::BOX;
	tPhysXColliderDesc.eType = CPhysXCollider::COLLIDERTYPE::STATIC;
	tPhysXColliderDesc.fDensity = 1.f;

	tPhysXColliderDesc.vAngles = vAngle;

	tPhysXColliderDesc.vPosition = vPos;
	tPhysXColliderDesc.vScale = vScale;

	CPhysXCollider* pPhysXCollider = CPhysXCollider::Create(CP_BEFORE_TRANSFORM, tPhysXColliderDesc);

	if (!pPhysXCollider)
		return E_FAIL;

	pPhysXCollider->Add_Trigger(TRIGGERDESC("Ground", COL_WALL, this, ZERO_VECTOR));

	Add_Component(pPhysXCollider);

	m_pTransform->Set_Scale(vScale);
	m_pTransform->Set_World(WORLD_POS, vPos);
	_float4 vQuat = XMQuaternionRotationRollPitchYawFromVector(vAngle.XMLoad());
	m_pTransform->MatrixRotationQuaternion(vQuat);
	m_pTransform->Make_WorldMatrix();
	return S_OK;
}
