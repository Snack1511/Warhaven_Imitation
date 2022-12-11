#pragma once
#include "CState_Combat.h"

BEGIN(Client)
class CState_Combat_SkillR abstract
	: public CState_Combat
{

protected:
	CState_Combat_SkillR();
	virtual ~CState_Combat_SkillR();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_uint m_iWalkDelay = 0;

};

END