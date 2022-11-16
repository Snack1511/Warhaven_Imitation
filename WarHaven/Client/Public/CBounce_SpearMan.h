#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CBounce_SpearMan
	: public CState
{
	DECLARE_STATE(CBounce_SpearMan);

private:
	CBounce_SpearMan();
	virtual ~CBounce_SpearMan();

public:
	static CBounce_SpearMan* Create();

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