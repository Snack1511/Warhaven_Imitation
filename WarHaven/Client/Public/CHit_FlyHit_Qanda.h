#pragma once
#include "CState_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CHit_FlyHit_Qanda
	: public CState_Hit
{
	DECLARE_STATE(CHit_FlyHit_Qanda);

private:
	CHit_FlyHit_Qanda();
	virtual ~CHit_FlyHit_Qanda();

public:
	static CHit_FlyHit_Qanda* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	bTest = false; 

};

END