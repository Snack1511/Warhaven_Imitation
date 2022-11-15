#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CWarrior_Attack_Sting_L
	: public CState_Blendable
{
	DECLARE_STATE(CWarrior_Attack_Sting_L);

private:
	CWarrior_Attack_Sting_L();
	virtual ~CWarrior_Attack_Sting_L();

public:
	static CWarrior_Attack_Sting_L* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);
	
};

END