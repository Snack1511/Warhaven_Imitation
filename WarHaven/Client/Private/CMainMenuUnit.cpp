#include "stdafx.h"
#include "CMainMenuUnit.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CMainMenuUnit::CMainMenuUnit()
{
}

CMainMenuUnit::~CMainMenuUnit()
{
}

CMainMenuUnit* CMainMenuUnit::Create(const UNIT_MODEL_DATA& tUnitModelData)
{
	CMainMenuUnit* pInstance = new CMainMenuUnit;

	if (FAILED(pInstance->SetUp_Model(tUnitModelData)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CMainMenuUnit");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CMainMenuUnit");
		return nullptr;
	}

	return pInstance;
}

void CMainMenuUnit::On_Die()
{

}

void CMainMenuUnit::SetUp_Colliders(_bool bPlayer)
{
	


}

void	CMainMenuUnit::SetUp_HitStates(UNIT_TYPE eUnitType)
{

	
}


HRESULT CMainMenuUnit::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);



	//추가적으로 Animator 만들어야댐.

	//  attack, hit, etc, parkour, L_Base, R_Base 를 기본적으로 fbx에 추가합니다.
	//  기본적으로 L_Base 가 없는 Unit Mesh 가 있으면 L_Base 를 제거하고 Add_Animation 을 수행하자.

	//0. R_Base
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/warrior/SKEL_Warrior_Base_R.fbx");
	if (!pAnimator)
		return E_FAIL;

	//1. L_Base
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Base_L.fbx");

	//2. Attack
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Attack.fbx");

	//3. hit
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_Hit.fbx");

	//4. ETC
	pAnimator->Add_Animations(L"../bin/resources/animations/warrior/SKEL_Warrior_ETC.fbx");


	Add_Component(pAnimator);


	return S_OK;
}

HRESULT CMainMenuUnit::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CMainMenuUnit::Start()
{
	CGameObject::Start();

	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	m_pModelCom->Set_ShaderPass(MODEL_PART_FACE, VTXANIM_PASS_FACE);




	/* Warrior Idle */
	m_pAnimator->Set_CurAnimIndex(ANIM_BASE_R, 11, ANIM_DIVIDE::eDEFAULT);
	m_pAnimator->Set_InterpolationTime(ANIM_BASE_R, 11, 0.1f);
	m_pAnimator->Set_AnimSpeed(ANIM_BASE_R, 11, 1.f);

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vMyPos = vCamPos + GAMEINSTANCE->Get_CurCamLook() * 0.8f;
	vMyPos.x += 0.2f;
	vMyPos.y -= 1.4f;
	m_pTransform->Set_World(WORLD_POS, vMyPos);
	m_pTransform->Set_Look(GAMEINSTANCE->Get_CurCamLook() * -1.f);

	return S_OK;
}

void CMainMenuUnit::OnEnable()
{
	CGameObject::OnEnable();
}

void CMainMenuUnit::OnDisable()
{
	CGameObject::OnDisable();
}

void CMainMenuUnit::My_Tick()
{
	
}

void CMainMenuUnit::My_LateTick()
{
}
