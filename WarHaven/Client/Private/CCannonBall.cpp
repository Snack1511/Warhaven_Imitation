#include "stdafx.h"
#include "CCannonBall.h"

#include "UsefulHeaders.h"

#include "CUtility_PhysX.h"
#include "CColorController.h"

#include "CCannonBoom.h"

CCannonBall::CCannonBall()
{
}

CCannonBall::~CCannonBall()
{
}

void CCannonBall::Shoot_Cannon(CPlayer* pOwnerPlayer, _float4 vShootPos, _float4 vShootDir)
{
	m_pOwnerPlayer = pOwnerPlayer;
	ENABLE_GAMEOBJECT(this);

	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = vShootPos;

	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 25.f);
	_float4 vDir = vShootDir.Normalize();
	vDir *= m_fForcePower;
	pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	//pActor->addTorque(CUtility_PhysX::To_PxVec3(vDir));
	m_pActor = pActor;

}

HRESULT CCannonBall::Initialize_Prototype()
{
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, L"../bin/resources/meshes/weapons/ball/CannonBall.fbx",
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationX(XMConvertToRadians(90.0f))
	);

	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);




	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_NORMALMAPPING
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);



	/* PhysX */
	CMeshContainer* pMesh = (pModel->Get_MeshContainers().back().second);

	FACEINDICES32* pIndices = pMesh->CMesh::Get_Indices();
	_uint iNumPrimitive = pMesh->Get_NumPrimitive();

	_uint iNumVertices = pMesh->Get_NumVertices();
	_float3* pVerticesPos = pMesh->Get_VerticesPos();

	GAMEINSTANCE->Create_ConvexMesh(
		pVerticesPos,
		iNumVertices,
		pIndices,
		iNumPrimitive,
		&m_pConvexMesh);

	if (!m_pConvexMesh)
		return E_FAIL;

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	m_pCannonBoom = CCannonBoom::Create();
	if (!m_pCannonBoom)
		return E_FAIL;
	m_pCannonBoom->Initialize();

	return S_OK;
}

HRESULT CCannonBall::Initialize()
{
	return S_OK;
}

HRESULT CCannonBall::Start()
{
	__super::Start();

	if (m_pCannonBoom)
	{
		CREATE_GAMEOBJECT(m_pCannonBoom, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pCannonBoom);
	}
	

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (42.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_SKEL;

	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);


	return S_OK;
}

void CCannonBall::My_Tick()
{
}

void CCannonBall::My_LateTick()
{
	/* PhysX 따라가기 */
	PxTransform tTransform = m_pActor->getGlobalPose();
	_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
	m_pTransform->Get_Transform().matMyWorld = matPhysX;

	m_pTransform->Make_WorldMatrix();

	_float fPower = CUtility_PhysX::To_Vector(m_pActor->getLinearVelocity()).Length();

	if (fPower < 15.f)
		On_Boom();
}

void CCannonBall::OnEnable()
{
	__super::OnEnable();
	

}

void CCannonBall::OnDisable()
{
	__super::OnDisable();
	Safe_release(m_pActor);

}

void CCannonBall::On_Boom()
{
	DISABLE_GAMEOBJECT(this);
	if (m_pCannonBoom)
		m_pCannonBoom->Boom(m_pOwnerPlayer, m_pTransform->Get_World(WORLD_POS));

}
