#pragma once
#include "CState.h"


BEGIN(Client)
class CState_Catch_Catching_Priest 
	: public CState
{
	DECLARE_STATE(CState_Catch_Catching_Priest);

private:
	CState_Catch_Catching_Priest();
	virtual ~CState_Catch_Catching_Priest();


public:
	static CState_Catch_Catching_Priest* Create();

public:
	virtual HRESULT		Initialize() override;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END