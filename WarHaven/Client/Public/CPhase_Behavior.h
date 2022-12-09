#pragma once
#include "CPhase.h"
/*        
    3. �ൿ �ܰ�
        --> ���õ� Ÿ�ٿ� ������ ������ �޼��ϱ� ���� Unit�� ���¸� �����ϴ� �ܰ�
        --> �ൿ ���� �� �����޼� �ܰ�� �Ѿ �� ����
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
