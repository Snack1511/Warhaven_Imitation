#pragma once
#include "CState_Common_Land.h"

BEGIN(Engine)
class CAnimator;
END
 
BEGIN(Client)
class CState_Common_Land_Qanda
	: public CState_Common_Land
{
	DECLARE_STATE(CState_Common_Land_Qanda);

private:
	CState_Common_Land_Qanda();
	virtual ~CState_Common_Land_Qanda();

public:
	static CState_Common_Land_Qanda* Create();

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