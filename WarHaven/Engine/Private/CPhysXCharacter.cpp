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
	
		//닿은 곳이 만약
		hit.worldPos.y;
		//내 발 위치의 y랑 비슷하면 땅이랑 닿은거구
		//닿은곳이 머 발 위치의 y보다 높다면 아닌거제

		_float fContactY = hit.worldPos.y;
		_float	fMyFootY = hit.controller->getFootPosition().y;

		//닿은곳이 발 위치보다 아래면 (땅에 착지)
		if (fMyFootY + 0.1f > fContactY)
		{
			m_bAirTemp = false;

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

	m_bAirTemp = true;
	m_pPxController->move(PxVec3(vMove.x, vMove.y, vMove.z), 0.f, fDT(0), m_tControllerFilters);


	//m_pPxController->setUpDirection(PxVec3(1.f, 1.f, 0.f));
	//원래 움직였어야했을 y보다 적게 움직이믄 air는 false이고
	// 
	//move 호출되고 나면 contact호출댐 move에서 air false 됨

	//air 는 항상 true로 바꿔나

	//만약 move에서 air false가 안댔다면
	//

	//땅에서 떨어진 상황
<<<<<<< HEAD
	if (!m_bAir)
	{
		//땅에 붙음
		m_pPhysicsCom->Get_Physics().bAir = false;
	}
	else
	{
		//땅에서 떨어져있는데 air가 false면 점프시키기
=======



	//공중에 있다는 판정
	if (m_bAirTemp)
	{
		//만약 땅에있었는데 공중에 있따는 판정이 나오면
>>>>>>> 221113_TH
		if (!m_pPhysicsCom->Get_Physics().bAir)
		{
			m_pPhysicsCom->Set_Jump(0.f);
		}



	}
	else // 땅에 닿음
	{
		//땅에 닿았으면 air false로
		m_pPhysicsCom->Get_Physics().bAir = false;


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
	//충돌체들 뼈위치에 오프셋 더해서 부착
	/*for (auto& tCollider : m_ColliderList)
	{
		tCollider.pController->setPosition();
		tCollider.pController->setUpDirection();
	}*/
}
