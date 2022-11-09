#include "stdafx.h"
#include "CState.h"

class CIdle_Warrior :
    public CState
{
    DECLARE_STATE(CIdle_Warrior)

private:
    CIdle_Warrior();
    virtual ~CIdle_Warrior();

public:
    static CIdle_Warrior* Create();

public:
    // CState을(를) 통해 상속됨
    virtual HRESULT Initialize()	override;
    virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
    virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
    virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
    virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};
