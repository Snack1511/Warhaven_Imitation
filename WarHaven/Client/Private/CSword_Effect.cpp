#include "stdafx.h"
#include "CSword_Effect.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "CUser.h"

#include "Transform.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"
#include "CUtility_File.h"
#include "Functor.h"
#include "CColorController.h"

CSword_Effect::CSword_Effect()
{
}

CSword_Effect::~CSword_Effect()
{
}

CSword_Effect* CSword_Effect::Create()
{
	CSword_Effect* pInstance = new CSword_Effect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CSword_Effect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CSword_Effect::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_wstrPath = L"../bin/resources/meshes/effects/FBX/weapon/LongSword_Effect.fbx";
	m_matTrans = XMMatrixScaling(1.f, 1.f, 1.f);
	m_hcMyCode = HASHCODE(CSword_Effect);
	m_pFollowTarget = PLAYER;
	m_vOffsetPos = _float4(0.f, 0.f, 0.f);
	m_vEffectFlag = _float4(0.95f, 0.95f, 1.f, 1.f);
	m_vStartScale = _float4{ 0.01f, 0.01f, 0.01f, 0.f };
	m_vFadeInTargetScale  = _float4{ 0.01f, 0.01f, 0.01f, 0.f };
	m_vFadeOutTargetScale = _float4{ 0.01f, 0.01f, 0.01f, 0.f };
	m_eCurFadeType = FADEOUTREADY;
	m_fFadeOutStartTime = 0.f;
	m_fFadeOutTime = 0.1f;
	m_fTargetAlpha = 0.5f;
	

	__super::Initialize_Prototype();

	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_REFLECTION, L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_MRH.dds");
	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_DIFFUSE, L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_B.dds");
	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_NORMALS, L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_N.dds");

	return S_OK;
}

HRESULT CSword_Effect::Initialize()
{
	__super::Initialize();
	/*m_pTransform->Set_Scale(_float4{ 0.01f, 0.01f, 0.01f, 0.f });
	m_pTransform->Make_WorldMatrix();*/

	return S_OK;
}

HRESULT CSword_Effect::Start()
{
	__super::Start();

	m_pTransform->Get_Transform().matMyWorld = GET_COMPONENT_FROM(m_pFollowTarget, CModel)->Get_BoneMatrix("0B_R_WP1");

	return S_OK;
}

void CSword_Effect::My_Tick()
{

	__super::My_Tick();
}

void CSword_Effect::My_LateTick()
{
	
	__super::My_LateTick();

	

}

void CSword_Effect::OnEnable()
{
	m_pTransform->Get_Transform().matMyWorld = GET_COMPONENT_FROM(m_pFollowTarget, CModel)->Get_BoneMatrix("0B_R_WP1");

	__super::OnEnable();
}

void CSword_Effect::OnDisable()
{
	__super::OnDisable();

}
