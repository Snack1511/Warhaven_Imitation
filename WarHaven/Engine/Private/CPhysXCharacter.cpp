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
	
		//���� ���� ����
		hit.worldPos.y;
		//�� �� ��ġ�� y�� ����ϸ� ���̶� �����ű�
		//�������� �� �� ��ġ�� y���� ���ٸ� �ƴѰ���

		_float fContactY = hit.worldPos.y;
		_float	fMyFootY = hit.controller->getFootPosition().y;

		//�������� �� ��ġ���� �Ʒ��� (���� ����)
		if (fMyFootY + 0.1f > fContactY)
		{
			m_bColGround = true;

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


	_float4 vMove = vDir * fSpeed * fDT(0);

	vMove += vFall;

	m_bColGround = false;
	m_pPxController->move(PxVec3(vMove.x, vMove.y, vMove.z), 0.f, fDT(0), m_tControllerFilters);


	//���� move���� m_bColGround�� true�� �ȴ�ٸ�
	//���� ������Ȳ
	if (m_bColGround)
	{
		//���� ����
		m_pPhysicsCom->Get_Physics().bAir = false;
	}
	else
	{
		//���� ������ �������µ� air�� false��
		if (!m_pPhysicsCom->Get_Physics().bAir)
		{
			//������
			m_pPhysicsCom->Set_Jump(0.f);
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
