#include "stdafx.h"
#include "CCannonBoom.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CCollider_Sphere.h"

CCannonBoom::CCannonBoom()
{
}

CCannonBoom::~CCannonBoom()
{
}

void CCannonBoom::Boom(CPlayer* pOwnerPlayer, _float4 vPos)
{
	ENABLE_GAMEOBJECT(this);
	m_pTransform->Set_World(WORLD_POS, vPos);
	m_pTransform->Make_WorldMatrix();
	m_pOwnerPlayer = pOwnerPlayer;
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cannon_Ground", vPos);

	CFunctor::Play_Sound_SetRange(L"Effect_CannonShoot", CHANNEL_EFFECTS, Get_Transform()->Get_World(WORLD_POS), 100.f);

	if (pOwnerPlayer->Get_Team()->Get_TeamType() == eTEAM_TYPE::eBLUE)
		m_pColliderCom->Set_ColIndex(COL_BLUEFLYATTACKGUARDBREAK);
	else
		m_pColliderCom->Set_ColIndex(COL_REDFLYATTACKGUARDBREAK);

}

void CCannonBoom::Boom_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionEnter(pOtherObj, eOtherColType, eMyColType, vHitPos);
	pOtherObj->CallBack_CollisionEnter(pFireUnit, eMyColType, eOtherColType, vHitPos);
}

void CCannonBoom::Boom_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionStay(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionStay(pFireUnit, eMyColType, eOtherColType);
}

void CCannonBoom::Boom_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	CUnit* pFireUnit = m_pOwnerPlayer->Get_CurrentUnit();
	pFireUnit->CallBack_CollisionExit(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionExit(pFireUnit, eMyColType, eOtherColType);
}

HRESULT CCannonBoom::Initialize_Prototype()
{
	m_pColliderCom = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, 6.f, COL_BLUEFLYATTACKGUARDBREAK, ZERO_VECTOR, DEFAULT_TRANS_MATRIX);
	m_pColliderCom->Initialize();
	Add_Component(m_pColliderCom);
	return S_OK;
}

HRESULT CCannonBoom::Initialize()
{
	return S_OK;
}

HRESULT CCannonBoom::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CCannonBoom::Boom_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CCannonBoom::Boom_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CCannonBoom::Boom_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return S_OK;
}

void CCannonBoom::My_Tick()
{
	if (m_fEnableAcc > 0.f)
		m_fEnableAcc -= fDT(0);
	else
		DISABLE_GAMEOBJECT(this);
}

void CCannonBoom::My_LateTick()
{
}

void CCannonBoom::OnEnable()
{
	__super::OnEnable();
	m_fEnableAcc = m_fEnableTime;
}

void CCannonBoom::OnDisable()
{
	__super::OnDisable();

}
