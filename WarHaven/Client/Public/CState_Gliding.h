#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Gliding
	: public CState
{
	DECLARE_STATE(CState_Gliding);

private:
	CState_Gliding();
	virtual ~CState_Gliding();

public:
	static CState_Gliding* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	m_bReturn = false;

	_float m_fSndTime = 0.f;
	_uint m_iSndIdx = 0;

};

END