#pragma once
#include "CPhase.h"
/*        
    3. 행동 단계
        --> 선택된 타겟에 도달해 목적을 달성하기 위해 Unit의 상태를 변경하는 단계
        --> 행동 종료 후 목적달성 단계로 넘어갈 수 있음
*/
BEGIN(Client)
class CAIController;
class CAIPersonality;
class CPhase_Behavior :
    public CPhase
{
protected:
    CPhase_Behavior(CAIController* pOwner);
    virtual ~CPhase_Behavior();
public:
    static CPhase_Behavior* Create(CAIController* pOwner);
protected:
    virtual HRESULT Initialize();
protected:
    virtual PhaseState Enter_Phase();
    virtual PhaseState Continue_Phase();
    virtual PhaseState Exit_Phase();
};
END
