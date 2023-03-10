#pragma once
#include "CJump_Player.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CJump_SpearMan_R
	: public CJump_Player
{
	DECLARE_STATE(CJump_SpearMan_R);

private:
	CJump_SpearMan_R();
	virtual ~CJump_SpearMan_R();

public:
	static CJump_SpearMan_R* Create();

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