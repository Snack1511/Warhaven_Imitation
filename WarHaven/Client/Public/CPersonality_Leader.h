#pragma once
#include "CAIPersonality.h"
BEGIN(Client)
class CPersonality_Leader :
    public CAIPersonality
{
protected:
    CPersonality_Leader(CTable_Conditions* pConditionTable);
    virtual ~CPersonality_Leader();
public:
    static CPersonality_Leader* Create(CTable_Conditions* pConditionTable);

public:
    virtual HRESULT Initailize();
};
END

