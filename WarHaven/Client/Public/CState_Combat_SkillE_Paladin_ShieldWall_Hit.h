#pragma once
#include "CState_Combat_SkillE.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CState_Combat_SkillE_Paladin_ShieldWall_Hit
	: public CState_Combat_SkillE
{
	DECLARE_STATE(CState_Combat_SkillE_Paladin_ShieldWall_Hit);

private:
	CState_Combat_SkillE_Paladin_ShieldWall_Hit();
	virtual ~CState_Combat_SkillE_Paladin_ShieldWall_Hit();

public:
	static CState_Combat_SkillE_Paladin_ShieldWall_Hit* Create();

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