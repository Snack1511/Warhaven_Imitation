#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CBounce_Priest
	: public CState
{
	DECLARE_STATE(CBounce_Priest);

private:
	CBounce_Priest();
	virtual ~CBounce_Priest();

public:
	static CBounce_Priest* Create();

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