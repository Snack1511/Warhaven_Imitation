#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CPaladin_ShieldWall_Loop
	: public CState
{
	DECLARE_STATE(CPaladin_ShieldWall_Loop);

private:
	CPaladin_ShieldWall_Loop();
	virtual ~CPaladin_ShieldWall_Loop();

public:
	static CPaladin_ShieldWall_Loop* Create();

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