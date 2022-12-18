#include "stdafx.h"
#include "CCannon.h"

#include "UsefulHeaders.h"

#include "CCollider_Sphere.h"

CCannon::CCannon()
{
}

CCannon::~CCannon()
{
}

void CCannon::Cannon_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CCannon::Cannon_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CCannon::Cannon_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

HRESULT CCannon::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/meshes/map/accessories/Cannon_Anim.fbx");
	pAnimator->Initialize();
	if (!pAnimator)
		return E_FAIL;
	Add_Component(pAnimator);
	m_pAnimator = pAnimator;

	_float4x4 matIdentity;
	matIdentity.Identity();

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/map/accessories/Cannon_30.fbx",
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(0.0f)) * XMMatrixRotationZ(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f)));

	pModel->Add_Model(L"../bin/resources/meshes/map/accessories/Cannon_30.fbx", 1);
	pModel->Initialize();
	Add_Component(pModel);

	if (FAILED(pModel->SetUp_AnimModel_LOD()))
		return E_FAIL;

	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);


	CCollider_Sphere* pCollider = CCollider_Sphere::Create(0, 2.f, COL_CANNON, _float4(0.f, 1.f, 0.f), DEFAULT_TRANS_MATRIX);
	pCollider->Initialize();
	Add_Component(pCollider);


	return S_OK;
}

HRESULT CCannon::Initialize()
{
    return S_OK;
}

HRESULT CCannon::Start()
{
	__super::Start();

	m_pAnimator->Set_CurAnimIndex(0, 0);
	m_pAnimator->Set_InterpolationTime(0, 0, 0.1f);
	m_pAnimator->Set_AnimSpeed(0, 0, 1.f);

    return S_OK;
}

_float4 CCannon::Get_ControlPos()
{
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos -= m_pTransform->Get_World(WORLD_LOOK) * 1.f;
	vPos.y += 1.f;
	return vPos;
}

void CCannon::Control_Cannon(CPlayer* pPlayer)
{
	m_pCurOwnerPlayer = pPlayer;
}

void CCannon::Shoot_Cannon()
{
	m_pAnimator->Set_CurAnimIndex(0, 0);
	m_pAnimator->Set_InterpolationTime(0, 0, 0.1f);
}

void CCannon::My_Tick()
{
	if (!m_pCurOwnerPlayer)
		return;

	if (!m_pCurOwnerPlayer->Get_CurrentUnit()->Is_Valid())
		return;
}
