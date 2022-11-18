#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_End_WarHammer
	: public CState
{
	DECLARE_STATE(CGuard_End_WarHammer);

private:
	CGuard_End_WarHammer();
	virtual ~CGuard_End_WarHammer();

public:
	static CGuard_End_WarHammer* Create();

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