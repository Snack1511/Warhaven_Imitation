#pragma once
#include "CAIPersonality.h"
BEGIN(Client)
class CPersonality_Sub :
    public CAIPersonality
{
protected:
    CPersonality_Sub(CTable_Conditions* pConditionTable);
    virtual ~CPersonality_Sub();
public:
    static CPersonality_Sub* Create(CTable_Conditions* pConditionTable);

public:
    virtual HRESULT Initailize();
};
END

