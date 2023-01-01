#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CBounce_Lancer
	: public CState
{
	DECLARE_STATE(CBounce_Lancer);

private:
	CBounce_Lancer();
	virtual ~CBounce_Lancer();

public:
	static CBounce_Lancer* Create();

public:
	// CPlayer_Bounce을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END