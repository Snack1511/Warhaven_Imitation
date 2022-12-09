#pragma once
#include "CAIPersonality.h"
BEGIN(Client)
class CPersonality_Default :
    public CAIPersonality
{
protected:
    CPersonality_Default(CTable_Conditions* pConditionTable);
    virtual ~CPersonality_Default();
public:
    static CPersonality_Default* Create(CTable_Conditions* pConditionTable);

public:
    virtual HRESULT Initailize();
};
END

