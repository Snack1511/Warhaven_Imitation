#include "stdafx.h"
#include "CEffect.h"

#include "Model.h"
#include "Texture.h"
#include "CShader.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Renderer.h"
#include "CModel_Renderer.h"
#include "Physics.h"
#include "GameInstance.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}


void CEffect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
}

void CEffect::Set_EffectFlag(_float4 vEffectFlag)
{
	m_vEffectShaderFlag = vEffectFlag;

	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectShaderFlag);

}

HRESULT CEffect::Initialize_Prototype()
{

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXEFFECTS,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	Add_Component<CRenderer>(CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, m_eEffectPassType));
	

	return S_OK;
}

HRESULT CEffect::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectShaderFlag);

	GET_COMPONENT(CModel)->Set_ShaderPassToAll(m_eEffectPassType);

	return S_OK;
}

HRESULT CEffect::Start()
{
	__super::Start();

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, placeholders::_2);
	
	return S_OK;
}

void CEffect::My_Tick()
{

}

void CEffect::My_LateTick()
{
	Update_Fade();

	Push_UV();
}

void CEffect::Update_Fade()
{
	m_fFadeTimeAcc += fDT(0);

	switch (m_eCurFadeType)
	{
	case FADEINREADY:
		if (m_fFadeTimeAcc >= m_fFadeInStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEIN;
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
			m_fCurUVPlusX = 0.f;
			m_fCurUVPlusY = 0.f;
		}
		else
		{
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
		}


		break;

	case FADEIN:
		if (m_fFadeTimeAcc >= m_fFadeInTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUTREADY;
			m_pTransform->Set_Scale(m_vFadeInTargetScale);
			m_fAlpha = m_fTargetAlpha;
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeInTime;
			_float4 vScale =
				XMVectorLerp(m_vStartScale.XMLoad(), m_vFadeInTargetScale.XMLoad(), fRatio);

			m_fAlpha = m_fTargetAlpha * fRatio;
			m_pTransform->Set_Scale(vScale);

		}

		break;

	case FADEOUTREADY:
		if (m_fFadeTimeAcc >= m_fFadeOutStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUT;
		}

		break;

	case FADEOUT:
		if (m_fFadeTimeAcc >= m_fFadeOutTime)
		{
			DISABLE_GAMEOBJECT(this);
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeOutTime;
			_float4 vScale =
				XMVectorLerp(m_vFadeInTargetScale.XMLoad(), m_vFadeOutTargetScale.XMLoad(), sqrtf(fRatio));

			m_fAlpha = m_fTargetAlpha * (1.f - fRatio);
			m_pTransform->Set_Scale(vScale);

		}

		break;

	default:
		break;
	}

	m_pTransform->Make_WorldMatrix();
}

void CEffect::Push_UV()
{
	m_fCurUVPlusX += m_fUVSpeedX * fDT(0);
	m_fCurUVPlusY += m_fUVSpeedY * fDT(0);
}

HRESULT CEffect::SetUp_Model(wstring pFilePath)
{
	_float4x4			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));

	CModel* pModel = CModel::Create(CP_AFTER_RENDERER, TYPE_NONANIM, pFilePath, TransformMatrix);

	if (!pModel)
		return E_FAIL;


	pModel->Initialize();

	Add_Component(pModel);


	return S_OK;
}

