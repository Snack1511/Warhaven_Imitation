#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Jump_Archer
	: public CState
{
	DECLARE_STATE(CSprint_Jump_Archer);

private:
	CSprint_Jump_Archer();
	virtual ~CSprint_Jump_Archer();

public:
	static CSprint_Jump_Archer* Create();

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