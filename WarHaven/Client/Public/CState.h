#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CAnimator;
class CGameObject;
END

BEGIN(Client)

class CUnit;

class CState
{
public:
	struct KEYFRAME_EVENT
	{
		enum EVENT_TYPE {EVENT_MOVE, EVENT_ATTACK, EVENT_ATTACKEND, EVENT_END};
		_uint	iKeyFrame = 0;
		EVENT_TYPE	eEventType = EVENT_END;
		_bool	bExecuted = false;

	};

public:
	enum STATE_DIRECTION {
		STATE_DIRECTION_NW, STATE_DIRECTION_NE, STATE_DIRECTION_N, 
		STATE_DIRECTION_SW, STATE_DIRECTION_SE, STATE_DIRECTION_S, 
		STATE_DIRECTION_W, 
		STATE_DIRECTION_E, 
		STATE_DIRECTION_END};

protected:
	CState();
	virtual ~CState();

	friend class CState_Manager;
	friend class CUnit;

public:
	virtual CState* Clone() PURE;

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType) {}

public:
	void	Set_AnimType(ANIM_TYPE eAnimType) { m_eAnimType = eAnimType; }
	void	Set_AnimIndex(_uint iAnimIndex) { m_iAnimIndex = iAnimIndex; }

public:
	virtual HRESULT		Initialize()	PURE;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) PURE;

protected:
	void Re_Enter(CUnit* pOwner, CAnimator* pAnimator, _float fInterpolationTime = -1.f, _float fAnimSpeed = -1.f);

protected:
	CUnit* m_pOwner = nullptr;

protected:
	vector<STATE_TYPE>		m_vecAdjState;
	STATE_TYPE			m_eStateType = STATE_END;
	ANIM_DIVIDE			m_eAnimDivide = ANIM_DIVIDE::eDEFAULT;
	ANIM_TYPE			m_eAnimType = ANIM_BASE_R;
	_uint				m_iAnimIndex = 0;
	_float				m_fAnimSpeed = 1.f;
	_float				m_fInterPolationTime = 0.1f;

	_float				m_fTimeAcc = 0.f;

	_bool				m_bExecuted = false;

	_uint				m_iStateChangeKeyFrame = 0;

	_float				m_fMyMaxLerp = 0.f; 
	_float				m_fMyAccel = 0.f;
	_float				m_fMaxSpeed = 0.f;

	_bool				m_bMoveTrigger = true;
	_bool				m_bAttackTrigger = false;

	_int				m_iFinishedFrame = 0;

	_int				m_iDirectionAnimIndex[STATE_DIRECTION_END];
	_float				m_iDirectionAnimSpeed[STATE_DIRECTION_END];

protected:
	vector<KEYFRAME_EVENT>	m_vecKeyFrameEvent;



protected:
	_uint	Get_Direction(); // 8����
	_uint	Get_Direction_Four(); // 4����
	
	_uint	Move_Direction_Loop(CUnit* pOwner, CAnimator* pAnimator, _float fInterPolationTime);

	void	Change_Location_Loop(_uint iDirection, CAnimator* pAnimator, _float fInterPolationTime);

	_uint	Move(_uint iDirection, CUnit* pOwner);


protected:
	void	Physics_Setting(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax);
	



protected:
	void					Add_KeyFrame(_uint iKeyFrameIndex, _uint iEventType);
	void					Check_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator);
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) {};

protected:
	CHANNEL_CLIENT	m_eChannel = CHANNEL_EFFECTS;
	wstring	m_wstrSoundKey;
	_float	m_fLoopSoundAcc = 0.f;
	_float	m_fSoundLoopTime = 0.1f;

	_bool	m_bRight = true;


protected:
	virtual STATE_TYPE		Check_Condition(CUnit* pOwner, CAnimator* pAnimator) PURE;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) {};
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator) {};


	
};

END