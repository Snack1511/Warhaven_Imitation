#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CQanda_Shoot_Sniping
	: public CState
{
	DECLARE_STATE(CQanda_Shoot_Sniping);

private:
	CQanda_Shoot_Sniping();
	virtual ~CQanda_Shoot_Sniping();

public:
	static CQanda_Shoot_Sniping* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


private:
	list<CGameObject*>	m_SnipingTarget;
	list<CGameObject*>	m_Mateors;
};

END