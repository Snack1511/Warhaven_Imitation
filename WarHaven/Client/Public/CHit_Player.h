#pragma once
#include "CState_Hit.h"

BEGIN(Client)
class CColorController;

class CHit_Player
	: public CState_Hit
{
	DECLARE_STATE(CHit_Player);

private:
	CHit_Player();
	virtual ~CHit_Player();

public:
	static CHit_Player* Create();

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