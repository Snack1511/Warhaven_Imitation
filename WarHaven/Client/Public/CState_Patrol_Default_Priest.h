#pragma once
#include "CState_Patrol_Default.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Default_Priest
	: public CState_Patrol_Default
{
	DECLARE_STATE(CState_Patrol_Default_Priest);

private:
	CState_Patrol_Default_Priest();
	virtual ~CState_Patrol_Default_Priest();

public:
	static CState_Patrol_Default_Priest* Create();

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