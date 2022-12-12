#include "stdafx.h"
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

HRESULT CColorController::Add_ColorControll(const COLORDESC& tColorDesc)
{
	m_ColorDesclist.push_back(tColorDesc);

	m_ColorDesclist.back().iOriginAnimIndex = m_pTargetAnimator->Get_CurAnimIndex();

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
		m_vOriginColor[i] = ZERO_VECTOR;
	}

	if(TYPE_ANIM == m_pTargetModel->Get_ModelType())
		m_pTargetAnimator = GET_COMPONENT_FROM(m_pOwner, CAnimator);
}

void CColorController::Tick()
{
	if (!m_pTargetModel)
		return;

	for (auto iter = m_ColorDesclist.begin(); iter != m_ColorDesclist.end();)
	{
		if (LerpColor(*iter))
		{
			++iter;
		}
		else
			iter = m_ColorDesclist.erase(iter);

	}
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

_bool CColorController::LerpColor(COLORDESC& tColorDesc)
{
	switch (tColorDesc.eFadeStyle)
	{
	case Client::CColorController::TIME:
		return Fade_Time(tColorDesc);
		break;
	case Client::CColorController::KEYFRAME:
		return Fade_KeyFrame(tColorDesc);
		break;
	case Client::CColorController::COLORDESC::FADE_END:
		break;
	default:
		break;
	}

	return true;
}

_bool CColorController::Fade_Time(COLORDESC& tColorDesc)
{
	tColorDesc.fFadeTimeAcc += fDT(0);

	switch (tColorDesc.eCurFadeType)
	{
	case COLORDESC::FADEINREADY:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeInStartTime)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEIN;

			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
		}
		else
		{
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
		}

		break;

	case COLORDESC::FADEIN:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeInTime)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEOUTREADY;

			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vTargetColor);
		}
		else
		{
			_float fRatio = tColorDesc.fFadeTimeAcc / tColorDesc.fFadeInTime;
			_float4 vLerpColor =
				XMVectorLerp(m_vOriginColor[tColorDesc.iMeshPartType].XMLoad(), tColorDesc.vTargetColor.XMLoad(), fRatio);

			tColorDesc.vCurColor = vLerpColor;
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vCurColor);
		}

		break;

	case COLORDESC::FADEOUTREADY:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeOutStartTime)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEOUT;
		}

		break;

	case COLORDESC::FADEOUT:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeOutTime)
		{
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
			tColorDesc.eCurFadeType = COLORDESC::FADE_END;
			return false;
		}
		else
		{
			_float fRatio = tColorDesc.fFadeTimeAcc / tColorDesc.fFadeOutTime;
			_float4 vLerpColor =
				XMVectorLerp(tColorDesc.vTargetColor.XMLoad(), m_vOriginColor[tColorDesc.iMeshPartType].XMLoad(), fRatio);

			tColorDesc.vCurColor = vLerpColor;

			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vCurColor);
		}

		break;

	default:
		break;
	}

	return true;
}

_bool CColorController::Fade_KeyFrame(COLORDESC& tColorDesc)
{
	/* AnimIndex°¡ ÀüÀÌ¶û ´Ù¸£¸é °Á false return*/
	_uint iCurIndex = m_pTargetAnimator->Get_CurAnimIndex();
	if (iCurIndex != tColorDesc.iOriginAnimIndex)
	{
		m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);

		return false;
	}

	_uint iCurFrame = m_pTargetAnimator->Get_CurAnimFrame();
	
	tColorDesc.fFadeTimeAcc += fDT(0);

	switch (tColorDesc.eCurFadeType)
	{
	case COLORDESC::FADEINREADY:
		if (iCurFrame >= tColorDesc.iStartKeyFrame)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEIN;
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
		}
		else
		{
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
		}

		break;

	case COLORDESC::FADEIN:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeInTime)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEOUTREADY;

			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vTargetColor);
		}
		else
		{
			_float fRatio = tColorDesc.fFadeTimeAcc / tColorDesc.fFadeInTime;
			_float4 vLerpColor =
				XMVectorLerp(m_vOriginColor[tColorDesc.iMeshPartType].XMLoad(), tColorDesc.vTargetColor.XMLoad(), fRatio);

			tColorDesc.vCurColor = vLerpColor;
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vCurColor);
		}

		break;

	case COLORDESC::FADEOUTREADY:
		if (iCurFrame >= tColorDesc.iEndKeyFrame)
		{
			tColorDesc.fFadeTimeAcc = 0.f;
			tColorDesc.eCurFadeType = COLORDESC::FADEOUT;
		}

		break;

	case COLORDESC::FADEOUT:
		if (tColorDesc.fFadeTimeAcc >= tColorDesc.fFadeOutTime)
		{
			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, m_vOriginColor[tColorDesc.iMeshPartType]);
			tColorDesc.eCurFadeType = COLORDESC::FADE_END;
			return false;

		}
		else
		{
			_float fRatio = tColorDesc.fFadeTimeAcc / tColorDesc.fFadeOutTime;
			_float4 vLerpColor =
				XMVectorLerp(tColorDesc.vTargetColor.XMLoad(), m_vOriginColor[tColorDesc.iMeshPartType].XMLoad(), fRatio);

			tColorDesc.vCurColor = vLerpColor;

			m_pTargetModel->Set_RimLightFlag(tColorDesc.iMeshPartType, tColorDesc.vCurColor);
		}

		break;

	default:
		break;
	}

	return true;


}