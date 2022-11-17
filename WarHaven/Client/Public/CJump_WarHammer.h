#pragma once
#include "CState.h"

BEGIN(Client)
class CJump_WarHammer abstract
	: public CState
{

protected:
	CJump_WarHammer();
	virtual ~CJump_WarHammer();


public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_int	iPlaceJumpAnimIndex = 0; // ���ڸ� ���� AnimIndex;

};

END