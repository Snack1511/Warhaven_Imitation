#include "stdafx.h"
#include "CBarricade.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CUnit_WarHammer.h"

#include "CCollider_Sphere.h"

CBarricade::CBarricade()
{
}

CBarricade::~CBarricade()
{
}

CBarricade* CBarricade::Create(CUnit_WarHammer* pOwner)
{
	CBarricade* pInstance = new CBarricade;


	if (FAILED(pInstance->__super::SetUp_Model(L"../Bin/Resources/Meshes/Map/Environments/Module/Fence/SM_Module_Fence_Wood08a.fbx")))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CBarricade");
		return nullptr;
	}

	//if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	//{
	//	SAFE_DELETE(pInstance);
	//	Call_MsgBox(L"Failed to SetUp_Model : CBarricade");
	//	return nullptr;
	//}

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CBarricade");
		return nullptr;
	}


	

	return pInstance;
}

HRESULT CBarricade::Initialize_Prototype(CUnit_WarHammer* pOwner)
{
	__super::Initialize_Prototype();


	m_pOwner = pOwner;


	//CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_R.fbx");
	//if (!pAnimator)
	//	return E_FAIL;


	//CPhysXCharacter::PHYSXCCTDESC tDesc;
	//tDesc.fRadius = 0.5f;
	//tDesc.fHeight = 1.0f;
	//CPhysXCharacter* pPhysXCharacter = CPhysXCharacter::Create(CP_BEFORE_TRANSFORM, tDesc);
	//Add_Component(pPhysXCharacter);

	/////* PhysX Collider */

	//CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	//if (!pCController)
	//	return E_FAIL;

	//Add_Component(pCController);

	//Add_Component(CPhysics::Create(0));


	return S_OK;
}

HRESULT CBarricade::Initialize()
{
	__super::Initialize();


	//m_pStructCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, 0.5f, 0, _float4(0.f, 0.f, 0.f, 0.f), DEFAULT_TRANS_MATRIX);

	//Add_Component(m_pStructCollider);


	//m_pPhysics = GET_COMPONENT(CPhysics);

	//m_pPhysXCharacter = GET_COMPONENT(CPhysXCharacter);
	//if (!m_pPhysXCharacter)
	//	return E_FAIL;

	return S_OK;
}

HRESULT CBarricade::Start()
{
	 __super::Start();

	return S_OK;
}

void CBarricade::OnEnable()
{
	__super::OnEnable();

	// m_pOwner->Creaete_Barricade();

}

void CBarricade::OnDisable()
{
	__super::OnDisable();

	// m_pOwner->Disable_Barricade(this);
}

void CBarricade::My_Tick()
{
	// 만약 스텟이 0이면 Destroy 하도록 하자.
	if (KEY(N, TAP))
		m_pOwner->Disable_Barricade(this);

	__super::My_Tick();
}

void CBarricade::My_LateTick()
{
	

	__super::My_LateTick();
}
