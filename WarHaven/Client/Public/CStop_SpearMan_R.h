#pragma once
#include "CStop_SpearMan.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CStop_SpearMan_R
	: public CStop_SpearMan
{
	DECLARE_STATE(CStop_SpearMan_R);

private:
	CStop_SpearMan_R();
	virtual ~CStop_SpearMan_R();

public:
	static CStop_SpearMan_R* Create();

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