#include "stdafx.h"
#include "CCannon.h"

#include "UsefulHeaders.h"
#include "CCamera_Follow.h"
#include "CScript_FollowCam.h"
#include "CCollider_Sphere.h"

#include "CCameraCollider.h"

#include "CTeamConnector.h"

#include "HIerarchyNode.h"
#include "CCannonBall.h"

#include "CUI_Trail.h"
CCannon::CCannon()
{
}

CCannon::~CCannon()
{
}

void CCannon::Cannon_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CCannon::Cannon_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CCannon::Cannon_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

HRESULT CCannon::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/meshes/map/accessories/Cannon_Anim.fbx");
	pAnimator->Initialize();
	if (!pAnimator)
		return E_FAIL;
	Add_Component(pAnimator);
	m_pAnimator = pAnimator;

	_float4x4 matIdentity;
	matIdentity.Identity();

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/map/accessories/Cannon_30.fbx",
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(270.0f)) * XMMatrixRotationZ(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f)));

	pModel->Add_Model(L"../bin/resources/meshes/map/accessories/Cannon_30.fbx", 1);
	pModel->Set_AnimModelRadius(4.f);
	pModel->Initialize();
	Add_Component(pModel);

	if (FAILED(pModel->SetUp_AnimModel_LOD()))
		return E_FAIL;

	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	
	m_pBonePitch = pModel->Find_HierarchyNode("0B_Cannon_Pitch");
	if (!m_pBonePitch)
		return E_FAIL;


	CCollider_Sphere* pCollider = CCollider_Sphere::Create(0, 2.f, COL_CANNON, _float4(0.f, 1.f, 0.f), DEFAULT_TRANS_MATRIX);
	pCollider->Initialize();
	Add_Component(pCollider);


	

	m_pCannonBall = CCannonBall::Create();
	m_pCannonBall->Initialize();

	
	CUI_Trail* pUI_Trail = CUI_Trail::Create(CP_BEFORE_RENDERER, 4, 0.2f, -0.1f, 3.f, ZERO_VECTOR, _float4(1.f, 1.f, 1.f, 1.f),
		L"../bin/resources/textures/effects/warhaven/texture/T_ArrowUI_01_FX.dds",
		L"../bin/resources/textures/White.png"
	);

	if (!pUI_Trail)
		return E_FAIL;

	m_pUI_Trail = pUI_Trail;

	return S_OK;
}

HRESULT CCannon::Initialize()
{

    return S_OK;
}

HRESULT CCannon::Start()
{
	__super::Start();

	if (m_pUI_Trail)
	{
		CREATE_GAMEOBJECT(m_pUI_Trail, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pUI_Trail);
	}

	m_pCannonCam = CCamera_Follow::Create(this, nullptr);
	m_pCannonCam->Initialize();

	_float4 vPos = m_pTransform->Get_MyWorld(WORLD_POS);
	vPos.y += 3.5f;
	m_pCannonCam->Get_Transform()->Set_World(WORLD_POS, vPos);

	_float4 vLook = m_pTransform->Get_MyWorld(WORLD_LOOK);
	m_pCannonCam->Get_Transform()->Set_Look(vLook);
	m_pCannonCam->Get_Transform()->Make_WorldMatrix();

	CREATE_GAMEOBJECT(m_pCannonCam, GROUP_CAMERA);
	GAMEINSTANCE->Add_Camera_Level(L"CannonCam", m_pCannonCam);
	DISABLE_GAMEOBJECT(m_pCannonCam);

	m_pAnimator->Set_CurAnimIndex(0, 0);
	m_pAnimator->Set_InterpolationTime(0, 0, 0.1f);
	m_pAnimator->Set_AnimSpeed(0, 0, 0.f);

	CREATE_GAMEOBJECT(m_pCannonBall, GROUP_EFFECT);
	DISABLE_GAMEOBJECT(m_pCannonBall);

    return S_OK;
}

_float4 CCannon::Get_ControlPos()
{
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos -= m_pTransform->Get_World(WORLD_LOOK) * 1.4f;
	vPos.y += 1.f;

	return vPos;
}

void CCannon::Control_Cannon(CPlayer* pPlayer)
{
	m_pCurOwnerPlayer = pPlayer;
	if (pPlayer->IsMainPlayer())
	{
		GET_COMPONENT_FROM(m_pCannonCam, CScript_FollowCam)->Start_LerpType(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_CANNON);
		GAMEINSTANCE->Change_Camera(L"CannonCam");
	}

	CUser::Get_Instance()->SetActive_CannonUI(true);

	ENABLE_GAMEOBJECT(m_pUI_Trail);
}

void CCannon::Exit_Cannon()
{
	CUser::Get_Instance()->SetActive_CannonUI(false);

	if (m_pCurOwnerPlayer->IsMainPlayer())
	{
		GAMEINSTANCE->Change_Camera(L"PlayerCam");
	}
	m_pCurOwnerPlayer = nullptr;

	DISABLE_GAMEOBJECT(m_pUI_Trail);


}

void CCannon::Shoot_Cannon()
{
	if (m_fCannonCoolAcc > 0.f)
		return;

	m_pCannonCam->Start_ShakingCamera(0.8f, 0.5f);

	m_fCannonCoolAcc = m_fCannonCoolTime;


	m_pAnimator->Set_CurAnimIndex(0, 0);
	m_pAnimator->Set_InterpolationTime(0, 0, 0.1f);
	m_pAnimator->Set_AnimSpeed(0,0,1.f);


	_float4x4 BoneMatrix = m_pBonePitch->Get_BoneMatrix();
	_float4 vFirePos = BoneMatrix.XMLoad().r[3];
	_float4 vBoneLook = BoneMatrix.XMLoad().r[0];
	vFirePos += vBoneLook * 500.f;
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cannon_Shoot", vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
	/*CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Cannon_Shoot_4"), vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Cannon_Shoot_0"), vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Cannon_Shoot_1"), vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Cannon_Shoot_2"), vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Cannon_Shoot_3"), vFirePos, m_pTransform->Get_WorldMatrix(MARTIX_NOTRANS));*/
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Cannon_Smoke", this, m_pTransform->Get_World(WORLD_POS));

	m_pCannonBall->Shoot_Cannon(m_pCurOwnerPlayer, vFirePos, vBoneLook);

}

_bool CCannon::Can_ControlCannon(CPlayer* pPlayer)
{
	if (pPlayer->Get_Team()->Has_CannonTrigger() && !m_pCurOwnerPlayer)
		return true;

	return false;
}

_float CCannon::Lerp_Position(_float fCurPosition, _float fTargetPosition, _float fRange)
{
	_float fLength = fTargetPosition - fCurPosition;
	_float fSign = 1.f;

	if (fLength < 0.f)
	{
		fSign = -1.f;
		fLength *= -1.f;
	}

	if (fLength < fRange * fDT(0))
		return -9999.f;

	_float fResult = (fCurPosition + fDT(0) * m_fCannonMoveSpeed * fSign);


	return fResult;
}

void CCannon::My_Tick()
{
	if (!m_pCurOwnerPlayer)
		return;

	if (!m_pCurOwnerPlayer->Get_CurrentUnit()->Is_Valid())
		return;

	if (m_fCannonCoolAcc > 0.f)
		m_fCannonCoolAcc -= fDT(0);
	else
	{
		m_fCannonCoolAcc = 0.f;
		CUser::Get_Instance()->SetActive_CannonCoolTime(false);
	}

	CUser::Get_Instance()->Set_CannonCoolTime(m_fCannonCoolAcc, m_fCannonCoolTime);

	if (KEY(LBUTTON, TAP))
	{
		Shoot_Cannon();

		CUser::Get_Instance()->SetActive_CannonCoolTime(true);
	}

	if (m_pAnimator->Is_CurAnimFinished())
	{
		m_pAnimator->Set_CurAnimIndex(0, 0);
		m_pAnimator->Set_InterpolationTime(0, 0, 0.1f);
		m_pAnimator->Set_AnimSpeed(0, 0, 0.f);
	}

}

void CCannon::My_LateTick()
{
	if (!m_pCurOwnerPlayer)
		return;

	if (!m_pCurOwnerPlayer->Get_CurrentUnit()->Is_Valid())
		return;

	if (GET_COMPONENT_FROM(m_pCannonCam, CCameraCollider)->Is_Valid())
		DISABLE_COMPONENT(GET_COMPONENT_FROM(m_pCannonCam, CCameraCollider));


	/* 위 아래만 꺾어줘야함 */

	_float4x4 matOffset;
	_float4 vCurCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);

	m_fCannonMoveSpeed;

	/* Radian 값으로 보간을 하자. */
	_float fTargetPitch, fTargetYaw;
	_float fRange = m_fCannonMoveSpeed * 1.5f;

	_float4 vCamLookNoY = vCurCamLook;
	vCamLookNoY.y = 0.f;
	vCamLookNoY.Normalize();
	_float fDot = vCurCamLook.Dot(vCamLookNoY);
	fTargetPitch = acosf(fDot);

	if (vCurCamLook.y < 0.f)
		fTargetPitch *= -1.f;


	_float fNewPitch = Lerp_Position(m_fCurPitch, fTargetPitch, fRange);
	if (fNewPitch > -9998.f)
		m_fCurPitch = fNewPitch;

	matOffset = XMMatrixRotationAxis(m_pTransform->Get_World(WORLD_UP).XMLoad(), m_fCurPitch);

	/* 좌우 */
	fDot = vCamLookNoY.Dot(m_pTransform->Get_World(WORLD_LOOK));
	fTargetYaw = acosf(fDot);
	if (vCurCamLook.z > 0.f)
		fTargetYaw *= -1.f;

	_float fNewYaw = Lerp_Position(m_fCurYaw, fTargetYaw, fRange);
	if (fNewYaw > -9998.f)
		m_fCurYaw = fNewYaw;

	

	matOffset = matOffset.XMLoad() * XMMatrixRotationAxis(m_pTransform->Get_World(WORLD_RIGHT).XMLoad(), m_fCurYaw);


	//Pitch : x축, 위 아래
	m_pBonePitch->Set_PrevMatrix(matOffset);

	
	_float4x4 BoneMatrix = m_pBonePitch->Get_BoneMatrix();
	_float4 vFirePos = BoneMatrix.XMLoad().r[3];
	_float4 vBoneLook = BoneMatrix.XMLoad().r[0];
	vFirePos += vBoneLook * 500.f;
	
	m_pUI_Trail->Clear_Nodes();
	m_pUI_Trail->Add_Node(vFirePos);

	vBoneLook.Normalize();
	vFirePos += vBoneLook * 2.f;
	m_pUI_Trail->Add_Node(vFirePos);

	vFirePos += vBoneLook * 2.f;
	m_pUI_Trail->Add_Node(vFirePos);

	m_pUI_Trail->ReMap_TrailBuffers();

}
