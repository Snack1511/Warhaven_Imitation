#pragma once
#include "CState.h"

BEGIN(Client)
class CColorController;

class CWarrior_Oxen_Loop
	: public CState
{
	DECLARE_STATE(CWarrior_Oxen_Loop);

private:
	CWarrior_Oxen_Loop();
	virtual ~CWarrior_Oxen_Loop();

public:
	static CWarrior_Oxen_Loop* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	void Play_Skill(_uint iChangeIndex, _float fInterPolationTime, CUnit* pOwner, CAnimator* pAnimator);

private:
	_int	m_iCurFinishedFrame = 0; // Finished �� 3�� ȣ��Ǹ� Ȳ�Һ��Ⱑ ��ҵȴ�.
	_int	m_iEndFinishedFrame = 0;

};

END