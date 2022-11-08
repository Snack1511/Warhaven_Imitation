#include "CColorController.h"

#include "CShader.h"
#include "Texture.h"
#include "Transform.h"

#include "GameInstance.h"
#include "GameObject.h"

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

void CColorController::Set_ColorControll(_float4 vStartColor, _float4 vEndColor, _float EndTime)
{
	m_vStartColor = vStartColor;
	m_vEndColor = vEndColor;
	m_fEndTime = EndTime;
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
	m_fTimeAcc += fDT(0);

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

}

void CColorController::OnDisable()
{
}

void CColorController::OnDead()
{
}

void CColorController::LerpColor()
{
	_float fRatio = m_fTimeAcc / m_fEndTime;

	m_vStartColor = XMVectorLerp(m_vStartColor.XMLoad(), m_vEndColor.XMLoad(), fRatio);

}
