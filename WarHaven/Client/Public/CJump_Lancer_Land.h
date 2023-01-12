#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END
 
BEGIN(Client)
class CJump_Lancer_Land
	: public CState
{
	DECLARE_STATE(CJump_Lancer_Land);

private:
	CJump_Lancer_Land();
	virtual ~CJump_Lancer_Land();

public:
	static CJump_Lancer_Land* Create();

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