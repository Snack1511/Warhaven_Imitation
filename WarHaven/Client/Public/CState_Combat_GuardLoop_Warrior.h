#pragma once
#include "CState_Combat.h"

BEGIN(Client)
class CState_Combat_GuardLoop_Warrior
	: public CState_Combat
{
	DECLARE_STATE(CState_Combat_GuardLoop_Warrior);

private:
	CState_Combat_GuardLoop_Warrior();
	virtual ~CState_Combat_GuardLoop_Warrior();

public:
	static CState_Combat_GuardLoop_Warrior* Create();

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