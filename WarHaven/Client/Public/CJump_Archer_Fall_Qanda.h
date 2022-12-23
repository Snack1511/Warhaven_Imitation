
#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CJump_Archer_Fall_Qanda
	: public CState
{
	DECLARE_STATE(CJump_Archer_Fall_Qanda);

private:
	CJump_Archer_Fall_Qanda();
	virtual ~CJump_Archer_Fall_Qanda();

public:
	static CJump_Archer_Fall_Qanda* Create();

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
