#pragma once
#include "CValkyrie_Attack_HorizontalDown.h"


BEGIN(Client)
class CColorController;

class CValkyrie_Attack_HorizontalDown_L
	: public CValkyrie_Attack_HorizontalDown
{
	DECLARE_STATE(CValkyrie_Attack_HorizontalDown_L);

private:
	CValkyrie_Attack_HorizontalDown_L();
	virtual ~CValkyrie_Attack_HorizontalDown_L();

public:
	static CValkyrie_Attack_HorizontalDown_L* Create();

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