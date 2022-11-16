#include "..\Public\PhysXCollider.h"
#include "PhysX_Manager.h"
#include "Transform.h"

#include "CMesh_Terrain.h"
#include "GameObject.h"
#include "CMesh.h"

CPhysXCollider::CPhysXCollider(_uint iGroupID)
	: CComponent(iGroupID)
{
}

CPhysXCollider::CPhysXCollider(const CPhysXCollider& rhs)
	: CComponent(rhs)
	, m_vMaxVelocity(rhs.m_vMaxVelocity)
	, m_ColliderDesc(rhs.m_ColliderDesc)
{
	//복사생성자만들기
	CreatePhysActor(m_ColliderDesc);


}

CPhysXCollider::~CPhysXCollider()
{
	Release();
}

CPhysXCollider* CPhysXCollider::Create(_uint iGroupID, const PHYSXCOLLIDERDESC& tPhysXColliderDesc)
{

	CPhysXCollider* pInstance = new CPhysXCollider(iGroupID);

	pInstance->m_ColliderDesc = tPhysXColliderDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CPhysXCollider");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

CPhysXCollider* CPhysXCollider::Create(_uint iGroupID, CMesh* pMesh, CTransform* pWorldTransform)
{
	CPhysXCollider* pInstance = new CPhysXCollider(iGroupID);

	if (FAILED(pInstance->SetUp_StaticMeshActor(pMesh, pWorldTransform)))
	{
		Call_MsgBox(L"Failed to SetUp_StaticMeshActor : CPhysXCollider");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

CPhysXCollider* CPhysXCollider::Create_Convex(_uint iGroupID, CMesh* pMesh, CTransform* pWorldTransform)
{
	CPhysXCollider* pInstance = new CPhysXCollider(iGroupID);

	if (FAILED(pInstance->SetUp_StaticConvexActor(pMesh, pWorldTransform)))
	{
		Safe_Delete(pInstance);
		return nullptr;
		Call_MsgBox(L"Failed to SetUp_StaticMeshActor : CPhysXCollider");
	}

	return pInstance;
}

HRESULT CPhysXCollider::Initialize_Prototype()
{

	CreatePhysActor(m_ColliderDesc);

	return S_OK;
}



HRESULT CPhysXCollider::Initialize()
{
	return S_OK;
}

void CPhysXCollider::Tick()
{
	//여기서 transform을 피직스에넣고 피직스 연산후
}

void CPhysXCollider::Late_Tick()
{
	//여기서 갱신시키기
	if (m_pRigidDynamic)
	{
		_float4	vPos = Get_Position();
		m_pOwner->Get_Transform()->Set_World(WORLD_POS, vPos);
	}
	

	

	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_ColliderDesc.eType == COLLIDERTYPE::DYNAMIC)
	{
		_float4 vQuaternion = _float4(Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w);
		m_pOwner->Get_Transform()->MatrixRotationQuaternion(vQuaternion);
	}
	

}

void CPhysXCollider::Release()
{
	Safe_release(m_ColliderDesc.pConvecMesh);
	Safe_release(m_ColliderDesc.pMaterial);
	Safe_release(m_ColliderDesc.pTriangleMesh);
	Safe_release(m_pRigidDynamic);
	Safe_release(m_pRigidStatic);
}

void CPhysXCollider::OnEnable()
{
	__super::OnEnable();
	
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
		m_pRigidDynamic->wakeUp();

	}
}

void CPhysXCollider::OnDisable()
{
	__super::OnDisable();

	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->putToSleep();
		m_pRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

	}
}


void CPhysXCollider::Synchronize_Transform(CTransform * pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	pTransform->Set_World(WORLD_POS, vPos);
	pTransform->MatrixRotationQuaternion(vQuaternion);
}

void CPhysXCollider::Synchronize_Transform_Position(CTransform * pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	pTransform->Set_World(WORLD_POS, vPos);
}

void CPhysXCollider::Synchronize_Transform_Rotation(CTransform * pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	pTransform->MatrixRotationQuaternion(vQuaternion);
}

void CPhysXCollider::Synchronize_Collider(CTransform * pTransform)
{
	_vector vPos = pTransform->Get_World(WORLD_POS).XMLoad();
	_vector vQuaternion = pTransform->Get_Quaternion().XMLoad();
	Set_Position(vPos, vQuaternion);
}

void CPhysXCollider::PutToSleep()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->putToSleep();
}

void CPhysXCollider::WakeUp()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->wakeUp();
}

void CPhysXCollider::Attach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->attachShape(*pShape);
}

void CPhysXCollider::Detach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->detachShape(*pShape);
}

HRESULT CPhysXCollider::Add_Force(_vector _vForce)
{
	PxVec3	vForce;
	vForce = { XMVectorGetX(_vForce), XMVectorGetY(_vForce), XMVectorGetZ(_vForce) };

	if (m_pRigidDynamic)
		m_pRigidDynamic->addForce(vForce);

	PxVec3	vMaxVel = { XMVectorGetX(m_vMaxVelocity), XMVectorGetY(m_vMaxVelocity), XMVectorGetZ(m_vMaxVelocity) };
	PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();

	_vector	XDir = { 1.f, 0.f, 0.f, 0.f };
	_vector	ZDir = { 0.f, 1.f, 0.f, 0.f }; //vector2Dot으로 계산 하기 때문

	_vector	VelocityVec2 = { vVelocity.x, vVelocity.z, 0.f, 0.f };

	_vector vNormalizedVelocity = XMVector2Normalize(VelocityVec2);

	//m_vMaxVelocity
	_vector	vSpeed = XMVector2Length(VelocityVec2);
	_float	fSpeed = min(XMVectorGetX(vSpeed), XMVectorGetX(m_vMaxVelocity));

	vNormalizedVelocity *= fSpeed;

	_vector	XSpeed = XDir * XMVectorGetX(XMVector2Dot(XDir, vNormalizedVelocity));
	_vector	ZSpeed = ZDir * XMVectorGetX(XMVector2Dot(ZDir, vNormalizedVelocity));

	vVelocity.x = XMVectorGetX(XSpeed);
	vVelocity.z = XMVectorGetY(ZSpeed);
	//vVelocity.x = min(fabsf(vVelocity.x), vMaxVel.x) * vVelocity.x / vVelocity.x;
	//vVelocity.y = min(fabsf(vVelocity.y), vMaxVel.y) * vVelocity.y / vVelocity.y;
	//vVelocity.z = min(fabsf(vVelocity.z), vMaxVel.z) * vVelocity.z / vVelocity.z;

	m_pRigidDynamic->setLinearVelocity(vVelocity);

	return S_OK;
}

HRESULT CPhysXCollider::Clear_Force()
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->clearForce(PxForceMode::eACCELERATION);
		m_pRigidDynamic->clearForce(PxForceMode::eFORCE);
		m_pRigidDynamic->clearForce(PxForceMode::eIMPULSE);
		m_pRigidDynamic->clearForce(PxForceMode::eVELOCITY_CHANGE);
	}
	//m_pRigidDynamic->setMassSpaceInertiaTensor(PxVec3(0.f));
	m_pRigidDynamic->putToSleep();
	m_pRigidDynamic->wakeUp();
	return S_OK;
}

HRESULT CPhysXCollider::Clear_Velocity()
{
	m_pRigidDynamic->setLinearVelocity(PxVec3(0.f));
	return S_OK;
}

HRESULT CPhysXCollider::Add_LinearVelocityResistance(_vector vResistanceRate)
{
	PxVec3	vResistRate = {XMVectorGetX(vResistanceRate), XMVectorGetY(vResistanceRate), XMVectorGetZ(vResistanceRate) };
	PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();
	vVelocity.x *= vResistRate.x;
	vVelocity.y *= vResistRate.y;
	vVelocity.z *= vResistRate.z;

	m_pRigidDynamic->setLinearVelocity(vVelocity);

	return S_OK;
}

_vector CPhysXCollider::Get_Position()
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	return XMVectorSet(Transform.p.x, Transform.p.y, Transform.p.z, 1.f);
}

_vector CPhysXCollider::Get_Velocity()
{
	if (m_pRigidDynamic)
	{
		PxVec3 Velocity = m_pRigidDynamic->getLinearVelocity();
		return XMVectorSet(Velocity.x, Velocity.y, Velocity.z, 0.f);
	}

	return _vector{ 0.f, 0.f, 0.f, 0.f };
}

_float CPhysXCollider::Get_Mess()
{
	assert(m_pRigidDynamic);

	PxReal fMess = m_pRigidDynamic->getMass();
	return fMess;
}

_vector CPhysXCollider::Get_AngularVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getAngularVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

_vector CPhysXCollider::Get_LinearVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getLinearVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

HRESULT CPhysXCollider::Set_Position(_vector _vPos, _vector _vQuaternion)
{
	PxTransform	Transform;
	PxVec3	vPos(XMVectorGetX(_vPos), XMVectorGetY(_vPos), XMVectorGetZ(_vPos));
	PxQuat	vQuaternion(XMVectorGetX(_vQuaternion), XMVectorGetY(_vQuaternion), XMVectorGetZ(_vQuaternion), XMVectorGetW(_vQuaternion));

	Transform.p = vPos;
	Transform.q = vQuaternion;

	if (m_pRigidDynamic)
		m_pRigidDynamic->setGlobalPose(Transform);

	if (m_pRigidStatic)
		m_pRigidStatic->setGlobalPose(Transform);

	return S_OK;
}

HRESULT CPhysXCollider::Set_Position(_vector _vPos)
{
	PxTransform	Transform;
	PxVec3	vPos(XMVectorGetX(_vPos), XMVectorGetY(_vPos), XMVectorGetZ(_vPos));

	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	Transform.p = vPos;

	if (m_pRigidDynamic)
		m_pRigidDynamic->setGlobalPose(Transform);

	if (m_pRigidStatic)
		m_pRigidStatic->setGlobalPose(Transform);

	return S_OK;
}

void CPhysXCollider::Set_Scale(_vector vScale)
{
	//PxShape* pShape[1];
	//ZeroMemory(pShape, sizeof(PxShape) * 16);
	if (m_pRigidDynamic)
	{
		//m_pRigidDynamic->getShapes(pShape, 1);
		m_pRigidDynamic->release();
		m_pRigidDynamic = nullptr;

		m_ColliderDesc.vScale = vScale;
		CreatePhysActor(m_ColliderDesc);

		//m_pRigidDynamic->attachShape(*pShape[0]);

		for (size_t i = 0; i < 16; ++i)
		{
			if (nullptr != m_ConvexMeshes[i])
			{
				PxConvexMeshGeometry PxGeometry;
				PxGeometry.convexMesh = m_ConvexMeshes[i];
				PxMeshScale	vMeshScale;
				vMeshScale.scale.x = XMVectorGetX(vScale);
				vMeshScale.scale.y = XMVectorGetY(vScale);
				vMeshScale.scale.z = XMVectorGetZ(vScale);
				PxGeometry.scale = vMeshScale;
				PxShape*	pShape;
				PxMaterial*	pMaterial = m_ColliderDesc.pMaterial;

				CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, &pShape);
				m_pRigidDynamic->attachShape(*pShape);
			}
		}
	}

	if (m_pRigidStatic)
	{
		//m_pRigidStatic->getShapes(pShape, 16);
		m_pRigidStatic->release();
		m_pRigidStatic = nullptr;

		m_ColliderDesc.vScale = vScale;
		CreatePhysActor(m_ColliderDesc);


		for (size_t i = 0; i < 16; ++i)
		{
			if (nullptr != m_ConvexMeshes[i])
			{
				PxConvexMeshGeometry PxGeometry;
				PxGeometry.convexMesh = m_ConvexMeshes[i];
				PxMeshScale	vMeshScale;
				vMeshScale.scale.x = XMVectorGetX(vScale);
				vMeshScale.scale.y = XMVectorGetY(vScale);
				vMeshScale.scale.z = XMVectorGetZ(vScale);
				PxGeometry.scale = vMeshScale;
				PxShape*	pShape;
				PxMaterial*	pMaterial = m_ColliderDesc.pMaterial;

				CPhysX_Manager::Get_Instance()->Create_Shape(PxGeometry, pMaterial, &pShape);

				m_pRigidStatic->attachShape(*pShape);
			}
		}
	}
}

void CPhysXCollider::Set_ActorFlag(PxActorFlag::Enum eFlag, _bool bState)
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->setActorFlag(eFlag, bState);
	}
}


void CPhysXCollider::Delete_Collider()
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->release();
		m_pRigidDynamic = nullptr;
	}

	if (m_pRigidStatic)
	{
		m_pRigidStatic->release();
		m_pRigidStatic = nullptr;
	}
}

void CPhysXCollider::Create_Collider()
{
	if (!m_pRigidDynamic && !m_pRigidStatic)
	{
		CreatePhysActor(m_ColliderDesc);
	}
}


void CPhysXCollider::CreatePhysActor(PHYSXCOLLIDERDESC PhysXColliderDesc)
{
	PxTransform	Transform;
	Transform.p = PxVec3(
		PhysXColliderDesc.vPosition.x,
		PhysXColliderDesc.vPosition.y,
		PhysXColliderDesc.vPosition.z);

	_float4 vQuaternion;
	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYawFromVector(PhysXColliderDesc.vAngles.XMLoad()));
	Transform.q = PxQuat(
		vQuaternion.x,
		vQuaternion.y,
		vQuaternion.z,
		vQuaternion.w);

	switch (PhysXColliderDesc.eType)
	{
	case COLLIDERTYPE::DYNAMIC:
	case COLLIDERTYPE::YFIXED_DYNAMIC:
		Create_DynamicActor(PhysXColliderDesc, Transform, PhysXColliderDesc.pConvecMesh);
		break;
	case COLLIDERTYPE::STATIC:
		Create_StaticActor(PhysXColliderDesc, Transform, PhysXColliderDesc.pConvecMesh);
		break;
	case COLLIDERTYPE::ZONE:

		break;
	default:
		//MSG_BOX("Failed to CreateActor : eCollidertype is wrong");
		break;
	}
}

void CPhysXCollider::Create_DynamicActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case COLLIDERSHAPE::SPHERE:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxSphereGeometry(PhysXColliderDesc.vScale.x), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity);
		break;

	case COLLIDERSHAPE::BOX:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxBoxGeometry(PhysXColliderDesc.vScale.x * 0.5f, PhysXColliderDesc.vScale.y * 0.5f, PhysXColliderDesc.vScale.z * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity);
		break;

	case COLLIDERSHAPE::CYLINDER:
	case COLLIDERSHAPE::CONVECMESH:
	{
		PxConvexMeshGeometry PxGeometry; (PhysXColliderDesc.pConvecMesh);
		PxGeometry.convexMesh = PhysXColliderDesc.pConvecMesh;
		PxMeshScale	vScale;
		vScale.scale.x = PhysXColliderDesc.vScale.x;
		vScale.scale.y = PhysXColliderDesc.vScale.y;
		vScale.scale.z = PhysXColliderDesc.vScale.z;
		PxGeometry.scale = vScale;
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxGeometry, CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity);
		//m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
		//	PxConvexMeshGeometry(PhysXColliderDesc.pConvecMesh), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	}
	case COLLIDERSHAPE::SHAPE_END:
		Call_MsgBox(L"Failed to create DynamicActor : eShape is wrong");
		break;
	default:
		break;
	}

	
}

void CPhysXCollider::Create_StaticActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case COLLIDERSHAPE::SPHERE:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxSphereGeometry(PhysXColliderDesc.vScale.x), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::BOX:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxBoxGeometry(PhysXColliderDesc.vScale.x * 0.5f, PhysXColliderDesc.vScale.y * 0.5f, PhysXColliderDesc.vScale.z * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::CYLINDER:
	case COLLIDERSHAPE::CONVECMESH:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxConvexMeshGeometry(PhysXColliderDesc.pConvecMesh), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::TRIANGLEMESH:
		break;

	case COLLIDERSHAPE::SHAPE_END:
		Call_MsgBox(L"Failed to create StaticActor : eShape is wrong");
		break;
	default:
		break;
	}
}

HRESULT CPhysXCollider::SetUp_StaticMeshActor(CMesh* pMesh, CTransform* pWorldTransform)
{
	_uint iNumVertices = 0;
	_float3* pVerticesPos = nullptr;
	CMesh_Terrain* pTerrain = dynamic_cast<CMesh_Terrain*>(pMesh);
	if (pTerrain)
	{
		iNumVertices = pTerrain->Get_PhysXNumVertices();
		pVerticesPos = pTerrain->Get_PhysXVerticesPos();
	}
	else
	{
		iNumVertices = pMesh->Get_NumVertices();
		pVerticesPos = pMesh->Get_VerticesPos();
	}
	
	CPhysX_Manager::Get_Instance()->Create_TriangleMesh(
		pVerticesPos,
		iNumVertices,
		pMesh->Get_NumPrimitive(),
		&m_ColliderDesc.pTriangleMesh
	);

	if (!m_ColliderDesc.pTriangleMesh)
		return E_FAIL;

	_float4 vQuat = pWorldTransform->Get_Quaternion();
	_float4 vPos = pWorldTransform->Get_World(WORLD_POS);
	_float4 vScale = pWorldTransform->Get_Scale();
	PxTransform	tTransform;
	tTransform.p = PxVec3(vPos.x, vPos.y, vPos.z);
	tTransform.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

	PxMeshScale	pXScale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));
	
	m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(tTransform,
		PxTriangleMeshGeometry(m_ColliderDesc.pTriangleMesh, pXScale), CPhysX_Manager::SCENE_CURRENT);

	m_ColliderDesc.eType = COLLIDERTYPE::STATIC;

	if (!m_pRigidStatic)
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysXCollider::SetUp_StaticConvexActor(CMesh* pMesh, CTransform* pWorldTransform)
{
	_uint iNumVertices = 0;
	_float3* pVerticesPos = nullptr;
	CMesh_Terrain* pTerrain = dynamic_cast<CMesh_Terrain*>(pMesh);

	if (pTerrain)
		return E_FAIL;

	FACEINDICES32* pIndices = pMesh->Get_Indices();
	_uint iNumPrimitive = pMesh->Get_NumPrimitive();

	iNumVertices = pMesh->Get_NumVertices();
	pVerticesPos = pMesh->Get_VerticesPos();
	
	CPhysX_Manager::Get_Instance()->Create_ConvexMesh(
		pVerticesPos,
		iNumVertices,
		pIndices,
		iNumPrimitive,
		&m_ColliderDesc.pConvecMesh);

	if (!m_ColliderDesc.pConvecMesh)
	{
		return E_FAIL;
	}

	_float4 vQuat = pWorldTransform->Get_Quaternion();
	_float4 vPos = pWorldTransform->Get_World(WORLD_POS);
	_float4 vScale = pWorldTransform->Get_Scale();
	PxTransform	tTransform;
	tTransform.p = PxVec3(vPos.x, vPos.y, vPos.z);
	tTransform.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

	PxMeshScale	pXScale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

	m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(tTransform,
		PxConvexMeshGeometry(m_ColliderDesc.pConvecMesh, pXScale), CPhysX_Manager::SCENE_CURRENT);

	m_ColliderDesc.eType = COLLIDERTYPE::STATIC;

	if (!m_pRigidStatic)
		return E_FAIL;

	return S_OK;
}

