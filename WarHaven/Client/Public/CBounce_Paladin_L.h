#pragma once
#include "CBounce_Paladin.h"

BEGIN(Engine)
END

BEGIN(Client)

class CColorController;

class CBounce_Paladin_L
	: public CBounce_Paladin
{
	DECLARE_STATE(CBounce_Paladin_L);

private:
	CBounce_Paladin_L();
	virtual ~CBounce_Paladin_L();

public:
	static CBounce_Paladin_L* Create();

public:
	// CPlayer_Bounce��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END