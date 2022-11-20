#pragma once
#include "CState.h"

class CState_Hit abstract
	: public CState
{
protected:
	CState_Hit();
	virtual ~CState_Hit();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;

public:

	_uint			m_iHitLeftIndex = 3;
	_uint			m_iHitRightIndex = 4;
	/* ÂßÂß Ãß°¡ÇÏ¼À*/


};

