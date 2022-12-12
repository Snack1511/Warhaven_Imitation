#pragma once
#include "CState_Combat_Attack_Fiona.h"

BEGIN(Client)
class CState_Combat_Attack_HorizontalMiddle_Fiona abstract
	: public CState_Combat_Attack_Fiona
{

protected:
	CState_Combat_Attack_HorizontalMiddle_Fiona();
	virtual ~CState_Combat_Attack_HorizontalMiddle_Fiona();


public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


};

END