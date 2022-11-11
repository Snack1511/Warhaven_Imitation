#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Oxen_Loop_Attack
	: public CState
{
	DECLARE_STATE(CWarrior_Oxen_Loop_Attack);

private:
	CWarrior_Oxen_Loop_Attack();
	virtual ~CWarrior_Oxen_Loop_Attack();

public:
	static CWarrior_Oxen_Loop_Attack* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void Play_Skill(_uint iChangeIndex, _float fInterPolationTime, CUnit* pOwner, CAnimator* pAnimator);

};

END