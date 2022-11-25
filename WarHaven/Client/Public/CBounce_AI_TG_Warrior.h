#pragma once
#include "CState.h"

BEGIN(Client)
class CBounce_AI_TG_Warrior abstract
	: public CState
{

protected:
	CBounce_AI_TG_Warrior();
	virtual ~CBounce_AI_TG_Warrior();


public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;



};

END