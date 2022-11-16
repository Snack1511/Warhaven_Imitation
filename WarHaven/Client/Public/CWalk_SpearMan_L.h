#pragma once
#include "CWalk_SpearMan.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CWalk_SpearMan_L
	: public CWalk_SpearMan
{
	DECLARE_STATE(CWalk_SpearMan_L);

protected:
	CWalk_SpearMan_L();
	virtual ~CWalk_SpearMan_L();

public:
	static CWalk_SpearMan_L* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
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