#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CUnit;

class CState
{
protected:
	enum State_Direction { 
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
	void	Set_AnimType(ANIM_TYPE eAnimType) { m_eAnimType = eAnimType; }
	void	Set_AnimIndex(_uint iAnimIndex) { m_iAnimIndex = iAnimIndex; }

public:
	virtual HRESULT		Initialize()	PURE;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) PURE;

protected:
	void Change_Animation(_uint iCurAnimIndex, _uint iChangeIndex, CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE End_Animation(_uint iChangeIndex, ANIM_TYPE eAnimType, STATE_TYPE eStateType, _bool bStateChange, CUnit* pOwner, CAnimator* pAnimator);


protected:
	vector<STATE_TYPE>		m_vecAdjState;
	STATE_TYPE			m_eStateType = STATE_END;

	ANIM_TYPE			m_eAnimType = ANIM_BASE_R;

	_uint				m_iAnimIndex = 0;

	_float				m_fAnimSpeed = 1.f;
	_float				m_fInterPolationTime = 0.1f;

	_float				m_fTimeAcc = 0.f;

	_bool				m_bExecuted = false;

	_uint				m_iStateChangeKeyFrame = 0;

protected:
	CHANNEL_CLIENT	m_eChannel = CHANNEL_EFFECTS;
	wstring	m_wstrSoundKey;
	_float	m_fLoopSoundAcc = 0.f;
	_float	m_fSoundLoopTime = 0.1f;

	_bool	m_bRight = true;

	_int	m_iLoopAnimIndex = 0;
	_int	m_iBeginAnimIndex = 0;
	_int	m_iEndAnimIndex = 0;
	_int	m_iPlayAnimIndex = 0;



public:
	virtual STATE_TYPE		Check_Condition(CUnit* pOwner, CAnimator* pAnimator) PURE;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) {};
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator) {};

	
};

END