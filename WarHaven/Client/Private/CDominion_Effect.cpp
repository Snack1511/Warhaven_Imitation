#include "stdafx.h"
#include "CDominion_Effect.h"

#include "GameInstance.h"
#include "Model.h"
#include "CShader.h"
#include "UsefulHeaders.h"
#include "CEffect.h"

#include "CTeamConnector.h"
#include "CTrigger_Paden.h"

CDominion_Effect::CDominion_Effect()
{
}

CDominion_Effect::~CDominion_Effect()
{
}

CDominion_Effect* CDominion_Effect::Create(_float4 vScale, _float4 vPos, _uint iTriggerType)
{
	CDominion_Effect* pInstance = new CDominion_Effect;

	pInstance->m_iTriggerType = iTriggerType;
	pInstance->m_pTransform->Set_Scale(vScale);
	pInstance->m_pTransform->Set_World(WORLD_POS, vPos);
	pInstance->m_pTransform->Make_WorldMatrix();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CDominion_Effect");
		return nullptr;
	}

	return pInstance;
}

void CDominion_Effect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
	pShader->Set_RawValue("g_vFlag", &m_vEffectFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pShader->Set_RawValue("g_fColorPower", &m_fColorPower, sizeof(_float));
	pShader->Set_RawValue("g_vPlusColor", &m_vPlusColor, sizeof(_float4));

}

void CDominion_Effect::Set_DominionColor(CTeamConnector* pConqueredTeam)
{

	if (!pConqueredTeam)
		m_vPlusColor = _float4(1.f, 1.f, 1.f);

	else if (pConqueredTeam->IsMainPlayerTeam()) //파란색
	{
		/*기존에 있던 이펙트 페이드 아웃*/
		for (auto& elem : m_Aura)
		{
			static_cast<CEffect*>(elem)->Start_FadeOut();
		}
		m_Aura.clear();

		switch ((CTrigger_Paden::ePADEN_TRIGGER_TYPE)m_iTriggerType)
		{
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eMAIN:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eRESPAWN:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCANNON:
			m_Aura = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BlueAura_C", m_pTransform->Get_World(WORLD_POS));
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCNT:
			break;
		default:
			break;
		}

		m_vPlusColor = _float4(0.3f, 0.7f, 1.f);
	}

	else if (!pConqueredTeam->IsMainPlayerTeam()) //빨간색
	{
		for (auto& elem : m_Aura)
		{
			static_cast<CEffect*>(elem)->Start_FadeOut();
		}
		m_Aura.clear();

		switch ((CTrigger_Paden::ePADEN_TRIGGER_TYPE)m_iTriggerType)
		{
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eSTART:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eMAIN:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eRESPAWN:
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCANNON:
			m_Aura = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RedAura_C", m_pTransform->Get_World(WORLD_POS));
			break;
		case Client::CTrigger_Paden::ePADEN_TRIGGER_TYPE::eCNT:
			break;
		default:
			break;
		}

		m_vPlusColor = _float4(1.f, 0.f, 0.f);
	}

}

HRESULT CDominion_Effect::Initialize_Prototype()
{
	m_wstrPath = L"../bin/resources/meshes/effects/FBX/DominianArea/SM_Dominian_Area_01.fbx";

	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_fUVSpeedX = 0.f;
	m_fUVSpeedY = 0.f;
	
	m_iPassType = VTXEFFECT_PASS_DOMINION;
	m_vEffectFlag = SH_EFFECT_NONE;
	m_fAlpha = 0.5f;
	m_fColorPower = 0.5f;
	m_vPlusColor = _float4(1.f, 1.f, 1.f);



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
		L"../bin/resources/Textures/White.png"); //mask

	/*GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_DIFFUSE,
		L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_B.dds");*/ //color

	/*GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_NORMALS,
		L"../bin/resources/Textures/effects/Warhaven/weapon/T_WP_LongSword0006_A00_N.dds");*/ //noise

	return S_OK;
}

HRESULT CDominion_Effect::Initialize()
{
	return S_OK;
}

HRESULT CDominion_Effect::Start()
{
	__super::Start();

	GET_COMPONENT(CModel)->Set_ShaderPassToAll(m_iPassType);

	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectFlag);

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CDominion_Effect::Set_ShaderResource, this, placeholders::_1, placeholders::_2);


	
	return S_OK;
}

void CDominion_Effect::My_Tick()
{

}

void CDominion_Effect::My_LateTick()
{

	m_fCurUVPlusY += m_fUVSpeedY * fDT(0);
	m_fCurUVPlusX += m_fUVSpeedX * fDT(0);

	if (fabs(m_fCurUVPlusX) >= 1.f)
		m_fCurUVPlusX = 0.f;
	if (fabs(m_fCurUVPlusY) >= 1.f)
		m_fCurUVPlusY = 0.f;

}
