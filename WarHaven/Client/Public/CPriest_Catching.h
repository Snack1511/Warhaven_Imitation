#pragma once
#include "CState.h"


BEGIN(Client)
class CPriest_Catching 
	: public CState
{
	DECLARE_STATE(CPriest_Catching);

	enum class Enum {eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE};

	//이 클래스 상속받아서 쓰면 애니메이션 중에 움직일 수 있음
private:
	CPriest_Catching();
	virtual ~CPriest_Catching();


public:
	static CPriest_Catching* Create();

public:
	virtual HRESULT		Initialize() override;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//L, R 여부
	ANIM_TYPE	m_eAnimLeftorRight = ANIM_BASE_R;

	/* 몇 프레임까지 블렌드 할 지.
	1. 이 프레임까지만 걸으면서 공격함
	2. 이 프레임에 도달하면 앞으로 살짝 전진함
	3. 공격 시작 프레임
	4. 칼 잔상 생성 켜짐
	5. Trail Effect 켜짐 */
	_uint		m_iStopIndex = 9999;

	/* 
	1. 공격이 종료되는 프레임
	2. 칼 잔상 생성 꺼짐
	3. Trail Effect 꺼짐 */
	_uint		m_iAttackEndIndex = 9999;

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

	/*Land 시 움직일 수 있도록 설정*/
	_bool	m_bLandMove = false;

	_uint	m_iKeyAwayAnimIndex = 0; // 뗄 때 애니메이션 인덱스
	_uint	m_iMinCancelAnimIndex = 0;  // 애니메이션 바뀌는 최소값

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


private:
	/*사전 설정 아니고, 안에서 쓰는 것*/
	Enum				m_eEnum = Enum::eIDLE;
	_bool				m_bBlendable = true;
	_bool				m_bCycling = false;
	_bool				m_bHitEffect = false;

private:
	_bool		m_bAfterEffect = false;
	_bool		m_bBlood = false;
	_float		m_fCreateTimeAcc = 0.f;
	_float		m_fCreateTime = 1.f;
	_float		m_fCreateTimeSlow = 0.07f;
	void		Create_SoilEffect(); 

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