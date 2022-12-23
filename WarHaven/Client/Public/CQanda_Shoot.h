#pragma once
#include "CShootAttack_Qanda.h"

BEGIN(Client)
class CColorController;

class CQanda_Shoot
	: public CShootAttack_Qanda
{
	DECLARE_STATE(CQanda_Shoot);

private:
	CQanda_Shoot();
	virtual ~CQanda_Shoot();

public:
	static CQanda_Shoot* Create();

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