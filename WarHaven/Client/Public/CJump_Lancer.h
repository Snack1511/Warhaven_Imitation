#pragma once
#include "CState.h"


BEGIN(Client)
class CJump_Lancer
	: public CState
{
	DECLARE_STATE(CJump_Lancer);

private:
	CJump_Lancer();
	virtual ~CJump_Lancer();

public:
	static CJump_Lancer* Create();

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
	_uint iPlaceJumpAnimIndex = 0;
	_uint iFallKeyFrame = 0;
};

END