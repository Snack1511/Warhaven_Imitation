#pragma once
#include "CState_Combat_Run.h"

BEGIN(Client)
class CState_Combat_Default_Archer abstract
	: public CState_Combat_Run
{

protected:
	CState_Combat_Default_Archer();
	virtual ~CState_Combat_Default_Archer();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	STATE_TYPE	Near_Enemy(CUnit* pOwner, _bool bUseAdjSkill);


};

END