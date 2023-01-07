#pragma once
#include "CState_Hit.h"

BEGIN(Engine)
END


BEGIN(Client)
class CAI_SandBack_Hit
	: public CState_Hit
{
	DECLARE_STATE(CAI_SandBack_Hit);

private:
	CAI_SandBack_Hit();
	virtual ~CAI_SandBack_Hit();

public:
	static CAI_SandBack_Hit* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	void	Test_Attack(STATE_TYPE ePrevType); // 공격 상태로 가는 거 테스트용 : bTestAttack

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;




private:
	_bool	bTestGuard = false;  // 가드에서 맞고 Idle 로 가는 거 방지 : 테스트용
	_bool	bTestAttack = false;  // 어택에서 맞고 Idle 로 가는 거 방지 : 테스트용
	_bool	bTestAttackVertical = false;  // 어택에서 맞고 Idle 로 가는 거 방지 : 테스트용
	_bool	bWarHammerIdle = false;

	_bool	m_HitRight = false;

};

END