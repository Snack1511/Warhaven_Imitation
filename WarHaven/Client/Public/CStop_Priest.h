#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CStop_Priest
	: public CState
{
	DECLARE_STATE(CStop_Priest);

private:
	CStop_Priest();
	virtual ~CStop_Priest();

public:
	static CStop_Priest* Create();

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