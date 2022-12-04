#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CColorController;

class CArcher_Attack_Cancel
	: public CState_Blendable
{
	DECLARE_STATE(CArcher_Attack_Cancel);

private:
	CArcher_Attack_Cancel();
	virtual ~CArcher_Attack_Cancel();

public:
	static CArcher_Attack_Cancel* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END