#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CSlide_Loop_Player
	: public CState
{
	DECLARE_STATE(CSlide_Loop_Player);

private:
	CSlide_Loop_Player();
	virtual ~CSlide_Loop_Player();

public:
	static CSlide_Loop_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

//private:
//	_bool	Change_Animation_Run(_uint iBeginAttackAnim, _uint iAttackAnim, _uint iAttackAnim, CAnimator* pAnimator);

private:
	_bool	m_bPlayRun = false;

};

END