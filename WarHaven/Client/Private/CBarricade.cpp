#include "stdafx.h"
#include "CBarricade.h"

#include "UsefulHeaders.h"

#include "CColorController.h"

#include "Model.h"
#include "CAnimator.h"

#include "CBoneCollider.h"

#include "CUnit_WarHammer.h"

CBarricade::CBarricade()
{
}

CBarricade::~CBarricade()
{
}

CBarricade* CBarricade::Create(CUnit_WarHammer* pOwner)
{
	CBarricade* pInstance = new CBarricade;


	if (FAILED(pInstance->__super::SetUp_Model(L"../Bin/Resources/Meshes/Map/Environments/Module/Fence/SM_Module_Fence_Wood08a.dat")))
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

	// Create_GameObject 로 객체의 포인터, 객체의 추가할 그룹 Enum
	// 바리게이트 생성
	// 생성뒤 Disable -> 원본객체가 보이기 않기 위해서
	
	// 필요할 때 마다 포인터 가지고 와서 클론하고 클론한 객체를 Create::() 수행
	// Create -> 원본객체 생성 Delete -> 원본객체 삭제
	// Create_Object

	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.
	
	

	//CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_R.fbx");
	//if (!pAnimator)
	//	return E_FAIL;

	//
	////1. L_Base
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Base_L.fbx");

	////2. Attack
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_Attack.fbx");

	////3. hit
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_HIT.fbx");

	////4. ETCK
	//pAnimator->Add_Animations(L"../bin/resources/animations/WarHammer/SKEL_Engineer_ETC.fbx");


	//Add_Component(pAnimator);

	///* PhysX Collider */

	//CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	//if (!pCController)
	//	return E_FAIL;

	//Add_Component(pCController);

	//CBoneCollider::BONECOLLIDERDESC tDesc;
	//// 칼 길이

	//tDesc.fHeight = 0.9f;
	//// 칼 두께
	//tDesc.fRadius = 0.1f;
	//// 칼 붙일 뼈
	//tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("0B_R_WP1");

	////칼 오프셋(로컬)
	//tDesc.vOffset = _float4(0.f, 0.f, -100.f);

	return S_OK;
}

HRESULT CBarricade::Initialize()
{
	__super::Initialize();

	return S_OK;
}

HRESULT CBarricade::Start()
{
	 __super::Start();
	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);

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

	// m_pOwner->Destroy_Barricade(this);
}
