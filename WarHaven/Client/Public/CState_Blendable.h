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
	//L, R 여부
	ANIM_TYPE	m_eAnimLeftorRight = ANIM_BASE_R;
	//몇프레임까지 블렌드 할 지 (공격 실행 프레임)
	_uint		m_iStopIndex = 9999;
	//Idle 인덱스
	_uint		m_iIdle_Index = 9999;

	//Run, Walk 인덱스
	_uint				m_iRunAnimIndex[STATE_DIRECTION_END] = {};
	_uint				m_iWalkAnimIndex[STATE_DIRECTION_END] = {};

	//Jump 인덱스
	_uint				m_iJumpAnimIndex[STATE_DIRECTION_END] = {};

	//Jump Fall 인덱스
	_uint				m_iJumpFallIndex = 9999;
	//Land 인덱스
	_uint				m_iLandIndex = 9999;

	STATE_TYPE			m_eWalkState = STATE_END;
	STATE_TYPE			m_eRunState = STATE_END;
	STATE_TYPE			m_eJumpState = STATE_END;
	STATE_TYPE			m_eFallState = STATE_END;
	STATE_TYPE			m_eLandState = STATE_END;
	STATE_TYPE			m_eIdleState = STATE_END;

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