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
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos) override;

private:
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);



};

END