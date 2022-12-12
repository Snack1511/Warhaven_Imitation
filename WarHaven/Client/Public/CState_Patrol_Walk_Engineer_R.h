#pragma once
#include "CState_Patrol_Walk_Engineer.h"

BEGIN(Client)
class CState_Patrol_Walk_Engineer_R
	: public CState_Patrol_Walk_Engineer
{
	DECLARE_STATE(CState_Patrol_Walk_Engineer_R);

protected:
	CState_Patrol_Walk_Engineer_R();
	virtual ~CState_Patrol_Walk_Engineer_R();

public:
	static CState_Patrol_Walk_Engineer_R* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END