#pragma once
#include "CState_Common_Bounce.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CState_Common_Bounce_Lancer
	: public CState_Common_Bounce
{
	DECLARE_STATE(CState_Common_Bounce_Lancer);

private:
	CState_Common_Bounce_Lancer();
	virtual ~CState_Common_Bounce_Lancer();

public:
	static CState_Common_Bounce_Lancer* Create();

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