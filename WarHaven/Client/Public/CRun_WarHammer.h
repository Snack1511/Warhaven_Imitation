#pragma once
#include "CState.h"

BEGIN(Client)
class CRun_WarHammer abstract
	: public CState
{

protected:
	CRun_WarHammer();
	virtual ~CRun_WarHammer();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_uint		m_iCurDirection = 0;
	_float		m_fOriRunSpeed = 0.f;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END

