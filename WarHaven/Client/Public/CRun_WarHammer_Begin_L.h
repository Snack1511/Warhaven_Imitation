#pragma once
#include "CRun_WarHammer_Begin.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_WarHammer_Begin_L
	: public CRun_WarHammer_Begin
{
	DECLARE_STATE(CRun_WarHammer_Begin_L);

private:
	CRun_WarHammer_Begin_L();
	virtual ~CRun_WarHammer_Begin_L();

public:
	static CRun_WarHammer_Begin_L* Create();

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