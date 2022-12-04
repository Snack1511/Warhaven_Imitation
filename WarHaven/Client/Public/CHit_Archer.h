#pragma once
#include "CState_Hit.h"

BEGIN(Client)
class CColorController;

class CHit_Archer
	: public CState_Hit
{
	DECLARE_STATE(CHit_Archer);

private:
	CHit_Archer();
	virtual ~CHit_Archer();

public:
	static CHit_Archer* Create();

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