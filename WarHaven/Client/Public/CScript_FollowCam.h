#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CNavigation;
class CPhysics;
END

BEGIN(Client)

struct DEFAULTFOLLOWDESC
{
	//_float	fDefaultFOV = 1.22173f;
	_float	fDefaultFOV = 0.785398f;
	//_float	fDefaultDistance = 2.5f;
	_float	fDefaultDistance = 3.3f;
	_float4 vDefaultOffset = _float4(0.6f, 1.5f, 0.f, 1.f);
	_float	fDefaultLerpTime = 1.5f;
	_float	fMaxDistance = 1.5f;
};

struct LERPDESC
{
	_bool			bCameraMove = false;

	_float			fCameraMouseMoveLerpTime = 0.f;
	_float			fCameraDirectingLerpTime = 0.f;
	_float			fCameraOffsetLerpTime = 0.f;
	_float			fCameraFOVLerpTime = 0.f;
	_float			fCameraDistanceLerpTime = 0.f;

	_float4			vTargetOffset;
	_float			fTargetFOV;
	_float			fTargetDistance;

	_float			fMaxDistance = 1.5f;
};

class CScript_FollowCam final
	: public CScriptable
{
	friend class CCamera_Follow;

public:
	enum CAMERA_LERP_TYPE {
		CAMERA_LERP_DEFAULT,
		CAMERA_LERP_SPRINT,
		CAMERA_LERP_ZOOM,
		CAMERA_LERP_ZOOMMAX,
		CAMERA_LERP_CANNON,
		CAMERA_LERP_QANDA,
		CAMERA_LERP_LANCER,
		CAMERA_LERP_BREEZE,
		CAMERA_LERP_RUSH,
		CAMERA_LERP_ZOOMOUT,

		CAMERA_LERP_END, 
	};


	DECLARE_PROTOTYPE(CScript_FollowCam);

private:
	CScript_FollowCam(_uint iGroupIdx);
	virtual ~CScript_FollowCam();

public:
	LERPDESC& Get_Desc(_uint iIdx) { return m_arrLerpDesc[iIdx]; }
	DEFAULTFOLLOWDESC& Get_DefaultDesc() { return m_tDefaultDesc; }

public:
	static CScript_FollowCam* Create(_uint iGroupIdx, CGameObject* pTarget);

public:
	void	Start_ShakingCamera(_float fPower, _float fTime);
	void	Synchronize_Position();

public:
	void	Start_LerpType(CAMERA_LERP_TYPE eType);
	void	Start_FOVLerp(_float fTargetFOV = 0.785398f);

	void	On_FixedTarget(_bool bActivate, class CUnit* pLockedTarget);

	void	Set_FollowTarget(CGameObject* pTarget) { m_pFollowTarget = pTarget; }

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	_bool	m_bIsMainPlayerCam = false;

	//들어오는 힘, 시간 계속 받아서 흔듬
	_bool	m_bShaking = false;
	_float	m_fShakingTimeAcc = 0.f;
	_float	m_fShakingTime = 0.f;
	_float	m_fShakingPower = 0.f;
	_float			m_fCurShakingPower = 0.f;

private:
	_bool			m_bTargetLocked = false;
	class CUnit*	m_pLockedTarget = nullptr;

	_bool			m_bMouseVisible = false;
	CPhysics*		m_pPhyscisCom = nullptr;

private:
	CGameObject*			m_pFollowTarget = nullptr;
	CNavigation*			m_pTargetsNavi = nullptr;
	LERPDESC				m_arrLerpDesc[CAMERA_LERP_END];
	CAMERA_LERP_TYPE		m_eCurrentLerpType = CAMERA_LERP_DEFAULT;

	DEFAULTFOLLOWDESC		m_tDefaultDesc;

	_bool					m_bCameraMoving = false;
	_bool					m_bOffsetLerping = false;
	_bool					m_bFOVLerping = false;
	_bool					m_bDistanceLerping = false;
	_float					m_fCurrentDistance = 2.5f;
	_float					m_fMaxDistance = 0.5f;
	_float					m_fCamSpeed = 30.f;
	//_float					m_fCamSpeed = 60.f;

	_float					m_fCameraMoveLerpTime = 0.f;
	_float					m_fCameraDistanceLerpTime = 0.f;
	_float					m_fCameraOffsetLerpTime = 0.f;
	_float					m_fCameraFOVLerpTime = 0.f;


	_float			m_fCameraMoveTimeAcc = 0.f;
	_float			m_fTimeAcc = 0.f;
	_float			m_fFOVTimeAcc = 0.f;
	_float4			m_vCurrentOffset;


	_float4			m_vOriginLook;
	_float4			m_vTargetLook;
	_float4			m_vOriginOffset;
	_float			m_fOriginFOV;
	_float			m_fOriginDistance;
	_float			m_fTargetFOV = 0.f;


private:
	void	Update_CameraByMouse();
	void	Update_CameraByDirecting();
	void	Update_Lerp();
	void	Update_Shaking();

private:
	void	Find_TargetPos();

private:
	void	Start_CameraDirecting();

};

END