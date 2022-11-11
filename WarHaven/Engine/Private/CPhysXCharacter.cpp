#include "CPhysXCharacter.h"

#include "GameInstance.h"
#include "PhysX_Manager.h"

CPhysXCharacter::CPhysXCharacter(_uint iGroupID)
	: CPhysXCollider(iGroupID)
{
}

CPhysXCharacter::~CPhysXCharacter()
{
}

CPhysXCharacter* CPhysXCharacter::Create(_uint iGroupID, const PHYSXCCDDESC& tPhysXCCDDesc)
{
	CPhysXCharacter* pInstance = new CPhysXCharacter(iGroupID);

	pInstance->m_tPhysXCCDDesc = tPhysXCCDDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CPhysXCharacter");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CPhysXCharacter::Initialize_Prototype()
{
	CPhysX_Manager::Get_Instance()->Create_CapsuleController(m_tPhysXCCDDesc.fRadius, m_tPhysXCCDDesc.fHeight, &m_pPxController);

	if (!m_pPxController)
		return E_FAIL;

	return S_OK;
}

HRESULT CPhysXCharacter::Initialize()
{
	return S_OK;
}

void CPhysXCharacter::Tick()
{
}

void CPhysXCharacter::Late_Tick()
{
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