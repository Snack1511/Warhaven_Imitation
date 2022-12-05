#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Loop_Paladin
	: public CState
{
	DECLARE_STATE(CSprint_Loop_Paladin);

private:
	CSprint_Loop_Paladin();
	virtual ~CSprint_Loop_Paladin();

public:
	static CSprint_Loop_Paladin* Create();

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