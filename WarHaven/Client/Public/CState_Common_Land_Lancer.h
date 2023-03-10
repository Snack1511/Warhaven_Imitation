#pragma once
#include "CState_Common_Land.h"

BEGIN(Engine)
class CAnimator;
END
 
BEGIN(Client)
class CState_Common_Land_Lancer
	: public CState_Common_Land
{
	DECLARE_STATE(CState_Common_Land_Lancer);

private:
	CState_Common_Land_Lancer();
	virtual ~CState_Common_Land_Lancer();

public:
	static CState_Common_Land_Lancer* Create();

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