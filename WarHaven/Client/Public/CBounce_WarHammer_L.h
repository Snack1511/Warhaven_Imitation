#pragma once
#include "CBounce_WarHammer.h"

BEGIN(Engine)
END

BEGIN(Client)

class CColorController;

class CBounce_WarHammer_L
	: public CBounce_WarHammer
{
	DECLARE_STATE(CBounce_WarHammer_L);

private:
	CBounce_WarHammer_L();
	virtual ~CBounce_WarHammer_L();

public:
	static CBounce_WarHammer_L* Create();

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