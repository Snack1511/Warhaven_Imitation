#pragma once
#include "CState_Combat_SkillR.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CState_Combat_SkillR_Breeze_Loop_Lancer
	: public CState_Combat_SkillR
{
	DECLARE_STATE(CState_Combat_SkillR_Breeze_Loop_Lancer);

private:
	CState_Combat_SkillR_Breeze_Loop_Lancer();
	virtual ~CState_Combat_SkillR_Breeze_Loop_Lancer();

public:
	static CState_Combat_SkillR_Breeze_Loop_Lancer* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END