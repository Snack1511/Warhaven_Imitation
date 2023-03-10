#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_Qanda
	: public CState
{
	DECLARE_STATE(CWalk_Qanda);

protected:
	CWalk_Qanda();
	virtual ~CWalk_Qanda();

public:
	static CWalk_Qanda* Create();

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