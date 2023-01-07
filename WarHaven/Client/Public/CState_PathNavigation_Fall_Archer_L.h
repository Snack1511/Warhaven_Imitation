#pragma once
#include "CState_PathNavigation_Fall_Archer.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CState_PathNavigation_Fall_Archer_L final
	: public CState_PathNavigation_Fall_Archer
{
	DECLARE_STATE(CState_PathNavigation_Fall_Archer_L);

private:
	CState_PathNavigation_Fall_Archer_L();
	virtual ~CState_PathNavigation_Fall_Archer_L();

public:
	static CState_PathNavigation_Fall_Archer_L* Create();

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