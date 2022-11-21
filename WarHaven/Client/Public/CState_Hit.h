#pragma once
#include "CState.h"

class CState_Hit abstract
	: public CState
{

public:
	enum HIT_STATE { HIT_STATE_E, HIT_STATE_N, HIT_STATE_S, HIT_STATE_W, HIT_STATE_END };
	enum GUARD_STATE { GUARD_STATE_F, GUARD_STATE_L, GUARD_STATE_R, GUARD_STATE_TOP, GUARD_STATE_END };
	enum GROGGY_STATE { GROGGY_STATE_E, GROGGY_STATE_N, GROGGY_STATE_S, GROGGY_STATE_W, GROGGY_STATE_PLACE, GROGGY_STATE_END };


protected:
	CState_Hit();
	virtual ~CState_Hit();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;



protected:
	_uint			m_iHitIndex[HIT_STATE_END]; // 히트 상태
	_uint			m_iGuardIndex[GUARD_STATE_END]; // 가드 성공
	_uint			m_iHitStabIndex[HIT_STATE_END]; // 찌르기 사망 애니메이션

	_uint			m_iGroggyIndex[GROGGY_STATE_END]; // 그로기 상태(가만히 기절 상태도 있음)

	_uint			m_iFallHitIndex = 0;
	_uint			m_iFlyHitIndex = 0;


	_uint			m_iHitLeftIndex = 3;
	_uint			m_iHitRightIndex = 4;
	/* 쭉쭉 추가하셈*/

private:
	void	Face_Change(_uint iDest, _uint iSour);

};

