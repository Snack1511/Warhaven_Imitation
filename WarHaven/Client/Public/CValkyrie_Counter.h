#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
class CHierarchyNode;
END

BEGIN(Client)
class CColorController;

class CValkyrie_Counter
	: public CState
{
	DECLARE_STATE(CValkyrie_Counter);

private:
	CValkyrie_Counter();
	virtual ~CValkyrie_Counter();

public:
	static CValkyrie_Counter* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	 

};

END