#pragma once
#include "CRun_SpearMan.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_SpearMan_L
	: public CRun_SpearMan
{
	DECLARE_STATE(CRun_SpearMan_L);

private:
	CRun_SpearMan_L();
	virtual ~CRun_SpearMan_L();

public:
	static CRun_SpearMan_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END