#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_Player
	: public CState
{
	DECLARE_STATE(CWalk_Player);

private:
	CWalk_Player();
	virtual ~CWalk_Player();

public:
	static CWalk_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	Change_Walk_Position(_uint iAnimIndex, ANIM_TYPE eAnimType)
	{
		if (m_iAnimIndex == iAnimIndex && m_eAnimType == eAnimType)
			return false;

		m_iAnimIndex = iAnimIndex;
		m_eAnimType = eAnimType;

		return true;
	}

};

END