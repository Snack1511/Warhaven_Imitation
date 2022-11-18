#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_Jump_Fall_WarHammer
	: public CState
{
	DECLARE_STATE(CSprint_Jump_Fall_WarHammer);

private:
	CSprint_Jump_Fall_WarHammer();
	virtual ~CSprint_Jump_Fall_WarHammer();

public:
	static CSprint_Jump_Fall_WarHammer* Create();

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