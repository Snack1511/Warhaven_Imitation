#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CRun_SpearmMan_Begin abstract
	: public CState
{

protected:
	CRun_SpearmMan_Begin();
	virtual ~CRun_SpearmMan_Begin();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void	On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	void	Change_Location_Begin(_uint iDirection, CAnimator* pAnimator);


};

END

