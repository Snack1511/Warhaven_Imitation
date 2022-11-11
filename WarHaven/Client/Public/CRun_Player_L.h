#pragma once
#include "CRun_Player.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_Player_L
	: public CRun_Player
{
	DECLARE_STATE(CRun_Player_L);

private:
	CRun_Player_L();
	virtual ~CRun_Player_L();

public:
	static CRun_Player_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END