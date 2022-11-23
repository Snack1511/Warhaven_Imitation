#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CState_BlendableCharge abstract
	: public CState
{
	enum class Enum { eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE };

	//이 클래스 상속받아서 쓰면 애니메이션 중에 움직일 수 있음
protected:
	CState_BlendableCharge();
	virtual ~CState_BlendableCharge();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//L, R 여부
	ANIM_TYPE	m_eAnimLeftorRight = ANIM_BASE_R;


	/* Idle 인덱스 넣으면 됨*/
	_uint		m_iIdle_Index = 9999;

	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iRunLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iRunRightAnimIndex[STATE_DIRECTION_END] = {};
	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iWalkLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iWalkRightAnimIndex[STATE_DIRECTION_END] = {};

	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iJumpLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*방향별 해당 애니메이션 인덱스 넣어주면 됨*/
	_uint				m_iJumpRightAnimIndex[STATE_DIRECTION_END] = {};

	/*해당 애니메이션 인덱스*/
	_uint				m_iJumpFallLeftIndex = 9999;
	/*해당 애니메이션 인덱스*/
	_uint				m_iJumpFallRightIndex = 9999;

	/*해당 애니메이션 인덱스*/
	_uint				m_iLandLeftIndex = 9999;
	/*해당 애니메이션 인덱스*/
	_uint				m_iLandRightIndex = 9999;

	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eWalkState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eRunState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eJumpState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eFallState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eLandState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eIdleState = STATE_END;
	/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eBounceState = STATE_END;

protected:
	/* 최대 차징 시간 */
	_uint				m_iMaxChargeFrame = 100;

	/* m_iMaxChargeFrame에 도달 시 이 상태로 넘어감. */
	STATE_TYPE			m_eChargeAttackState = STATE_END;

	/* 해당 키가 None이 되면 상태가 풀림 */
	KEY					m_eCurrentChargeKey = KEY::LAST;


	// 해당 키를 떼고 이 프레임만큼 지나면 상태가 풀림 
	_uint				m_iChargeChangeKeyFrame = 0;



	virtual void	On_MaxCharge(CUnit* pOwner, CAnimator* pAnimator);

protected:
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	/*사전 설정 아니고, 안에서 쓰는 것*/
	Enum				m_eEnum = Enum::eIDLE;
	_bool				m_bBlendable = true;
	_bool				m_bCycling = false;
	_bool				m_bChargeFinish = false;

private:
	void				Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType);
	void				On_EnumChange(Enum eEnum, CAnimator* pAnimator);

private:
	STATE_TYPE	Update_Walk(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Run(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Jump(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Fall(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Land(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Idle(CUnit* pOwner, CAnimator* pAnimator);
};

END