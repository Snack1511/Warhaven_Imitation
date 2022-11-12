#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Loop
	: public CState
{
	DECLARE_STATE(CSprint_Loop);

private:
	CSprint_Loop();
	virtual ~CSprint_Loop();

public:
	static CSprint_Loop* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


};

END