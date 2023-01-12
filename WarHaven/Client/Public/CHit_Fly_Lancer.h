#pragma once
#include "CState_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CHit_Fly_Lancer
	: public CState_Hit
{
	DECLARE_STATE(CHit_Fly_Lancer);

private:
	CHit_Fly_Lancer();
	virtual ~CHit_Fly_Lancer();

public:
	static CHit_Fly_Lancer* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	bTest = false; 

};

END