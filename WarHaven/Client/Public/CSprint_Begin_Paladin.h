#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Begin_Paladin
	: public CState
{
	DECLARE_STATE(CSprint_Begin_Paladin);

private:
	CSprint_Begin_Paladin();
	virtual ~CSprint_Begin_Paladin();

public:
	static CSprint_Begin_Paladin* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);




};

END