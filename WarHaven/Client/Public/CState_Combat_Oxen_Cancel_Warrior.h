#pragma once
#include "CState_Combat_SkillQ.h"

BEGIN(Client)
class CColorController;

class CState_Combat_Oxen_Cancel_Warrior
	: public CState_Combat_SkillQ
{
	DECLARE_STATE(CState_Combat_Oxen_Cancel_Warrior);

private:
	CState_Combat_Oxen_Cancel_Warrior();
	virtual ~CState_Combat_Oxen_Cancel_Warrior();

public:
	static CState_Combat_Oxen_Cancel_Warrior* Create();

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

};

END