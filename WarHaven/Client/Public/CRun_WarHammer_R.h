#pragma once
#include "CRun_WarHammer.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_WarHammer_R
	: public CRun_WarHammer
{
	DECLARE_STATE(CRun_WarHammer_R);

private:
	CRun_WarHammer_R();
	virtual ~CRun_WarHammer_R();

public:
	static CRun_WarHammer_R* Create();

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