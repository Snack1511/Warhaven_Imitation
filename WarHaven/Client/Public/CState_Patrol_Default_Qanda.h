#pragma once
#include "CState_Patrol_Default.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Default_Qanda
	: public CState_Patrol_Default
{
	DECLARE_STATE(CState_Patrol_Default_Qanda);

private:
	CState_Patrol_Default_Qanda();
	virtual ~CState_Patrol_Default_Qanda();

public:
	static CState_Patrol_Default_Qanda* Create();

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