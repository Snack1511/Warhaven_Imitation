#pragma once
#include "CState_Combat_Shoot_Archer.h"

BEGIN(Client)
class CColorController;

class CState_Combat_Attack_Archer_Shoot_Sniping
	: public CState_Combat_Shoot_Archer
{
	DECLARE_STATE(CState_Combat_Attack_Archer_Shoot_Sniping);

private:
	CState_Combat_Attack_Archer_Shoot_Sniping();
	virtual ~CState_Combat_Attack_Archer_Shoot_Sniping();

public:
	static CState_Combat_Attack_Archer_Shoot_Sniping* Create();

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