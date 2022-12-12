#pragma once
#include "CState_Patrol_Default_Fiona.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Default_Fiona_L
	: public CState_Patrol_Default_Fiona
{
	DECLARE_STATE(CState_Patrol_Default_Fiona_L);

private:
	CState_Patrol_Default_Fiona_L();
	virtual ~CState_Patrol_Default_Fiona_L();

public:
	static CState_Patrol_Default_Fiona_L* Create();

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