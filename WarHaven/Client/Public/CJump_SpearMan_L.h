#pragma once
#include "CJump_Player.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CJump_SpearMan_L
	: public CJump_Player
{
	DECLARE_STATE(CJump_SpearMan_L);

private:
	CJump_SpearMan_L();
	virtual ~CJump_SpearMan_L();

public:
	static CJump_SpearMan_L* Create();

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