#include "..\Public\PhysXCollider.h"
#include "PhysX_Manager.h"
#include "Transform.h"

CPhyXCollider::CPhyXCollider(_uint iGroupID)
	: CComponent(iGroupID)
{
}

CPhyXCollider::CPhyXCollider(const CPhyXCollider & rhs)
	: CComponent(rhs)
{
	//복사생성자만들기
}

CPhyXCollider::~CPhyXCollider()
{
}

CPhyXCollider* CPhyXCollider::Create(_uint iGroupID, const PHYSXCOLLIDERDESC& tPhysXColliderDesc)
{

	CPhyXCollider* pInstance = new CPhyXCollider(iGroupID);

	pInstance->m_ColliderDesc = tPhysXColliderDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CModel");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

HRESULT CPhyXCollider::Initialize_Prototype()
{

	CreatePhysActor(m_ColliderDesc);

	return S_OK;
}



HRESULT CPhyXCollider::Initialize()
{
	return S_OK;
}

void CPhyXCollider::Tick()
{
	//여기서 transform을 피직스에넣고 피직스 연산후
}

void CPhyXCollider::Late_Tick()
{
	//여기서 갱신시키기
}

void CPhyXCollider::Release()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->release();
	if (m_pRigidStatic)
		m_pRigidStatic->release();
}
void CPhyXCollider::Synchronize_Transform(CTransform * pTransform)
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

void CPhyXCollider::Synchronize_Transform_Position(CTransform * pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vPos = { Transform.p.x, Transform.p.y, Transform.p.z };
	pTransform->Set_World(WORLD_POS, vPos);
}

void CPhyXCollider::Synchronize_Transform_Rotation(CTransform * pTransform)
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	_vector vQuaternion = { Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w };
	pTransform->MatrixRotationQuaternion(vQuaternion);
}

void CPhyXCollider::Synchronize_Collider(CTransform * pTransform)
{
	_vector vPos = pTransform->Get_World(WORLD_POS).XMLoad();
	_vector vQuaternion = pTransform->Get_Quaternion().XMLoad();
	Set_Position(vPos, vQuaternion);
}

void CPhyXCollider::PutToSleep()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->putToSleep();
}

void CPhyXCollider::WakeUp()
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->wakeUp();
}

void CPhyXCollider::Attach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->attachShape(*pShape);
}

void CPhyXCollider::Detach_Shape(PxShape * pShape)
{
	if (m_pRigidDynamic)
		m_pRigidDynamic->detachShape(*pShape);
}

HRESULT CPhyXCollider::Add_Force(_vector _vForce)
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

HRESULT CPhyXCollider::Clear_Force()
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

HRESULT CPhyXCollider::Clear_Velocity()
{
	m_pRigidDynamic->setLinearVelocity(PxVec3(0.f));
	return S_OK;
}

HRESULT CPhyXCollider::Add_LinearVelocityResistance(_vector vResistanceRate)
{
	PxVec3	vResistRate = {XMVectorGetX(vResistanceRate), XMVectorGetY(vResistanceRate), XMVectorGetZ(vResistanceRate) };
	PxVec3	vVelocity = m_pRigidDynamic->getLinearVelocity();
	vVelocity.x *= vResistRate.x;
	vVelocity.y *= vResistRate.y;
	vVelocity.z *= vResistRate.z;

	m_pRigidDynamic->setLinearVelocity(vVelocity);

	return S_OK;
}

_vector CPhyXCollider::Get_Position()
{
	PxTransform	Transform;
	if (m_pRigidDynamic)
		Transform = m_pRigidDynamic->getGlobalPose();

	if (m_pRigidStatic)
		Transform = m_pRigidStatic->getGlobalPose();

	return XMVectorSet(Transform.p.x, Transform.p.y, Transform.p.z, 1.f);
}

_vector CPhyXCollider::Get_Velocity()
{
	if (m_pRigidDynamic)
	{
		PxVec3 Velocity = m_pRigidDynamic->getLinearVelocity();
		return XMVectorSet(Velocity.x, Velocity.y, Velocity.z, 0.f);
	}

	return _vector{ 0.f, 0.f, 0.f, 0.f };
}

_float CPhyXCollider::Get_Mess()
{
	assert(m_pRigidDynamic);

	PxReal fMess = m_pRigidDynamic->getMass();
	return fMess;
}

_vector CPhyXCollider::Get_AngularVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getAngularVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

_vector CPhyXCollider::Get_LinearVelocity()
{
	assert(m_pRigidDynamic);

	PxVec3 vVelocity = m_pRigidDynamic->getLinearVelocity();
	return XMVectorSet(vVelocity.x, vVelocity.y, vVelocity.z, 0.f);
}

HRESULT CPhyXCollider::Set_Position(_vector _vPos, _vector _vQuaternion)
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

HRESULT CPhyXCollider::Set_Position(_vector _vPos)
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

void CPhyXCollider::Set_Scale(_vector vScale)
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

void CPhyXCollider::Set_ActorFlag(PxActorFlag::Enum eFlag, _bool bState)
{
	if (m_pRigidDynamic)
	{
		m_pRigidDynamic->setActorFlag(eFlag, bState);
	}
}


void CPhyXCollider::Delete_Collider()
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

void CPhyXCollider::Create_Collider()
{
	if (!m_pRigidDynamic && !m_pRigidStatic)
	{
		CreatePhysActor(m_ColliderDesc);
	}
}

void CPhyXCollider::CreatePhysActor(PHYSXCOLLIDERDESC PhysXColliderDesc)
{
	PxTransform	Transform;
	Transform.p = PxVec3(
		XMVectorGetX(PhysXColliderDesc.vPosition),
		XMVectorGetY(PhysXColliderDesc.vPosition),
		XMVectorGetZ(PhysXColliderDesc.vPosition));

	_float4 vQuaternion;
	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYawFromVector(PhysXColliderDesc.vAngles));
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

void CPhyXCollider::Create_DynamicActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case COLLIDERSHAPE::SPHERE:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxSphereGeometry(XMVectorGetX(PhysXColliderDesc.vScale)), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::BOX:
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxBoxGeometry(XMVectorGetX(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetY(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetZ(PhysXColliderDesc.vScale) * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::CYLINDER:
	case COLLIDERSHAPE::CONVECMESH:
	{
		PxConvexMeshGeometry PxGeometry; (PhysXColliderDesc.pConvecMesh);
		PxGeometry.convexMesh = PhysXColliderDesc.pConvecMesh;
		PxMeshScale	vScale;
		vScale.scale.x = XMVectorGetX(PhysXColliderDesc.vScale);
		vScale.scale.y = XMVectorGetY(PhysXColliderDesc.vScale);
		vScale.scale.z = XMVectorGetZ(PhysXColliderDesc.vScale);
		PxGeometry.scale = vScale;
		m_pRigidDynamic = CPhysX_Manager::Get_Instance()->Create_DynamicActor(Transform,
			PxGeometry, CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.fDensity, PxVec3(0), PhysXColliderDesc.pMaterial);
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

void CPhyXCollider::Create_StaticActor(PHYSXCOLLIDERDESC PhysXColliderDesc, PxTransform Transform, PxConvexMesh* pConvexMesh)
{
	switch (PhysXColliderDesc.eShape)
	{
	case COLLIDERSHAPE::SPHERE:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxSphereGeometry(XMVectorGetX(PhysXColliderDesc.vScale)), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::BOX:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxBoxGeometry(XMVectorGetX(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetY(PhysXColliderDesc.vScale) * 0.5f, XMVectorGetZ(PhysXColliderDesc.vScale) * 0.5f),
			CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::CYLINDER:
	case COLLIDERSHAPE::CONVECMESH:
		m_pRigidStatic = CPhysX_Manager::Get_Instance()->Create_StaticActor(Transform,
			PxConvexMeshGeometry(PhysXColliderDesc.pConvecMesh), CPhysX_Manager::SCENE_CURRENT, PhysXColliderDesc.pMaterial);
		break;
	case COLLIDERSHAPE::SHAPE_END:
		Call_MsgBox(L"Failed to create StaticActor : eShape is wrong");
		break;
	default:
		break;
	}
}

