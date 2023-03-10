#pragma once
#include "CState_Common_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CState_Common_Hit_Priest
	: public CState_Common_Hit
{
	DECLARE_STATE(CState_Common_Hit_Priest);

private:
	CState_Common_Hit_Priest();
	virtual ~CState_Common_Hit_Priest();

public:
	static CState_Common_Hit_Priest* Create();

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