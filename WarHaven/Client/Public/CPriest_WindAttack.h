#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CPriest_WindAttack
	: public CState_Blendable
{
	DECLARE_STATE(CPriest_WindAttack);

private:
	CPriest_WindAttack();
	virtual ~CPriest_WindAttack();

public:
	static CPriest_WindAttack* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_uint m_iAttackStartAnimIndex = 0;
	_uint m_iAttackEndAnimIndex = 0;

};

END