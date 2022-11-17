#pragma once
#include "CWarHammer_Attack_Sting.h"

BEGIN(Client)
class CColorController;

class CWarHammer_Attack_Sting_L
	: public CWarHammer_Attack_Sting
{
	DECLARE_STATE(CWarHammer_Attack_Sting_L);

private:
	CWarHammer_Attack_Sting_L();
	virtual ~CWarHammer_Attack_Sting_L();

public:
	static CWarHammer_Attack_Sting_L* Create();

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