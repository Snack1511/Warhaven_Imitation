#pragma once
#include "CState_Combat_SkillQ_Engineer.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CState_Combat_SkillQ_AirSpike_Loop
	: public CState_Combat_SkillQ_Engineer
{
	DECLARE_STATE(CState_Combat_SkillQ_AirSpike_Loop);

private:
	CState_Combat_SkillQ_AirSpike_Loop();
	virtual ~CState_Combat_SkillQ_AirSpike_Loop();

public:
	static CState_Combat_SkillQ_AirSpike_Loop* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END