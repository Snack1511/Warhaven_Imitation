#pragma once
#include "CStop_Valkyrie.h"

BEGIN(Client)
class CColorController;

class CStop_Valkyrie_R
	: public CStop_Valkyrie
{
	DECLARE_STATE(CStop_Valkyrie_R);

private:
	CStop_Valkyrie_R();
	virtual ~CStop_Valkyrie_R();

public:
	static CStop_Valkyrie_R* Create();

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