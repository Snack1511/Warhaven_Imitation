#include "stdafx.h"
#include "CMesh_Particle.h"

#include "UsefulHeaders.h"

#include "CMesh_Cube.h"
#include "CUtility_PhysX.h"

#include "CInstanceMesh.h"

CMesh_Particle::CMesh_Particle()
{
}

CMesh_Particle::CMesh_Particle(const CMesh_Particle& _origin)
	: CEffect(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_bCloned(true)
	, m_pConvexMesh(_origin.m_pConvexMesh)
	, m_fLifeTime(_origin.m_fLifeTime)
{
	m_pInstanceMatrices = new _float4x4[m_iNumInstance];
	ZeroMemory(m_pInstanceMatrices, sizeof(_float4x4) * m_iNumInstance);


	/* 피직스 굽기 */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(tTransform));
	tTransform.q.w = 1.f;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxRigidDynamic* pActor = nullptr;

		pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.f);

		m_vecRigidDynamics.push_back(pActor);
		pActor->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, true);
		pActor->putToSleep();
	}

}

CMesh_Particle::~CMesh_Particle()
{
	if (!m_bCloned)
		Safe_release(m_pConvexMesh);


	SAFE_DELETE_ARRAY(m_pInstanceMatrices);
}

CMesh_Particle* CMesh_Particle::Create(wstring wstrModelFilePath, _uint iNumInstance, wstring strName, _float fLifeTime)
{
	CMesh_Particle* pInstance = new CMesh_Particle();

	pInstance->m_iNumInstance = iNumInstance;
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_hcMyCode = Convert_ToHash(strName);

	if (FAILED(pInstance->SetUp_MeshParticle(wstrModelFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshParticle : CMesh_Particle");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CMesh_Particle");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}



void CMesh_Particle::Start_Particle(_float4 vPos, _float4 vDir, _float fPower)
{

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxTransform tTransform;
		ZeroMemory(&tTransform, sizeof(PxTransform));
		tTransform.p.x = vPos.x;
		tTransform.p.y = vPos.y;
		tTransform.p.z = vPos.z;
		tTransform.q.w = 1.f;
		m_vecRigidDynamics[i]->setGlobalPose(tTransform);


		_float4	vNewDir =
		{
		vDir.x * frandom(0.9f, 1.1f),
		vDir.y * frandom(0.9f, 1.1f),
		vDir.z * frandom(0.9f, 1.1f),
		0.f
		};

		vNewDir *= fPower * frandom(0.9f, 1.1f);

		m_vecRigidDynamics[i]->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, false);
		//m_vecRigidDynamics[i]->wakeUp();
		m_vecRigidDynamics[i]->addForce(CUtility_PhysX::To_PxVec3(vNewDir));

	}
}

HRESULT CMesh_Particle::Initialize_Prototype()
{
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODELINSTANCE,
		VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements);

	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXINSTANCE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));


	Add_Component<CRenderer>(pRenderer);

	/* 피직스 굽기 */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(tTransform));
	tTransform.q.w = 1.f;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxRigidDynamic* pActor = nullptr;

		pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.f);

		if (!pActor)
			return E_FAIL;

		m_vecRigidDynamics.push_back(pActor);
		pActor->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, true);
		pActor->putToSleep();
	}




	return S_OK;
}

HRESULT CMesh_Particle::Initialize()
{
	return S_OK;
}

HRESULT CMesh_Particle::Start()
{
	CGameObject::Start();


	return S_OK;
}

void CMesh_Particle::OnEnable()
{
}

void CMesh_Particle::OnDisable()
{
	__super::OnDisable();
	m_fTimeAcc = 0.f;
}

HRESULT CMesh_Particle::SetUp_MeshParticle(wstring wstrModelFilePath)
{
	CModel* pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath.c_str(), m_iNumInstance, m_matTrans);
	Add_Component(pModelCom);


	//ConvexMesh 하나 구워놓고 일단
	CMesh* pMesh = static_cast<CMesh*>(pModelCom->Get_MeshContainers().front().second);

	GAMEINSTANCE->Create_ConvexMesh(
		pMesh->Get_VerticesPos(),
		pMesh->Get_NumVertices(),
		pMesh->Get_Indices(),
		pMesh->Get_NumPrimitive(),
		&m_pConvexMesh);

	if (!m_pConvexMesh)
		return E_FAIL;


	m_pInstanceMatrices = new _float4x4[m_iNumInstance];
	ZeroMemory(m_pInstanceMatrices, sizeof(_float4x4) * m_iNumInstance);

	return S_OK;
}

void CMesh_Particle::My_Tick()
{
	m_fTimeAcc += fDT(0);

	if (m_fTimeAcc >= m_fLifeTime)
		DISABLE_GAMEOBJECT(this);
}

void CMesh_Particle::My_LateTick()
{

	_float4x4 matWorldInv = m_pTransform->Get_WorldMatrix();
	matWorldInv.Inverse();

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxTransform tTransform = m_vecRigidDynamics[i]->getGlobalPose();

		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);

		//피직스월드매트릭스에 역행렬을 곱해야대

		m_pInstanceMatrices[i] = matPhysX * matWorldInv;
	}

	static_cast<CInstanceMesh*>(GET_COMPONENT(CModel)->Get_MeshContainers().front().second)
		->ReMap_Instances(m_pInstanceMatrices);

}
