#pragma once
#include "CAttack_Qanda.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CColorController;

class CQanda_Aiming
	: public CAttack_Qanda
{
	DECLARE_STATE(CQanda_Aiming);

private:
	CQanda_Aiming();
	virtual ~CQanda_Aiming();

public:
	static CQanda_Aiming* Create();

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