#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CState_Blendable abstract
	: public CState
{
	enum class Enum {eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE};

	//이 클래스 상속받아서 쓰면 애니메이션 중에 움직일 수 있음
protected:
	CState_Blendable();
	virtual ~CState_Blendable();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;


	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);

	STATE_TYPE	Update_Walk(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Run(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Jump(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Fall(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Land(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Idle(CUnit* pOwner, CAnimator* pAnimator);

protected:
	_bool		m_bAfterEffect = false;
	_float		m_fCreateTimeAcc = 0.f;
	_float		m_fCreateTime = 0.02f;
	_float		m_fCreateTimeSlow = 0.07f;
	void		Create_SwordAfterEffect();

protected:
	//L, R 여부
	ANIM_TYPE	m_eAnimLeftorRight = ANIM_BASE_R;
	//몇프레임까지 블렌드 할 지 (공격 실행 프레임)
	_uint		m_iStopIndex = 9999;

	//공격 끝나는 프레임
	_uint		m_iAttackEndIndex = 9999;

	//Idle 인덱스
	_uint		m_iIdle_Index = 9999;

	//Run, Walk 인덱스
	_uint				m_iRunLeftAnimIndex[STATE_DIRECTION_END] = {};
	_uint				m_iRunRightAnimIndex[STATE_DIRECTION_END] = {};
	_uint				m_iWalkLeftAnimIndex[STATE_DIRECTION_END] = {};
	_uint				m_iWalkRightAnimIndex[STATE_DIRECTION_END] = {};

	//Jump 인덱스
	_uint				m_iJumpLeftAnimIndex[STATE_DIRECTION_END] = {};
	_uint				m_iJumpRightAnimIndex[STATE_DIRECTION_END] = {};

	//Jump Fall 인덱스
	_uint				m_iJumpFallLeftIndex = 9999;
	_uint				m_iJumpFallRightIndex = 9999;
	//Land 인덱스
	_uint				m_iLandLeftIndex = 9999;
	_uint				m_iLandRightIndex = 9999;

	STATE_TYPE			m_eWalkState = STATE_END;
	STATE_TYPE			m_eRunState = STATE_END;
	STATE_TYPE			m_eJumpState = STATE_END;
	STATE_TYPE			m_eFallState = STATE_END;
	STATE_TYPE			m_eLandState = STATE_END;
	STATE_TYPE			m_eIdleState = STATE_END;
	STATE_TYPE			m_eBounceState = STATE_END;

protected:
	Enum				m_eEnum = Enum::eIDLE;
	_bool				m_bBlendable = true;
	_bool				m_bCycling = false;

protected:
	void				Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType);
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	void				On_EnumChange(Enum eEnum, CAnimator* pAnimator);

};

END