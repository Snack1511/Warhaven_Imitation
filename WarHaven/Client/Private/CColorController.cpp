#include "CColorController.h"

#include "CShader.h"
#include "Texture.h"
#include "Transform.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Model.h"

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

HRESULT CColorController::Set_ColorControll(CGameObject* pOwner, _uint iMeshPartType, _float4 vStartColor, _float4 vEndColor, _float EndTime)
{
	pTargetModel = GET_COMPONENT_FROM(pOwner, CModel);

	if (!pTargetModel)
		return E_FAIL;
	m_iMeshPartType = iMeshPartType;
	m_vStartColor = vStartColor;
	m_vEndColor = vEndColor;
	m_fEndTime = EndTime;
	m_fTimeAcc = 0.f;
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
}

void CColorController::Tick()
{
	if (!pTargetModel)	
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
	m_fTimeAcc += fDT(0);

	_float fRatio = m_fTimeAcc / m_fEndTime;

	m_vStartColor = XMVectorLerp(m_vStartColor.XMLoad(), m_vEndColor.XMLoad(), fRatio);

	pTargetModel->Set_ShaderColor(m_iMeshPartType, m_vStartColor);

	if (m_fTimeAcc >= m_fEndTime)
	{
		m_fTimeAcc = 0.f;

		pTargetModel = nullptr;
	}

}
