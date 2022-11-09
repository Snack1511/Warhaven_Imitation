#include "CColorController.h"

#include "CShader.h"
#include "Texture.h"
#include "Transform.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Model.h"
#include "CAnimator.h"

CColorController::CColorController(_uint iGroupIdx)
	:CComponent(iGroupIdx)
{
}

CColorController::~CColorController()
{
}

CColorController* CColorController::Create(_uint iGroupIdx)
{
	CColorController* pInstance = new CColorController(iGroupIdx);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CColorController");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CColorController::Set_ColorControll(const COLORDESC& tColorDesc)
{
	m_tColorDesc = tColorDesc;
	m_iCurModelPart = m_tColorDesc.iMeshPartType;
	m_eCurFadeType = COLORDESC::FADEINREADY;
	m_fFadeTimeAcc = 0.f;


	if (!m_pTargetAnimator)
		if (tColorDesc.eFadeStyle == KEYFRAME)
			return E_FAIL;
	
	return S_OK;
}

HRESULT CColorController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CColorController::Initialize()
{
	return S_OK;
}

void CColorController::Start()
{
	__super::Start();

	m_pTargetModel = GET_COMPONENT_FROM(m_pOwner, CModel);

	for (_uint i = 0; i < MODEL_PART_END; ++i)
	{
		m_vOriginColor[i] = m_pTargetModel->Get_Color(i);
	}

	if(TYPE_ANIM == m_pTargetModel->Get_ModelType())
		m_pTargetAnimator = GET_COMPONENT_FROM(m_pOwner, CAnimator);
}

void CColorController::Tick()
{
	if (!m_pTargetModel)
		return;

	LerpColor();
}

void CColorController::Late_Tick()
{
}

void CColorController::Release()
{
}

void CColorController::OnEnable()
{
	__super::OnEnable();
}

void CColorController::OnDisable()
{
	__super::OnDisable();
}

void CColorController::OnDead()
{
	__super::OnDead();
}

void CColorController::LerpColor()
{
	switch (m_tColorDesc.eFadeStyle)
	{
	case Client::CColorController::TIME:
		Fade_Time();
		break;
	case Client::CColorController::KEYFRAME:
		Fade_KeyFrame();
		break;
	case Client::CColorController::COLORDESC::FADE_END:
		break;
	default:
		break;
	}

		
}

void CColorController::Fade_Time()
{
	m_fFadeTimeAcc += fDT(0);

	switch (m_eCurFadeType)
	{
	case COLORDESC::FADEINREADY:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeInStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEIN;
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
		}
		else
		{
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
		}

		break;

	case COLORDESC::FADEIN:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeInTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEOUTREADY;

			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vTargetColor);
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_tColorDesc.fFadeInTime;
			_float4 vLerpColor =
				XMVectorLerp(m_vOriginColor[m_iCurModelPart].XMLoad(), m_tColorDesc.vTargetColor.XMLoad(), fRatio);

			m_tColorDesc.vCurColor = vLerpColor;
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vCurColor);
		}

		break;

	case COLORDESC::FADEOUTREADY:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeOutStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEOUT;
		}

		break;

	case COLORDESC::FADEOUT:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeOutTime)
		{
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
			m_eCurFadeType = COLORDESC::FADE_END;

		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_tColorDesc.fFadeOutTime;
			_float4 vLerpColor =
				XMVectorLerp(m_tColorDesc.vTargetColor.XMLoad(), m_vOriginColor[m_iCurModelPart].XMLoad(), fRatio);

			m_tColorDesc.vCurColor = vLerpColor;

			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vCurColor);
		}

		break;

	default:
		break;
	}
}

void CColorController::Fade_KeyFrame()
{
	_uint iCurFrame = m_pTargetAnimator->Get_CurAnimFrame();


	m_fFadeTimeAcc += fDT(0);

	switch (m_eCurFadeType)
	{
	case COLORDESC::FADEINREADY:
		if (iCurFrame >= m_tColorDesc.iStartKeyFrame)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEIN;
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
		}
		else
		{
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
		}

		break;

	case COLORDESC::FADEIN:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeInTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEOUTREADY;

			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vTargetColor);
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_tColorDesc.fFadeInTime;
			_float4 vLerpColor =
				XMVectorLerp(m_vOriginColor[m_iCurModelPart].XMLoad(), m_tColorDesc.vTargetColor.XMLoad(), fRatio);

			m_tColorDesc.vCurColor = vLerpColor;
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vCurColor);
		}

		break;

	case COLORDESC::FADEOUTREADY:
		if (iCurFrame >= m_tColorDesc.iEndKeyFrame)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = COLORDESC::FADEOUT;
		}

		break;

	case COLORDESC::FADEOUT:
		if (m_fFadeTimeAcc >= m_tColorDesc.fFadeOutTime)
		{
			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_vOriginColor[m_iCurModelPart]);
			m_eCurFadeType = COLORDESC::FADE_END;
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_tColorDesc.fFadeOutTime;
			_float4 vLerpColor =
				XMVectorLerp(m_tColorDesc.vTargetColor.XMLoad(), m_vOriginColor[m_iCurModelPart].XMLoad(), fRatio);

			m_tColorDesc.vCurColor = vLerpColor;

			m_pTargetModel->Set_ShaderColor(m_tColorDesc.iMeshPartType, m_tColorDesc.vCurColor);
		}

		break;

	default:
		break;
	}

}