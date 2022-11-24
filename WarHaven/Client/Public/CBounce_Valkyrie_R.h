#pragma once
#include "CBounce_Valkyrie.h"

BEGIN(Engine)
END

BEGIN(Client)
class CColorController;

class CBounce_Valkyrie_R
	: public CBounce_Valkyrie
{
	DECLARE_STATE(CBounce_Valkyrie_R);

private:
	CBounce_Valkyrie_R();
	virtual ~CBounce_Valkyrie_R();

public:
	static CBounce_Valkyrie_R* Create();

public:
	// CPlayer_Bounce을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	
};

END