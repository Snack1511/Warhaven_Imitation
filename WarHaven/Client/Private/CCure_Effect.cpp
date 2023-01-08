#include "stdafx.h"
#include "CCure_Effect.h"

#include "CUnit.h"
#include "GameInstance.h"
#include "Model.h"
#include "CShader.h"
#include "UsefulHeaders.h"
#include "CEffect.h"
#include "CEffects_Factory.h"

#include "CTeamConnector.h"
#include "CTrigger_Paden.h"

#include "HIerarchyNode.h"
#include "CRectEffects.h"
#include "CUI_UnitHUD.h"
#include "CUI_UnitHp.h"

CCure_Effect::CCure_Effect()
{
}

CCure_Effect::~CCure_Effect()
{

}

CCure_Effect* CCure_Effect::Create(CGameObject* pOwner)
{
	CCure_Effect* pInstance = new CCure_Effect;

	pInstance->m_pOwner = pOwner;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCure_Effect");
		return nullptr;
	}

	return pInstance;
}

void CCure_Effect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
	pShader->Set_RawValue("g_vFlag", &m_vEffectFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pShader->Set_RawValue("g_fColorPower", &m_fColorPower, sizeof(_float));
	pShader->Set_RawValue("g_vPlusColor", &m_vPlusColor, sizeof(_float4));

}

HRESULT CCure_Effect::Initialize_Prototype()
{
	
	

	m_wstrPath = L"../bin/resources/meshes/effects/FBX/shape/SM_Helix_01.FBX";

	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(90.f);
	m_fUVSpeedX = 0.f;
	m_fUVSpeedY = -2.f;

	m_iPassType = VTXEFFECT_PASS_AURA;
	m_vEffectFlag = SH_EFFECT_NOGLOW;
	m_fAlpha = 1.f;
	m_fColorPower = 1.f;
	m_vPlusColor = _float4(0.1f, 0.1f, 0.f);



	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXEFFECTS,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = nullptr;
	pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	Add_Component(CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, m_wstrPath.c_str(), m_matTrans));

	Add_Component(CPhysics::Create(CP_BEFORE_RENDERER));

	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_REFLECTION,
		L"../bin/resources/Textures/effects/Warhaven/Texture/T_Glow_08.dds"); //mask

	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_DIFFUSE,
		L"../bin/resources/Textures/effects/Warhaven/Texture/T_Pattern_30.dds"); //color 30 39 56

		/*GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_NORMALS,
			L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_N.dds");*/ //noise

	return S_OK;
}

HRESULT CCure_Effect::Initialize()
{


	return S_OK;
}

HRESULT CCure_Effect::Start()
{
	__super::Start();

	GET_COMPONENT(CModel)->Set_ShaderPassToAll(m_iPassType);

	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectFlag);

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CCure_Effect::Set_ShaderResource, this, placeholders::_1, placeholders::_2);

	m_Smoke.clear();
	m_Particle.clear();

	return S_OK;
}

void CCure_Effect::My_Tick()
{
	

	_float4x4 OwnerMatrix = GET_COMPONENT_FROM(m_pOwner, CModel)->Find_HierarchyNode("0B_L_WP1")->Get_BoneMatrix();
	_float4 vPos = OwnerMatrix.XMLoad().r[3];
	vPos.y += 0.2f;

	if (!m_pOther)
		return;

	_float4x4 OtherMatrix = GET_COMPONENT_FROM(m_pOther, CModel)->Find_HierarchyNode("0B_Spine1")->Get_BoneMatrix();
	_float4 vOtherPos = OtherMatrix.XMLoad().r[3];

	m_pTransform->Set_World(WORLD_POS, vPos);

	_float4 vLook = vOtherPos - vPos;

	_float fLength = vLook.Length();
	fLength *= 1.2f;

	m_pTransform->Set_Look(vLook.Normalize());

	m_pTransform->Set_Scale(_float4(1.f, 1.f, fLength));
}

void CCure_Effect::My_LateTick()
{

	m_fCurUVPlusY += m_fUVSpeedY * fDT(0);
	m_fCurUVPlusX += m_fUVSpeedX * fDT(0);

	if (fabs(m_fCurUVPlusX) >= 1.f)
		m_fCurUVPlusX = 0.f;
	if (fabs(m_fCurUVPlusY) >= 1.f)
		m_fCurUVPlusY = 0.f;

}

void CCure_Effect::OnEnable()
{
	__super::OnEnable();

	m_pOther = static_cast<CUnit*>(m_pOwner)->Get_CureObject();

	if (!m_pOther)
		return;

	if(m_Smoke.empty())
		m_Smoke = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cure_Smoke", m_pOther, ZERO_VECTOR);

	if (m_Particle.empty())
	{
		m_Particle = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cure_Particle", m_pOwner, ZERO_VECTOR);
		static_cast<CUnit*>(m_pOwner)->Create_Light(m_Particle.back(), _float4(0.f, 0.f, 0.f), 2.f, 0.f, 0.1f, 100.f, 0.1f, RGB(245, 245, 100), false);
		static_cast<CUnit*>(m_pOther)->Get_OwnerHUD()->Get_UnitHP()->SetActive_HealBlur(true);
	}

}

void CCure_Effect::OnDisable()
{
	__super::OnDisable();
		

	if (!m_Smoke.empty())
	{
		for (auto& elem : m_Smoke)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_Smoke.clear();
	} 

	if (!m_Particle.empty())
	{
		for (auto& elem : m_Particle)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_Particle.clear();
		static_cast<CUnit*>(m_pOther)->Get_OwnerHUD()->Get_UnitHP()->SetActive_HealBlur(false);
	}

}
