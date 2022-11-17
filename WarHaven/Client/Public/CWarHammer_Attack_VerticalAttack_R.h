#pragma once
#include "CWarHammer_Attack_VerticalAttack.h"

BEGIN(Client)
class CColorController;

class CWarHammer_Attack_VerticalAttack_R
	: public CWarHammer_Attack_VerticalAttack
{
	DECLARE_STATE(CWarHammer_Attack_VerticalAttack_R);

private:
	CWarHammer_Attack_VerticalAttack_R();
	virtual ~CWarHammer_Attack_VerticalAttack_R();

public:
	static CWarHammer_Attack_VerticalAttack_R* Create();

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