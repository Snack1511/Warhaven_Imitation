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
	//�������� ��
	if (m_fShakingTime > 0.f)
	{
		//���ο� ���� �� ���Ѿָ�

		if (fPower < m_fShakingPower)
			return;
	}

	m_fShakingPower = fPower;
	m_fShakingTime = fTime;
		 
}

void CScript_FollowCam::Start_LerpType(CAMERA_LERP_TYPE eType)
{
	if (m_eCurrentLerpType >= CAMERA_LERP_FINISHATTACK)
	{
		//���� ���� �ȳ�������
		if (m_bCameraMoving)
			return;
	}

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
	if (eType == CAMERA_LERP_FINISHATTACKDOWN)
	{
		m_vTargetLook += _float4(0.f, -1.3f, 0.f, 0.f);
		m_vTargetLook.Normalize();
	}
	else if (eType == CAMERA_LERP_FINISHATTACKUP)
	{
		m_vTargetLook += _float4(0.f, 0.6f, 0.f, 0.f);
		m_vTargetLook.Normalize();
	}
	else if (eType == CAMERA_LERP_FINISHATTACKBACK)
	{
		m_vTargetLook *= -1.f;
		m_vTargetLook += _float4(0.f, -0.2f, 0.f, 0.f);
		m_vTargetLook.Normalize();
	}
	m_fOriginDistance = m_fCurrentDistance;

	//������ 0, �ָ� �ִ밡 1
	_float fRatio = acosf(m_vTargetLook.Dot(m_vOriginLook)) / 3.141592f;
	if (fRatio < 0.3f)
		fRatio = 0.3f;
 	m_fCameraMoveLerpTime = m_arrLerpDesc[eType].fCameraDirectingLerpTime * fRatio;

	//�Ÿ� ������ max�� �ʿ��ϴ�..
	//�Ÿ��� 1�̻� ���̳��� �� ������
	//fRatio = fabs(m_arrLerpDesc[eType].fTargetDistance - m_fOriginDistance) / 1.f;
	m_fCameraDistanceLerpTime = m_arrLerpDesc[eType].fCameraDistanceLerpTime;

	fRatio = (m_arrLerpDesc[eType].vTargetOffset - m_vOriginOffset).Length() / m_tDefaultDesc.vDefaultOffset.Length();
	m_fCameraOffsetLerpTime = m_arrLerpDesc[eType].fCameraOffsetLerpTime * fRatio;

}

void CScript_FollowCam::Start_FOVLerp(_float fTargetFOV)
{
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


	m_arrLerpDesc[CAMERA_LERP_DASH].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 2.f;
	//m_arrLerpDesc[CAMERA_LERP_DASH].fMaxDistance = 0.1f;
	//m_arrLerpDesc[CAMERA_LERP_DASH].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.3f;
	m_arrLerpDesc[CAMERA_LERP_DASH].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;

	m_arrLerpDesc[CAMERA_LERP_BACKDASH].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 1.5f;

	m_arrLerpDesc[CAMERA_LERP_LEFT].vTargetOffset.x = m_tDefaultDesc.vDefaultOffset.x - 1.f;
	m_arrLerpDesc[CAMERA_LERP_RIGHT].vTargetOffset.x = m_tDefaultDesc.vDefaultOffset.x + 1.f;

	//m_arrLerpDesc[CAMERA_LERP_RUN].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 1.3f;

	m_arrLerpDesc[CAMERA_LERP_WALLJUMP].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 2.f;
	m_arrLerpDesc[CAMERA_LERP_WALLJUMP].vTargetOffset.x = 0.f;
	//m_arrLerpDesc[CAMERA_LERP_WALLJUMP].vTargetOffset.y = m_tDefaultDesc.vDefaultOffset.y - 0.5f;
	m_arrLerpDesc[CAMERA_LERP_WALLJUMP].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_WALLJUMP].bCameraMove = true;

	m_arrLerpDesc[CAMERA_LERP_DASH].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_QUICKDEFAULT].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.3f;
	//m_arrLerpDesc[CAMERA_LERP_WALLJUMP].fMaxDistance = 0.2f;

	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.55f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].vTargetOffset.x = m_tDefaultDesc.vDefaultOffset.x + 0.5f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].vTargetOffset.y = m_tDefaultDesc.vDefaultOffset.y - 0.5f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].bCameraMove = true;
	//m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].fMaxDistance = 0.2f;

	m_arrLerpDesc[CAMERA_LERP_ZOOM_LEFT] = m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT];
	m_arrLerpDesc[CAMERA_LERP_ZOOM_LEFT].vTargetOffset.x *= -1.f;

	m_arrLerpDesc[CAMERA_LERP_WIRE] = m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT];
	m_arrLerpDesc[CAMERA_LERP_WIRE].bCameraMove = false;

	m_arrLerpDesc[CAMERA_LERP_MINIZOOMRIGHT] = m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT];
	m_arrLerpDesc[CAMERA_LERP_MINIZOOMRIGHT].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.75f;
	m_arrLerpDesc[CAMERA_LERP_MINIZOOMRIGHT].vTargetOffset.x = m_tDefaultDesc.vDefaultOffset.x + 0.35f;
	m_arrLerpDesc[CAMERA_LERP_MINIZOOMRIGHT].bCameraMove = false;


	m_arrLerpDesc[CAMERA_LERP_FINISHATTACK] = m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT];
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACK].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACK].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;

	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKBACK] = m_arrLerpDesc[CAMERA_LERP_FINISHATTACK];
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKBACK].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.7f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKBACK].vTargetOffset.x = m_tDefaultDesc.vDefaultOffset.x + 0.3f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKBACK].vTargetOffset.y = m_tDefaultDesc.vDefaultOffset.y - 0.6f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKBACK].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.7f;

	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKFALL] = m_arrLerpDesc[CAMERA_LERP_FINISHATTACK];
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKFALL].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 1.2f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKFALL].vTargetOffset = m_tDefaultDesc.vDefaultOffset;

	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN] = m_arrLerpDesc[CAMERA_LERP_FINISHATTACK];
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].vTargetOffset.x -= 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].vTargetOffset.z -= 1.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].vTargetOffset.y += 1.f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].fCameraDirectingLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN].fCameraOffsetLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;


	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP] = m_arrLerpDesc[CAMERA_LERP_FINISHATTACKDOWN];
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP].vTargetOffset = m_tDefaultDesc.vDefaultOffset;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP].vTargetOffset.x += 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP].vTargetOffset.z = 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_FINISHATTACKUP].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;

	m_arrLerpDesc[CAMERA_LERP_KO].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 0.3f;
	m_arrLerpDesc[CAMERA_LERP_KO].fCameraOffsetLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.3f;
	m_arrLerpDesc[CAMERA_LERP_KO].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.3f;

	m_arrLerpDesc[CAMERA_LERP_HITHARD] = m_arrLerpDesc[CAMERA_LERP_FINISHATTACK];
	m_arrLerpDesc[CAMERA_LERP_HITHARD].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 1.5f;

	m_arrLerpDesc[CAMERA_LERP_ZOOM_RIGHT].bCameraMove = false;


	m_arrLerpDesc[CAMERA_LERP_NIGHTGUY].fTargetDistance = m_tDefaultDesc.fDefaultDistance * 1.3f;
	m_arrLerpDesc[CAMERA_LERP_NIGHTGUY].vTargetOffset = _float4(0.f, 1.5f, 0.f, 1.f);
	m_arrLerpDesc[CAMERA_LERP_NIGHTGUY].fCameraOffsetLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;
	m_arrLerpDesc[CAMERA_LERP_NIGHTGUY].fCameraDistanceLerpTime = m_tDefaultDesc.fDefaultLerpTime * 0.5f;


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
	ShowCursor(true); // ������ Ŀ�� �Ⱥ����� true�� �ٲ����

}

void CScript_FollowCam::Tick()
{
	if (KEY(TAB, TAP))
	{
		m_bMouseVisible = !m_bMouseVisible;

		if (!m_bMouseVisible)
			ShowCursor(false);
		else
			ShowCursor(true);


		CUser::Get_Instance()->Set_FixCursor(!m_bMouseVisible);
	}

	if (m_bMouseVisible)
		return;

	m_fShakingTime -= fDT(0);
	if (m_fShakingTime < 0.f)
		m_fShakingTime = 0.f;

	//0. Ÿ�� ���� ����
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
		//Look ����
		Update_CameraByDirecting();
	}
	else
	{
		//Look�� ���콺��
		Update_CameraByMouse();

	}
	//1. ���� ����
	Update_Shaking();

	

	
	
	//2. ����
	Update_Lerp();


	//3. �̵�
	Find_TargetPos();

}

void CScript_FollowCam::Late_Tick()
{
	//NaviŸ�� ���� ����
	

}

void CScript_FollowCam::Release()
{
}

void CScript_FollowCam::OnEnable()
{
	__super::OnEnable();
	//DISABLE_COMPONENT(m_pPhyscisCom);
	Start_LerpType(CAMERA_LERP_DEFAULT);

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

	//���콺 �̵��� ���� ȸ��
	_float fTurnSpeedX = ((_float)MOUSE_MOVE(MMS_X)) * MOUSE_DPI_X;
	_float fTurnSpeedY = ((_float)MOUSE_MOVE(MMS_Y)) * MOUSE_DPI_Y;

	CUtility_Transform::Turn_ByAngle(pTransform, pTransform->Get_MyWorld(WORLD_RIGHT), fTurnSpeedY);
	CUtility_Transform::Turn_ByAngle(pTransform, _float4(0.f, 1.f, 0.f, 0.f), fTurnSpeedX);


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
	//Ÿ������ ã��
	_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	_float4 vOffset = m_vCurrentOffset.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
	vOffset.y = m_vCurrentOffset.y;
	vPos += vOffset;

	vPos -= (m_pOwner->Get_Transform()->Get_World(WORLD_LOOK) * m_fCurrentDistance);

	_float4 vCurPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir = vPos - vCurPos;

	//���� ���̳��� �Ÿ�
	_float fRealDistance = vDir.Length();

	_float fRatio = (fRealDistance / m_fMaxDistance);
	fRatio = powf(fRatio, 2.f);

	_float fSpeed = m_fCamSpeed * fRatio;

	//�ִ� �Ÿ� ����
	if (fSpeed > 50.f)
		fSpeed = 50.f;

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


	if (m_fShakingTime > 0.f)
	{
		

		_float fShakingPower = m_fShakingTime * m_fShakingPower;

		_float4 vRandDir;
		vRandDir.x = frandom(-1.f, 1.f);
		vRandDir.y = frandom(-1.f, 1.f);
		vRandDir.z = frandom(-1.f, 1.f);
		vRandDir.w = 0.f;
		vRandDir.Normalize();

		m_vOriginLook = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_LOOK);
		m_vOriginLook += vRandDir * fShakingPower;
		m_pOwner->Get_Transform()->Set_Look(m_vOriginLook);
	}

}
