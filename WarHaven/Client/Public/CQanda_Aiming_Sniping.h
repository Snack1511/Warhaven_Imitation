#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CQanda_Aiming_Sniping
	: public CState
{
	DECLARE_STATE(CQanda_Aiming_Sniping);

private:
	CQanda_Aiming_Sniping();
	virtual ~CQanda_Aiming_Sniping();

public:
	static CQanda_Aiming_Sniping* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float				m_fCreateProjectileTime = 0.f;

};

END