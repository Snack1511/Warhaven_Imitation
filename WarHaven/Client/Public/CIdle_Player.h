#include "CState.h"

class CIdle_Player abstract :
    public CState
{

protected:
    CIdle_Player();
    virtual ~CIdle_Player();


public:
    // CState을(를) 통해 상속됨
    virtual HRESULT Initialize()	override;
    virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex) override;
    virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
    virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
    void        Late_Initialize(_uint iChangeAnimIndex, ANIM_TYPE eChangeAnimType, STATE_TYPE eChangeAnimState);

protected:
    _uint       m_iChangeAnimIndex = 0;
    ANIM_TYPE   m_eChangeAnimType = ANIM_END;
    STATE_TYPE  m_eChangeAnimState = STATE_END;


protected:
    virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};
