#pragma once
#include "CWalk_Valkyrie.h"

BEGIN(Client)
class CWalk_Valkyrie_L
	: public CWalk_Valkyrie
{
	DECLARE_STATE(CWalk_Valkyrie_L);

protected:
	CWalk_Valkyrie_L();
	virtual ~CWalk_Valkyrie_L();

public:
	static CWalk_Valkyrie_L* Create();

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