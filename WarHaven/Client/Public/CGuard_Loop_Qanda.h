#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Loop_Qanda
	: public CState
{
	DECLARE_STATE(CGuard_Loop_Qanda);

private:
	CGuard_Loop_Qanda();
	virtual ~CGuard_Loop_Qanda();

public:
	static CGuard_Loop_Qanda* Create();

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