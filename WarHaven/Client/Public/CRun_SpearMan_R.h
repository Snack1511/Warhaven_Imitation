#pragma once
#include "CRun_SpearMan.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_SpearMan_R
	: public CRun_SpearMan
{
	DECLARE_STATE(CRun_SpearMan_R);

private:
	CRun_SpearMan_R();
	virtual ~CRun_SpearMan_R();

public:
	static CRun_SpearMan_R* Create();

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