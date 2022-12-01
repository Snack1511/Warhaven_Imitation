#include "stdafx.h"
#include "CCamera_Follow.h"

#include "GameInstance.h"

#include "CScript_FollowCam.h"

#include "CNavigation.h"
#include "Physics.h"
#include "Transform.h"
#include "CUnit.h"

#include "CCameraCollider.h"
#include "CCell.h"

CCamera_Follow::CCamera_Follow()
{
}

CCamera_Follow::~CCamera_Follow()
{
}

CCamera_Follow* CCamera_Follow::Create(CGameObject* pTarget, CCell* pStartCell)
{
	CCamera_Follow* pInstance = new CCamera_Follow;

	if (FAILED(pInstance->SetUp_Components(pTarget, pStartCell)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CCamera_Follow");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCamera_Follow");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Start_ShakingCamera(_float fPower, _float fTime)
{
	m_pFollowScript->Start_ShakingCamera(fPower, fTime);
}

void CCamera_Follow::Synchronize_Position()
{
	m_pFollowScript->Synchronize_Position();
}

void CCamera_Follow::Create_CamCollider()
{
	list<CComponent*> ColList = Get_Component<CCameraCollider>();

	if (!ColList.empty())
		return;

	CCameraCollider::BONECOLLIDERDESC tDesc;
	tDesc.fHeight = 0.1f;
	tDesc.fRadius = 0.1f;
	tDesc.vOffset = ZERO_VECTOR;
	CCameraCollider* pPhysXCollider = CCameraCollider::Create(CP_BEFORE_RENDERER, tDesc);
	pPhysXCollider->Initialize();
	Add_Component(pPhysXCollider);
}
void CCamera_Follow::Delete_CamCollider()
{
	list<CComponent*> ColList = Get_Component<CCameraCollider>();

	if (!ColList.empty())
	{
		for (auto& elem : ColList)
		{
			DELETE_COMPONENT(elem, this);
		}
	}
	
}
void CCamera_Follow::Set_FollowTarget(CGameObject* pTarget)
{
	m_pFollowScript->Set_FollowTarget(pTarget);
	m_pTransform->Set_World(WORLD_POS, pTarget->Get_Transform()->Get_World(WORLD_POS));
	m_pTransform->Make_WorldMatrix();
	m_pParent = pTarget;

	
}

void CCamera_Follow::On_FixedTarget(_bool bActivate, CUnit* pLockedTarget)
{
	m_pFollowScript->On_FixedTarget(bActivate, pLockedTarget);
}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_pTransform->Set_ParentFlag(MATRIX_IDENTITY);

	Create_CamCollider();
	

	return S_OK;
}

HRESULT CCamera_Follow::Initialize()
{
	return GET_COMPONENT(CScript_FollowCam)->Initialize();
}

HRESULT CCamera_Follow::Start()
{
	//m_pNaviCom = GET_COMPONENT(CNavigation);
	m_pFollowScript = GET_COMPONENT(CScript_FollowCam);

	//if (!m_pNaviCom)
		//return E_FAIL;

	if (!m_pFollowScript)
		return E_FAIL;

	return __super::Start();
}

HRESULT CCamera_Follow::SetUp_Components(CGameObject* pTarget, CCell* pStartCell)
{
	Add_Component(CScript_FollowCam::Create(CP_BEFORE_NAVIGATION, pTarget));
	CPhysics* pPhyscisCom = CPhysics::Create(0);
	Add_Component(pPhyscisCom);
	m_pParent = pTarget;


	/**/

	/*CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, pStartCell, pPhyscisCom);

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	_float4 vStartPos;
	XMStoreFloat4(&vStartPos, pStartCell->Get_Point(CCell::POINT_A));
	vStartPos.w = 1.f;
	vStartPos.x += 0.1f;
	vStartPos.z += 0.1f;

	m_pTransform->Set_World(WORLD_POS, vStartPos);
	m_pTransform->Make_WorldMatrix();*/
	return S_OK;
}

void CCamera_Follow::Late_Tick()
{

}
