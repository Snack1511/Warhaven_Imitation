#pragma once
#include "CRun_SpearmMan_Begin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_SpearmMan_Begin_L
	: public CRun_SpearmMan_Begin
{
	DECLARE_STATE(CRun_SpearmMan_Begin_L);

private:
	CRun_SpearmMan_Begin_L();
	virtual ~CRun_SpearmMan_Begin_L();

public:
	static CRun_SpearmMan_Begin_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void	On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


};

END