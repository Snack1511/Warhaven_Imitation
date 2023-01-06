#pragma once
#include "CState_Patrol.h"

BEGIN(Client)
class CColorController;

class CState_Patrol_Cure_Begin_Priest
	: public CState_Patrol
{
	DECLARE_STATE(CState_Patrol_Cure_Begin_Priest);

private:
	CState_Patrol_Cure_Begin_Priest();
	virtual ~CState_Patrol_Cure_Begin_Priest();

public:
	static CState_Patrol_Cure_Begin_Priest* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


};

END