#pragma once
#include "CState_Common_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CState_Common_GuardHit_Qanda
	: public CState_Common_Hit
{
	DECLARE_STATE(CState_Common_GuardHit_Qanda);

private:
	CState_Common_GuardHit_Qanda();
	virtual ~CState_Common_GuardHit_Qanda();

public:
	static CState_Common_GuardHit_Qanda* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_uint	m_iShadowStepDelay = 10; 

};

END