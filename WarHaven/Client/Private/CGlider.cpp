#include "stdafx.h"
#include "CGlider.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CColorController.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CGlider::CGlider()
{
}

CGlider::~CGlider()
{
}

CGlider* CGlider::Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName,
	_float fRadianX, _float fRadianY, _float fRadianZ)
{
	CGlider* pInstance = new CGlider;

	pInstance->m_pOwnerUnit = pOwnerUnit;

	if (FAILED(pInstance->SetUp_Model(wstrModelFilePath, wstrAnimFilePath, strBoneName, fRadianX, fRadianY, fRadianZ)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CGlider");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CGlider");
		return nullptr;
	}

	return pInstance;
}

void CGlider::Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pAnimator->Set_CurAnimIndex(0, iAnimIndex);
	m_pAnimator->Set_InterpolationTime(0, iAnimIndex, fInterpolateTime);
	m_pAnimator->Set_AnimSpeed(0, iAnimIndex, fAnimSpeed);
}

void CGlider::SetUp_GliderTrail()
{
	_float fX = 180.f;
	_float fZ = 70.f; //+뒤
	_float fY = -50.f;

	SetUp_RTrail(
		_float4(fZ, fX, fY + 10.f),	//Weapon R세로
		_float4(fZ, fX, fY - 10.f),					//Weapon R
		_float4(fZ, fX + 10.f, fY),					 //Left	L
		_float4(fZ, fX - 10.f, fY),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		RGBA(245, 245, 200, 0.3f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		100
	);

	fX = 250.f;
	fZ = -320.f;
	fY = 40.f;

	SetUp_LTrail(
		_float4(fZ, fX, fY + 10.f),	//Weapon R세로
		_float4(fZ, fX, fY - 10.f),					//Weapon R
		_float4(fZ, fX + 10.f, fY),					 //Left	L
		_float4(fZ, fX - 10.f, fY),					//Right	L
		_float4(1.f, 0.f, 0.f, 0.f), // GlowFlow
		RGBA(245, 245, 200, 0.3f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_01.dds",
		100
	);


}

void CGlider::TurnOn_Trail(_bool bOn)
{
	if (!m_pRWing)
		return;

	m_pRWing->TurnOn_TrailEffect(bOn);
	m_pRWing2->TurnOn_TrailEffect(bOn);

	m_pLWing->TurnOn_TrailEffect(bOn);
	m_pLWing2->TurnOn_TrailEffect(bOn);
}


HRESULT CGlider::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);


	return S_OK;
}

HRESULT CGlider::Initialize()
{
	return S_OK;
}

HRESULT CGlider::Start()
{
	__super::Start();

	return S_OK;
}

void CGlider::OnEnable()
{
	__super::OnEnable();
	m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f));

	TurnOn_Trail(true);
}

void CGlider::OnDisable()
{
	__super::OnDisable();

	TurnOn_Trail(false);

}


HRESULT CGlider::SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, _float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, wstrAnimFilePath.c_str());
	pAnimator->Initialize();
	if (!pAnimator)
		return E_FAIL;
	Add_Component(pAnimator);
	m_pAnimator = pAnimator;

	_float4x4 matIdentity;
	matIdentity.Identity();

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrModelFilePath,
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(fRadianZ)) * XMMatrixRotationY(XMConvertToRadians(fRadianY)) * XMMatrixRotationX(XMConvertToRadians(fRadianX))
	);
	pModel->Add_Model(wstrModelFilePath, 1);
	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	m_pModelCom = GET_COMPONENT(CModel);


	m_pOwnerBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(strBoneName.c_str());
	if (!m_pOwnerBone)
		return E_FAIL;


	return S_OK;
}

void CGlider::SetUp_RTrail(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount)
{
	m_pRWing = CTrailEffect::Create(0, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode("0B_RWing005"), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath); //

	m_pRWing2 = CTrailEffect::Create(0, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode("0B_RWing005"), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath); //

	if (!m_pRWing)
		return;

	CREATE_GAMEOBJECT(m_pRWing, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pRWing, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pRWing2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pRWing2, CMesh))->Set_NoCurve();

	m_pRWing->Set_EffectFlag(SH_EFFECT_DEFAULT);
	m_pRWing2->Set_EffectFlag(SH_EFFECT_DEFAULT);

	m_pRWing->TurnOn_TrailEffect(false);
	m_pRWing2->TurnOn_TrailEffect(false);

}

void CGlider::SetUp_LTrail(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount)
{
	m_pLWing = CTrailEffect::Create(0, iTrailCount, vWeaponLow, vWeaponHigh,
		m_pModelCom->Find_HierarchyNode("0B_LWing005"), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath); //

	m_pLWing2 = CTrailEffect::Create(0, iTrailCount, vWeaponLeft, vWeaponRight,
		m_pModelCom->Find_HierarchyNode("0B_LWing005"), m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath); //

	if (!m_pLWing)
		return;

	CREATE_GAMEOBJECT(m_pLWing, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLWing, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pLWing2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pLWing2, CMesh))->Set_NoCurve();

	m_pLWing->Set_EffectFlag(SH_EFFECT_DEFAULT);
	m_pLWing2->Set_EffectFlag(SH_EFFECT_DEFAULT);

	m_pLWing->TurnOn_TrailEffect(false);
	m_pLWing2->TurnOn_TrailEffect(false);
}

void CGlider::Late_Tick()
{
	if (m_pAnimator->Get_CurAnimIndex() == 0 && m_pAnimator->Is_CurAnimFinished())
		Set_AnimIndex(1, 0.f, 2.f);

	switch (m_eCurState)
	{
	case Client::CGlider::eOpen:

		CFunctor::Play_Sound(L"Effect_GlidingBegin", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

		m_vScale += fDT(0) * 10.f;

		if (m_vScale.x >= 1.f)
		{
			m_eCurState = eLoop;
			m_pTransform->Set_Scale(_float4(1.f, 1.f, 1.f));
		}
		else
		{
			m_pTransform->Set_Scale(m_vScale);
		}



		break;

	case Client::CGlider::eClose:

		CFunctor::Play_Sound(L"Effect_GlidingEnd", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

		m_vScale -= fDT(0) * 10.f;

		m_pTransform->Set_Scale(m_vScale);

		if (m_vScale.x <= 0.f)
			DISABLE_GAMEOBJECT(this);

		break;

	default:
		break;
	}



	_float4x4		matBone = m_pOwnerBone->Get_BoneMatrix();

	m_pTransform->Get_Transform().matMyWorld = matBone;

	m_pTransform->Make_WorldMatrix();

}
