#pragma once
#include "CRun_Valkyrie.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CRun_Valkyrie_L
	: public CRun_Valkyrie
{
	DECLARE_STATE(CRun_Valkyrie_L);

private:
	CRun_Valkyrie_L();
	virtual ~CRun_Valkyrie_L();

public:
	static CRun_Valkyrie_L* Create();

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