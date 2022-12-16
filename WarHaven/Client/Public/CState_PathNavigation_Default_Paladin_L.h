#pragma once
#include "CState_PathNavigation_Default_Paladin.h"

BEGIN(Client)
class CState_PathNavigation_Default_Paladin_L
	: public CState_PathNavigation_Default_Paladin
{
	DECLARE_STATE(CState_PathNavigation_Default_Paladin_L);

protected:
	CState_PathNavigation_Default_Paladin_L();
	virtual ~CState_PathNavigation_Default_Paladin_L();

public:
	static CState_PathNavigation_Default_Paladin_L* Create();

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