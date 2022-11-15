#pragma once
#include "CStop_SpearMan.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CStop_SpearMan_L
	: public CStop_SpearMan
{
	DECLARE_STATE(CStop_SpearMan_L);

private:
	CStop_SpearMan_L();
	virtual ~CStop_SpearMan_L();

public:
	static CStop_SpearMan_L* Create();

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