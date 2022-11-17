#pragma once
#include "CRun_WarHammer.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_WarHammer_L
	: public CRun_WarHammer
{
	DECLARE_STATE(CRun_WarHammer_L);

private:
	CRun_WarHammer_L();
	virtual ~CRun_WarHammer_L();

public:
	static CRun_WarHammer_L* Create();

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