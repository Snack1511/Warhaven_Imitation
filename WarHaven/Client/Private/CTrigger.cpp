#include "stdafx.h"
#include "CTrigger.h"
#include "UsefulHeaders.h"

#include "CCollider_Sphere.h"

CTrigger::CTrigger()
{
}

CTrigger::~CTrigger()
{
}

void CTrigger::Trigger_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CTrigger::Trigger_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CTrigger::Trigger_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	if (m_eColGroup == COL_END || m_fRadius <= 0.f)
		return E_FAIL;

	Add_Component(CCollider_Sphere::Create(CP_AFTER_TRANSFORM, m_fRadius, m_eColGroup, ZERO_VECTOR, DEFAULT_TRANS_MATRIX));

	return S_OK;
}

HRESULT CTrigger::Initialize()
{
	return S_OK;	
}

HRESULT CTrigger::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CTrigger::Trigger_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CTrigger::Trigger_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CTrigger::Trigger_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return S_OK;
}
