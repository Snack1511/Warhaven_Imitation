#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CPriest_Catch_Begin
	: public CState_Blendable
{
	DECLARE_STATE(CPriest_Catch_Begin);

private:
	CPriest_Catch_Begin();
	virtual ~CPriest_Catch_Begin();

public:
	static CPriest_Catch_Begin* Create();

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