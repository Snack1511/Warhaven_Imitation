#pragma once
#include "CPaladin_Attack_HorizontalMiddle.h"

BEGIN(Client)
class CColorController;

class CPaladin_Attack_HorizontalMiddle_R
	: public CPaladin_Attack_HorizontalMiddle
{
	DECLARE_STATE(CPaladin_Attack_HorizontalMiddle_R);

private:
	CPaladin_Attack_HorizontalMiddle_R();
	virtual ~CPaladin_Attack_HorizontalMiddle_R();

public:
	static CPaladin_Attack_HorizontalMiddle_R* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_bool m_bTrigger = false;
};

END