#pragma once
#include "CWarrior_Attack_HorizontalUp.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_HorizontalUp_L
	: public CWarrior_Attack_HorizontalUp
{
	DECLARE_STATE(CWarrior_Attack_HorizontalUp_L);

private:
	CWarrior_Attack_HorizontalUp_L();
	virtual ~CWarrior_Attack_HorizontalUp_L();

public:
	static CWarrior_Attack_HorizontalUp_L* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);
	
};

END