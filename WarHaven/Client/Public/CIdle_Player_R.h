#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CIdle_Player_R
	: public CState
{
	DECLARE_STATE(CIdle_Player_R);

private:
	CIdle_Player_R();
	virtual ~CIdle_Player_R();

public:
	static CIdle_Player_R* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float m_fCreateTime = 0.016f; //주기
	_float m_fCreateTimeAcc = 0.f;

private:
	_uint	m_iChangeHandIndex = 0;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void	Switch_Right_And_Left(_uint iAnimType, _uint iAnimIndex, _uint iChangeAnimIndex, CAnimator* pAnimator);
};

END