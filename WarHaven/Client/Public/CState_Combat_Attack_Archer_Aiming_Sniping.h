#pragma once
#include "CState_Combat_Attack_Archer.h"

BEGIN(Client)
class CColorController;

class CState_Combat_Attack_Archer_Aiming_Sniping
	: public CState_Combat_Attack_Archer
{
	DECLARE_STATE(CState_Combat_Attack_Archer_Aiming_Sniping);

private:
	CState_Combat_Attack_Archer_Aiming_Sniping();
	virtual ~CState_Combat_Attack_Archer_Aiming_Sniping();

public:
	static CState_Combat_Attack_Archer_Aiming_Sniping* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END