#pragma once
#include "CAttack_Archer.h"

BEGIN(Client)
class CColorController;

class CArcher_Attack_Begin_Poison
	: public CAttack_Archer
{
	DECLARE_STATE(CArcher_Attack_Begin_Poison);

private:
	CArcher_Attack_Begin_Poison();
	virtual ~CArcher_Attack_Begin_Poison();

public:
	static CArcher_Attack_Begin_Poison* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

};

END