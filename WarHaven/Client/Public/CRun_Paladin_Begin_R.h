#pragma once
#include "CRun_Paladin_Begin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_Paladin_Begin_R
	: public CRun_Paladin_Begin
{
	DECLARE_STATE(CRun_Paladin_Begin_R);

private:
	CRun_Paladin_Begin_R();
	virtual ~CRun_Paladin_Begin_R();

public:
	static CRun_Paladin_Begin_R* Create();

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