#pragma once
#include "CState_Common_Bounce_Fiona.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CState_Common_Bounce_Fiona_R
	: public CState_Common_Bounce_Fiona
{
	DECLARE_STATE(CState_Common_Bounce_Fiona_R);

private:
	CState_Common_Bounce_Fiona_R();
	virtual ~CState_Common_Bounce_Fiona_R();

public:
	static CState_Common_Bounce_Fiona_R* Create();

public:
	// CPlayer_Bounce을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END