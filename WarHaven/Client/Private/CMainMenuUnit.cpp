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

CMainMenuUnit* CMainMenuUnit::Create(const UNIT_MODEL_DATA& tUnitModelData, CLASS_TYPE eClassType)
{
	CMainMenuUnit* pInstance = new CMainMenuUnit;

	pInstance->m_eClassType = eClassType;

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
	
	wstring wstrAnimPath;

	switch (m_eClassType)
	{
	case Client::WARRIOR:
		wstrAnimPath = L"../bin/resources/animations/warrior/SKEL_Warrior";
		break;
	case Client::SPEAR:
		return E_FAIL;
		break;
	case Client::ARCHER:
		wstrAnimPath = L"../bin/resources/animations/archer/SKEL_Archer";
		break;
	case Client::PALADIN:
		wstrAnimPath = L"../bin/resources/animations/paladin/SKEL_Paladin";
		break;
	case Client::PRIEST:
		wstrAnimPath = L"../bin/resources/animations/priest/SKEL_Priest";
		break;
	case Client::ENGINEER:
		wstrAnimPath = L"../bin/resources/animations/WarHammer/SKEL_Engineer";
		break;
	case Client::FIONA:
		wstrAnimPath = L"../bin/resources/animations/Valkyrie/SKEL_Fiona";
		break;
	case Client::QANDA:
		wstrAnimPath = L"../bin/resources/animations/qanda/SKEL_Qanda";
		break;
	case Client::HOEDT:
		return E_FAIL;
		break;
	case Client::LANCER:
		wstrAnimPath = L"../bin/resources/animations/lancer/SKEL_Lancer";
		break;
	case Client::CLASS_END:
		return E_FAIL;
		break;
	default:
		break;
	}

	wstring wstrTemp = wstrAnimPath + L"_Base_R.fbx";
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, wstrTemp);
	if (!pAnimator)
		return E_FAIL;

	//1. L_Base
	if (m_eClassType == PRIEST)
	{
		pAnimator->Add_Animations(L"../bin/resources/animations/Priest/A_LobbyIdle_Priest_01.fbx");

	}
	else if (m_eClassType == QANDA)
	{
		pAnimator->Add_Animations(L"../bin/resources/animations/qanda/SKEL_Qanda_Attack.fbx");

	}

	else
	{
		wstrTemp = wstrAnimPath + L"_Base_L.fbx";
		pAnimator->Add_Animations(wstrTemp);
	}
	

	//2. Attack
	wstrTemp = wstrAnimPath + L"_Attack.fbx";
	pAnimator->Add_Animations(wstrTemp);

	//3. hit
	wstrTemp = wstrAnimPath + L"_Hit.fbx";
	pAnimator->Add_Animations(wstrTemp);

	//4. ETC
	wstrTemp = wstrAnimPath + L"_ETC.fbx";
	pAnimator->Add_Animations(wstrTemp);


	Add_Component(pAnimator);


	return S_OK;
}

HRESULT CMainMenuUnit::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);

	m_pModelCom->Set_ShaderFlag(SH_LIGHT_BLOOM);

	//Set_ShaderNoSpec(L"SK_Warrior_Helmet_Rabbit_50");

	m_tUnitStatus.eWeapon = WEAPON_LONGSWORD;

	return S_OK;
}

HRESULT CMainMenuUnit::Start()
{
	CGameObject::Start();

	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	m_pModelCom->Set_ShaderPass(MODEL_PART_FACE, VTXANIM_PASS_FACE);

	ANIM_TYPE	eAnimType = ANIM_BASE_R;
	_uint		iAnimIndex = 11;

	switch (m_eClassType)
	{
	case Client::WARRIOR:
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		break;
	case Client::PALADIN:
		break;
	case Client::PRIEST:
		break;
	case Client::ENGINEER:
		break;
	case Client::FIONA:
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}

	m_pAnimator->Set_CurAnimIndex(eAnimType, iAnimIndex, ANIM_DIVIDE::eDEFAULT);
	m_pAnimator->Set_InterpolationTime(eAnimType, iAnimIndex, 0.1f);
	m_pAnimator->Set_AnimSpeed(eAnimType, iAnimIndex, 1.f);

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vMyPos = vCamPos + GAMEINSTANCE->Get_CurCamLook() * 0.8f;
	//_float4 vMyPos = vCamPos + GAMEINSTANCE->Get_CurCamLook() * 3.f;
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
