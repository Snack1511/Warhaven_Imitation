#pragma once
#include "CRun_Paladin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_Paladin_L
	: public CRun_Paladin
{
	DECLARE_STATE(CRun_Paladin_L);

private:
	CRun_Paladin_L();
	virtual ~CRun_Paladin_L();

public:
	static CRun_Paladin_L* Create();

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