#include "stdafx.h"
#include "CScript_FollowCam.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"
#include "Loading_Manager.h"

#include "CUser.h"
#include "CUnit.h"
#include "CNavigation.h"
#include "CCamera_Follow.h"
#include "Physics.h"

#include "Easing_Utillity.h"

CScript_FollowCam::CScript_FollowCam(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_FollowCam::~CScript_FollowCam()
{
}

CScript_FollowCam* CScript_FollowCam::Create(_uint iGroupIdx, CGameObject* pTarget)
{
	CScript_FollowCam* pInstance = new CScript_FollowCam(iGroupIdx);

	pInstance->m_pFollowTarget = pTarget;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_FollowCam");
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

void CScript_FollowCam::Start_ShakingCamera(_float fPower, _float fTime)
{
	//돌고있을 때
	if (m_bShaking)
	{
		//새로온 힘이 더 약한애면

		if (fPower < m_fCurShakingPower)
			return;
	}

	if (m_bIsMainPlayerCam)
		GAMEINSTANCE->Start_MotionBlur(fTime);

	m_fShakingPower = fPower;
	m_fShakingTime = fTime;
	m_fShakingTimeAcc = 0.f;
	m_bShaking = true;
}

void CScript_FollowCam::Synchronize_Position()
{
	_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	_float4 vOffset = m_vCurrentOffset.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
	vOffset.y = m_vCurrentOffset.y;
	vPos += vOffset;
	vPos -= (m_pOwner->Get_Transform()->Get_World(WORLD_LOOK) * m_fCurrentDistance);

	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vPos);
	m_pOwner->Get_Transform()->Make_WorldMatrix();
}

void CScript_FollowCam::Start_LerpType(CAMERA_LERP_TYPE eType)
{

	if (!m_pFollowTarget)
		return;

	m_eCurrentLerpType = eType;

	m_bCameraMoving = m_arrLerpDesc[eType].bCameraMove;
	//m_bFOVLerping = true;
	m_bOffsetLerping = true;
	m_bDistanceLerping = true;

	m_fTimeAcc = 0.f;
	m_fCameraMoveTimeAcc = 0.f;

	m_fMaxDistance = m_arrLerpDesc[eType].fMaxDistance;

	m_vOriginOffset =	m_vCurrentOffset;
	m_vOriginLook = m_pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	m_vTargetLook = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK);

	m_fOriginDistance = m_fCurrentDistance;

	//가까우면 0, 멀면 최대가 1
	_float fRatio = acosf(m_vTargetLook.Dot(m_vOriginLook)) / 3.141592f;
	if (fRatio < 0.3f)
		fRatio = 0.3f;
 	m_fCameraMoveLerpTime = m_arrLerpDesc[eType].fCameraDirectingLerpTime * fRatio;

	//거리 차이의 max가 필요하다..
	//거리가 1이상 차이나면 좀 빠르게
	//fRatio = fabs(m_arrLerpDesc[eType].fTargetDistance - m_fOriginDistance) / 1.f;
	m_fCameraDistanceLerpTime = m_arrLerpDesc[eType].fCameraDistanceLerpTime;

	fRatio = (m_arrLerpDesc[eType].vTargetOffset - m_vOriginOffset).Length() / m_tDefaultDesc.vDefaultOffset.Length();
	m_fCameraOffsetLerpTime = m_arrLerpDesc[eType].fCameraOffsetLerpTime * fRatio;

}

void CScript_FollowCam::Start_FOVLerp(_float fTargetFOV)
{
	return;

	m_bFOVLerping = true;
	m_fFOVTimeAcc = 0.f;
	m_fOriginFOV = static_cast<CCamera*>(m_pOwner)->Get_Proj().fFOV;
	m_fTargetFOV = fTargetFOV;
	m_fCameraFOVLerpTime = 0.3f;
}

void CScript_FollowCam::On_FixedTarget(_bool bActivate, CUnit* pLockedTarget)
{
	m_bTargetLocked = bActivate;
	m_pLockedTarget = pLockedTarget;
}

HRESULT CScript_FollowCam::Initialize_Prototype()
{
	for (_uint i = 0; i < CAMERA_LERP_END; ++i)
	{
		m_arrLerpDesc[i].fCameraMouseMoveLerpTime = m_tDefaultDesc.fDefaultLerpTime;
		m_arrLerpDesc[i].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime;
		m_arrLerpDesc[i].fCameraFOVLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.7f;
		m_arrLerpDesc[i].fCameraOffsetLerpTime = m_tDefaultDesc.fDefaultLerpTime;
		m_arrLerpDesc[i].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.7f;
		m_arrLerpDesc[i].fTargetFOV = m_tDefaultDesc.fDefaultFOV;
		m_arrLerpDesc[i].vTargetOffset = m_tDefaultDesc.vDefaultOffset;
		m_arrLerpDesc[i].fTargetDistance = m_tDefaultDesc.fDefaultDistance;
		m_arrLerpDesc[i].fMaxDistance = m_tDefaultDesc.fMaxDistance;
	}

	m_fTargetFOV = m_tDefaultDesc.fDefaultFOV * 0.5f;

	return S_OK;
}

HRESULT CScript_FollowCam::Initialize()
{
	m_arrLerpDesc[CAMERA_LERP_SPRINT].vTargetOffset.x = 0.f;

	//m_arrLerpDesc[CAMERA_LERP_ZOOM].vTargetOffset.y *= 1.2f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM].fTargetDistance *= 0.5f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM].fMaxDistance *= 0.4f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM].fCameraDistanceLerpTime *= 0.5f;


	m_arrLerpDesc[CAMERA_LERP_ZOOMMAX] = m_arrLerpDesc[CAMERA_LERP_ZOOM];
	/* 거리 */
	m_arrLerpDesc[CAMERA_LERP_ZOOMMAX].fTargetDistance *= 0.2f;


	m_arrLerpDesc[CAMERA_LERP_CANNON].fTargetDistance = 0.5f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].vTargetOffset.x = 0.f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].vTargetOffset.z = 0.f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].vTargetOffset.y = 3.5f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].fMaxDistance *= 0.4f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].fCameraDistanceLerpTime *= 0.1f;
	m_arrLerpDesc[CAMERA_LERP_CANNON].fCameraOffsetLerpTime *= 0.1f;

	m_arrLerpDesc[CAMERA_LERP_QANDA].fTargetDistance *= 0.6f;
	m_arrLerpDesc[CAMERA_LERP_QANDA].fMaxDistance *= 0.4f;
	m_arrLerpDesc[CAMERA_LERP_QANDA].fCameraDistanceLerpTime *= 0.5f;

	m_arrLerpDesc[CAMERA_LERP_ZOOMOUT].fTargetDistance *= 1.2f;
	m_arrLerpDesc[CAMERA_LERP_ZOOMOUT].fMaxDistance *= 1.2f;
	m_arrLerpDesc[CAMERA_LERP_ZOOMOUT].fCameraDistanceLerpTime *= 0.5f;

	m_arrLerpDesc[CAMERA_LERP_LANCER].fTargetDistance *= 1.25f;
	m_arrLerpDesc[CAMERA_LERP_LANCER].fMaxDistance *= 0.8f;
	m_arrLerpDesc[CAMERA_LERP_LANCER].fCameraDistanceLerpTime *= 0.5f;
	m_arrLerpDesc[CAMERA_LERP_LANCER].vTargetOffset.y += 0.4f;


	m_arrLerpDesc[CAMERA_LERP_BREEZE] = m_arrLerpDesc[CAMERA_LERP_LANCER];
	m_arrLerpDesc[CAMERA_LERP_BREEZE].fTargetDistance *= 1.3f;
	m_arrLerpDesc[CAMERA_LERP_BREEZE].vTargetOffset.y -= 0.2f;
	m_arrLerpDesc[CAMERA_LERP_BREEZE].vTargetOffset.x = 0.f;


	return S_OK;
}

void CScript_FollowCam::Start()
{
	//m_pTargetsNavi = GET_COMPONENT_FROM(m_pFollowTarget, CNavigation);
	m_pPhyscisCom = GET_COMPONENT_FROM(m_pOwner, CPhysics);
	DISABLE_COMPONENT(m_pPhyscisCom);
	m_pPhyscisCom->Get_Physics().fPlusAcc = 0.f;
	m_pPhyscisCom->Set_MaxSpeed(1500.f);
	Start_LerpType(CAMERA_LERP_DEFAULT);


	/*if (!m_pTargetsNavi)
	{
		Call_MsgBox(L"No navi In Target");
	}*/
	if (!m_pPhyscisCom)
	{
		Call_MsgBox(L"No m_pPhyscisCom In FollowCam");
	}
	if (!m_pOwner)
	{
		Call_MsgBox(L"No Owner In FollowCam");
		m_pOwner = GAMEINSTANCE->Find_Camera(L"PlayerCam");
	}
	//ShowCursor(true); // 툴에서 커서 안보여서 true로 바꿔놓음

}

void CScript_FollowCam::Tick()
{
	if (KEY(TAB, TAP))
	{
		m_bMouseVisible = !m_bMouseVisible;

		//if (!m_bMouseVisible)
		//	ShowCursor(false);
		//else
		//	ShowCursor(true);


		CUser::Get_Instance()->Set_FixCursor(!m_bMouseVisible);
	}

	if (m_bMouseVisible)
		return;



	//0. 타겟 고정 여부
	if (m_bTargetLocked)
	{
		_float4 vLook;

		_float4 vLockedTargetPos = m_pLockedTarget->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
		vLook = (vLockedTargetPos - vMyPos);

		m_vOriginLook = m_pOwner->Get_Transform()->Get_World(WORLD_LOOK);
		m_vTargetLook = vLook.Normalize();

		if (m_vTargetLook.y < -0.3f)
			m_vTargetLook.y = -0.3f;

		_float4 vDir = m_vTargetLook - m_vOriginLook;

		m_vOriginLook += vDir * 2.f * fDT(0);
		
		m_pOwner->Get_Transform()->Set_Look(m_vOriginLook);
		Update_CameraByMouse();

	}
	else if (m_bCameraMoving)
	{
		//Look 제어
		Update_CameraByDirecting();
	}
	else
	{
		//Look은 마우스로
		Update_CameraByMouse();

	}
	//1. 연출 여부
	Update_Shaking();

	

	
	
	//2. 러프
	Update_Lerp();


	//3. 이동
	Find_TargetPos();

}

void CScript_FollowCam::Late_Tick()
{
	//Navi타고 나서 보정
	

}

void CScript_FollowCam::Release()
{
}

void CScript_FollowCam::OnEnable()
{
	__super::OnEnable();
	//DISABLE_COMPONENT(m_pPhyscisCom);
	Start_LerpType(m_eCurrentLerpType);

}

void CScript_FollowCam::OnDisable()
{
	__super::OnDisable();
}

void CScript_FollowCam::Update_CameraByMouse()
{
	if (GetFocus() != g_hWnd)
		return;

	CTransform* pTransform = m_pOwner->Get_Transform();

	//마우스 이동에 따라 회전
	_float fTurnSpeedX = ((_float)MOUSE_MOVE(MMS_X)) * MOUSE_DPI_X * 0.5f;
	_float fTurnSpeedY = ((_float)MOUSE_MOVE(MMS_Y)) * MOUSE_DPI_Y * 0.5f;


	m_vOriginLook = CUtility_Transform::Turn_ByAngle(m_vOriginLook, pTransform->Get_MyWorld(WORLD_RIGHT), fTurnSpeedY);
	m_vOriginLook = CUtility_Transform::Turn_ByAngle(m_vOriginLook, _float4(0.f, 1.f, 0.f, 0.f), fTurnSpeedX);

	pTransform->Set_Look(m_vOriginLook);


	//CUtility_Transform::Turn_ByAngle(pTransform, )

	/*_long lWheelmMove = MOUSE_MOVE(MMS_WHEEL);
	m_fDistance -= (_float)lWheelmMove * fDT * 0.1f;*/


}

void CScript_FollowCam::Update_CameraByDirecting()
{
	m_fCameraMoveTimeAcc += fDT(0);

	if (m_fCameraMoveTimeAcc >= m_fCameraMoveLerpTime)
	{
		m_bCameraMoving = false;
		m_fCameraMoveTimeAcc = 0.f;
		return;
	}

	_float fRatio = m_fCameraMoveTimeAcc / m_fCameraMoveLerpTime;
	_float4 vLook = XMVectorLerp(m_vOriginLook.XMLoad(), m_vTargetLook.XMLoad(), sqrtf(fRatio));
	m_pOwner->Get_Transform()->Set_Look(vLook);

}

void CScript_FollowCam::Update_Lerp()
{
	m_fTimeAcc += fDT(0);

	if (m_bOffsetLerping)
	{
		if (m_fTimeAcc >= m_fCameraOffsetLerpTime)
		{
			m_bOffsetLerping = false;
		}
		else
		{
			_float fRatio = m_fTimeAcc / m_fCameraOffsetLerpTime;
			_float4 vOffset = XMVectorLerp(m_vOriginOffset.XMLoad(), m_arrLerpDesc[m_eCurrentLerpType].vTargetOffset.XMLoad(), sqrtf(fRatio));
			m_vCurrentOffset = vOffset;
		}
	}

	
	if (m_bFOVLerping)
	{
		m_fFOVTimeAcc += fDT(0);

		if (m_fFOVTimeAcc >= m_fCameraFOVLerpTime)
		{
			m_bFOVLerping = false;
		}
		else
		{
			_float fRatio = m_fFOVTimeAcc / m_fCameraFOVLerpTime;
			fRatio = sqrtf(fRatio);
			_float fFOV = (m_fOriginFOV * (1.f - fRatio)) + (m_fTargetFOV * fRatio);
			static_cast<CCamera*>(m_pOwner)->Get_Proj().fFOV = fFOV;
		}
	}

	if (m_bDistanceLerping)
	{
		if (m_fTimeAcc >= m_fCameraDistanceLerpTime)
		{
			m_bDistanceLerping = false;
		}
		else
		{
			_float fRatio = m_fTimeAcc / m_fCameraDistanceLerpTime;
			fRatio = sqrtf(sqrtf(fRatio));
			m_fCurrentDistance = (m_fOriginDistance * (1.f - fRatio)) + (m_arrLerpDesc[m_eCurrentLerpType].fTargetDistance * fRatio);
			
		}

	}
	
}

void CScript_FollowCam::Find_TargetPos()
{
	//타겟포스 찾기
	_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	_float4 vOffset = m_vCurrentOffset.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
	vOffset.y = m_vCurrentOffset.y;
	vPos += vOffset;

	vPos -= (m_pOwner->Get_Transform()->Get_World(WORLD_LOOK) * m_fCurrentDistance);

	_float4 vCurPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir = vPos - vCurPos;

	//실제 차이나는 거리
	_float fRealDistance = vDir.Length();

	_float fRatio = (fRealDistance / m_fMaxDistance);
	fRatio = powf(fRatio, 2.f);

	_float fSpeed = m_fCamSpeed * fRatio;

	//최대 거리 제한
	if (fSpeed > 30.f)
		fSpeed = 30.f;

	m_pPhyscisCom->Get_Physics().bAir = true;
	m_pPhyscisCom->Set_Dir(vDir);
	m_pPhyscisCom->Set_Speed(fSpeed);
}

void CScript_FollowCam::Start_CameraDirecting()
{
	m_bCameraMoving = true;
	m_fCameraMoveTimeAcc = 0.f;
	m_vOriginLook = m_pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	m_vTargetLook = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK);
}

void CScript_FollowCam::Update_Shaking()
{
	if (GAMEINSTANCE->Get_TimeSpeed(0) < 1.f)
		return;


	//power가 시간에 따라 줄어들자 그냥

	m_fShakingTimeAcc += fDT(0);

	if (m_fShakingTimeAcc >= m_fShakingTime)
	{
		m_bShaking = false;
		m_fShakingTimeAcc = 0.f;
		return;
	}

	if (m_bShaking)
	{
		m_fCurShakingPower = CEasing_Utillity::sinfOut(m_fShakingPower, 0.f, m_fShakingTimeAcc, m_fShakingTime);

		_float4 vRandDir;
		vRandDir.x = frandom(-1.f, 1.f);
		vRandDir.y = frandom(-1.f, 1.f);
		vRandDir.z = frandom(-1.f, 1.f);
		vRandDir.w = 0.f;
		vRandDir.Normalize();

		_float4 vNewLook = m_vOriginLook + (vRandDir * m_fCurShakingPower * fDT(0));
		m_pOwner->Get_Transform()->Set_Look(vNewLook);
	}

}
