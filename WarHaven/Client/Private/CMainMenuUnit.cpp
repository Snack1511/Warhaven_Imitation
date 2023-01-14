#include "stdafx.h"
#include "CMainMenuUnit.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CColorController.h"

#include "Camera.h"

#include "CBoneCollider.h"
#include "HIerarchyNode.h"
#include "CUnit_Lancer_Head.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"
#include "CAnimWeapon.h"

#include "CRectEffects.h"
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

		m_pMyLancerHead = CUnit_Lancer_Head::Create(L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0000_Face_A00_20.fbx",
			nullptr, this);

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

	if (m_pMyLancerHead)
	{
		CREATE_GAMEOBJECT(m_pMyLancerHead, GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pMyLancerHead);
	}


	m_pModelCom->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	m_pModelCom->Set_ShaderPass(MODEL_PART_FACE, VTXANIM_PASS_FACE);
	
	m_iAnimIndex = 11;
	m_fAnimSpeed = 1.f;
	m_eBaseType = ANIM_BASE_R;

	_float		finterPoleTime = 0.1f;

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vMyPos = vCamPos + GAMEINSTANCE->Get_CurCamLook() * 0.8f;

	vMyPos.x += 0.2f;
	vMyPos.y -= 1.4f;

	m_EyeFlare.clear();

	switch (m_eClassType)
	{
	case Client::WARRIOR:
		if (m_EyeFlare.empty())
			m_EyeFlare = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Warrior_Eye", this, ZERO_VECTOR);
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		m_iAnimIndex = 4;

		m_pAnimWeapon = CAnimWeapon::Create(L"../bin/resources/meshes/weapons/longbow/SK_WP_LongBow0005_A00_30.fbx",
			L"../bin/resources/meshes/weapons/longbow/LongBow_Anim2.fbx", this, "0B_L_WP1", 180.f, 180.f, 90.f);

		if (!m_pAnimWeapon)
			return E_FAIL;

		m_pAnimWeapon->Initialize();
		CREATE_GAMEOBJECT(m_pAnimWeapon, GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pAnimWeapon);

		break;
	case Client::PALADIN:
		m_pModelCom->Set_TransformMatrix(MODEL_PART_WEAPON_L, XMMatrixRotationZ(XMConvertToRadians(270.f)));
		vMyPos.z += 0.35f;
		m_iAnimIndex = 3;
		break;
	case Client::PRIEST:
		vMyPos.y += 0.2f;
		vMyPos.z -= 0.1f;
		m_iAnimIndex = 1;
		break;
	case Client::ENGINEER:
		m_iAnimIndex = 5;
		break;
	case Client::FIONA:
		m_eBaseType = ANIM_BASE_L;
		vMyPos.y += 0.15f;
		m_iAnimIndex = 3;
		break;
	case Client::QANDA:
		vMyPos.y += 0.25f;
		vMyPos.z -= 0.1f;
		m_iAnimIndex = 31;
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		vMyPos.y -= 1.f;
		//vMyPos.z += 0.25f;
		m_iAnimIndex = 10;
		break;
	case Client::CLASS_END:
		break;
	default:
		break;
	}

	m_pAnimator->Set_CurAnimIndex(m_eBaseType, m_iAnimIndex, ANIM_DIVIDE::eDEFAULT);
	m_pAnimator->Set_InterpolationTime(m_eBaseType, m_iAnimIndex, finterPoleTime);
	m_pAnimator->Set_AnimSpeed(m_eBaseType, m_iAnimIndex, m_fAnimSpeed);

	
	//_float4 vMyPos = vCamPos + GAMEINSTANCE->Get_CurCamLook() * 3.f;
	
	m_pTransform->Set_World(WORLD_POS, vMyPos);
	m_pTransform->Set_Look(GAMEINSTANCE->Get_CurCamLook() * -1.f);

	return S_OK;
}

void CMainMenuUnit::OnEnable()
{
	CGameObject::OnEnable();

	if(m_pMyLancerHead)
		ENABLE_GAMEOBJECT(m_pMyLancerHead);

	if (m_pAnimWeapon)
		ENABLE_GAMEOBJECT(m_pAnimWeapon);

	switch (m_eClassType)
	{
	case Client::WARRIOR:
		if (m_EyeFlare.empty())
			m_EyeFlare = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Warrior_Eye", this, ZERO_VECTOR);
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


}

void CMainMenuUnit::OnDisable()
{
	CGameObject::OnDisable();

	if (m_pMyLancerHead)
		DISABLE_GAMEOBJECT(m_pMyLancerHead);

	if (m_pAnimWeapon)
		DISABLE_GAMEOBJECT(m_pAnimWeapon);

	if (!m_EyeFlare.empty())
	{
		for (auto& elem : m_EyeFlare)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_EyeFlare.clear();
	}
}

void CMainMenuUnit::My_Tick()
{
	//_uint		iFrame = 0;

	//if (m_eClassType == PALADIN)
	//{
	//	if (m_pAnimator->Is_CurAnimFinished() && m_pAnimator->Get_CurAnimIndex() != 3)
	//	{
	//		
	//		ANIM_TYPE	eAnimType = ANIM_ATTACK;

	//		if (m_pAnimator->Get_CurAnimIndex() == 18)
	//			iFrame = 19;

	//		//else if (m_pAnimator->Get_CurAnimIndex() == 21)
	//		//	iFrame = 19;

	//		else if (m_pAnimator->Get_CurAnimIndex() == 19)
	//		{
	//			eAnimType = ANIM_BASE_R;
	//			iFrame = 3;
	//		}
	//			

	//		m_pAnimator->Set_CurAnimIndex(eAnimType, iFrame, ANIM_DIVIDE::eDEFAULT);
	//		m_pAnimator->Set_InterpolationTime(eAnimType, iFrame, 0.f);
	//		m_pAnimator->Set_AnimSpeed(eAnimType, iFrame, 1.f);

	//	}

	//}


	
}

void CMainMenuUnit::My_LateTick()
{

}

void CMainMenuUnit::ReFresh_Animation()
{
	m_pAnimator->Set_CurAnimIndex(m_eBaseType, m_iAnimIndex, ANIM_DIVIDE::eDEFAULT);
	m_pAnimator->Set_InterpolationTime(m_eBaseType, m_iAnimIndex, 0.f);
	m_pAnimator->Set_AnimSpeed(m_eBaseType, m_iAnimIndex, m_fAnimSpeed);
}
