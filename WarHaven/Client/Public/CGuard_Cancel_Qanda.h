#pragma once
#include "CState_Blendable.h"

BEGIN(Client)
class CGuard_Cancel_Qanda
	: public CState_Blendable
{
	DECLARE_STATE(CGuard_Cancel_Qanda);

private:
	CGuard_Cancel_Qanda();
	virtual ~CGuard_Cancel_Qanda();

public:
	static CGuard_Cancel_Qanda* Create();

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