#include "stdafx.h"
#include "CCamera_Default.h"

#include "GameInstance.h"
#include "Transform.h"

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

	m_pTransform->Set_World(WORLD_POS, _float4(-999.f, -999.f, -999.f));
	m_pTransform->Make_WorldMatrix();
	return S_OK;
}

HRESULT CCamera_Default::Initialize()
{

	return S_OK;
}
