#pragma once
#include "CState_Combat.h"

BEGIN(Client)
class CState_Combat_Stop_Lancer
	: public CState_Combat
{
	DECLARE_STATE(CState_Combat_Stop_Lancer);

protected:
	CState_Combat_Stop_Lancer();
	virtual ~CState_Combat_Stop_Lancer();

public:
	static CState_Combat_Stop_Lancer* Create();

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