#include "stdafx.h"
#include "CCamera_Default.h"

#include "GameInstance.h"

CCamera_Default::CCamera_Default()
{
}

CCamera_Default::~CCamera_Default()
{
}

HRESULT CCamera_Default::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_tProj.fFOV = XMConvertToRadians(70.f);

	return S_OK;
}

HRESULT CCamera_Default::Initialize()
{

	return S_OK;
}
