#include "stdafx.h"
#include "CCrowBoom.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CCollider_Sphere.h"

CCrowBoom::CCrowBoom()
{
}

CCrowBoom::~CCrowBoom()
{
}

void CCrowBoom::Boom(CPlayer* pOwnerPlayer, _float4 vPos)
{
	ENABLE_GAMEOBJECT(this);
	m_pTransform->Set_World(WORLD_POS, vPos);
	m_pTransform->Make_WorldMatrix();
	m_pOwnerPlayer = pOwnerPlayer;
	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cannon_Ground", vPos);

	if (CUser::Get_Instance()->Get_CurLevel() != LEVEL_TEST)
	{
		m_pColliderCom = GET_COMPONENT(CCollider_Sphere);

		if (pOwnerPlayer->Get_Team()->Get_TeamType() == eTEAM_TYPE::eRED)
			m_pColliderCom->Set_ColIndex(COL_REDGUARDBREAK);
		else
			m_pColliderCom->Set_ColIndex(COL_BLUEGUARDBREAK);
	}
	else
		m_pColliderCom->Set_ColIndex(COL_REDGUARDBREAK);

}

void CCrowBoom::Boom_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionEnter(pOtherObj, eOtherColType, eMyColType, vHitPos);
	pOtherObj->CallBack_CollisionEnter(pFireUnit, eMyColType, eOtherColType, vHitPos);
}

void CCrowBoom::Boom_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionStay(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionStay(pFireUnit, eMyColType, eOtherColType);
}

void CCrowBoom::Boom_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionExit(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionExit(pFireUnit, eMyColType, eOtherColType);
}

HRESULT CCrowBoom::Initialize_Prototype()
{
	m_pColliderCom = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, 2.f, COL_BLUEGUARDBREAK, ZERO_VECTOR, DEFAULT_TRANS_MATRIX);
	m_pColliderCom->Initialize();
	Add_Component(m_pColliderCom);
	return S_OK;
}

HRESULT CCrowBoom::Initialize()
{
	return S_OK;
}

HRESULT CCrowBoom::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CCrowBoom::Boom_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CCrowBoom::Boom_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CCrowBoom::Boom_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return S_OK;
}

void CCrowBoom::My_Tick()
{
	if (m_fEnableAcc > 0.f)
		m_fEnableAcc -= fDT(0);
	else
		DISABLE_GAMEOBJECT(this);
}

void CCrowBoom::My_LateTick()
{
}

void CCrowBoom::OnEnable()
{
	__super::OnEnable();
	m_fEnableAcc = m_fEnableTime;
}

void CCrowBoom::OnDisable()
{
	__super::OnDisable();

}
