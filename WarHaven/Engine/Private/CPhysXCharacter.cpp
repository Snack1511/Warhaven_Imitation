#include "CPhysXCharacter.h"

#include "GameInstance.h"
#include "PhysX_Manager.h"

#include "GameObject.h"
#include "Transform.h"

#include "Physics.h"
#include "CUtility_PhysX.h"


CPhysXCharacter::CPhysXCharacter(_uint iGroupID)
	: CPhysXCollider(iGroupID)
{
}

CPhysXCharacter::~CPhysXCharacter()
{
	Release();
}

CPhysXCharacter* CPhysXCharacter::Create(_uint iGroupID, const PHYSXCCTDESC& tPhysXCCTDESC)
{
	CPhysXCharacter* pInstance = new CPhysXCharacter(iGroupID);

	pInstance->m_tPhysXCCTDESC = tPhysXCCTDESC;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CPhysXCharacter");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}



void CPhysXCharacter::Set_Position(_float4 vPos)
{
	m_pPxController->setFootPosition(CUtility_PhysX::To_PxExtendedVec3(vPos));
}

void CPhysXCharacter::onShapeHit(const PxControllerShapeHit& hit)
{
	if (PxActorType::eRIGID_DYNAMIC == hit.actor->getType())
	{
		//�о����
		_float4 vOtherPos = CUtility_PhysX::To_Vector(hit.actor->getGlobalPose().p);
		_float4 vHitPos = CUtility_PhysX::To_Vector(hit.worldPos);

		_float4 vDir = vOtherPos - vHitPos;
		vDir.y = 0.f;
		vDir.Normalize();
		vDir *= (_float)hit.length;

		//vDir *= static_cast<PxRigidDynamic*>(hit.actor)->getInvMass();

		static_cast<PxRigidDynamic*>(hit.actor)->addForce(CUtility_PhysX::To_PxVec3(vDir), PxForceMode::eFORCE);
	}

	if (m_pPxController->getFootPosition().y + 0.1f > hit.worldPos.y)
	{
		//���� �� �븻�̶�
		//010 �� ����

		_float4 vNormal = CUtility_PhysX::To_Vector(hit.worldNormal);
		_float4 vUp = _float4(0.f, 1.f, 0.f, 0.f);

		vNormal.Normalize();

		_float fCosTheta = vNormal.Dot(vUp);
		m_fCurCosTheta = fCosTheta;
	}


}


HRESULT CPhysXCharacter::Initialize_Prototype()
{

	CPhysX_Manager::Get_Instance()->Create_CapsuleController(m_tPhysXCCTDESC.fRadius, m_tPhysXCCTDESC.fHeight, &m_pPxController, this);

	if (!m_pPxController)
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysXCharacter::Initialize()
{
	return S_OK;
}

void CPhysXCharacter::Start()
{
	__super::Start();
	m_pPhysicsCom = GET_COMPONENT_FROM(m_pOwner, CPhysics);
}

void CPhysXCharacter::Tick()
{
	_float fSpeed = m_pPhysicsCom->Calculate_Speed();
	_float fFallPower = m_pPhysicsCom->Calculate_FreeFall();

	if (fFallPower == 0.f)
		fFallPower = -0.1f;

	_float4 vNewPos, vOutPlanePos, vFinalPos, vDir;

	vDir = m_pPhysicsCom->Get_Physics().vDir;

	_float4 vFall = _float4(0.f, 1.f, 0.f, 0.f) * fFallPower;


	_float4 vMove = vDir * fSpeed * fDT(0) * m_fCurCosTheta;

	vMove += vFall;

	m_bColGround = false;

	//Į�� �ڲ� ����
	//Move�� �ƴ϶� set posi�� ���� �ȴ�
	//

	PxControllerCollisionFlags eFlag = m_pPxController->move(PxVec3(vMove.x, vMove.y, vMove.z), 0.f, fDT(0), m_tControllerFilters);

	if (eFlag & PxControllerCollisionFlag::eCOLLISION_DOWN)
		m_bColGround = true;

	
	
	//���� move���� m_bColGround�� true�� �ȴ�ٸ�
	//���� ������Ȳ
	if (m_bColGround)
	{
		//���� ����
		m_pPhysicsCom->Get_Physics().bAir = false;
		m_fTimeAcc = 0.f;
	}
	else
	{
		//���� ������ �������µ� air�� false��
		//0.1�� ���� �ð��� ���
		m_fCurCosTheta = 1.f;

		m_fTimeAcc += fDT(0);
		//if (m_fTimeAcc >= m_fColTime)
		{
			if (!m_pPhysicsCom->Get_Physics().bAir)
			{
				//������
				m_pPhysicsCom->Set_Jump(0.f);
				m_fTimeAcc = 0.f;
			}
		}

		

	}
	


}

void CPhysXCharacter::Late_Tick()
{
	PxExtendedVec3 vec3 = m_pPxController->getFootPosition();
	m_pOwner->Get_Transform()->Set_World(WORLD_POS, _float4(vec3.x, vec3.y+0.f, vec3.z));
	Update_Colliders();
}

void CPhysXCharacter::Release()
{
	Safe_release(m_pPxController);
}

void CPhysXCharacter::OnEnable()
{
	__super::OnEnable();
	
}

void CPhysXCharacter::OnDisable()
{
	__super::OnEnable();
}

void CPhysXCharacter::Update_Colliders()
{
	//�浹ü�� ����ġ�� ������ ���ؼ� ����
	/*for (auto& tCollider : m_ColliderList)
	{
		tCollider.pController->setPosition();
		tCollider.pController->setUpDirection();
	}*/
}
