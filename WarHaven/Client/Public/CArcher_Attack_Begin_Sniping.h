#pragma once
#include "CAttack_Archer.h"

BEGIN(Client)
class CColorController;

class CArcher_Attack_Begin_Sniping
	: public CAttack_Archer
{
	DECLARE_STATE(CArcher_Attack_Begin_Sniping);

private:
	CArcher_Attack_Begin_Sniping();
	virtual ~CArcher_Attack_Begin_Sniping();

public:
	static CArcher_Attack_Begin_Sniping* Create();

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